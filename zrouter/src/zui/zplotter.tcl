package provide ade::ui 2.0

#       Class: ZPlotter
# Description: Zui plotter
#      Author: Manolis M. Tsangaris
#
itcl::class ZPlotter {
    inherit LTopLevelEditor LBinder
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Plot {data {tag ""}} {
	if { $tag == "" } { set tag main }
	
	if ![info exists TOOL($tag)] {
	    mkplotter $tag $tag
	}
        $nb Highlight $tag
        $nb Raise $tag
	if { $data == "demo" } {
	    set data $EXAMPLE(demo1)
	}
	plot $tag $data 
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable TOOL
    protected variable nb
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        pack [mktabs $This.tabs] -side top -expand 1 -fill both
        wm title $This "ADE/UI: Plotter"
	mkplotter main Main
    }
    destructor {
        foreach t [array names TOOL] {
            $TOOL($t) delete
        }
        $nb delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mkplotter {tag title} {
	set wt [$nb Add $tag  $title].$tag
	LPlotter $wt -window 60
	set TOOL($tag) $wt
	pack $wt -side top -expand 1 -fill both
	$wt Bind <<select-canvas>> $this eSelectedCanvas $tag
    }
    protected method mktabs {w} {
        set nb [LSNotebook $w]
	set w
    }
    protected method show_to {tab text} {
        $nb Highlight $tab
        if { [string match +* $text] } {
            $TOOL($tab) Show [string range $text 1 end]
        } else {
            $TOOL($tab) Clear
            $TOOL($tab) Show $text
        }
    }

    protected method plot {tag descr} {
	$TOOL($tag) Clear
	set pl $TOOL($tag)
	foreach {a val} $descr {
	    switch $a {
		title  { $TOOL($tag) Configure configure -title $val }
		xtitle { $TOOL($tag) Configure axis config x -title $val }
		ytitle { $TOOL($tag) Configure axis config y -title $val }
		data {
		    foreach {dset values} $val {
			$TOOL($tag) AddSet $dset $dset
			foreach {x y} $values {
			    $TOOL($tag) Plot $dset $x $y
			}
		    }
		}
	    }
	}
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eSelectedCanvas {tag x y av} {
	array set A $av
	if { $A(name) == "" } { return }
	$TOOL($tag) Show "$A(name): $A(x) $A(y)"
    }
    # -------------------------------------------------- Common Initialization
    common EXAMPLE
    set EXAMPLE(demo1) {
	title "Demo plot"
	xtitle "time"
	ytitle "money"
	data {
	    set1 {
		1970 10
		1975 20
		1980 30
		1985 40
		1990 20
		1995 40
		2000 45
		2005 55
	    }
	    set2 {
		1970 13
		1975 23
		1980 20
		1985 10
		1990 24
		1995 43
		2000 48
		2005 39
	    }
	}
    }
}
