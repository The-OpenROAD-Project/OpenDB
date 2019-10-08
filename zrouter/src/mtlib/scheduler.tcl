package provide mtlib 1.0

# --------------------------------------------------------------------------
#       Class: LScheduler
# Description: Simple Serial Scheduler. Run commands later (after $timeout)
#              Do not run command again if already scheduled (based on its tag)
#
itcl::class LScheduler {
    inherit LObject  LBinder
    # Event <<error>>   tag cmd error errorInfo  # flag a command error
    # Event <<result>>  tag cmd res     # flag a successful command
    # -------------------------------------------------- Public Variables --
    public variable timeout idle      ;# when to run commands
    # -------------------------------------------------- Public Methods ----
    # -- Queue the command to run. Do not accept if tag already in the queue
    public method Run {tag cmd} {
        # is this command still active? No need to run again
        if [info exists ACTIVE($tag)] {
            LLog scheduler info "$tag: already active ($ACTIVE($tag))"
            LLog scheduler info "$tag: cmd: $ACTIVE($tag)"
            return ""
        }
        set ACTIVE($tag) $cmd
        LLog scheduler info "queue : $tag $cmd"
        $Q Enq [list $tag $cmd]
        return $tag
    }
    # -- simply disable this tag (and any commands with this tag)
    # -- we should actually remove all commands with this tag from the queue
    public method Cancel {tag} {
        if [info exists ACTIVE($tag)] { unset ACTIVE($tag) }
    }
    public method Print {} {
        notice "Pending Tasks"
        foreach tid [lsort [array names TASK]] {
            notice "$tid $CMD($tid)"
        }
    }
    # -------------------------------------------------- Virtual Methods ---
    protected variable TASK ""      ; # background task id
    protected variable ACTIVE       ; # set if the tagged command is active
    protected variable Q            ; # a fifo queue
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set  Q [LSQUEUE $this.queue]
        $Q Bind <<new>> $this eNewItem
    }
    destructor {
        if { $TASK != "" } { 
            after cancel $TASK
        }
        $Q delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -- run a scheduled command from the queue
    public method eRun {} {
        set tag ""
        while { [set tcmd [$Q Deq]] != "" } { 
            LLog scheduler info "dqueue: $tcmd"
            foreach {tag cmd} $tcmd {break}
            if ![info exists ACTIVE($tag)]  {
                LLog scheduler info "dqueue: $tag: inactive!"
                set tag "" ; continue
            }
            break
        }
        if { $tag == "" } {
            set TASK ""
            return
        }

        LLog scheduler info "run   : $tag $cmd"

        set n [catch {uplevel #0 $cmd} rc]

        # '$cmd' could enter the idleloop
        # SERIOUS RACE CONDITION HERE!
        # ONLY AFTER THE COMMAND IS DONE, WE MARK THIS TASK AS DONE!
        unset ACTIVE($tag)

        # schedule the next one
        set TASK [after $timeout "$this eRun"]

        if { $n } { 
	    global errorInfo
            Event <<error>> $tag $cmd $rc $errorInfo
        } else {
            Event <<result>> $tag $cmd $rc
        }
    }
    # -- schedule a run if nothing is running
    public method eNewItem {} {
        if { $TASK != "" } { return }
        set TASK [after $timeout "$this eRun"]
    }
    # -------------------------------------------------- Common Initialization
}
