package provide mtlib 1.0
#
#       Class: ADEMaster
# Description: Manage p2p slaves defined using the ADESlave object
#
itcl::class ADEMaster {
    inherit LObject LBinder
    # -------------------------------------------------- Public Variables --
    public variable port 1234  ;# port to listen to
    public variable tag master ;#
    public variable debug 0    ;# 
    public variable nospawn 0  ;# if set, spawn will not happen
    # -------------------------------------------------- Public Methods ----
    public method StartSlave {tag script} {
	$this startSlave $tag $script
    }
    public method Send {slaveTag event {attrs ""}} {
	$this sendSlave $slaveTag $event $attrs
    }
    public method OnEvent {body} {
	$eventHandler OnEvent $body
    }
    # -- slave status
    public method Status {} {
	slaveStatus
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable SLAVES 0 ;# keep there the number of slaves created
    protected variable SLAVE    ;# keep there the number of slaves created
    protected variable TASK     ;# keep track of slaves
    protected variable SLAVETAG ;# map socket to slave tag
    protected variable p2p      ;# p2p server
    protected variable eventHandler ;# to support OnEvent calls
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
	eval configure $args
	set eventHandler [LEventHandler $this.ehandler]
	set p2p [LP2P $this.p2p -tag $tag]
	$p2p Bind <<connected>>     $this eSlaveConnected
	$p2p Bind <<disconnected>>  $this eSlaveDisconnected
	$p2p Bind <<message>>       $this eSlaveMessage
	dlog "listening to $port"
	$p2p Listen $port@0.0.0.0	

    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method spawnScript {task script} {
	dlog "spawning $task: $script"
	if {$nospawn} {
	    dlog "PLEASE DO IT MANUALLY!"
	} else {
	    uplevel 2 [list spawn $task $script]
	}
    }

    protected method handleEvent {event args} {
	dlog "handling event: '$event' $args"
	if [$eventHandler IsHandled $event] {
	    eval [concat $eventHandler Handle $event $args]
	} else {
	    eval [concat Event $event $args]
	}
    }

    protected method dlog {msg} {
	if {$debug} {
	    puts stderr "ADEMASTER: $msg"
	}
    }
    # -------------------------------------------------- INTERFACE
    # -- display slave status
    protected method slaveStatus {} {
	set out ""
	foreach tag [lsort [array names SLAVE]] {
	    array set S $SLAVE($tag)
	    append out [format "%15s %15s %s" $tag $S(status) $S(remote)]
	    append out "\n"
	}
	set out
    }
    # -- create a slave
    protected method startSlave {tag script} {
	global env
	set task $this.slave[incr SLAVES]
	set SLAVE($tag) [list task $task peer "" remote "" status unconnected]
	# -- insert some environment values here
	set cmd ""
	append cmd "set env(ADESLAVETAG) $tag;"
	append cmd "set env(ADEADDRESS) $port@[info hostname];"
	append cmd $script
	dlog "new script: $cmd"
	spawnScript $task $cmd
    }
    # -- send a message to a slave
    protected method sendSlave {slaveTag event attrs} {
	if ![info exists SLAVE($slaveTag)] {
	    error "slave $slaveTag does not exist"
	}
	array set S $SLAVE($slaveTag)
	if { $S(peer) == "" } {
	    error "slave $slaveTag has not connected yet"
	}
	$p2p Send [list $event $attrs] $S(peer)
    }
    # -------------------------------------------------- EVENTS
    public method eSlaveConnected {sock host port} {
	dlog "slave connect: $port@$host ($sock)"
    }
    public method eSlaveDisconnected {sock} {
	dlog "slave disconnect: $sock"
	if [info exists SLAVETAG($sock)] {
	    set tag $SLAVETAG($sock)
	    unset SLAVE($tag)
	    unset SLAVETAG($sock)
	    handleEvent $tag.disconnected
	} else {
	    dlog "stray connection at socket $sock (closed)"
	    # stray connection
	}
    }
    # -- messages contain an event field and a dictionary
    public method eSlaveMessage {sock message} {
	dlog "client message: $message"
	set event [lindex $message 0]
	set attrs [lrange $message 1 end]
	switch -- $event {
	    login {
		array set A $attrs
		set SLAVETAG($sock) $A(tag)
		if [info exists SLAVE($A(tag)] {
		    array set S $SLAVE($A(tag))
		}
		set S(peer) $sock
		set S(remote) [fconfigure $sock -peername]
		set S(status) connected
		set SLAVE($A(tag)) [array get S]
		handleEvent $A(tag).connected
	    }
	    default {
		if ![info exists SLAVETAG($sock)] {
		    dlog "stray client message: $message"
		    return
		}
		eval [concat handleEvent $SLAVETAG($sock).$event $attrs]
	    }
	}
    }
    # -------------------------------------------------- Common Initialization
}
#
#       Class: ADESlave
# Description:
#
itcl::class ADESlave {
    inherit LObject LBinder
    # -------------------------------------------------- Public Variables --
    public variable tag ""    ;# our tag to identify us
    public variable server "" ;# where to connect
    public variable debug 0
    # -------------------------------------------------- Public Methods ----
    # -- send an event to the master
    public method Send {event {dict {}}} {
	$p2p Send [list $event $dict]
    }
    public method OnEvent {body} {
	$eventHandler OnEvent $body
    }
    public method Wait {} {
	vwait forever
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable p2p ""
    protected variable eventHandler ;# to support OnEvent calls
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
	eval configure $args
	global env
	set p2p [LP2P $this.p2p]
	set eventHandler [LEventHandler $this.ehandler]

	if { $server == "" } { 
	    if ![info exists env(ADEADDRESS)] {
		error "ADESLAVE: cannot find ADEADDRESS in the environment"
	    }
	    $this configure -server $env(ADEADDRESS)
	}
	if { $tag == "" } { 
	    if ![info exists env(ADESLAVETAG)] {
		error "ADESLAVE: cannot find ADESLAVETAG in the environment"
	    }
	    $this configure -tag $env(ADESLAVETAG)
	}

	$p2p Bind <<connected>>    $this eMasterConnected
	$p2p Bind <<disconnected>> $this eMasterDisconnected
	$p2p Bind <<message>>      $this eMasterMessage

	# -- connect and send the tag
	dlog "connecting to $server"
	$p2p Connect $server
	$p2p Send [list login tag [$this cget -tag]]
	handleEvent connected
    }
    destructor {
	$p2p delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method dlog {msg} {
	if {$debug} {
	    puts stderr "ADEMASTER: $msg"
	}
    }
    protected method handleEvent {event args} {
	dlog "event $event $args"
	if [$eventHandler IsHandled $event] {
	    eval [concat $eventHandler Handle $event $args]
	} else {
	    eval [concat Event $event $args]
	}
    }
    # ---------------------------------------------------- Callbacks -------
    public method eMasterConnected {sock host port} {
	handleEvent connected host $host port $port
    }
    public method eMasterDisconnected {sock} {
	handleEvent disconnected
    }
    # -- messages contain an event field and a dictionary
    public method eMasterMessage {sock message} {
	set event [lindex $message 0]
	eval [concat handleEvent $event [lrange $message 1 end]]
    }
    # -------------------------------------------------- Common Initialization
}
