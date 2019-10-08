package require Itcl
package provide mtlib 1.0

#
# Simple 2way RPC package for TCL
# Identical to LRPC except that it supports two way blocking rpcs
#

#       Class: LRPC2
# Description: Simple RPC Client/Server. Accept remote connections run commands
#      Author: Manolis M. Tsangaris -- mt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2007
#
itcl::class LRPC2 {
    inherit LRPCBase LSBinder
    # event <<login>>
    # event <<logout>>
    # event <<error>>
    # -------------------------------------------------- Public Variables --
    public variable server 0 {rpcs_enable $server } ;# I am a server?
    public variable prefix "uplevel #0" ;#  
    # -------------------------------------------------- Protected Variables 
    protected variable svc          ;# our local socket to connect to
    protected variable PeerData     ;# per peer data collected so far
    protected variable WAIT         ;# per peer reason for waiting, "" otherw
    protected variable speer ""     ;# currently active peer (inbound)
    protected variable peer ""      ;# currently active peer (outbound)
    protected variable Sema         ;# Semaphores for each peer
    protected variable EOC .        ;# end of command 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args 
        # setup the server socket
    }
    destructor { cleanup }
    # -------------------------------------------------- Public Methods ----
    # return out address
    public method Address {} {
        foreach {ip _ pport} [fconfigure $svc -sockname] { break }
        if { $ip == "0.0.0.0" } { set ip [info hostname] }
        return $pport@$ip
    }
    # -- connect to a peer
    public method Connect {address {tries 5}} {
        rpc_parse_addr $address pport phost
        if { $phost == "" } { set phost localhost }
        set peer [rpc_connect $phost $pport $tries]
    }
    # -- disconnected from a peer
    public method Disconnect {{apeer ""}} {
        if { $apeer == "" } { set apeer $peer }
        peer_cleanup $apeer
    }
    # -- send a command to the remote peer
    # -- Send cmd           (to the default peer)
    # -- Send apeer cmd     (to the apeer)
    public method Send {npeer cmd} {
        # use eval to absorb the return value
        #    return result
        #    error  error
        eval [rpc_run $npeer $cmd]
    }
    # allow a command to send data directly using a stream
    #    scmd is the sending command
    #    rcmd is the receiving command
    # use a new channel, to make sure there is no interference with rpc2's
    #
    public method SendStream {mpeer scmd rcmd} {
        # open another connection
        foreach {ip _ pport} [fconfigure $mpeer -peername] { break }
        set npeer [rpc_connect $ip $pport 5]
        fconfigure $npeer -buffering full -encoding binary -translation binary
        if { [catch {rpc_send_stream $npeer $scmd $rcmd} rc] } {
            catch {close $npeer}
            return -code error "$rc"
        } 
        return $rc
    }
    # -- send to an ip address (port@host)
    public method SendStreamTo {_addr scmd rcmd} {
	# -- parse addr
	foreach {_port _ip} [split $_addr @] { break }
        set npeer [rpc_connect $_ip $_pport 5]
        fconfigure $npeer -buffering full -encoding binary -translation binary
        if { [catch {rpc_send_stream $npeer $scmd $rcmd} rc] } {
            catch {close $npeer}
            return -code error "$rc"
        } 
        return $rc
    }
    # -- scan a number of ports, run the command and return the results
    #   rpc_scan $host $from $to $cmd
    # -- create a local proxy of the remote object using this connection
    # -- scan a number of ports, run the command and return the results
    #   rpc_scan $host $from $to $cmd
    # -- create a local proxy of the remote object using this connection
    public method MakeProxy {robj} { }

    # -- return the currently active peer
    public method Peer {} {
        set peer
    }

    # list server state
    public method Info {} {
        set res "clients: \n"
        foreach fp [lsort [array names WAIT]] {
            append res "$fp: $WAIT($fp)"
            catch {fconfigure $fp -peername} in
            append res " ($in)"
            catch {eof $fp} eof
            append res " (eof=$eof)"
            append res \n
        }
        set res
    }
    # -------------------------------------------------- Private Methods ---
    # ------------------------------------------------------------ RPC SERVER
    private method rpcs_enable {enable} {
        if !$enable { return }
        if { [scan $port "%d-%d" p1 p2] != 2 } {
            if { [scan $port "%d-" p1] == 1 } {
                set p2 32000
            } else {
                set p1 $port
                set p2 $port
            }
        }
        set port [rpcs_port_try $p1 $p2]
    }
    # try several ports, and return the port number variable
    # set the listening socket 'svc'
    private method rpcs_port_try {p1 p2} {
        set svc ""
        for {set port $p1} { $port <= $p2 } { incr port } { 
            set cmd [list socket -server "$this eSLogin"]
            if { $host != "" } {
                append cmd " -myaddr $host $port"
            } else {
                append cmd " $port"
            }
            if { ![catch {eval $cmd} rc ] } {
                set svc $rc
                return $port
            }
        }
        error "could not find any free port $p1-$p2 ($rc)"
    }
    protected method cleanup { } {
        catch { close $svc }
        foreach peer [array names PeerData] {
            peer_cleanup $peer
        }
    }

    # -- client has sent some input
    # -- collect it and act on it
    protected method peer_input {fp} {
        debug "input: $fp"
        # try to read one line
        # check if we have failed, or we lost the connection
        if { [catch {gets $fp line} rc]  || ($rc == -1) } {
            if { $PeerData($fp) != "" } {   # in the middle of a command
                warning "$fp: incomplete command $PeerData($fp)"
            }
            $this eSLogout $fp
            peer_cleanup $fp
            return
        }
        set rc [string trim $line]
        debug "$fp< '$rc'"

        if { $rc == $EOC } {                    # end of command?
            debug "peer_input: $fp: complete command"
            peer_process $fp
        } else { 
            append PeerData($fp) $line "\n"
        }
    }
    # process a complete set of data from this peer
    # it could be a command to run, or the result of a remote command
    protected method peer_process {fp} {
        set peer $fp
        set cmd [lindex $PeerData($fp) 0]
        switch $cmd {
            eval {
                # evaluate the remote command and send the result
                set res [leval $PeerData($fp)]
                srpc_send $fp $res
                srpc_send $fp $EOC
                set PeerData($fp) ""
            }
            stream {
                # eliminate async handlers
                fileevent $fp r ""
                fconfigure $fp -buffering full

                # run the command the peer has sent us
                append PeerData($fp) " $fp"
                debug  "rstream: running receiver"
                set res [leval $PeerData($fp)]

                # is the channel still closed?
                if [catch {eof $fp}] {
                    debug  "rstream: receiver finished & closed channel"
                    peer_cleanup $fp
                    return
                } else {
                    debug  "rstream: receiver finished. leaving $fp alone"
                    stream_cleanup $fp
                    return
                }
            }
            return {
                if { $WAIT($fp) != "result" } {
                    warning "unsolicited 'return' in '$WAIT($fp)' state"
                }
                # reply to the
                set cmd [lindex $PeerData($fp) 0]

                # wakeup the recepient of the peer data 
                $Sema($fp) Signal
            }
        }
        waiting_on $fp any
    }

    protected method srpc_send {peer cmd} {
        debug "$peer> $cmd"
        catch {puts $peer $cmd}
    }
    # ------------------------------------------------------------ OUTBOUND CMD

    # send a command to run to our connected peer
    protected method rpc_run {peer cmd} {
        debug "$peer:about to run: $cmd"
        if { $WAIT($peer) == "result" } {
            debug "$peer: $cmd: cannot send while waiting for results"
            error "$peer: $cmd: cannot send while waiting for results"
        }
        waiting_on $peer result
        # send the command and EOC mark
        srpc_send $peer [list eval $cmd]
        srpc_send $peer $EOC

        # wait until results arrive
        debug "rpc: $peer: waiting on reply"
        $Sema($peer) Wait
        set rc $PeerData($peer)
        set PeerData($peer) ""
        set WAIT($peer) any
        debug "rpc: $peer: reply received"
        set rc
    }
    # try to connect a few times
    protected method rpc_connect {phost pport tries} {
        while { $tries > 0 } {
            if { ! [catch {socket $phost $pport} rc] } {
                set fp $rc
                # get ready to accept commands from peer
                peer_setup $fp
                debug "connect: $fp connected: [fconfigure $fp -peername]"
                return $fp
            }
            incr tries -1
        }
        error "Connect: cannot connect to $pport@$phost"
    }

    # -- request peer to receive a stream:
    # -- scmd generates the stream to be consumed by rcmd on peer
    # -- we make a silent assumption that scmd will close the channel
    public method rpc_send_stream {peer scmd rcmd} {
        # send the remote command to READ data from us
        debug "sstream: sending to $rcmd"
        srpc_send $peer [list stream $rcmd]
        srpc_send $peer $EOC
        flush $peer

        # execute a local command to WRITE DATA TO $peer
        debug "sstream: exec scmd: $scmd"
        if [catch {uplevel 2 $scmd $peer} rc] {
            warning "rpc2: send_stream: scmd: $scmd: $rc"
        }
        return $rc
    }
    # ------------------------------------------------------------- port scan
    protected method rpc_scan {host from to cmd} {
        set out ""
        for {set p $from} { $p <= $to } { incr p } {
            lappend out $p@$host 
            if [catch {Connect $p@$host 1} res] {
                lappend out [list error $res]
            } elseif { [catch {Run $cmd} res] } {
                lappend out [list error $res]
            } else {
                lappend out $res
            }
            catch { close $peer }
        }
        set out
    }
    # ------------------------------------------------------------- PROXY
    # create a proxy object for a remote object
    protected method rpc_mkproxy {cmd obj} {}
    # ------------------------------------------------------------- NOTICE
    protected method notice {msg} {
        LLog rpc2 notice $msg 
    }
    protected method debug {msg} {
        LLog rpc2 info $msg 
    }
    protected method warning {msg} {
        LLog rpc2 warning $msg 
    }
    protected method peer_setup {fp} {
        fileevent $fp r "$this eSPeerData $fp"
        fconfigure $fp -buffering line -encoding binary -translation binary
        set WAIT($fp) any
        set PeerData($fp) ""
        set Sema($fp) [LSemaphore #auto]
    }
    # cleanup the state related to this peer
    protected method peer_cleanup {fp} {
        # already closed?
        if ![info exists WAIT($fp)] { return }
        if { $WAIT($fp) == "result" } {
            # give waiting caller the chance to figure out the loss of a peer
            set PeerData($fp) [list error [list $fp: peer died]]
            debug "cleanup: $fp: wakeing up rpc"
            $Sema($fp) Signal
        }
        debug "cleanup: $fp: closing channel"
        unset PeerData($fp)
        unset WAIT($fp)
        $Sema($fp) delete
        catch {close $fp}
        if { $peer == "$fp" } {
            set peer ""
        }
    }
    # cleanup the state related to a stream channel
    protected method stream_cleanup {fp} {
        debug "cleanup: $fp: cleaning up stream channel"
        unset PeerData($fp)
        unset WAIT($fp)
        $Sema($fp) delete
    }
    protected method waiting_on {peer state} {
        set WAIT($peer) $state
    }

    # ------------------------------------------------------------- PEER
    # --- eval a local command on behalf of the peer
    # --- pass the right errorInfo stack
    public method leval {cmd} {
        global errorInfo
        # remove the eval prefix
        set cmd [lrange $cmd 1 end]
        # add the user defined prefix
        set cmd [concat $prefix $cmd]
        if { [catch {eval $cmd} rc] } {
            set err [errorinfo_fix $errorInfo]
            return [list return -code error -errorinfo $err $rc]
        }
        list return $rc
    }
    # fix error info to include the useful part of the stack trace
    protected method errorinfo_fix {einfo} {
        set el [split $einfo \n]
        set el [lrange $el 0 end-3]
        set ei ""
        foreach e $el {
            append ei $e \n
        }
        append ei "----RPC ($host $port)---"
        set ei
    }
    # ------------------------------------------------------- CALLBACKS
    # ------------------------------------------------------------- SERVER
    # --- a peer is about to login to our server
    # --- expect the peer to login
    public method eSLogin {fp peer pport} {
        peer_setup $fp
        debug "connected: $fp [fconfigure $fp -peername]"
        Event! <<login>> $fp
    }
    # --- a peer is about to logout
    public method eSLogout {fp} {
        debug "$fp: logout"
        Event! <<logout>> $fp
    }
    # --- a peer is sending data
    public method eSPeerData {fp} {
        peer_input $fp
    }
    # ------------------------------------------------------------- CLIENT
    # -------------------------------------------------- Common Initialization
}
