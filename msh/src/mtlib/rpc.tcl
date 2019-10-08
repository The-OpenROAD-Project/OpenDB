package require Itcl
package provide mtlib 1.0

#
# Simple minded RPC package for TCL
#
#

#
#       Class: LSBinder
# Description: Simple Binder
#      Author: Manolis M. Tsangaris -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 1995-2004
#
itcl::class LSBinder {
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Protected Variables 
    protected variable Bind
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    # -------------------------------------------------- Public Methods ----
    # bind an event to a function
    public method Bind {event args} {
        set Bind($event) $args
    }
    # generate an event
    public method Event {event args} {
        if ![info exists Bind($event)] { error "unsupported $event" }
        # run peer command one level up, so it can access peer if needed
        uplevel 1 [concat $Bind($event) $args]
    }
    public method Event! {event args} {
        if ![info exists Bind($event)] { return "" }
        uplevel 1 [concat $Bind($event) $args]
    }
    public method Redirect {event binder} {
        set Bind($event) [list $binder Event $event]
    }
    # ---------------------------------------------------- PROC
    # check if the server can accept connections
    # for the time being, we cannot do anythnig better than opening a connection
    proc ServerCheck {host port} {
        if [catch {socket $host $port} res] {
            return 0
        }
        catch {close $res}
        return 1
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LRPCBase
# Description: Base RPC functionality
#      Author: Manolis M. Tsangaris -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 1995-2004
#
itcl::class LRPCBase {
    # -------------------------------------------------- Public Variables --
    public variable debug "" {debug_set $debug}          ;# see usage below
    # -- port can be NNN or NNN- or NNN-MMM to try a range of ports
    public variable address 1234@localhost { rpc_parse_addr $address port host}
    public  variable port 1234       ; # actual port used 
    # -------------------------------------------------- Protected Variables 
    protected variable host localhost
    private variable DEBUG
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} { eval configure $args }
    destructor { }
    public method delete {} { itcl::delete object $this }
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    public method eError {args} {
        global errorInfo
        set out [concat [$this info class] Error $args]
        puts stderr $out
        puts stderr $errorInfo
    }
    # -------------------------------------------------- Private Methods ---
    # accept port@host or host:port
    public method rpc_parse_addr {address PORT HOST} {
        upvar $HOST host ; upvar $PORT port
        if { [regexp {^([^@]*)@(.*)$} $address _ port host] == 1 } {
            return
        }
        if { [regexp {^([^:]*):(.*)$} $address _ host port] == 1 } {
            return
        }
        if { [regexp {^([^:@]+)$} $address _ port ] == 1 } {
            set host ""
            return
        }
        error "parsing '$address': expected port@host or host:port (port=n1-n2)"
    }
    # ------------------------------------------------------- DEBUG
    protected method dprint {mod msg} {
        if { [info exists DEBUG($mod)] || [info exists DEBUG(*)] } {
            puts stderr "debug:$mod: $msg"
            flush stderr
        } 
    }
    # usage: -debug 1 ----> turn on debugging fully
    #        -debug *            -||-
    #        -debug mod  -> debug information for mod only
    private method debug_set {_debug} {
        if { [string match "+*" $_debug] } {
            set _debug [string range $_debug 1 end]
        } else {
            catch {unset DEBUG}
        }
        if { [scan $_debug "%d" d ] == 1 } {
            set DEBUG(level) $d
            if { $d > 0 } {
                set DEBUG(*) 1
            }
            return
        }
        foreach d $_debug {
            if { $d == "all" } { set d * }
            if ![info exists DEBUG($d)] {
                set DEBUG($d) 0
            }
            incr DEBUG($d)
        }
    }
    # -------------------------------------------------- Common Initialization
}
#
#       Class: LRPCServer
# Description: Simple RPC Server. Accept Remote connections and run commands
#      Author: Manolis M. Tsangaris -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 1995-2004
#
itcl::class LRPCServer {
    inherit LRPCBase LSBinder
    # event <<login>>
    # event <<logout>>
    # -------------------------------------------------- Public Variables --
    public variable restricted 1;# are client commnds restricted?
                                ;# if so, run them through the event system
                                ;# else run the freely at uplevel #0
    public variable prefix ""   ;# command prefix for remote cmds
    # -------------------------------------------------- Protected Variables 
    protected variable svc
    protected variable PeerCmd
    # currently active peer
    protected variable peer ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args 
        set port [init_port $port]
    }
    destructor { cleanup }
    # -------------------------------------------------- Public Methods ----
    public method Address {} {
        foreach {ip _ pport} [fconfigure $svc -sockname] { break }
        if { $ip == "0.0.0.0" } { set ip [info hostname] }
	if { $ip == "" } { set ip [info hostname] }
        return $pport@$ip
    }
    # send an event to the peer
    public method SendEvent {peer args} {
        dprint rpcserver "! $args"
        if [catch {puts $peer [concat event $args]} err] {
            dprint rpcserver "! event failed: $args ($err)"
        }
        dprint rpcserver "! sent"
    }
    # -------------------------------------------------- Virtual Methods ---
    # --- a peer is about to login
    public method eLogin {fp peer pport} {
        fconfigure $fp -buffering line
        fileevent $fp r "$this eCmd $fp"
        set PeerCmd($fp) ""
        Event! <<login>> $fp $peer $pport
    }
    # --- a peer is about to logout
    public method eLogout {fp} {
        Event! <<logout>> $fp
    }
    # --- a peer has requested a command to run
    public method eCmd {fp} {
        rpc_cmd $fp
    }
    # --- run a command on behalf of the peer 
    # You may choose to run this differently. We simply run it within
    # the context of this object handling the connection
    public method eRunCmd {fp line} {
        set peer $fp
        if { $restricted } {
            set cmd [concat Event $line]
        } else {
            if { $prefix == "" } {
                set cmd $line
            } else {
                set cmd [concat $prefix $line]
            }
        }
        global errorInfo
        # remove the eval prefix
        # add the user defined prefix
        if { [catch {eval $cmd} rc] } {
            set err [errorinfo_fix $errorInfo]
            return [list return -code error -errorinfo $err $rc]
        }
        list return $rc
    }
    protected method errorinfo_fix {einfo} {
        set el [split $einfo \n]
        set el [lrange $el 0 end-3]
        set ei ""
        foreach e $el {
            append ei $e \n
        }
        append ei "---- RPC ($host $port) ---"
        set ei
    }
    # -------------------------------------------------- Private Methods ---
    private method init_port {port} {
        if { [scan $port "%d-%d" p1 p2] != 2 } {
            if { [scan $port "%d-" p1] == 1 } {
                set p2 32000
            } else {
                set p1 $port
                set p2 $port
            }
        }
        return [port_try $p1 $p2]
    }
    private method port_try {p1 p2} {
        for {set port $p1} { $port <= $p2 } { incr port } { 
            if { $host != "" } {
                set cmd [list socket -server "$this eLogin" -myaddr $host $port]
            } else {
                set cmd [list socket -server "$this eLogin" $port]
            }
            if { ![catch {eval $cmd} res ] } {
                set svc $res
                return $port
            }
        }
        error "could not find any free port $p1-$p2"
    }
    protected method cleanup { } {
        catch { close $svc }
    }

    # -- client has sent some input
    protected method rpc_cmd {fp} {
        set line "EOF"
        dprint rpcserver "<= (reading)"
        if { [catch {gets $fp line} rc] } {
            if { $PeerCmd($fp) != "" } {
                $this eError rcmd.eof $fp $rc
            } else {
                $this eLogout $fp
            }
            catch { close $fp } 
            return
        }
        set rc $line
        dprint rpcserver "<= $line"

        if { ($PeerCmd($fp) == "") && [eof $fp] } { 
            dprint rpcserver "?  peer $fp died"
            $this eLogout $fp
            catch { close $fp } 
            return
        }
        set rcmd [append PeerCmd($fp) $rc "\n"]
        if ![info complete $rcmd] {
            dprint rpcserver "?  incomplete peer cmd $PeerCmd($fp)"
            return
        }
        set PeerCmd($fp) ""

        set rc [$this eRunCmd $fp $rcmd]

        dprint rpcserver "-> ``$rc''"
        catch {puts $fp $rc}
        catch {puts $fp .}
    }
    # -------------------------------------------------- Common Initialization
}

