package provide ade::zui 2.0

#       Class: ZUISettingsDialog
# Description: Zui settings dialog
#      Author: Manolis M. Tsangaris
#
itcl::class ZUISettingsDialog {
    inherit LSettingsDialog
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

	configure -descr {
	    field balloon choice {list 0 No 1 Yes} Balloon "Enable Balloon Info"
	    field stream  choice {list 0 No 1 Yes} Streaming  "Enable Streaming"
	}
	configure -title "ZUI:Settings"
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method eValidate {values rr} {
	upvar $rr R
	puts values=$values
	return 1
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
