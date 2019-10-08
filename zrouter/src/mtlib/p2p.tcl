package provide mtlib 1.0

# Simple P2P package for TCL
# 
# Theory of operation:
#
# 1. A P2P Server accepts a remote TCP connection from a P2P Client
#    and a two way communication starts.
# 2. Any peer may send messages to the other in a async way, without execting
#    a reply.
# 3. The message may be interpreted as a command, or a reply, depending on the
# mood of each side.
# 4. The only thing the implementation facilitates is the integrity of a message
#    which is framed and then send over. To further formalize the communication
#    messages are sent as a pair of attribute/value pairs. Non blocking sockets
#    sort of guarantee that the peers will not block.
#

#       Class: P2P
# Description: P2P Server or Client
#
itcl::class LP2P {
    inherit LObject LBinder
    # Event <<connected>>    sock host port
    # Event <<disconnected>> sock
    # Event <<message>>      sock message
    # -------------------------------------------------- Public Variables --
    public variable ssl  "" { $this eSSLConfigure $ssl }
    public variable tag  ""
    public variable debug 0
    # -------------------------------------------------- Public Methods ----
    # -- in case of a server
    public method Listen {addr} {
	$this p2p_listen $addr
    }
    # -- in case of a client
    public method Connect {addr} {
	$this p2p_connect $addr
    }
    # -- send a message to the peer for evaluation
    public method Send {message {peer ""}} {
	$this p2p_send_message $message $peer
    }
    # -- are we connected to a peer?
    public method Connected {} { 
	if { $peersock == "" } {
	    return 0
	} else {
	    return 1
	}
    }
    # -- return our local address
    public method Address {} {
	if { $sock == "" } { return "" } 
	foreach {_host _hostname _port} [fconfigure $sock -sockname] { break }
	return $_port@$_host
    }
    # -- return the lasttime the channel was used
    public method Lasttime { } { set lasttime }

    # -- Disconnect a peer
    public method Disconnect {peer} {
	$this cleanup $peer
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable sock ""           ;# listening socket
    protected variable PEERDATA          ;# peer data
    protected variable peersock ""       ;# socket of the peer to send commands
    protected variable lasttime 0        ;# last time the connection was used
    protected variable SSL               ;# ssl certificates
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
	eval configure $args
	set lasttime 0
    }
    destructor {
	foreach _sock [array names PEERDATA] {
	    $this cleanup $_sock
	}
	if { $sock != ""     } { catch {close $sock} } 
	if { $peersock != "" } { catch {close $peersock} } 
    }
    # -------------------------------------------------- Private Methods ---
    # -- handle a disconnection of a peer
    protected method disconnected {_sock} {
	if ![$this cleanup $_sock] { return }
	log "$_sock: client disconnected"
	if { $peersock == $sock } {
	    set peersock ""
	}
	$this Event <<disconnected>> $_sock
    }
    protected method cleanup {_sock} {
	set lasttime 0
	# -- ignore multiple attempts to cleanup
	if ![info exists PEERDATA($_sock)] { return 0 }
	catch {close $_sock}
	unset PEERDATA($_sock)
	return 1
    }
    # -- parse an address of the form port@host
    protected method address_parse {addr pp} {
	upvar $pp P
	foreach {P(port) P(host)} [split $addr @] { break }
	set P(address) $addr
    }
    # -- return peer information
    protected method peer_info {_sock pp} {
	upvar $pp P
	array set P {host {} hostname {} port {}}
	foreach {P(host) P(hostname) P(pport)} \
	    [fconfigure $_sock -peername] { break }
    }
    #  -- setup a server socket
    protected method srv_socket {cback host port} {
	if { $ssl != "" } {
	    set fp [$this make_tls_server_socket $host $port $cback]
	    log "SSL listening on $port@$host"
	} else {
	    set fp [socket -myaddr $host -server $cback $port]
	    log "listening on $port@$host"
	}
	set fp
    }
    # -- setup a client socket
    protected method client_socket {host port} {
	if { $ssl != "" } {
	    set fp [$this make_tls_client_socket $host $port]
	    if { $SSL(handshake) } { 
		while { [tls::handshake $fp] != 1} { }
		log "SSL handshake ok"
	    }
	} else {
	    set fp [socket $host $port]
	}
	set fp
    }
    # -- setup TLS socket command depending on options
    protected method tls_setup_sock {args} {
	package require tls
	set cmd $args
	foreach tag {cafile certfile keyfile} {
	    if {$SSL($tag) != "" } {
		lappend cmd -$tag $SSL($tag)
	    }
	}
	if { $SSL(password) != "" } {
	    lappend cmd -password [list $this eSSLPassword]
	}
	foreach tag "ssl2 ssl3 tls1 require request" {
	    lappend cmd -$tag $SSL($tag)
	}
	set cmd
    }
    # -- create a TLS server socket
    protected method make_tls_server_socket {host port cback} {
	set cmd [$this tls_setup_sock tls::socket -server $cback -myaddr $host]
	lappend cmd $port
	set fp [eval $cmd]
	log "tls server socket: $cmd ($fp)"
	set fp
    }
    # -- create a TLS server socket
    protected method make_tls_client_socket {host port} {
	set cmd [$this tls_setup_sock tls::socket]
	lappend cmd $host $port
	set fp [eval $cmd]
	log "tls client socket: $cmd ($fp)"
	set fp
    }
    # -- log tls information
    protected method log_tls_info {_sock} {
	if { ($ssl == "") } { return }
	set si [tls::status $_sock]
	log "SSL connection info:"
	foreach {attr val} $si {
	    log [format "-: %12s: %s" $attr $val]
	}
    }
    # ----------------------------------------------- Interface Implementation
    # -- listen on an address
    protected method p2p_listen {addr} {
	if { $tag == "" } { set tag "server" }
	if { $sock != "" } { catch {close $sock}; set sock "" }
	$this address_parse $addr P
	set sock [$this srv_socket\
	    [list $this eAccept inbound] 0.0.0.0 $P(port)]
    }
    # -- connect to an P2P server
    # -- setup connection
    protected method p2p_connect {addr} {
	if { $tag == "" } { set tag "client" }
	if { $sock != "" } { catch {close $sock}; set sock "" }
	$this address_parse $addr P
	set sock [$this client_socket $P(host) $P(port)]
	$this eAccept outbound $sock $P(host) $P(port)
    }
    # -- request a command to be executed remotely
    protected method p2p_send_message {message peer} {
	global errorInfo

	# -- record the last time the channel was used
	set lasttime [clock seconds]

	set einfo $errorInfo
	if { $peer == "" } { set peer $peersock }
	if { $peer == "" } { 
	    error "not connected to a peer (yet)"
	}
	$this p2p_send_frame $peer message $message
    }

    # -- receive a remote message
    protected method p2p_recv_message {_sock message} {
	set lasttime [clock seconds]
	switch [lindex $message 0] {
	    message { $this Event <<message>> $_sock [lindex $message 1]}
	    default {
		log "warning: ignoring unexpected message '$message'"
	    }
	}
    }
    # -- send a request/reply frame
    protected method p2p_send_frame {_sock code msg} {
	if [catch {
	    puts $_sock [list $code $msg]
	    puts $_sock "."
	    flush $_sock
	} rc] {
	    log "$_sock: transport error: $rc"
	    $this disconnected $_sock
	    return
	}
	set rc
    }

    protected method log {msg} {
	if { $debug } {
	    puts stderr "p2p:$tag: $msg"
	}
    }
    # ----------------------------------------------- CallBacks
    # -- accept a connection from a server
    public method eAccept {mode _sock _host _port} {
	# -- record the last time the channel was used
	set lasttime [clock seconds]

	log "new connection ($mode) : $_sock: $_port@$_host"
	# -- this is only needed for "-accept 1" cases
	# -- but it is also useful to track connected clients
	set PEERDATA($_sock) ""
	# -- if we handle commands, do the bindings
	if { $peersock != "" } {
	    log "warning: already connected to $peersock"
	}
	# -- make sure that SSL processing does take place
	if { ($ssl != "") && $SSL(handshake) } { 
	    while { [tls::handshake $_sock] != 1} { }
	    log "SSL handshake ok"
	}
	$this log_tls_info $_sock
	log "Accepting remote msgs from $_sock"
	fileevent $_sock r "$this ePeerMessage $_sock"
	set peersock $_sock
	fconfigure $_sock -blocking false
	$this Event <<connected>> $_sock $_host $_port
    }
    # -- receive and process a remote command
    public method ePeerMessage {_sock} {
	# -- record the last time the channel was used
	set lasttime [clock seconds]
	if [eof $_sock] {
	    $this disconnected $_sock
	    return
	}
	# -- broken connection?
	if {[catch {gets $_sock line} rc] } {
	    $this disconnected $_sock
	    return
	}
	# -- broken connection?
	if {$rc == -1} {
	    $this disconnected $_sock
	    return
	}
	# -- incomplete line?
	if [fblocked $_sock] { return }

	# -- end of the frame?
	if { $line != "." } {
	    append PEERDATA($_sock) $line \n
	    return
	}
	set message $PEERDATA($_sock)
	set PEERDATA($_sock) ""
	$this p2p_recv_message $_sock $message
    }
    # -- provide the ssl password
    # -- FIXME: this is probably not secure!
    public method eSSLPassword {} {
	return $SSL(password)
    }
    # -- called when -ssl is configured
    public method eSSLConfigure {options} {
	if ![info exists SSL] {
	    # -- SSL defaults
	    array set SSL {cafile {} certfile {} keyfile {}}
	    array set SSL {password {} handshake 1}
	    array set SSL {ssl2 1 ssl3 1 tls1 0 require 1 request 1}
	}
	array set SSL $options
	foreach f {cafile certfile keyfile} {
	    set file $SSL($f)
	    if { $file == "" } { continue }
	    if ![file exists $file] {
		error "$f: SSL file: '$file' does not exist"
	    }
	}
    }
    # -------------------------------------------------- Common Initialization
    # -------------------------------------------------- TESTING
}