# Object Name: LRPCClient
# Description: Generic RPC client. Connect to a remote server and run commands
#      Author: Manolis M. Tsangaris -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 1995-2004
#
itcl::class LRPCClient {
    inherit LRPCBase LSBinder
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Protected Variables 
    protected variable peer ""
    protected variable eventcmd ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} { 
        eval  configure $args
        init
    }
    destructor { cleanup }
    # -------------------------------------------------- Public Methods ----
    # -- connect to a server
    public method Connect {address {tries 5}} {
        rpc_parse_addr $address pport phost
        if { $phost == "" } { set phost localhost }
        rpc_connect $phost $pport $tries
    }
    # -- disconnected from a server
    public method Disconnect {} {
        catch {close $peer}
    }
    public method Run {args} {
        set r [rpc_rcmd $args]
    }
    # -- scan a number of ports, run the command and return the results
    public method Scan {host from to cmd} {
        rpc_scan $host $from $to $cmd
    }
    # -- create a local object proxy
    public method MakeProxy  {robj {local ""}} { }

    # -------------------------------------------------- Virtual Methods ---
    # Simply collect any ``event'' output
    # possibly a result of a trigger command
    public method eResponse {} {
        if { [gets $peer line] == -1 } {
            catch {close $peer}
            set eventcmd ""
            return ""
        }
        rpc_event_process $line
    }
    # -------------------------------------------------- Private Methods ---
    protected method init { } {
    }
    protected method cleanup { } {
        catch { close $peer }
    }
    protected method rpc_rcmd {cmd} {
        if { $peer == "" } {
            error "not connected"
        }
        # send the command over ...
        dprint rpc "-> $cmd"
        puts $peer $cmd

        # read a complete TCL expression as an answer:
        # return    RETURN_VALUE 
        # error     ERROR_CODE 
        set res ""

        while { [catch {gets $peer line} err] == 0} {
            if { $err == -1 } { break }
            # due to TCP buffering, there may be unprocessed events
            if { [string match "event *" $line] } {
                dprint rpc "delayed event $line"
                rpc_event_process $line
                continue
            }
            dprint rpc " <= $line"
            if { $line == "." } {
                # simply evaluate the returned command
                dprint rpc " -- COMPLETE"
                eval $res
                # NOT REACHED!
                return
            }
            append res $line\n
        }
        catch { close $peer } 
        error "rpc: peer $address died"
    }
    # try multiple times, just in case ...
    protected method rpc_connect {phost pport tries} {
        catch { close $peer }
        for { } { $tries > 0 } {incr tries -1} {
            if { ! [catch {set peer [socket $phost $pport]}] } {
                fileevent $peer r "$this eResponse"
                fconfigure $peer -buffering line
                return ""
            }
        }
        error "Connect: cannot connect to $pport@$phost"
    }

    # process an event ...
    protected method rpc_event_process {line} {
        append eventcmd  $line "\n"

        if { ![info complete $eventcmd] } { return }

        # eventcmd should be: Event command args
        # -- the event will be actually executed on idle
        switch [lindex $eventcmd 0] {
            event {
                after idle $this Event [lrange $eventcmd 1 end]
            }
            default {
                puts stderr "RPCClient: unknown cmd $eventcmd"
            }
        }
        set eventcmd  ""
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
    # ------------------------------------------------------- proxy
    protected method rpc_mkproxy {cmd obj local} {}
    # -------------------------------------------------- Common Initialization
}

