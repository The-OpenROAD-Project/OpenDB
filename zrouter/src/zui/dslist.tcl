package provide ade::ui 2.0

#       Class: DSList
# Description: Display Data Sets
#      Author: Manolis M. Tsangaris
#
itcl::class DSList {
    inherit LTopLevel
    # -------------------------------------------------- Public Variables --
    public variable depth 10 ;# remember the $depth most recent datasets
    # -------------------------------------------------- Public Methods ----
    public method ShowDataSet {ds} {
	ds_add $ds
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable dslist ""
    protected variable PRIORITY         ;# priority
    protected variable DSET
    protected variable LIST ""          ;# list
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set dslist [listbox $This.lb -bg white -width 40 -height 20]
	pack $dslist -side top -expand 1 -fill both
	$this configure -title "ZUI: Data Sets"
    }
    destructor {
	destroy $dslist
    }
    # -------------------------------------------------- Private Methods ---
    # -- add a dataset
    protected method ds_add {ds} {
	set PRIORITY($ds) [clock seconds]
	set nlist ""
	set done 0
	foreach nds $LIST {
	    if { !$done && ($PRIORITY($ds) > $PRIORITY($nds)) } {
		lappend nlist $ds
		incr done
	    }
	    if { ($ds == $nds) } { continue }
	    lappend nlist $nds
	}
	if { !$done } { lappend nlist $ds }
	$dslist delete 0 end
	set LIST $nlist
	foreach ns $nlist {
	    $dslist insert end [$ns cget -info]
	}
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
