package provide smpx 1.0


# Demo code for SMPX

#
#       Class: SMPXDemo
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class SMPXDemo {
    inherit LSMPXClient
    # -------------------------------------------------- Public Variables --
    public method Test {} {
	set reg 0
	set addr 1234@localhost 
	if [catch {d Register pt $addr} rc] {
	    puts "register: $rc"
	} else {
	    incr reg
	}
	foreach n {1 2 3} {
	    d Reserve pt
	    Wait S ; parray S
	    after 4000
	    puts [d Release pt $S(address)]
	}
	if { $reg } {
	    d UnRegister pt $addr
	}
    }
    # -- for debugging
    protected method d {args} {
        notice smpx-client "run: $args"
        set rc [uplevel $args]
        notice smpx-client $rc
        set rc
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
