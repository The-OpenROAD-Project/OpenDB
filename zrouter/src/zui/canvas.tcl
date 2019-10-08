package provide ade::ui 2.0

#
#       Class: CCanvas
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class CCanvas {
    inherit LFrame LBinder
    # event 
    # Event <<touch>> button x y
    # Event <<progress>> percent
    # Event <<selected-region>> x0 y0 x1 y1 reason
    # Event <<selecting-region>> x0 y0 x1 y1
    # Event <<message>> msg
    # Event <<key>>     key
    # Event <<cursor>>  key
    # Event <<gesture-start>>
    # Event <<gesture-end>>
    # -------------------------------------------------- Public Variables --
    public variable colorize type
    #public variable layers ""
    public variable colors "" { 
	array set COLOR $colors
    }
    public variable bbox ""   { 
	if { $bbox != "" } {
	    $zc   BBoxSet $bbox
	}
    }
    public variable filters "" ; # filters used for this data set
    public variable invy 0
    public variable deltax 1 ;# selection increment - x dir
    public variable deltay 1 ;# selection increment - y dir
    public variable selcolor yellow
    public variable keepwbb 0   ;# should we keep world bb on autofit?

    public variable opts "" { array set OPT $opts } 
    # -------------------------------------------------- Public Methods ----
    public method Clear {} {
	$zc Clear all
        set SELECTED ""
        catch {unset ID}
        catch {unset BG}
        catch {unset FILTER}
    }
    # select an object : return 1 if the object exists
    public method Select {tag attrs} {
	if { $tag == "" } { 
	    SelectionClear
	    $zc IdleRefresh
	    return 0
	}
	array set A $attrs
	# -- for now, draw selected objects using the lasso selection
	eval $zc AddSelection sel $A(bb)
	$zc IdleRefresh
	return 1
    }
    public method SelectionClear {} {
	$zc ClearSelection all
    }
    # -- apply filters
    # -- it is possible that our dataset does not have all the objects
    # -- filtered. In that case, we will return a list of missing object types
    public method Visible {nfilters} {
	cv_visible $nfilters
    }
    # -- return all marks found
    public method Marks {tag} {
	set out ""
	foreach m [$zc Marks $tag] {
	    lappend out $m $m
	}
	set out
    }
    # -- show objects based on their marks
    # -- if Mark=0 do not show
    # -- if Mark=1 show normal
    # -- if Mark=2 show highlighted
    public method ViewMarks {marks} {
	set FILTER(marks) ""
	foreach {mark val} $marks {
	    lappend FILTER(marks) mark:$mark $val
	}
	# -- apply filter to the canvas view
	cv_filter_apply FILTER
    }

    # -- add one object to the canvas
    public method AddObj {tag ss} {
        upvar $ss S
	$zc AddCADObjects $tag [list [array get S]]

	$zc Render $tag
    }
    # -- add a list of objects to canvas
    public method AddObjects {tag objects {stream ""}} {
	cv_load_objects $tag $objects $stream
    }

    # -- add a data set
    public method AddDSet {dset} {
	cv_load_dset $dset
    }


    # -- Draw & Connect Objects with start (or as a chain)
    public method Group {start list {chain 0}} {
        $zc Group $start $list $chain
    }

    # return the cursor region at x y, in real coordinates
    public method CursorRegion {x y} {
        $zc Canvas2World [expr $x - 1] [expr $y - 1] x0 y0 
	$zc Canvas2World [expr $x + 1] [expr $y + 1] x1 y1
	list $x0 $y0 $x1 $y1
    }

    # return the pixel grid size in world coordinates
    # rows and columns represent how many pixels we want to consider 
    # for the grid
    public method PixelGrid {rows cols x0 y0 x1 y1} {
        $zc PixelGrid $rows $cols $x0 $y0 $x1 $y1
    }

    # -- define the rubber band selection box
    public method SetSelectionBox {x0 y0 x1 y1} {
	$zc ClearSelection all
	$zc AddSelection sel $x0 $y0 $x1 $y1
    }

    # -- return the real bbox of the objects drawn
    public method GetBBox {{tag all}} {
	$zc BBoxGet
    }
    public method Dump {} {
        set im [image create photo]
        update idletasks ; update
        $im configure -format window -data [$zc Window]
        set im
    } 
    public method Zoom {mode} {
	$zc Zoom $mode
    }
    # Return the Full BBox of the display list
    public method FullBBox {} {
	$zc FullBBox
    }

    # -- return the canvas window
    public method CanvasWindow {} {
	$zc CanvasWindow
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable OPT               ;# options
    protected variable cmd
    protected variable COLOR
    protected variable N 0
    protected variable ID
    protected variable FILTER
    protected variable zc

    # -- background of selected items
    protected variable BG

    # -- keep statistics
    protected variable total0 0
    protected variable NObjects 0
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
       eval configure $args
       zc_make $This.zc
    }

    destructor {
	$zc delete
    }
    # -------------------------------------------------- Private Methods ---
    private method zc_make {w} {
	package require ade::ui::zc 1.0
	if { $OPT(tk) } {
	    set zc [TkZC $w -width 400 -height 400 -keepwbb $keepwbb]
	} else {
	    package require ade::ui::zc_qt 1.0
	    set zc [QTZC $w -width 400 -height 400 -keepwbb $keepwbb]
	}
	$zc configure -automark $OPT(automark)
	pack $w -side top -expand 1 -fill both
	$zc winconfig -border 1 -relief sunken

	switch -- @$OPT(zcdebug) {
	    @all {
		$zc Debug qt.paint
		$zc Debug qt.refresh
	    }
	    @? {
		puts stderr "use: all qt, qt.paint, qt.refresh ..."
	    }
	    @ { }
	    default {
		foreach tag $OPT(zcdebug) { 
		    $zc Debug $tag
		}
	    }
	}

	$zc Redirect <<selecting-region>> $this
	$zc Redirect <<selected-region>>  $this
	$zc Redirect <<touch>>            $this
	$zc Redirect <<gesture-start>>    $this
	$zc Redirect <<gesture-end>>      $this
	$zc Redirect <<key>>              $this
	$zc Redirect <<cursor>>           $this
	$zc Redirect <<enter>>            $this
	$zc Redirect <<leave>>            $this

	$zc Bind     <<setup>>            $this eCanvasSetup
	$zc Bind     <<stats>>            $this eCanvasStats
	$zc Bind     <<debug>>            $this eCanvasDebug

	return $w
    }

    # -- apply current filter to the canvas view
    protected method cv_filter_apply {ff} {
	upvar $ff F
	set VALL ""
	# -- merge all filters
	foreach tag [array names F] {
	    foreach {a v} $F($tag) {
		lappend VALL $a $v
	    }
	}
	$zc VisibilitySet $VALL
    }

    protected method cv_visible {nfilters} {
        array set F $nfilters
        set ON "" ; set OFF ""
	array set VIS {}
        # -- collect visible and invisible tags
        foreach attr [array names F] {
            foreach {value visibility} $F($attr) {
                if { $visibility } {
		    set VIS($attr:$value) 1
                    lappend ON $attr:$value
                } else {
		    set VIS($attr:$value) 0
                    lappend OFF $attr:$value
                }
            }
        }
	set FILTER(layers) ""
	set FILTER(hierarchy) ""
	set FILTER(type) ""
	set FILTER(htype) ""

	foreach a [array names VIS] {
	    switch -glob -- $a {
		layer:. { continue }
		type:.      { continue }
		hierarchy:. { continue }

		layer:* {
		    lappend  FILTER(layers) $a $VIS($a)
		    continue
		}
		type:* {
		    set vis $VIS($a)
		    foreach {_ npath} [split $a :] { break }
		    set path [split $npath /]
		    set h [lindex $path 0]
		    set t [lindex $path 1]
		    if { ($h == "") || ($t == "") }  { continue }
		    lappend FILTER(htype) htype:$npath $vis
		}
	    }
	}
	cv_filter_apply FILTER
    }

    # -- load objects to the canvas
    protected method cv_load_objects {tag objects stream} {
	set t0 [clock clicks -milliseconds]
	if { $objects == "-"  } {
	    $zc AddCADObjectStream $tag $stream
	} else {
	    $zc AddCADObjects $tag $objects
	}
	set t1 [clock clicks -milliseconds]
	set dt [expr 1.0*($t1 - $t0)/1000.0]
	if { $dt == 0 } { set dt 1 }
	array set S [$zc Stats]
	set N [expr $S(total) - $total0] 

	set total0 $S(total)
	set NObjects $N

	report_rate loaded $dt

	$zc Render $tag
	# -- if bbox is not defined, compute one based on the
	# -- the bbox of all objects loaded in
        if { $bbox == "" } {
	    configure -bbox [FullBBox]
        }
    }
    # -- render a data set into the canvas
    # -- eventually, it will obsolete cv_load_objects
    protected method cv_load_dset {dset} {
	$zc RenderDSet $dset

	# -- if bbox is not defined, compute one based on the
	# -- the bbox of all objects loaded in

        if { $bbox == "" } {
	    configure -bbox [FullBBox]
        }
    }
    # -------------------------------------------------------------------------
    protected method timeit {verb name N args} {
        set now [clock clicks -milliseconds]
        uplevel $args
        set dt [expr [clock clicks -milliseconds] - $now]
	if { $dt == 0 } { set dt 1 }
        set msg [format "%s %d %s in %.2f seconds %.0f %s/sec" \
		$verb $N $name [expr $dt/1000.0] [expr 1000.0*$N/ $dt] $name]
	Event <<message>> $msg
    }
    proc warning {msg} {
        ::warning canvas $msg
    }
    proc notice {msg} {
        ::notice canvas $msg
    }
    # -- post a message event about the objects just loaded/rendered
    protected method report_rate {verb dt {prefix ""}} {
	if { $NObjects } {
	    if { $dt == 0 } { set dt 1 }
	    set msg [format "%s: %.1fK obj, %.1f K/sec - %.0fs" \
		$verb [expr $NObjects/1000.0]\
		[expr $NObjects/$dt/1000.0] $dt]
	} else {
	    set msg "$verb: no objects ($dt seconds)"
	}

	Event <<message>> "$prefix$msg"
    }
    # -------------------------------------------------- Callbacks
    public method eCanvasSetup {} {
       $zc ColorsSet [array get COLOR]
    }
    # -- debugging for streaming
    public method eStream {fp} {
	while { [gets $fp line] != -1 } {
	    if { $line == "." } { break }
	    puts $line
	}
    }
    # -- statistics
    public method eCanvasStats {mod args} {
	array set S $args
	report_rate rendered $S(time) +
    }
    public method eCanvasDebug {tag msg} {
	puts "**** $tag $msg"
    }

    public method ePanelBBox { bb } {
        if { !$OPT(tk) } {
            $zc setChipBBox $bb
        }
    }

    # -------------------------------------------------- Common Initialization
    proc fixme {args} {
	puts stderr "FIX: $args"
	return ""
    }
}
