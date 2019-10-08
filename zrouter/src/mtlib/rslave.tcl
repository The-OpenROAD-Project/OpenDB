package provide mtlib 1.0
#
# LRSlave is a local proxy of a remote object, capable of running 
# remote commands and receiving events asynchronously
#
# The user should subclass LRemoteProxy and LRemoteSlave

#       Class: LRemoteProxy
# Description: Simple remote slave to send async events
#      Author: Manolis M. Tsangaris
#
# if in async mode, LRemoteProxy will post an event when
# the remote slave is created
#
itcl::class LRemoteProxy {
    inherit LObject LBinder
    # Event event args
    # Event <<connected>> status          ;# when connected to remote slave
                                          ;# status=1 if success
    # Event <<died>>                      ;# remote peer died
    # -------------------------------------------------- Public Variables --
    public variable address "" {$this rp_connect $address} ;# address to connect
    public variable rpcdebug 0 { $rpc configure -debug $rpcdebug }
    public variable async 0    ;# asynchrounsly connect to remote slave
    # -------------------------------------------------- Public Methods ----
    # -- send a remote command -- receive an event as reply
    public method Send {command {event "rcmd"} args} {
	# -- make sure the remote slave is connected
	$this eRSlaveConnected?
	set id [incr serial]
	set HANDLER($id) [list $event $args]
	$rpc Run Eval $id $command 
    }
    # -- run a command synchronously
    public method Run {command} {
	$rpc Run Exec $command 
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable sched ""        ;# scheduler
    private variable rpc ""          ;# rpc client
    private variable serial 0        ;# remote command serial number
    private variable HANDLER         ;# how to handle the return code
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set rpc   [LRPCClient $this.srv -debug $rpcdebug]

	$rpc Bind <<return>> $this eEvent
	$rpc Bind <<logout>> $this ePeerDisconnected

	set sched [LScheduler $this.sched]
	$sched Bind <<error>> $this eSchedulerError
	if { $address != "" } { $this rp_connect $address }
    }
    destructor {
	$rpc   delete
	$sched delete
    }
    # -------------------------------------------------- Private Methods ---
    private method rp_connect {addr} {
	if { $rpc == "" } { return }
	if { $addr == "" } { return }
	$rpc Connect $addr
    }
    # ----------------------------------------------- Interface Implementation
    # -- use this shorthand for notice
    protected method eNotice {msg} {
	puts stderr "notice: [$this info class]: $msg"
    }
    # ----------------------------------------------- CallBacks
    public method eEvent {serial rc} {
	if ![info exists HANDLER($serial)] {
	    LLog remproxy warning "no handler for serial #$serial"
	    return
	}
	foreach {ev argv} $HANDLER($serial) { break }
	unset HANDLER($serial)
	set cmd [concat $this Event $ev $argv $rc]
	$sched Run $serial $cmd
    }
    # -- make sure the remote slave is connected
    public method eRSlaveConnected? {} {
    }
    # -- Just report a scheduler error. We should probably do more
    # -- here, so the remote client nows the outcome of his command
    # -- but this kind of error should be rare.
    public method eSchedulerError {tag cmd error errorInfo} {
	$this eNotice "scheduler: $tag: $cmd: $errorInfo"
    }
    # -- remote peer disconnected
    public method ePeerDisconnected {_} {
	Event <<died>>
    }
    # -------------------------------------------------- Common Initialization
}


# --------------------------------------------------------------------------
#       Class: LRemoteSlave
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class LRemoteSlave {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable address "" { rp_address $address }
    public variable rpcdebug 0 { $rpc configure -debug $rpcdebug }
    # -------------------------------------------------- Public Methods ----
    # -- evaluate a command sent by remote
    public method Eval {serial cmd} {
	$this eNotice "eval $serial $cmd"
	# -- notice the peer
	upvar peer peer
	set tag $peer.$serial
	# -- schedule the command for later
	$sched Run $tag [list $this eDelayedRun $peer $serial $cmd]
	return "$serial"
    }
    public method Exec {cmd} {
	eval $cmd
    }
    public method Test {args} {
	return $args
    }
    public method Address { } { $rpc Address }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable rpc ""          ;# rpc server
    private variable sched ""        ;# scheduler
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set sched [LScheduler $this.sched]
	$sched Bind <<error>> $this eSchedulerError
    }
    destructor {
	$rpc   delete
	$sched delete
    }
    # -------------------------------------------------- Private Methods ---
    private method rp_address {addr} {
	if { $addr == "" } { return }
	if { $rpc != "" } { $rpc delete }
	set rpc   [LRPCServer $this.srv -restricted 0 \
	    -prefix "$this" -debug 0 -address $addr]
    }
    # ----------------------------------------------- Interface Implementation
    # -- use this shorthand for notice
    protected method eNotice {msg} {
	puts stderr "notice: [$this info class]: $msg"
    }
    # ----------------------------------------------- CallBacks
    public method eDelayedRun {peer serial cmd} {
	$this eNotice "run: $peer $serial $cmd"
	# -- the command will run at the top level and in the context
	# -- of the current object
	set cmd [concat uplevel #0 $this $cmd]
	if { [catch $cmd rc] } {
	    set ret [list error $rc]
	} else {
	    set ret [list return $rc]
	}
	$this eNotice "result: $ret"
	if [catch {$rpc SendEvent $peer <<return>> $serial $ret} rc] {
	    $this eNotice "send event: $serial $peer: $rc"
	}
    }
    # -- Just report a scheduler error. We should probably do more
    # -- here, so the remote client nows the outcome of his command
    # -- but this kind of error should be rare, given that the
    # -- actual user command is evaluated using catch in eDelayedRun
    public method eSchedulerError {tag cmd error errorInfo} {
	$this eNotice "scheduler: $tag: $cmd: $errorInfo"
    }
    # -------------------------------------------------- Common Initialization
}
