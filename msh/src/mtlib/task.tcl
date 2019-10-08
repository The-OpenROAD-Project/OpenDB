package provide mtlib 1.0

#
#       Class: LSTask
# Description: Simple Background Task
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2006
#
itcl::class LSTask {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable timeout idle            ;# when to run
    public variable cmd ""                  ;# my command
    public variable start 1                 ;# auto start?
    public variable bgerror 1               ;# generate a bgerror?
    # -------------------------------------------------- Public Methods ----
    public method Start {} {
	# -- already scheduled?
        if { $task != "" } { return }
        set task [after $timeout [list $this eRun]]
    }
    # -- cancel/stop task
    public method Cancel {} {
        if { $task == "" } { return 0}
	after cancel $task
	set task ""
	return 1
    }
    # -- is the task active?
    public method Active {} {
	if { $task != "" } { return 1 }
	return 0
    }
    # -- exit code
    public method Status {} {
	return $STATUS
    }
    # -- return code
    public method RC {} {
	return $RC
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable task ""
    protected variable STATUS -1
    protected variable RC ""
    protected variable deleted 0
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	if { $start } { 
	    Start
	}
    }
    destructor {
	set deleted 1
        Cancel
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -- run the command from the idle loop
    public method eRun {} {
	set task ""
        set STATUS [catch {uplevel #0 $cmd} RC]
	# -- in case we have been deleted, do not do much!
	if { $deleted } { return }

	Signal $STATUS
	if { $bgerror && ($STATUS != 0) } {
	    error "$cmd: $RC" 
	}
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LTaskMGR
# Description: Simple (idle) task manager
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris
#
itcl::class LTaskMGR {
    # -------------------------------------------------- Public Variables --
    public variable timeout 0           ;# task timeout in milliseconds
    # -------------------------------------------------- Public Methods ----
    # -- run a command once only
    public method Once {command} {
        if ![info exists TaskID($command)] {
            task_schedule $command
        }
        set TaskID($command)
    }
    public method Cancel {command} {
        set tid TaskID($command)
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable TaskID
    protected variable TaskCMD
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
        foreach t [array names Task] {
            catch {after cancel $t}
        }
    }
    public method delete {} { itcl::delete object $this }
    # -------------------------------------------------- Private Methods ---
    protected method task_schedule {command} {
        if { $timeout == "0" } { set ts idle } else { set ts $timeout }
        set tid [set TaskID($command) [after $ts $command]]
        set TaskCMD($tid) $command
    }
    public method eTask {id} {
        uplevel #0 $TaskCMD($id)
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LSTaskQueue
# Description: Run commands on the idle loop safely
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2006
#
# A common problem is when the idle loop is enabled, while the idle
# loop hanlder is still active. LSAfter solves this problem
# by internally queueing the command, only after the handler has
# finished execution
#
itcl::class LSTaskQueue {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable bgerror 1               ;# generate a bgerror?
    # -------------------------------------------------- Public Methods ----
    public method Run {cmd {timeout idle}} {
	lappend QUEUE [list $timeout $cmd]

	# -- already scheduled?
        if { $task != "" } { return }
	task_schedule
    }
    # -- cancel/stop task -- clean the QUEUE
    public method Cancel {} {
	set QUEUE ""
        if { $task == "" } { return 0}
	after cancel $task
	set task ""
	return 1
    }
    # -- is the queue active?
    public method Active {} {
	if { $task != "" } { return 1 }
	return 0
    }
    # -- exit code of the last command
    public method Status {} {
	return $STATUS
    }
    # -- return code of the last command
    public method RC {} {
	return $RC
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable task ""
    protected variable CMD ""
    protected variable STATUS -1
    protected variable RC ""
    protected variable QUEUE ""                        ;# command queue
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
        Cancel
    }
    # -------------------------------------------------- Private Methods ---
    # -- schedule the next item in the queue
    protected method task_schedule {} {
	set CMD ""
	if {$QUEUE == ""} {return 0}
	foreach {timeout CMD} [lindex $QUEUE 0] { break }
	if { $CMD == "" } {return 0}

	# -- remove command from the QUEUE
	set QUEUE [lrange $QUEUE 1 end]

	set task [after $timeout [list $this eRun]]
	return 1
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -- run the command from the idle loop
    public method eRun {} {
	set task ""
	# -- any items in the queue?
	if { $CMD == "" } { return }

        set STATUS [catch {uplevel #0 $CMD} RC]
	if { $bgerror && ($STATUS != 0) } {
	    error "$CMD: $RC" 
	}
	Signal $STATUS

	# -- schedule next queue items
	task_schedule
    }
    # -------------------------------------------------- Common Initialization
}
