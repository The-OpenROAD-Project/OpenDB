package provide ade::ui 2.0

#
#       Class: AdapterManager
# Description: adapter manager
#      Author: Manolis M. Tsangaris
#
# To manage visualizers of remote gui adapters
#
itcl::class AdapterManager {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -- handle adapter events
    public method Handle {class event args} {
	am_handle $class $event $args
    }
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable HANDLER
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method mkhandler {class {stylesheet ""}} {
	set cl UIViz_$class 

	if { [itcl::find classes $cl] == "" } {
	    puts stderr "WARNING: $class: no visualization class '$cl'"
	}
	set h [$cl $this.h_#auto -stylesheet $stylesheet]
	set HANDLER($class) $h
	set h
    }
    # ----------------------------------------------- Interface Implementation
    protected method am_handle {class event argv} {
	if ![info exists HANDLER($class)] {
	    mkhandler $class
	}
	switch $event {
	    <<newstates>> {
		eval $HANDLER($class) NewStates $argv
	    }
	    <<stylesheet>> {
		if [info exists HANDLER($class)] {
		    # -- make sure any previous window appears
		    # -- one of those days we should fix vMakeButtons
		    update idletasks ; update
		    # -- now it is safe to delete
		    $HANDLER($class) delete
		}
		mkhandler $class [lindex $argv 0]
	    }
	    default {
		puts "unknown adapter event: $argv"
	    }
	}
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
