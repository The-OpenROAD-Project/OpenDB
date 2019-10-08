# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB
package provide mtlib 1.0

package require Itcl
#
#       Class: LSharedVarBase
# Description: Base class for common stuff
#      Author: Manolis M. Tsangaris  -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LSharedVarBase {
	inherit LNObject
    # -------------------------------------------------- Public Variables --
	public variable host ""        ;# host & port to run/connect
	public variable port ""
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Protected Variables 
	private variable MAGIC .EOR.   ;# magic number to separate fields
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
		eval configure $args
	}
    destructor { }
    # -------------------------------------------------- Private Methods ---
	# ----------------------------------------------- Interface Implementation
	# -- get the new value from the peer
	# --   return 1 if no value was sent (subscription)
	# --   return 2 if value was sent 
	# --   return 0 on error
	# -- The protocol is simple here: the name of the variable is sent first
	# -- followed (optionally) by its value
	# -- NOTE: This is a very simplistic framing protocol, and it may change
	protected method cmd_get {peer CMD} {
		upvar $CMD cmd
		set cmd ""
		while { [gets $peer line] != -1 } {
			if { $line == $MAGIC } {
				break
			}
			append cmd $line \n
		}
		notice "cmd_get [string trim $cmd]"
		lindex $cmd 0
	}
	# send a value to the peer
	protected method cmd_send {peer cmd} {
		puts $peer $cmd
		puts $peer $MAGIC
		flush $peer
	}
	# -- return the peername 
	protected method pname {fp} {
		if [catch {fconfigure $fp -peername} p] {
			return "unconnected"
		}
		foreach {ip host port} $p { break }
		return $port@host
	}
	# -------------------------------------------------------------
}
#
#       Class: LSharedVarServer
# Description: Simple Shared Variable server (influenced by tequila)
#      Author: Manolis M. Tsangaris  -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 2004
#
# The server accepts connections from clients, subscriptions and
# value updates of shared variables.
# It will broadcast the new values to all clients subscribed
#
# Note that all communication is one way (half duplex)
# as no response is expected from either side.

itcl::class LSharedVarServer {
	inherit LSharedVarBase
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Protected Variables 
	private variable CLIENT      ;# array to hold active clients
	private variable SUB         ;# array to hold subscriptions
	private variable svc ""      ;# the connected socket
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
		eval configure $args
		socket_setup $host $port
	}
    destructor {
		client_cleanup *
	}
    # -------------------------------------------------- Private Methods ---
	# set up a client socket
	protected method socket_setup {host port} {
		if { $host == "" } {
			set srv [socket -server "$this eConnect" $port]
		} else {
			set srv [socket -server "$this eConnect" -myaddr $host $port]
		}
	}
	# ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Callbacks      ----
	# -- a new client connects to the server
	public method eConnect {peer host port} {
		notice "new client: $port@$host ($peer)"
		set CLIENT($peer) 1 
		fileevent $peer r "$this eNewValue $peer"
	}
	# -- a new value has been received by a client
	public method eNewValue {peer} {
    	switch [cmd_get $peer cmd] {
        	subscribe {
				set var [lindex $cmd 1]
				set val [lindex $cmd 2]
            	# set subscription
            	set SUB($peer,$var) $peer
				# if current value exists, send that too
				upvar #0 $var V
            	if ![info exists V] {
					set V $val
				}
				notice "providing initial value of $var ($V) to $peer"
				set cmd [list update $var $V]
                if [catch {cmd_send $peer $cmd}] {
                   	client_cleanup $peer
                }
        	}
        	incr {
				# increment the variable; send the new value
				set var [lindex $cmd 1]
				upvar #0 $var val
				incr val
				value_broadcast - $var $val
			}
        	update {
				# inform the rest of the subscribers to the new value
				set var [lindex $cmd 1]
				upvar #0 $var val
				set val [lindex $cmd 2]
				value_broadcast $peer $var $val
			}
        	default {
            	client_cleanup $peer
        	}
    	}
	}

	# broadcast new value to all subscribers except for ipeer
	# which (presumably) did the update
	protected method value_broadcast {ipeer var val} {
		set cmd [list update $var $val]
		foreach sub [array names SUB *,$var] {
			set peer $SUB($sub)
			set p [pname $peer]
			if { $peer == $ipeer } {
				notice "$p: not updating (sender)"
				continue
			}
			notice "$p:updating"
			if [catch {cmd_send $peer $cmd}] {
				client_cleanup $peer
			}
		}
	}

	# -------------------------------------------------------------
	# -- cleanup the state of a client
	private method client_cleanup {{pat *}} {
		foreach peer [array names CLIENT $pat] {
			foreach sub [array names SUB $peer,*] {
				unset SUB($sub)
			}
			notice "cleaning up $peer ([pname $peer])"
			catch {close $peer}
			unset CLIENT($peer)
		}
	}
	
}

#
#       Class: LSharedVarClient
# Description: Simple Shared Variable server (influenced by tequila)
#      Author: Manolis M. Tsangaris  -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LSharedVarClient {
	inherit LSharedVarBase
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
	# share a global variable
	public method Share {var initval} {
		uplevel [list trace variable $var w [list $this eChanged $peer]]
		# -	- subscribe to this variable
		cmd_send $peer [list subscribe $var $initval]
	}
	public method Incr {var} {
		cmd_send $peer [list incr $var]
	}
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
	private variable peer  "" ; # connection to the var server
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
		eval configure $args
		peer_connect
	}
    destructor {
		if { $peer != "" } {
			peer_cleanup
		}
	}
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- Callbacks      ----
	# -- called by "trace variable" on variable write
	public method eChanged {peer var _ _} {
		upvar $var V
		# do not send back server updates to the server
		# if this is the case, server_update variable will be active on caller
		if [uplevel info exists server_update] { return }

		cmd_send $peer [list update $var $V]
	}

	# -- called when the server has sent us a new variable value
	public method eReceive {peer} {
		switch [cmd_get $peer cmd] {
			update {
				set var [lindex $cmd 1]
				set val [lindex $cmd 2]
				notice "value update: $var $val"
				# mark this variable trace, so we do not to send back
				# server updates
				set server_update 1
				upvar #0 $var V
				set V $val
			}
			default {
				peer_cleanup
				return
			}
		}
	}
	# ----------------------------------------------- Interface Implementation
	private method peer_cleanup {} {
		notice "cleaning up peer ($peer)"
		catch {close $peer}
		set peer ""
	}
	private method peer_connect {} {
		if { $host == "" } { set host localhost }
		set peer [socket $host $port]
		fileevent $peer r "$this eReceive $peer"
	}
    # -------------------------------------------------- Common Initialization
}
