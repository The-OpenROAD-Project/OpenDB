package require Itcl
package provide mtlib 1.0

#
# Semaphore implements a simple semaphore class, with two styles of
# usage:
#       Signal/Wait, as in the traditional usage
#       Wait timeout, to wait a specified number of deciseconds
#
#       Class: Semaphore
# Description: Simple Semaphore implementation
#      Author: Manolis M. Tsangaris -- mt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2001
#
itcl::class LSemaphore {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    method Wait {{delay ""}} {
        if { $delay == "" } {
            ::s_wait $this
            return 0
        }
        set task [after $delay $this Signal]
        ::s_wait $this
        return 0
    }
    method Signal {} {
        # -- cancel any pending timeout
        if { $task != "" } { after cancel $task ; set task "" }
        ::s_signal $this
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable task ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        uplevel #0 set gSema($this) 0
    }
    destructor {
        if { $task != "" } { after cancel $task }
        global gSema
        if $gSema($this)  {
            ::s_signal $this
        }
        unset gSema($this)
    }
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- Common Initialization
}

set gSemaphore 0

proc s_wait {this} {
    global gSema
    global gSemaphore
    if ![info exists gSema($this)] { return }
    while { $gSema($this) == 0 } {
        vwait gSemaphore
        if ![info exists gSema($this)] { return }
    }
    incr gSema($this) -1
}
proc s_signal {this} {
    global gSema
    global gSemaphore
    if ![info exists gSema($this)] { return }
    incr gSema($this)
    incr gSemaphore
}

itcl::class Semaphore {
    inherit LSemaphore
}
