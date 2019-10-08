#
# -- provide platform dependent support
package provide ade::platform 1.0

namespace eval ::platform {
    variable STREAMING 0        ;# should we stream (used by cmd.tc)
    variable LICENSING oui      ;# should we ask for licenses
    variable ZC_TK 0            ;# can we use zc/tk only?

# -- called by prog.tcl
proc init {} {
    variable STREAMING
    variable LICENSING
    variable ZC_TK

    switch $::tcl_platform(platform) {
	windows {
	    # -- no streaming in windows
	    set stream 0
	    set STREAMING 0
	    set LICENSING nono
	    set ZC_TK 1
	}
	unix {
	    set STREAMING 1
	    set LICENSING oui
	    set ZC_TK 0
	}
    }
    global env
    if [info exists env(ADS_NO_LICENSE)] {
	set LICENSING nono
    }
    # -- set the tcl math precision
    global tcl_precision
    set tcl_precision 17
}

proc licensing {} {
    variable LICENSING
    if { $LICENSING == "nono" } { return 0 }
    return 1
}
# -- return 32/64 bit mode
proc getBits {} {
    if { [catch {::sys::getBits} rc] } {
	return 32
    }
    return $rc
}
}