# parse a url like address spec
# accept port@host or host:port
proc LParseURL {url r} {
    upvar $r R
    if { [regexp {^([^@]+)@(.+)$} $url _ R(port) R(host)] == 1 } {
        return
    }
    if { [regexp {^([^:]+):(.+)$} $url _ R(host) R(port)] == 1 } {
        return
    }
    if { [regexp {^([^:@]+)$} $url _ R(port) ] == 1 } {
        set host R(localhost)
        return
    }
    error "$url: expected port@host or \[host:\]port"
}

# connect to any server specified in the path
proc LConnectTo {path} {
    set errors ""
    set sep ""
    foreach p $path {
        LParseURL $p A
        if {![catch {socket $A(host) $A(port)} res]} {
            return $res
        }
        append errors "$sep$p: $res" ; set sep "\n"
    }
    return -code error $errors
}

#
# LRPCClient
#
# Create a proxy for a remote object
itcl::body LRPCClient::MakeProxy {robj {local ""}} {
    rpc_mkproxy "$this Run" $robj $local
}

itcl::body LRPCClient::rpc_mkproxy {cmd obj proc} {
    set rcmd ""
    if { $proc == "" } {
        set proc ::[namespace tail $obj]
    } else {
        set proc ::$proc
    }
    append rcmd "# --- proxy for $obj\n"
    append rcmd "proc $proc {ncmd args} {\n"
    append rcmd "    switch \$ncmd {\n"
    set cmds [eval $cmd LPublicMethods $obj]
    foreach fn $cmds {
        # -- now generate a case statement for this method
        append rcmd "        $fn {\n"
        append rcmd "           eval $cmd $obj $fn \$args\n"
        # -- if destructor has been called, remove proxy also
        if { $fn == "delete" } {
            append rcmd "           rename $proc {}\n"
        }
        append rcmd "        }\n"
    }
    append rcmd "        default {\n"
    append rcmd "           error \"unknown command: '\$ncmd'\n(use: $cmds)\n\""
    append rcmd "        }\n"
    append rcmd "  }\n"
    append rcmd "}\n"
    uplevel #0 $rcmd
    set proc
}
