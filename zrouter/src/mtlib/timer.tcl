package provide mtlib 1.0

#
#       Class: LTimer
# Description: Simple Timer & Resource measurement facility
#
itcl::class LTimer {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -- mark this instance and start the counter
    # -- stop the previous counter if any
    public method Mark {state} {
        timer_mark $state
    }

    public method Report_puts { } {
        set fmt1 "%-18s %8s"
        set fmt  "%-18s %8.2f"
        puts [format $fmt1 "State" "Time(sec)"]
        foreach {state val} [timer_get] {
            array set S $val
            puts [format $fmt $state $S(time)]
            unset S
        }
    }
    # -- print counters
    public method Report {} {
        set fmt1 "%-18s %8s"
        set fmt  "%-18s %8.2f"
        notice [format $fmt1 "State" "Time(sec)"]
        foreach {state val} [timer_get] {
            array set S $val
            notice [format $fmt $state $S(time)]
            unset S
        }
    }
    # -- get all counters
    public method Get {} {
        timer_get
    }
    public method Test {} {
        timer_test
    }
    # -- stop the timer
    public method Stop {} {
        # -- has not started?
        if { $STATE == "" } { return }
        # -- already stopped?
        if { $END($STATE) != "" } { return }
        set END($STATE) [timer_read]
    }
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable STATE ""             ;# current state
    protected variable START                ;# when the state was entered
    protected variable END                  ;# when the state changed
    protected variable STATES ""            ;# sequence of states
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method timer_read {} {
        clock clicks -millisec
    }
    # -- record a state change
    protected method timer_mark {state} {
        set START($state) [timer_read]
        Stop
        set END($state) ""
        set STATE $state
        lappend STATES $state
    }
    # -- collect the current timer state status
    protected method timer_get {} {
        # -- note the end of the last state
        Stop
        set res ""
        foreach state $STATES {
            lappend res $state [timer_diff $END($state) $START($state)]
        }
        # -- append the total time, if any
        set a [lindex $STATES 0]
        set b [lindex $STATES end]
        if { ($a != "") && ($b != "")} {
            lappend res total [timer_diff $END($b) $START($a)]
        }
        set res
    }
    # -- compute the "elapsed" resources
    protected method timer_diff {end start} {
        list time [expr ($end - $start)/1000.0]
    }
    protected method timer_test {} {
        Mark one
        after 1000
        Mark two
        after 2000
        Report
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
