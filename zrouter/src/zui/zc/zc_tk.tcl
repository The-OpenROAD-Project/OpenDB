#
# BSD 3-Clause License
#
# Copyright (c) 2019, Nefelus Inc
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#
#       Class: TkZC
# Description: Tk implementation of ZC
#      Author: Manolis M. Tsangaris
#
# unless specified, all coordinates are world coordinates
itcl::class TkZC {
    inherit ZC
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Protected Variables 
    protected variable cv ""
    protected variable VIEWPORT             ;# viewport attributes
    protected variable VISIBLE              ;# visible types/layers/etc
    protected variable COLORMAP             ;# colormap
    protected variable SLICE                ;# keep track of all slices
    protected variable SCALE                ;# scaling parameters
    protected variable OBJECTCOUNT          ;# total number of object / type
    protected variable SEL                  ;# holds selection tags
    protected variable MARKS                ;# record all marks
    protected variable DLIST                ;# display list options
    protected variable SET                  ;# data sets loaded
    protected variable DSET                 ;# ZDS data sets loaded
    protected variable DSBBOX               ;# data set bbox
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	# -- make sure that the frame container has no border
	winconfig -border 0
	pack [set cv [canvas $This.zc]] -side top -expand 1 -fill both
	$cv configure -relief sunken -border 1 -bg black
	bind $cv <Configure> "$this eResize"
        # -- apply main canvas bindings
        $zcg BindWin $cv
	array set SET {}
	array set DSET {}
	array set DLIST {}
	array set VISIBLE {}
	array set COLORMAP {}

	array set SCALE "x0 0 y0 0"

	# -- notify that the canvas window is setup
	# -- allow parent to bind the event (at the end of constructor)
	after idle $this Event <<setup>>
    }
    # ----------------------------------------------- Private methods
    # -- add an object to the canvas
    # -- offset it by xr0 yr0
    private method tkzc_cad_object_add {tag ss xr0 yr0} {
	array set S $ss
        set tags ""
	set id $S(id)
        set bb "" ; set dd ""
        if [info exists S(bb)] { set bb $S(bb) ; unset S(bb) }
        if [info exists S(dd)] { set dd $S(dd) ; unset S(dd) }

        # -- add default empty mark if none exists
        if ![info exists S(mark)] { set S(mark) "" }

	if ![info exists OBJECTCOUNT($S(type))] {
	    set OBJECTCOUNT($S(type)) 1
	} else {
	    incr OBJECTCOUNT($S(type))
	}

        foreach t "layer hierarchy type mark" {
            lappend tags $t:$S($t)
        }
	# -- normalize tag, define the slice
	set slice $tags

	# -- keep track of all slices
	set SLICE($slice) ""

        if { $bb != "" } {
            foreach {x0 y0 x1 y1} $bb { break }
	    if { $xr0 || $yr0 } {
		set x0 [expr $x0 + $xr0] ; set y0 [expr $y0 + $xy0]
		set y0 [expr -$y0+$xr0] ; set y1 [expr -$y1+$yr0]
	    } else {
		set y0 [expr -$y0] ; set y1 [expr -$y1]
	    }
            # -- via has two additional diagonal lines
            switch $S(type) {
                via {
                    set n [$cv create rectangle $x0 $y0 $x1 $y1 -outline gray50]
                    set n1 [$cv create line $x0 $y0 $x1 $y1]
		    $cv itemconfigure $n1 -tags [list $slice]
                }
                default {
                    set n [$cv create rectangle $x0 $y0 $x1 $y1 -outline ""]
                }
            }
        }
        # -- draw a closed polygon
        if { $dd != "" } {
            set points ""
            foreach {x0 y0} $dd {
                lappend points $x0 [expr -$y0]
            }
            lappend points [lindex $dd 0] [expr -[lindex $dd 1]]
            set n [eval $cv create polygon $points]
        }
        $cv itemconfigure $n -tags [list $slice]
    }

    # -- apply visibility rules : need to apply F to SLICES
    # -- F contains attributes like layer:0 htype:a/b mark:foo
    public method tkzc_visibility {ff} {
	upvar $ff F
	foreach tag [array names F] {
	    foreach {t path} [split $tag :] { break }
	    if { $t == "htype" } {
		foreach {h t} [split $path /] { break }
		set k [list hierarchy:$h type:$t]
		set F($k) $F($tag)
		unset F($tag)
	    }
	}
	# -- collect all on/off/highlighted tags
        set ON "" ; set OFF "" ; set HI ""

	# -- filter all slices
	foreach slice [array names SLICE] {
	    # -- check if slice is visible
	    foreach {l h t m} $slice { break }
	    if { [tkzc_is_visible $l [list $h $t] $m F] } {
		lappend ON $slice
	    } else {
		lappend OFF $slice
	    }
	}
	# -- apply visibile
        foreach tag $ON  { $cv itemconfig $tag -state normal }

        # -- apply not visible
        foreach tag $OFF { $cv itemconfig $tag -state hidden }
    }
    # -- apply filters to a slice
    # -- a slice is visible if no one of its components has been turned off
    protected method tkzc_is_visible {layer key mark ff} {
	upvar $ff F
	if { ![info exists F($layer)] } {
	    return 0
	}
	if { !$F($layer) } {
	    return 0
	}
	if { ![info exists F($key)]} {
	    return 0
	}
	if { !$F($key) } {
	    return 0
	}
	# -- if no marks accept the slice
	if { ![info exists F($mark)] } { return 1 }

	# -- if empty mark, accept the slice
	if { $mark == "mark:" } { return 1}

	return  $F($mark)
    }

    # -- color all objects based on COLORMAP
    protected method tkzc_colorset {cc} {
	upvar $cc COLORMAP
        array set COLOR {}
	# -- at first, select the color of each slice
	foreach slice [array names SLICE] {
	    foreach tag $slice {
		if [info exists COLORMAP($tag)] {
		    set COLOR($slice) $COLORMAP($tag)
		    break
		}
	    }
	    if ![info exists COLOR($slice)] {
		set COLOR($slice) purple
	    }
	}
	foreach tag [array names COLOR] {
	    $cv itemconfigure $tag -fill $COLOR($tag)
	}
    }
    # -- rescale canvas to fit all data in it
    # -- if keepwbb is set, the bounding box will be used to scale objects. 
    # -- otherwise use the total object bbox
    protected method tkzc_fit2bbox {} {
	if ![info exists VIEWPORT(x0)] { return }

        # -- reset scaling coefficients
        array set SCALE [list x0 0 y0 0 sx 1.0 sy 1.0]
        set _width   [winfo width $cv] ; set _height  [winfo height $cv]

	# -- y-axis has been inverted
	set y0 [expr  -1 * $VIEWPORT(y0)]
	set y1 [expr  -1 * $VIEWPORT(y1)]
	# fix y0 y1 ...
	if { $y0 > $y1 } { swap y0 y1 }

	set x0 $VIEWPORT(x0)
	set x1 $VIEWPORT(x1)

        # -- first move objects to their origin
        set SCALE(x0) $x0 ; set SCALE(y0) $y0
        set dx [expr  - $x0]; set dy [expr  - $y0]
        $cv move all $dx $dy

        # compute scale to achieve auto-fit
        set sx [expr 1.0*($_width )/($x1 - $x0)]
        set sy [expr 1.0*($_height)/($y1 - $y0)]

        # -- pick the lowest scale, if we are to respect aspect ratio
        if { $aspect } {
            if { $sx > $sy } { set sx $sy }
            set sy $sx
        }
        set SCALE(sx) $sx ; set SCALE(sy) $sy
        $cv scale all 0 0 $sx $sy
        #notice "fit2bbox mapped  $x0 $y0 $x1 $y1 -> ${_width}x$_height"
        #notice "fit2bbox scaling $SCALE(x0) $SCALE(y0) $SCALE(sx) $SCALE(sy)"
    }

    # -- make a note of our bbox -- keep it in world coordinates
    protected method tkzc_bbox_set {bb} {
	set VIEWPORT(bb) $bb
        if { $bb == "" } {
            return
        }
        if {[scan $bb "%f %f %f %f" x0 y0 x1 y1] != 4 } {
            return
        }
	if { $x0 > $x1 } { swap x0 x1 }
	if { $y0 > $y1 } { swap y0 y1 }
	set VIEWPORT(bb) [list $x0 $y0 $x1 $y1]
        set VIEWPORT(x0) $x0 ; set VIEWPORT(x1) $x1
        set VIEWPORT(y0) $y0 ; set VIEWPORT(y1) $y1
    }
    # -- locate the object closest to x y but visible
    public method tkzc_locate {x y pp} {
        upvar $pp P
        set t [$cv find closest $x $y 0]
        if { $t == "" } { return 0 }
        set nt $t
        while 1 {
            if { [$cv itemcget $nt -state] != "hidden" } {
                set sel 0
                foreach tg [$cv gettags $nt] {
                    if { $tg == "selection" } {
                        incr sel ; break
                    }
                }
                if { !$sel } { break }
            }
            set nt [$cv find closest $x $y 0 $nt]
            if { $nt == $t } { return 0 }
        }
        set P(id) $nt
	set P(tags) [$cv gettags $nt]
	return 1
    }

    protected method tkzc_selection_draw {} {
	foreach tag [array names SEL] {
	    foreach sel $SEL($tag) {
		foreach {x0 y0 x1 y1} $sel { break } 
		zc_world2canvas $x0 $y0 X0 Y0
		zc_world2canvas $x1 $y1 X1 Y1
		$cv create rectangle $X0 $Y0 $X1 $Y1 \
		    -outline yellow -tags sel:$tag -width 2
	    }
	}
    }
    # ----------------------------------------------- Interface Implementation
    protected method zc_clear {tags} {
	if { ($tags == "all") || ($tags == "" ) } {
	    set tags [array names DLIST]
	}
	foreach tag $tags {
	    $cv delete $tag
	    if [info exists MARK($tag)]  { unset MARK($tag) }
	    if [info exists DLIST($tag)] { unset DLIST($tag) }
	    if [info exists DSET($tag)]  {
		$DSET($tag) Release
		unset DSET($tag)
	    } elseif [info exists SET($tag)] {
		unset SET($tag)
	    }
	}
    }

    protected method zc_bbox_set {bbox} {
	tkzc_bbox_set $bbox
	IdleRefresh
    }
    protected method zc_colors_set {colorlist} {
	array set COLORMAP $colorlist
	IdleRefresh
    }
    protected method zc_visibility_set {attrlist} {
	catch {unset VISIBLE}
	array set VISIBLE $attrlist
	IdleRefresh
    }

    # -- refresh the display
    protected method zc_refresh {} {
	$cv delete all
	# -- draw the data sets
        foreach tag [array names DLIST] {
	    # -- is there a zdataset?
	    if [info exists DSET($tag)] {
		set objects [$DSET($tag) Get]
	    } elseif [info exists SET($tag)] {
		set objects $SET($tag)
	    } else {
		continue
	    }

	    # -- retrieve the offsets
	    set x0 0 ; set y0 0
	    foreach {x0 y0} $DLIST($tag) { break }

            foreach obj $objects {
                if [catch {array set S $obj} rc] {
		    continue
		}
		tkzc_cad_object_add $S(id) $obj $x0 $y0
		unset S
	    }
	}
	# -- apply visibility
	tkzc_visibility VISIBLE

	# -- apply colors
	tkzc_colorset COLORMAP

	# -- fit objects to bbox
	tkzc_fit2bbox

	# -- draw selection (if any)
	tkzc_selection_draw
    }

    # -- add a dataset to the display list
    protected method zc_render {tags x y} {
	if { $tags == "all" } {
	    set tags [array get DLIST]
	}
	foreach tag $tags {
	    set DLIST($tag) [list $x $y]
	}
	#IdleRefresh
    }
    # -- render a dataset. Keep its tag around
    protected method zc_render_dset {dset x y} {
	set tag [$dset cget -tag]
	set DLIST($tag) [list $x $y]

	# -- handle the case the DSET is already there
	if { ![info exists DSET($tag)] || ($DSET($tag) != $dset) } {
	    set DSET($tag) $dset
	    $DSET($tag) Use
	}
	#IdleRefresh
    }

    # -- remove a dataset
    protected method zc_unrender {tag} {
	unset DLIST($tag)
	$cv delete cad:$tag
	if [info exists DSET($tag)] {
	    $DSET($tag) Release
	    unset DSET($tag)
	}
	if [info exists SET($tag)] {
	    unset SET($tag)
	}
    }

    # -- return all slices so far
    protected method zc_slices {} {
	array names SLICE
    }

    # -- return canvas window
    protected method zc_canvas_window {} { set cv }
    # -------------------------------------------------------------- images
    protected method zc_image_add {tag image x y x1 y1} {
    }
    protected method zc_image_add_stream {tag channel x y x1 y1} {
    }
    # -------------------------------------------------------------- selection
    protected method zc_selection_add {tag x0 y0 x1 y1} {
	lappend SEL($tag) [list $x0 $y0 $x1 $y1]
    }

    protected method zc_selection_clear {tags} {
        if { $tags == "all" } {
	    set tags [array names SEL]
	}
	foreach tag $tags {
	    $cv delete sel:$tag
	    catch {unset SEL($tag)}
	}
        return 1
    }
    # -------------------------------------------------------------- stats
    protected method zc_stats {} {
	array set S "slices [array size SLICE]"
	array set S [array get OBJECTCOUNT]
	set sum 0
	foreach n [array names OBJECTCOUNT] {
	    set sum [expr $sum + $OBJECTCOUNT($n)]
	}
	set S(total) $sum
	array get S
    }
    # -- convert canvas coordinates to problem coordinates
    protected method zc_canvas2world {X Y xx yy} {
        upvar $xx x
        upvar $yy y

	# -- it is possible to run this before sx has been established
	if ![info exists SCALE(sx)] {
	    set SCALE(sx) 1
	    set SCALE(sy) 1
	}

        # X = (x - x0) * sx => x = X/sx + x0
        set x [expr $X /$SCALE(sx) + $SCALE(x0)]

        # Y = (y + y0) * sy => y = Y/sy + y0
        set y [expr $Y /$SCALE(sy) + $SCALE(y0)]

	# -- due to inversion
        set y [expr -1.0 * $y]
    }

    # --- convert world to canvas coordinates
    protected method zc_world2canvas {x y XX YY} {
        upvar $XX X
        upvar $YY Y

        # X = (x - x0) * sx
        set X [expr ($x - $SCALE(x0))* $SCALE(sx)]

        # Y = (y - y0) * sy
	# -- invert
        set y [expr -1.0 * $y]
        set Y [expr ($y - $SCALE(y0))* $SCALE(sy)]
    }

    # -- move the canvas by dx dy world coordinates
    protected method zc_move {dx dy} {
    }

    # -------------------------------------------------------------- datasets
    # -- clear data sets
    protected method zc_objects_clear {tags} {
	if { $tags == "all" } { set tags [array names SET] }
	foreach tag $tags {
	    if ![info exists SET($tag)] { continue }
	    unset SET($tag)
	    unset DSBBOX($tag)
	    unset MARKS($tag)
	}
    }

    # -- add a list of objects
    protected method zc_cad_objects_add {tag objlist} {
	if ![info exists SET($tag)] { set SET($tag) "" }
	array set M {}
	foreach obj $objlist {
	    if [catch {array set S $obj} rc] {
		warning "add_objects: $rc: $obj"
		continue
	    }
	    lappend SET($tag) $obj
	    if [info exists S(mark)] { foreach m $S(mark) { set M($m) "" } }
	    unset S
	}
	set DSBBOX($tag) ""
	set MARKS($tag) [array names M]
    }

    # -- add a stream of objects
    protected method zc_cad_object_add_stream {tag channel} {
	if ![info exists SET($tag)] { set SET($tag) "" }
	array set M {}
	while { [gets $channel line] != -1 } {
	    if { $line == "." } { break }
	    if { $line == "" } { continue }
	    set S(mark) ""
	    foreach {S(layer) S(hierarchy) S(type) x0 y0 dx dy} $line {
		break
	    }
	    if ![info exists S(layer)] {
		warning "add_stream: ignoring <$line>"
		continue
	    }
	    set S(id) 0
	    set S(bb) [list $x0 $y0 [expr $x0 + $dx] [expr $y0 + $dy]]
	    lappend SET($tag) [array get S]
	    foreach m $S(mark) { set M($m) "" }
	    unset S
	}
	set DSBBOX($tag) ""
	set MARKS($tag) [array names M]
    }

    protected method zc_bbox {tag} {
	if {[info exists DSBBOX($tag)] && ($DSBBOX($tag) != "") } {
	    return $DSBBOX($tag)
	}
	if [info exists DSET($tag)] {
	    set DSBBOX($tag) [$DSET($tag) BBox]
	    return $DSBBOX($tag)
	}
	set ax0 ""
	if ![info exists SET($tag)] {
	    puts stderr "******* OOPS! set $tag does not exist"
	    return ""
	}
	foreach obj $SET($tag) {
	    array set S $obj
	    foreach {x0 y0 x1 y1} $S(bb) { break }
	    if { $x0 > $x1 } { swap x0 x1 }
	    if { $y0 > $y1 } { swap y0 y1 }
	    if { $ax0 == "" } { 
		foreach {ax0 ay0 ax1 ay1} [list $x0 $y0 $x1 $y1] { break }
	    } else {
		if { $x0 < $ax0 } { set ax0 $x0 }
		if { $y0 < $ay0 } { set ay0 $y0 }
		if { $x1 > $ax1 } { set ax1 $x1 }
		if { $y1 > $ay1 } { set ay1 $y1 }
	    }
	    unset S
	}
	set DSBBOX($tag) [list $ax0 $ay0 $ax1 $ay1]
    }
    protected method zc_dlist {} {
	array names DLIST
    }
    # -------------------------------------------------------------- marks

    # -- return all marks of a dataset (or all datasets)
    protected method zc_marks {tags} {
	if { $tags == "all" } {
	    set tags [array names SET]
	}
	array set M {}
	foreach tag $tags { 
	    if ![info exists MARKS($tag)] { continue }
	    foreach m $MARKS($tag) { set M($m) "" }
	}
	array names M
    }

    # -- turn on or off marks
    protected method tkzc_marks_view {mm on off hi} {
        array set M $mm
	upvar $on ON ; upvar $off OFF; upvar $hi HI
        # -- collect all visibile/invisible/highlighted tags
        foreach mark [array names M] {
            switch $M($mark) {
                0 { lappend OFF mark:$mark }
                1 { lappend ON  mark:$mark }
                2 { lappend HI  mark:$mark }
            }
        }
    }
    # ----------------------------------------------- CallBacks
    # -- dragged
    public method eDraggingRegion {dx dy} {
	$cv move all $dx $dy
    }
    # -- our window has been resized
    public method eResize {} {
        set nheight [winfo height $cv]
        set nwidth [winfo width $cv]
        # -- has the size really changed?
        if { ($width == $nwidth) && ($height == $nheight) }  {
            return
        }
        set width $nwidth ; set height $nheight
	IdleRefresh
    }
    destructor { }
}
