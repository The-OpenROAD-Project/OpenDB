package provide ade::ui 2.0

#
#       Class: ZHistory
# Description: Keep the ZUI history
#      Author: Manolis M. Tsangaris
#
itcl::class ZHistory {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # push history
    public method Push {state} {
        set STATE([incr NOW]) $state
        # erase any forward history
        for {set n [expr $NOW + 1]} { [info exists STATE($n)] } { incr n } {
            unset STATE($n)
        }
    }
    # -- go back to the history line
    public method Back {pp} {
        upvar $pp P
        incr NOW -1
        if ![info exists STATE($NOW)] {
            incr NOW
            return 0
        }
        array set P $STATE($NOW)
        return 1
    }
    # -- go forward to history line
    public method Next {pp} {
        upvar $pp P
        incr NOW
        if ![info exists STATE($NOW)] {
            incr NOW -1
            return 0
        }
        array set P $STATE($NOW)
        return 1
    }
    public method Clear {} {
        set NOW 0
        catch {unset STATE}
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable STATE
    protected variable NOW 0
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
