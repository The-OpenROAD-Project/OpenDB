package provide ade::ui 2.0

#       Class: CPanel
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class CPanel {
    inherit LFrame LBinder
    #  Event <<select-filter>> filter     (toggle a filter field)
    #  Event <<global-set-filter>> filter (set all filter fields to 1)
    # -------------------------------------------------- Public Variables --
    public variable bbox ""
    public variable colors ""
    public variable rpc ""
    # -------------------------------------------------- Public Methods ----
    # -- show the filters defined
    public method ShowFilters {expr} {
        filter_set $expr
        selection_init
    }
    public method Selection {} {
        array get SELECTION
    }
    public method LoadBV {tiles} {
        $bview LoadTiles $tiles
    }
    public method SelectionSet {args} {
        eval $bview SelectionSet $args 
    }
    public method Monitor {} {
        $MV Monitor
    }

    # -- set a filter to a value, no events will be produced
    public method Set! {filters} {
        foreach {filter fv} $filters {
            foreach {a v} $fv {
                # skip the root node
                if { $a == "." } { continue }
                $FILTER($filter) Select! $a $v
            }
        }
    }

    public method SelectAll {} {
        $bview SelectAll
    }

    # -- add a bookmark
    public method BookMarkAdd {bid bbox} {
	$bview BookMarkAdd $bid $bbox
    }
    # -- show a data set on the overview
    public method ShowDataSet {ds} {
	$bview ShowDataSet $ds
    }

    # -- show status
    public method ShowStatus {txt} {
	$wstatus configure -text $txt
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable bview
    protected variable wfilt
    protected variable FILTER
    protected variable MV                  ;# multiprocessing viewer
    protected variable wstatus             ;# status viewer (cursor coords)
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        winconfig -border 1 -relief sunken
        set bview [BView $This.bview -width 180 -height 140]
        pack $This.bview -side top  -fill both
	set wstatus [label $This.status -border 1 -relief sunken]
	pack $wstatus -side top -fill x

        $bview Redirect <<selected-region>> $this 
        $bview Redirect <<bookmark-add>>  $this
        $bview Redirect <<bookmark-goto>> $this
        $bview Redirect <<bookmark-delete>> $this
        $bview Redirect <<cursor>> $this

        LBridge bbox "[namespace current]::$bview configure -bbox"

	mktabs $This.tabs

        selection_init
    }
    # -- add tools to tabs
    protected method mktabs {w} {
        set nb [LSNotebook $w]

        set wt [$nb Add filters Filters].filters
        pack [mk_filters $wt] -fill both -expand 1 -side bottom

        set wt [$nb Add multi Farm].multi
        set MV [MSHViewer $wt]
        pack $wt -side top

        # get all events
        $MV Redirect <<task-info>> $this
        LBridge rpc "[namespace current]::$MV configure -rpc"

	pack $w -side top -fill both -expand 1
    }
    destructor {
        $bview delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mk_filters {w} {
        frame $w -class Filter  -border 1 -relief sunken
        set wfilt $w.f
        pack [frame $wfilt] -side top -fill both 
        set w
    }

    # ----------------------------------------------- Interface Implementation
    protected method filter_set {filters} {
        foreach flt [array names FILTER] {
            $FILTER($flt) delete
        }
        destroy $wfilt
        pack [frame $wfilt -padx 5 -pady 5 ] -side top -fill both 
        set N 0
        foreach {filter flname fdef} $filters {
            set w $wfilt.f$N
            label $w-l -text $flname
	    bind $w-l <1> "$this eFilterGlobalSet $filter"

            set FILTER($filter) [LSTree $w -terminals 1 -timeout 800]
            if { $filter == "layer" } {
		foreach {t c} $colors {
		    set C([lindex [split $t :] 1]) $c
		}
                $FILTER($filter) configure -colors [array get C]
            }
            grid $w-l -padx 1 -pady 2 -sticky ew -column $N -row 0
            grid $w   -padx 1 -pady 2 -sticky nsew -column $N -row 1

            foreach {f fname val} $fdef {
                $w Add $f $fname
                $w Select $f $val
            }
            $w Bind <<selected>> $this eFilterSet $filter
            incr N
        }
    }
    protected  method selection_init {} {
        array set SELECTION {x0 0 y0 0}
        set SELECTION(x1) [$bview cget -width]
        set SELECTION(y1) [$bview cget -height]
        set SELECTION(filters) [array get FILTER]
    }
    # ----------------------------------------------- events
    public method eFilterSet {filter value} {
        set res ""
        foreach f [array names FILTER] {
            lappend res $f [$FILTER($f) Get]
        }
        Event <<select-filter>> $res
    }
    # -- called when one of the filter types is hit
    # -- it will set all filter fields to 1 and recompute the new value
    # -- of the filters 
    public method eFilterGlobalSet {filter} {
        set res ""
        foreach tag [array names FILTER] {
	    if { $tag != "$filter" } {
		lappend res $tag [$FILTER($tag) Get]
		continue
	    }
	    # -- set all fields
	    set r ""
	    foreach {field _} [$FILTER($filter) Get] {
		lappend r $field 1
	    }
	    lappend res $filter $r
        }
        Event <<global-set-filter>> $res
    }
    # -------------------------------------------------- Common Initialization
}
