package provide mtlib 1.0
#
#       Class: LAsyncEval
# Description:
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2007
#
# Run commands asynchrously, using the event loop
# Notify of the outcome using Events
#
itcl::class LAsyncEval {
    inherit LObject LBinder
    # Event <<error>>  seq tags rc errorInfo
    # Event <<return>> seq tags rc
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -- each command is identified by $tags
    public method Run {cmd {tags ""}} {
	# -- last tag is the sequence number
	set  seq cmd[incr SEQ]
	$queue Enq [list $seq $cmd $tags]
	# -- return the sequence number of the command
	set seq
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable queue ""
    protected variable task ""
    protected variable SEQ 0
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set queue [LSQUEUE $this.queue]
	set task [LSTask $this.task -timeout idle -cmd "$this eRun" -start 0]
	$queue Bind <<new>> $this eRun
    }
    destructor {
	$task delete
	$queue delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eRun {} {
	set cmd [$queue Deq]
	if { $cmd == "" } { return }
	foreach {seq cmd tags} $cmd {break}
	# -- run the command in uplevel
	if { [catch {uplevel #0 $cmd} rc] } {
	    # -- return errors
	    global errorInfo
	    Event <<error>> $seq $tags $rc $errorInfo
	} else {
	    # -- return value
	    Event <<return>> $seq $tags $rc
	}
	# -- start the task again (to process the next item in the queue)
	$task Start
    }
    # -------------------------------------------------- Common Initialization
}
