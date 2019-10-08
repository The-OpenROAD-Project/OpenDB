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

package provide ade::ui::zc 1.0
# -- no need for package require. See ../pkgIndex.tcl

#
#       Class: QTZC
# Description: QT implementation of ZC
#      Author: Mattias Hembruch
#
# unless specified, all coordinates are world coordinates

itcl::class QTZC {
    inherit ZC
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods
    # -------------------------------------------------- Protected Variables 
    protected variable prefix  "::"              ;# where the .so is loaded
    protected variable mname   "tkqt"       ;# module name
    protected variable qt      "::tkqt"     ;# full access name

    protected variable DLIST                 ;# list of tags to display
    protected variable OLIST                 ;# dataset list
    private   variable ILIST                 ;# image list
    protected variable SEL                   ;# SELECTION
    private   variable BBOX                  ;# bbox

    protected variable WID   0               ;# internal window ID used by qt
    protected variable WIN ""                ;# canvas window

    protected variable CHANGED               ;# remember what has changed
    protected variable DSET                  ;# datasets being rendered
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    
        # -- create a frame to put qt window (make it blue for fun)
        set WIN "$This.cv"
	# -- "" means to tk not to update the color
	# -- 0 border so the qt window has the same size as the frame
        frame $WIN -bg "" -border 0
        pack $WIN -side top -expand 1 -fill both

        # -- defer the creation of qt until the window has been exposed
        bind $WIN <Expose> "$this eCreate"
        array set DLIST {}
        array set DSET {}
        zc_debug_set qt "tkqt calls"
    }
    destructor { }
    # ----------------------------------------------- Private methods
    protected method qt {cmd args} {
        dprint qt.$cmd "$args"
        set rc [eval ${qt}::$cmd $args]
        dprint qt.$cmd "=$rc"
        return $rc
    }

    # -- locate the object closest to x y but visible
    public method qtzc_locate {x y pp} {
        fixme "locate: $x, $y, $pp"
    }

    protected method qtzc_tags {type} {
        set nt [qt getTagSize $type]
        set out ""
        for { set i 0 } { $i < $nt } { incr i } {
	   set m [qt getTagName $type $i]
	   if { $m == "" } { continue }
           lappend out $m
        }
        set out
    }
    protected method changed {tag} {
	set CHANGED($tag) 1
    }
    # ----------------------------------------------- Interface Implementation
    # -- clear the display
    protected method zc_clear {ntag} {
	if { $ntag == "all" } {
	    set tags [zc_dlist]
	} else {
	    set tags $ntag
	}
	foreach tag $tags {
	    if [info exists ILIST($tag)] {
		qt eraseImage $tag
		unset ILIST($tag)
		continue
	    }
	    if [info exists OLIST($tag)] {
		qt eraseDataset $tag
		unset OLIST($tag)
	    }
	    if [info exists DSET($tag)] {
		$DSET($tag) Release
		unset DSET($tag)
	    }
	    # -- eliminate this tag from the display list
	    if [info exists DLIST($tag)] {
		unset DLIST($tag)
	    }
	}
    }

    # -- set the bbox
    protected method zc_bbox_set {bbox} {
	changed main
	if { $bbox == "" } { return }
        foreach {x0 y0 x1 y1} $bbox { break }
        if { $x0 > $x1 } { swap x0 x1 }
        if { $y0 > $y1 } { swap y0 y1 }
        array set BBOX [list x0 $x0 x1 $x1 y0 $y0 y1 $y1]

        set w [expr {$x1 - $x0}]
        set h [expr {$y1 - $y0}]
        foreach a {x0 y0 w h} {
            set $a [expr int([set $a])]
        }
        set BBOX(w) $w
        set BBOX(h) $h

        #puts "new BBOX: "
        #parray BBOX
	# -- reset any remaining scolling offset
        qt resetScroll $WID
        qt setBBox $WID $x0 $y0 $w $h
    }

    # -- return the bbox of all objects
    public method zc_bbox_get {} {
        if [array size BBOX] {
            list $BBOX(x0) $BBOX(y0) $BBOX(x1) $BBOX(y1)
        } else {
            return ""
        }
    }

    protected method zc_colors_set {colorlist} {
        changed main
        qt selectColormap cmap_$WID
        foreach {layer color} $colorlist {
            foreach {dummy l} [split $layer ":"] { break; }
            qt setBrush $WID $l $color
            qt setPen   $WID $l $color
        } 

        qt setBrush $WID 11 red
        qt setPen $WID 11 red

        #zc_specialColors_set $colorlist
        zc_specialColors_set {layer:0 white layer:1 yellow layer:2 red}
    }

    public method zc_specialColors_set { colorlist } {
        changed main
        qt selectColormap cmap_s$WID
        foreach {layer color} $colorlist {
            foreach {dummy l} [split $layer ":"] { break; }
            qt setBrush $WID $l $color
            qt setPen   $WID $l $color
        } 
    }

    protected method zc_visibility_set {attrlist} {
        changed main
        foreach {obj status} $attrlist {
            set nl [split $obj ":"]
            foreach { cat type } $nl {
                switch $cat {
                    layer {
                        qt setLayerStatus $WID $type $status
                    }
                    htype - type - hierarchy {
                        qt setTagStatus $cat $type $status
                    }
		    mark {
			foreach set [array names OLIST] {
			    qt setTagStatus marks-$set $type $status
			}
			foreach set [array names DSET] {
			    qt setTagStatus marks-$set $type $status
			}
		    }
                    default { warning "unknown tag: $cat" }
                }
            }
        }
	# -- need to refresh eventually
	IdleRefresh
    }

    # -- render
    protected method zc_render {tag x y} {
	changed main
        set DLIST($tag) [list $x $y]
        IdleRefresh
    }
    protected method zc_unrender {tag} {
	changed main
        if { $tag == "all" } {
            set tags [array names DLIST]
        }
        foreach tag $tags {
            unset DLIST($tag)
	    if [info exists DSET($tag)] {
		$DSET($tag) Release
		unset DSET($tag)
	    }
	    if [info exists OLIST($tag)] {
		qt eraseDataset $tag
	    }
	    if [info exists ILIST($tag)] {
		qt eraseImage $tag
	    }
        }
	# -- need to refresh eventually
        IdleRefresh
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
        IdleRefresh
    }

    # -- refresh the display (render again all datasets)
    protected method zc_refresh {} {
	set cnt 0 
	# -- avoid refresh, if the main display items have not changed
	if [info exists CHANGED(main)] {
	    qt setDataset $WID "" 0 0 
	    qt setImage $WID "" 0 0

	    # -- enable all lists
	    foreach tag [array names DLIST] {
		set x0 0 ; set y0 0
		# -- collect the offsets
		foreach {x0 y0} $DLIST($tag) { break }

		if { [info exists ILIST($tag)]} {
		    qt setImage   $WID $tag $x0 $y0
		} elseif { [info exists DLIST($tag)]} {
		    qt setDataset $WID $tag $x0 $y0
		} elseif { [info exists DSET($tag)]} {
		    qt setDataset $WID [$DSET($tag) cget -tag] $x0 $y0
		}
	    }
	    set cnt 0
	    qt clear $WID
	    set cnt [qt refresh $WID]
	    unset CHANGED(main)
	}
    qt refreshSelection $WID
    qt paint $WID

        return $cnt
    }

    # -- refresh the information layer only 
    protected method zc_refresh_info {} {
        qt refreshSelection $WID
        qt paint $WID
    }

    # -- repaint the damaged canvas
    # -- there has been no change to any state of the zc
    protected method zc_repaint {} {
        qt paint $WID
	# -- make sure the pixmap is updated on screen
        update idletasks
    }


    # -- return all slices so far
    protected method zc_slices {} {
        set ns [qt getSliceSize]
        set sl {}
        set nl [qt getMaxLayers]

        for { set i 0 } { $i < $ns } { incr i } {
            set nm [qt getSliceName $i]
            set sp [split $nm "/"]
            foreach {hier type} $sp { break; }

            for { set layer 0 } { $layer <= $nl } { incr layer } {
                lappend sl "hierarchy:$hier layer:$layer type:$type"
            }
        }
        return $sl
    }
    protected method zc_image_add {tag image x0 y0 x1 y1} {
        set ILIST($tag) ""
	set data [$image data -format png]
	exec cat > $image.ppm.64 <<$data
	qt addImage64 $tag $data [string length $data] $x0 $y0 $x1 $y1
    }
    protected method zc_image_add_stream {tag channel x0 y0 x1 y1} {
	fixme "image_add_stream"
    }

    protected method zc_selection_add {tag x0 y0 x1 y1} {
	changed info
        foreach a "x0 y0 x1 y1" {
            set $a [expr int([set $a])]
        }

        if { $x0 > $x1 } { swap x0 x1 }
        if { $y0 > $y1 } { swap y0 y1 }

	if ![info exists SEL($tag)] {
	    set SEL($tag) 0
	}
	incr SEL($tag)
        set list [list id sel.$SEL($tag) type bbox hierarchy inst layer 0]
        lappend list bb [list $x0 $y0 $x1 $y1]

        qt addObject $tag $list

        qt setSelection $WID "" 0 0
        qt setSelection $WID $tag 0 0
    }

    protected method zc_selection_clear {tags} {
	changed info
        if { $tags == "all" } { set tags [array names SEL] }
        if { $tags == "" } { return 0 }
        foreach tag $tags {
            catch {unset SEL($tag)}
            qt eraseDataset $tag
        }
        qt setSelection $WID "" 0 0
        return 1
    }

    # -- return canvas window
    protected method zc_canvas_window {} { set WIN }
    # --------------------------------------------------------------- dsets
    protected method zc_objects_clear {tag} {
	changed main
        qt eraseDataset $tag
    }

    protected method zc_cad_objects_add {tag objlist} {
	changed main
        set OLIST($tag) ""
        qt addObjects $tag $objlist
    }

    protected method zc_cad_object_add_stream {tag channel} {
	changed main
	set OLIST($tag) ""
        set a [qt readFile $tag $channel]
        return $a
    }

    # -- return all known marks of a data set
    protected method zc_marks {tag} {
        qtzc_tags "marks-$tag"
    }
    
    # -- return all known tags
    protected method zc_tags {} {
        qtzc_tags tags
    }

    protected method zc_exists {id} {
	error "unimplemented"
    }

    # -- get thebbox of a data set
    protected method zc_bbox {tag} {
        qt getDataBBox $tag
    }

    protected method zc_dlist {} {
	concat [array names DLIST] [array names ILIST]
    }
    # ---------------------------------------------------------------  private
    protected method qt_get_stats { } {
        set nt [qt getTagSize htype]
        set tl { }
        set sl ""
        set sum 0
        for { set i 0 } { $i < $nt } { incr i } {
            set tm [qt getTagName htype $i]
            set tc [qt getTagCount htype $i]
	    set sum [expr $sum + $tc]
            lappend sl $tm $tc
        }
        lappend sl total $sum
	set n 0
        foreach arg [array names DLIST] {
	    incr n [llength [zc_marks $arg]]
        }
	lappend sl marks $n
        return $sl
    }

    protected method zc_stats {} {
        array set S "slices [qt getSliceSize]"
        array set S [qt_get_stats]
        array get S
    }

    protected method zc_canvas2world {X Y xx yy} {
        upvar $xx x
        upvar $yy y
        set rc [qt screen2World  $WID [expr int($X)] [expr int($Y)]]
        foreach {x y} $rc { break }
    }

    protected method zc_world2canvas {x y XX YY} {
        upvar $XX X
        upvar $YY Y
        set rc [qt world2Screen $WID [expr int($x)] [expr int($y)]]
        foreach {X Y} $rc { break }
    }
    # ----------------------------------------------- CallBacks
    # -- as a result of the Configure event
    # -- the window has been resized. Tell qt canvas about it
    public method eResized { } {
        set nwidth  [winfo width $This]
        set nheight [winfo height $This]
        # -- has the size really changed?
        if { ($width == $nwidth) && ($height == $nheight) }  {
            return
        }
	# -- update the size
	set width $nwidth  ; set height $nheight
        #qt setOverscanColor $WID "blue"
        qt resize $WID $nwidth $nheight
        changed main
        IdleRefresh
    }

    public method setSelectionPen {width color} {
        qt selectColormap cmap_$WID
        qt setSelPen $WID $width $color
    }

    public method setSelection { tag {x 0} {y 0} } {
        qt setSelection $WID $tag $x $y
    }

    # -- run when the main window appears on the screen
    public method eCreate {} {
        bind $WIN <Expose> ""
        set WID [qt newArea]

        set width  [winfo width $This]
        set height [winfo height $This]
        qt resize $WID $width $height
	set nwidth $width; set nheight $height

        qt newWindow $WID [winfo id $WIN]

        qt setColormap $WID cmap_$WID
        qt setSpecialColormap $WID cmap_s$WID
        qt setOverscanColor $WID wheat
        #qt setOverscanColor $WID green
        #qt setScaleFactor $WID 5 5
        #qt setScaleFactor $WID 1.5 1.5
        qt setScaleFactor $WID 2.5 2.5

        # -- apply main canvas bindings
        $zcg BindWin $WIN
        bind $WIN <Configure> "$this eResized"

        setSelectionPen 2 yellow

        # -- notify that the window is setup
        Event <<setup>>
    }
    # -- dragged
    public method eDraggingRegion {dx dy} {
        qt scroll $WID $dx $dy
        qt paint $WID
    }

    public method adjust {w l o y z} {
        qt adjust $WID $w $l $o $y $z
        qt refresh $WID
        qt paint $WID
    }

    public method grabImage {name} {
        qt grabImage $WID $name
    }

    public method setImageBBox {name x0 y0 x1 y1} { 
        qt setImageBBox $name $x0 $y0 $x1 $y1
        set ILIST($name) [list 0 0]
        set DLIST($name) [list 0 0]
        changed main
    }

    public method getDataSetData { tag } {
        set dl [qt getDataSetData $tag]
        return $dl
    }

    public method setChipBBox { bbox } {
        foreach {x0 y0 x1 y1} $bbox { break }
        if { $x0 > $x1 } { swap x0 x1 }
        if { $y0 > $y1 } { swap y0 y1 }

        set w [expr abs($x1 - $x0)]
        set h [expr abs($y1 - $y0)]

        qt setChipBBox $WID $x0 $y0 $w $h
    }
}
