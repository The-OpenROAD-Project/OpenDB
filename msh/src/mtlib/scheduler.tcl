package provide mtlib 1.0

# --------------------------------------------------------------------------
#       Class: LScheduler
# Description: Simple Serial Scheduler. Run commands later (after $timeout)
#              Do not run command again if already scheduled (based on its tag)
#      Author: Manolis M. Tsangaris  -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris
#
itcl::class LScheduler {
    inherit LObject  LBinder
    # Event <<error>>   tag cmd error   # flag a command error
    # Event <<result>>  tag cmd res     # flag a successful command
    # -------------------------------------------------- Public Variables --
    public variable timeout idle      ;# when to run commands
    # -------------------------------------------------- Public Methods ----
    public method Run {tag cmd} {
        LLog scheduler info "run: $tag: $cmd"
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
    # simply disable this tag
    public method Cancel {tag} {
        if [info exists TASK($tag)] { unset TASK($tag) }
    }
    public method Print {} {
        puts "Pending Tasks"
        foreach tid [lsort [array names TASK]] {
            puts "$tid $CMD($tid)"
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
    # run a scheduled command from the queue
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
            Event <<error>> $tag $cmd $rc
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
