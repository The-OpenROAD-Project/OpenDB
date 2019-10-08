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
#       Class: ZC
# Description: Abstract class for ZCanvas (just defines the protocol)
#      Author: Manolis M. Tsangaris 
#
# unless specified, all coordinates are world coordinates
#
itcl::class ZC {
    inherit LFrame LBinder LSLog
    # Event <<touch>>         button x y X Y   ;# touch detection
    # Event <<motion>>        button x y X Y   ;# touch detection
    # Event <<leave>>         
    # Event <<enter>>         
    # Event <<progress>>      msg percent      ;# percentage of work completed
    # Event <<selected-region>> x0 y0 x1 y1    ;# region selected
    # Event <<gesture-start>> gesture          ;# gesture starts
    # Event <<gesture-end>>   gesture          ;# gesture ends
    # Event <<setup>>                          ;# window is setup
    # Event <<stats>> refresh time sec         ;# window is setup
    # Event <<stats>> repaint time sec         ;# repaint time
    # Event <<debug>> tag msg                  ;# debugging
    # Event <<key>>   key                      ;# some key has been pressed
    # Event <<cursor>>  X Y                    ;# report cursor coords
    # -------------------------------------------------- Public Variables --
    public variable aspect  1     ;# should we keep the aspect ratio?
    public variable timeout 50  ;# when to refresh after IdleRefresh call
    public variable zoomf   .10
    public variable panf    .01
    public variable useset    0     ;# use set operations if possible
    public variable keepwbb  0      ;# should we keep world BB on autofit?
    public variable automark 0      ;# generate marks from type
    public variable nwidth 0        ;# the real width/height of the canvas
    public variable nheight 0
    # -------------------------------------------------- Public Methods ----
    # -- Clear selected data sets
    public method Clear {{tag all}} {
	$this zc_clear $tag
    }
    # ------------------------------------------------------- debugging
    # -- set/query debugging levels
    public method Debug {{level ""}} {
	$this zc_debug $level
    }
    public method DebugSet {tag info} {
	$this zc_debug_set $tag $info
    }
    # ------------------------------------------------------- VIEWPORT settings
    # -- define the viewport bbox in world coordinates
    # --- bb: {x0 y0 x1 y1}
    public method BBoxSet {bbox} {
	$zcg configure -bbox $bbox
	$this zc_bbox_set $bbox
    }
    public method BBoxGet {} {
	$this zc_bbox_get
    }

    # -- define colormap per object type/layer
    # -- colorlist: {type color ... layer color}
    public method ColorsSet {colorlist} {
	$this zc_colors_set $colorlist
    }

    # -- define visibility of layers/types
    # -- attrilist: list of types/layers to be visible
    public method VisibilitySet {attrlist} {
	$this zc_visibility_set $attrlist
    }
    # -- compute the full bbox of the display list
    public method FullBBox {} {
	$this zc_fullbbox
    }

    # -- return the canvas tk window
    public method CanvasWindow {} {
	$this zc_canvas_window
    }
    # ------------------------------------------------------- Refresh & Render
    # -- render tag to the current context
    # -- optionally place it relative to x, y
    public method Render {tag {x 0} {y 0}} {
	$this zc_render $tag $x $y
    }
    # -- render a dataset (optionally place it relative to x, y)
    public method RenderDSet {dset {x 0} {y 0}} {
	$this zc_render_dset $dset $x $y
    }
    # -- remove tag from the current context
    # -- if tag = all unrender everything
    public method UnRender {tag} {
	$this zc_unrender $tag
    }
    # -- make sure that objects are drawn in the canvas respecting all
    # -- viewport settings
    # -- forced refresh
    public method Refresh {} {
	# -- cancel any pending refresh
	$this zc_refresh_cancel
	# -- perform refresh
	set t0 [clock clicks -milliseconds]
	$this zc_refresh
	set t1 [clock clicks -milliseconds]
	set dt [format %.2f [expr 1.0*($t1 - $t0)/ 1000.0]]
	Event <<stats>> refresh time [format %.2f $dt]
    }
    # -- refresh once, after the idle loop
    public method IdleRefresh {} {
	$this zc_refresh_schedule
    }
    public method Repaint {} {
	$this zc_repaint
    }
    # -- refresh the information layer only (immediately)
    public method RefreshInfo {} {
	$this zc_refresh_info
    }
    # ------------------------------------------------------- Object Loading
    # -- ZC supports cad objects, images, or other drawables (tagged)

    # -- clear stored objects (tagged by tag) of any type
    public method ClearObjects {{tag all}} {
	$this zc_objects_clear $tag
    }

    # -- return all object slices found so far
    public method Slices {} {
	$this zc_slices
    }
    # -- return all Marks in the currently displayed data set
    public method Marks {{tag all}} {
	$this zc_marks $tag
    }
    # -- see if this object exists
    public method Exists {id} {
	$this zc_exists $id
    }
    # -- object bbox matching tag
    public method BBox {tag} {
	$this zc_bbox $tag
    }
    # -- produce the list of tags currently in the display list
    public method DList {} {
	$this zc_dlist
    }
    # -- dump window -- return an image
    public method Dump {to} {
	$this zc_dump $to
    }
    # -- connect bboxes with arrows
    public method Group {start group chain} {
	puts "fixme group $start $group $chain"
	$this zc_group $start $group $chain
    }
    # ------------------------------------------------------- Object:CAD 
    # -- load (incrementally) a list of CAD objects
    # -- tag: a name for this set of objects. Append to an existing tagged set
    # -- objlist: a list of objects (see README.ADE for the format)
    public method AddCADObjects {tag objlist} {
	$this zc_cad_objects_add $tag $objlist
    }

    # -- streaming version of AddObjects (see README.ADE for protocol)
    public method AddCADObjectStream {tag channel} {
	$this zc_cad_object_add_stream $tag $channel
    }

    # ------------------------------------------------------- Object:Image
    # -- place an image (tkimage) to x y 
    public method AddImage {tag image x y x1 y1} {
	$this zc_image_add $tag $image $x $y $x1 $y1
    }

    # -- streaming version (see README.ADE)
    public method AddImageStream {tag channel x y x1 y1} {
	$this zc_image_add_stream $tag $channel $x $y $x1 $y1
    }
    # ------------------------------------------------------- Object:Selection
    # -- add a selection rectangle
    public method AddSelection {tag x0 y0 x1 y1} {
	$this zc_selection_add $tag $x0 $y0 $x1 $y1
    }
    # -- clear any selection -- all if tag=all
    public method ClearSelection {tag} {
	$this zc_selection_clear $tag
    }
    # ------------------------------------------------------- Information Layer
    # -- add information in the information layer
    public method AddInfo {tag list} {
	$this zc_info_add $tag $list
    }
    public method ClearInfo {tag} {
	$this zc_info_clear $tag
    }
    # ------------------------------------------------------- Object:Info
    # -- return statistics (per class instances)
    public method Stats {} {
	$this zc_stats
    }
    # ------------------------------------------------------- ZOOM& PAN
    # -- Zoom the canvas
    public method Zoom {mode} {
	$this zc_zoom $mode
    }
    # -- move the entire canvas by dx dy world coordinates
    public method Move {dx dy} {
	$this zc_move $dx $dy
    }
    # -------------------------------------------------------- Scaling
    public method Canvas2World {X Y xx yy} {
	upvar $xx x
	upvar $yy y
	$this zc_canvas2world $X $Y x y
    }
    public method World2Canvas {x y XX YY} {
	upvar $XX X
	upvar $YY Y
	$this zc_world2canvas $x $y X Y
    }
    # return the pixel grid size in world coordinates
    # rows and columns: size of the pixel grid
    public method PixelGrid {rows cols x0 y0 x1 y1} {
        set _width   [winfo width $This] ; set _height  [winfo height $This]
        set dx [expr abs($x1-$x0)] ; set dy [expr abs($y1-$y0)]
        list [expr $rows*$dx/$_width] [expr $cols*$dy/$_height]
    }
    # -------------------------------------------------- Private Variables --
    private variable TASK
    protected variable zcg ""
    protected variable DEBUGINFO           ;# debug rules
    protected variable DEBUGIT             ;# active debug list
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	winconfig -border 0
	# $This is set to the window where the canvas should be packed
	set TASK(refresh) ""
	# -- create the gesture processor
	set zcg [ZCGesture $this.zcg -zc $this]
	LBridge zoomf   "$zcg configure -zoomf"
	LBridge panf    "$zcg configure -panf"
	# LBridge timeout "$zcg configure -timeout"
	$zcg Bind <<selecting-region>> $this eSelectingRegion
	$zcg Bind <<dragging-region>>  $this eDraggingRegion
	$zcg Bind <<selected-region>>  $this eSelectedRegion
	$zcg Bind <<touch>>            $this eTouch
	$zcg Bind <<exposed>>          $this eExposed
	$zcg Redirect <<gesture-start>> $this
	$zcg Redirect <<gesture-end>>   $this

	$zcg Redirect <<key>>           $this
	$zcg Redirect <<cursor>>        $this
	$zcg Redirect <<enter>>        $this
	$zcg Redirect <<leave>>        $this
    }
    destructor {
	$this zc_refresh_cancel
	foreach {_ task} [array get TASK] {
	    if { $task != "" } { after cancel $task }
	}
	$zcg delete
    }
    # ----------------------------------------------- Methods
    # ----------------------------------------------- Interface Implementation
    protected method zc_clear {tag} {
    }
    protected method zc_bbox_set {bbox} {
    }
    protected method zc_bbox_get {} {
    }
    protected method zc_colors_set {colorlist} {
    }
    protected method zc_visibility_set {attrlist} {
    }
    protected method zc_refresh {} {
    }
    protected method zc_objects_clear {tag} {
    }
    protected method zc_cad_objects_add {tag objlist} {
    }
    protected method zc_cad_object_add_stream {tag channel} {
    }
    protected method zc_slices {} {
    }
    protected method zc_dlist {} {
    }
    protected method zc_bbox {} {
    }
    protected method zc_image_add {tag image x0 y0 x1 y1} {
    }
    protected method zc_image_add_stream {tag channel x0 y0 x1 y1} {
    }
    protected method zc_selection_add {tag x0 y0 x1 y1} {
    }
    protected method zc_selection_clear {tag} {
    }
    protected method zc_info_add {tag list} {
    }
    protected method zc_info_clear {tag} {
    }
    protected method zc_render {tag x y} {
    }
    protected method zc_render_dset {dset x y} {
    }
    protected method zc_unrender {tag} {
    }
    protected method zc_group {start group chain} { }
    protected method zc_canvas_window {} {}

    #-- schedule a refresh
    protected method zc_refresh_schedule {} {
	if { $TASK(refresh) != "" } {
	    after cancel $TASK(refresh)
	}
	set TASK(refresh) [after $timeout "$this eRefresh"]
    }

    #-- cancel a pending refresh
    protected method zc_refresh_cancel {} {
	if { $TASK(refresh) == "" } { return }
	after cancel $TASK(refresh)
	set TASK(refresh) ""
    }
    # -- information layer refresh only
    protected method zc_refresh_info {} {
    }

    protected method zc_stats {} {
    }
    protected method zc_marks {tag} {
    }
    protected method zc_exists {id} {
    }
    protected method zc_zoom {zfactor} {
	$zcg Zoom $zfactor
    }
    # -- move canvas by dx dy
    protected method zc_move {dx dy} {
    }

    # -- map canvas to world coordinates
    protected method zc_canvas2world {X Y xx yy} {
	upvar $xx x
	upvar $yy y
    }

    # -- map world to canvas coordinates
    protected method zc_world2canvas {x y XX YY} {
	upvar $XX X
	upvar $YY Y
    }

    # -- repaint the canvas (but not refresh)
    protected method zc_repaint {} {
    }

    # -- fit all objects to the right bbox
    protected method zc_fullbbox {} {
	foreach {ax0 ay0 ax1 ay1} {{} {} {} {}} { break }
	# -- iterate through all display lists
	foreach tag [$this zc_dlist] {
	    set x0 ""
	    foreach {x0 y0 x1 y1} [$this zc_bbox $tag] { break }
	    # -- empty set?
	    if { $x0 == "" } { continue }

            if { $x0 > $x1 } { swap x0 x1 }
            if { $y0 > $y1 } { swap y0 y1 }
            if { $ax0 == "" } {
		foreach {ax0 ay0 ax1 ay1} "$x0 $y0 $x1 $y1" { break }
		continue
            } 
	    if { $x0 < $ax0 } { set ax0 $x0 }
	    if { $y0 < $ay0 } { set ay0 $y0 }
	    if { $x1 > $ax1 } { set ax1 $x1 }
	    if { $y1 > $ay1 } { set ay1 $y1 }
        }
	if { $ax0 == "" } { 
	    return
	}
	# -- enlarge by 1/10
	set dx [expr ($ax1 - $ax0)/20.0]
	set dy [expr ($ay1 - $ay0)/20.0]
	set ax0 [expr $ax0 - $dx] ; set ax1 [expr $ax1 + $dx]
	set ay0 [expr $ay0 - $dy] ; set ay1 [expr $ay1 + $dy]
	list $ax0 $ay0 $ax1 $ay1
    }

    # ----------------------------------------------- Debug
    # -- set the debugging filter
    public method zc_debug {arg} {
	if { $arg == "" } {
	    return [array get DEBUGINFO]
	}
	if { $arg == "-" } { 
	    catch {unset DEBUGIT}
	    return
	}
	set DEBUGIT($arg) ""
    }
    # -- set the debugging mode
    public method zc_debug_set {tag info} {
	set DEBUGINFO($tag) $info
    }
    # -- debugging output
    protected method dprint {tag msg args} {
	if ![array size DEBUGIT] { return }
	set d 0
	foreach ntag [array names DEBUGIT] {
	    if { $tag == $ntag } { incr d ; break }
	}
	if { $d == 0 } {
	    foreach ntag [array names DEBUGIT] {
		if [string match $ntag* $tag] {
		    incr d ; break
		}
	    }
	}
	if { $d } {
	    if { $args != "" } { set msg [eval format [list $msg] $args] }
	    Event <<debug>> $tag [LTrim $msg 70]
	}
    }
    # ----------------------------------------------- CallBacks
    # -- this is called by the idle loop, to issue a refresh
    public method eRefresh {} {
	set TASK(refresh) ""
	$this Refresh
    }
    # -- called when the region is changing
    # -- ZC implementation can display the selected region here
    public method eSelectingRegion {X0 Y0 X1 Y1 reason} {
	if { $reason != "drag" } {
	    ClearSelection box
	    AddSelection box $X0 $Y0 $X1 $Y1
	    # -- make sure the selection is immediately redrawn
	    RefreshInfo
	}
	Event <<selecting-region>> $X0 $Y0 $X1 $Y1 $reason
    }



    # -- called when the region is changing by dragging (canvas coords)
    # -- ZC implementation can move the canvas around here
    public method eDraggingRegion {dx dy} {
    }

    # -- called when the region has been selected
    # -- ZC implementation can display the selected region here
    public method eSelectedRegion {X0 Y0 X1 Y1 reason} {
	Event <<selected-region>> $X0 $Y0 $X1 $Y1 $reason
    }

    # -- Mouse button touched the canvas (canvas coordinates)
    public method eTouch {b x y} {
        Event <<touch>> $b $x $y
    }

    # -- The canvas has been exposed, need to repaint the window
    # -- no need to recalculate anything
    public method eExposed {} {
	$this zc_repaint
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: ZCGesture
# Description: Gesture processor for ZC
#      Author: Manolis M. Tsangaris
#
itcl::class ZCGesture {
    inherit LObject LBinder
    # Event <<selecting-region>> $X0 $Y0 $X1 $Y1 reason
    # Event <<dragging-region>> $dx $dy
    # Event <<selected-region>> $X0 $Y0 $X1 $Y1 reason
    # Event <<gesture-start>> gesture
    # Event <<gesture-end>>   gesture
    # Event <<touch>> $b $x $y
    # -------------------------------------------------- Public Variables --
    public variable enabled  1                  ;# is selection enabled?
    public variable zoomf  0.1                  ;# zoom factor
    public variable panf   0.02                 ;# pan factor
    public variable bbox    "" {bbox_set $bbox} ;# current bbox
    public variable selcolor red                ;# rubber band selection color
    public variable timeout 1000                ;# zoom/pan timeout
    public variable zc  ""                      ;# canvas engine
    public variable minmove 5                  ;# minmove for dragging
    # -------------------------------------------------- Public Methods ----
    public method BindWin {win} {
	zcg_bind_win $win
    }
    public method Zoom {mode} {
	$this eZoom $mode
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable X0 0           ;# BBOX (real world coords)
    protected variable Y0 0
    protected variable X1 0
    protected variable Y1 0
    protected variable TX0 0          ;# touchdown coordinates
    protected variable TY0 0          ;# 
    protected variable TX1 0          ;# last drag coordinates
    protected variable TY1 0          ;# 
    protected variable selecting 0    ;# set if we are selecting a bb
    protected variable REASON ""      ;# reason for pan/zoom
    protected variable task ""        ;# delayed task
    protected variable WIN ""
    protected variable GESTURE 0      ;# gesture event count
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set task ""
    }
    destructor {
	if { $task != "" } { after cancel $task }
    }
    # -------------------------------------------------- Private Methods ---
    # -- note the new bbox
    protected method bbox_set {bb} {
	if { $bb == "" } {
	    foreach {X0 Y0 X1 Y1} "0 0 0 0" { break }
	} else {
	    foreach {X0 Y0 X1 Y1} $bb { break }
	}
    }
    # ----------------------------------------------- Interface Implementation
    protected method zcg_bind_win {w} {
	bind $w <Enter>                  "$this Event <<enter>>; focus $w"
	bind $w <Leave>                  "$this Event <<leave>>; focus ."

        bind $w <Shift-ButtonPress-1>    "$this eSelectStart  %x %y"
        bind $w <Shift-B1-Motion>        "$this eSelectMotion %x %y"
        bind $w <Shift-ButtonRelease-1>  "$this eSelectStop %b %s"

        bind $w <ButtonPress-1>          "$this eDragStart %x %y"
        bind $w <B1-Motion>              "$this eDrag      %x %y"
        bind $w <ButtonRelease-1>        "$this eDragEnd   %x %y"
        bind $w <Motion>                 "+$this eCursor    %x %y"

        bind $w <KeyPress-Left>          "$this eMove %K"
        bind $w <KeyPress-Right>         "$this eMove %K"
        bind $w <KeyPress-Up>            "$this eMove %K"
        bind $w <KeyPress-Down>          "$this eMove %K"

        bind $w <KeyPress-space>         "$this eZoom in"
        bind $w <Control-KeyPress-space> "$this eZoom out"

        bind $w <KeyPress-Escape>        "$this Event <<key>> escape"
        bind $w <KeyPress-Delete>        "$this Event <<key>> delete"
        bind $w <KeyPress>               "$this Event <<key>> %K"

        bind $w <Shift-3>                "$this eShow %x %y"

        bind $w <ButtonPress-2>          "$this eTouch 2 %x %y"
        bind $w <ButtonPress-3>          "$this eTouch 3 %x %y"
        bind $w <ButtonPress-3>          "$this eTouch 3 %x %y"
        bind $w <Expose>                 "$this Event <<exposed>>"
	set WIN $w
    }
    # ----------------------------------------------- CallBacks
    # ----------------------------------------------------------- select
    public method SelectClear { } {
        puts "Clearing Selection!"
        SelectionC2
	    $zc RefreshInfo
    }
    public method eSelectStart {x y} {
        if {! $enabled } { return }
        catch {after cancel $task}
        set REASON "lasso"
	# -- remember where you touched
        set TX0 $x 
        set TY0 $y 
        set TX1 $x 
        set TY1 $y 
        set selecting 1
        $zc Clear box
    }
    # -- called when we move & drag
    public method eSelectMotion {x y} {
        if {! $enabled } { return }
	# -- in case we  missed the SelectStart event
	if {!$selecting} {
	    eSelectStart $x $y
	    return
	}
        set TX1 $x
        set TY1 $y
	$zc Canvas2World $TX0 $TY0 x0 y0
	$zc Canvas2World $TX1 $TY1 x1 y1
        eval Event <<selecting-region>> $x0 $y0 $x1 $y1 select

        eCursor $x $y
    }
    public method eSelectStop { args } {
	set task ""
        #puts "eSelectStop: $args"
        if {! $enabled } { return }
        if { $TX0 == "" }  { return }
	# -- no movement?
        if { ($TX0 == $TX1) && ($TY0 == $TY1) } {
            return
        }

        # -- mark selection as invalid
        set selecting 0

	set reason $REASON
        set REASON ""
	$zc Canvas2World $TX0 $TY0 x0 y0
	$zc Canvas2World $TX1 $TY1 x1 y1
	if { $x0 > $x1 } { swap x0 x1 }
	if { $y0 > $y1 } { swap y0 y1 }
	Event <<selected-region>> $x0 $y0 $x1 $y1 $reason
    }

    # ------------------------------------------------------------ moving
    # -- process the relative move keypresses
    # -- but do not actually redraw, until the user has stopped moving
    # -- change the bbox attributes
    public method eMove {k} {
        if {! $enabled } { return }


        set REASON "panning"
        catch {after cancel $task}
	set DX [expr abs($X1 - $X0)*$panf]
	set DY [expr abs($Y1 - $Y0)*$panf]
        set dx 0 ; set dy 0
        switch $k {
            Left  { set dx [expr -$DX] }
            Right { set dx $DX }
            Down  { set dy [expr -$DY] }
            Up    { set dy $DY }
        }
        set X0 [expr $X0 + $dx] ; set Y0 [expr $Y0 + $dy]
        set X1 [expr $X1 + $dx] ; set Y1 [expr $Y1 + $dy]
	Event <<selecting-region>> $X0 $Y0 $X1 $Y1 move

	if { $task != "" } { after cancel $task }
        set task [after $timeout "$this eMoveEnd"]
    }
    public method eMoveEnd {} {
        #puts "eMoveEnd"
        if {! $enabled } { return }
	set task ""
	Event <<selected-region>> $X0 $Y0 $X1 $Y1 $REASON
    }

    # -- Update Cursor Position
    public method eCursor { x y } {
        if {! $enabled } { return }
        $zc Canvas2World $x $y X Y
	Event <<cursor>> $x $y $X $Y
    }
    # ----------------------------------------------------------- dragging
    # -- called when dragging is about to start
    public method eDragStart {x y} {
        if {! $enabled } { return }
        catch {after cancel $task}
	# -- remember where you touched
	set TX0 $x ; set TY0 $y
	set TX1 $x ; set TY1 $y
        set X0 $x  ; set Y0 $y 
        set X1 $x  ; set Y1 $y 
        set selecting 1
        $zc Clear box
    }
    # -- drag the canvas around
    # -- X0,Y0,X1,Y1 are set to the new bbox corners
    public method eDrag {x y} {
        if {! $enabled } { return }
	set dx [expr -$x + $TX0]
	set dy [expr -$y + $TY0]

	# -- ignore small movements
	if { [expr sqrt($dx*$dx + $dy*$dy)] < $minmove } {
	    return
	}
	if { $GESTURE == 0 } {
	    # -- notify on beginning of gesture
	    Event <<gesture-start>> drag
	}
	incr GESTURE
	# -- obtain canvas width & height
	set w [$zc cget -nwidth] ; set h [$zc cget -nheight]

	$zc Canvas2World [expr $dx +  0] [expr $dy +  0] AX0 AY0
	$zc Canvas2World [expr $dx + $w] [expr $dy + $h] AX1 AY1

	Event <<selecting-region>> $AX0 $AY0 $AX1 $AY1 drag

	Event <<dragging-region>>  [expr $x - $TX1 ] [expr $y -$TY1]
	set TX1 $x ; set TY1 $y
    }
    # -- end of drag - update selected region
    # -- ignore the final coords
    public method eDragEnd {_ _} {
        if {! $enabled } { return }
	set dx [expr $TX0 - $TX1]
	set dy [expr $TY0 - $TY1]

	# -- well, this might be a touch event
	if { [expr sqrt($dx*$dx + $dy*$dy)] < $minmove } {
	    if { $GESTURE } {
		Event <<gesture-end>> drag
		set GESTURE 0
	    }
	    Event <<touch>> 1 $TX0 $TY0
	    ###foreach {X0 Y0 X1 Y1} $bbox { break }
	    return
	}

	# -- notify on end of gesture
	Event <<gesture-end>> drag
	set GESTURE 0

	# -- obtain canvas width & height
	set w [$zc cget -nwidth] ; set h [$zc cget -nheight]

	$zc Canvas2World [expr $dx +  0] [expr $dy +  0] X0 Y0
	$zc Canvas2World [expr $dx + $w] [expr $dy + $h] X1 Y1

	Event <<selected-region>> $X0 $Y0 $X1 $Y1 dragged
    }
    protected method bbox_print {comment x0 y0 x1 y1} {
	set dx [expr $x1 - $x0]
	set dy [expr $y1 - $y0]
	puts \
	    [format "%20s: %8.0f %8.0f %8.0f %8.0f (%8.0f %8.0f)"\
		$comment $x0 $y0 $x1 $y1 $dx $dy]
    }

    # ------------------------------------------------------------- ZOOM
    # -- perform zoom in/out
    public method eZoom {mode} {
	if { $bbox == "" } { return }

	if { $REASON != "zoom" } {
	    foreach {X0 Y0 X1 Y1} $bbox { break }
	    set REASON zoom
	}

	if { $X0 > $X1 } { swap X0 X1 }
	if { $Y0 > $Y1 } { swap Y0 Y1 }

	switch -- @$mode {
	    @in {
		set DX [expr -1.0*($X1 - $X0)*$zoomf]
		set DY [expr -1.0*($Y1 - $Y0)*$zoomf]
	    }
	    @out {
		set DX [expr 1.0*($X1 - $X0)*$zoomf]
		set DY [expr 1.0*($Y1 - $Y0)*$zoomf]
	    }
	    default {
		set DX [expr 1.0*($X1 - $X0)*$mode]
		set DY [expr 1.0*($Y1 - $Y0)*$mode]
	    }
	}
        set X0 [expr $X0 - $DX] ; set Y0 [expr $Y0 - $DY]
        set X1 [expr $X1 + $DX] ; set Y1 [expr $Y1 + $DY]

	Event <<selecting-region>> $X0 $Y0 $X1 $Y1 zoom

	# -- defer the timeout some more
	if { $task != "" } { after cancel $task }
	set task [after $timeout "$this eZoomDone"]
    }
    # -- zoom factor selection is done now
    public method eZoomDone {} {
        #puts "eZoomDone"
        set REASON ""
	set task ""
	Event <<selected-region>> $X0 $Y0 $X1 $Y1 zoom
    }
    # -- mouse button touchdown
    # -- schedule an event for later
    public method eTouch {b x y} {
	set TX0 $x ; set TY0 $y
	set TX1 $x ; set TY1 $y
	$this Event <<touch>> $b $x $y
    }
    # -------------------------------------------------- Common Initialization
}
