package provide ade::ui 2.0

#
#       Class: BView
# Description: Bird's eye view
#      Author: Manolis M. Tsangaris
#
itcl::class BView {
    inherit LFrame LBinder
    # Event <<selected-region>> x0 y0 x1 y1
    # Event <<bookmark-add>>
    # Event <<bookmark-goto>>
    # Event <<cursor>> X Y        ;# cursor position
    # -------------------------------------------------- Public Variables --
    public variable width  "200"
    public variable height "200"
    public variable bbox    ""  { bbox_set $bbox } 
    public variable invy    1   ; # invert y axis?
    # -------------------------------------------------- Public Methods ----
    public method LoadTiles {tiles} {
        clear
        tiles_load $tiles
    }
    public method SelectionSet {x0 y0 x1 y1} {
	array set VIEW [list x0 $x0 y0 $y0 x1 $x1 y1 $y1]
        $csel SelectionSet $x0 $y0 $x1 $y1 
    }
    public method Colors { } {
        array get COLOR
    }
    public method SelectAll {} {
        $csel SelectAll
    }
    # -- add a bookmark
    public method BookMarkAdd {bid bbox} {
	bv_mark_add $bid $bbox
    }
    public method BookMarkDelete {bid} {
	set tag mark:$bid
	$cv delete $tag
    }
    # -- show a dataset in the overview window
    public method ShowDataSet {ds} {
	bv_dset_show $ds
    }
    public method AutoFit {} {
	$csel AutoFit
    }

    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable cv
    protected variable csel
    protected variable VIEW       ;# -- current view
    protected variable COLOR      ;# -- per layer color
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set cv [canvas $This.cv -border 1 -relief sunken -bg gray50 ]
 	pack $cv -side top -expand 1 -fill both 

        LBridge width  "$cv configure -width"
        LBridge height "$cv configure -height"

        set csel [CSelector $this.csel -canvas $cv -invy $invy -selcolor black] 
        # -- redirect events to us
        $csel Redirect <<selected-region>> $this
        $csel Redirect <<cursor>>          $this

        array set COLOR {
            default indianred
            0 forestgreen
            1 indianred
            2 darkseagreen
            3 blue
            4 lightblue
            5 orange
            6 gray
            7 purple
            8 orange
            15 indianred
        }
	$csel Bind <<key>> $this eKey
	$csel Bind <<hit>> $this eHit
    }
    destructor {
        $csel delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method clear {} {
        catch {after cancel $task}
        $cv delete all
        $csel Reset
    }
    # ----------------------------------------------- Interface Implementation
    protected method bbox_set {bb} {
        # foreach {X0 Y0 X1 Y1} $bb { break }
        $csel SetBB $bb
    }
    protected method tiles_load {tiles} {
        # make sure that our window stabilizes ...
        update idletasks ; update

        $cv delete all
        # obtain the chip bbox
        if { $bbox == "" } { 
            # -- nothing loaded -- give up!
            return
        }
        if { $tiles == "" } {
            set tiles [list [list type tile id t1 bb $bbox]]
        }
        set btiles 0
        set err ""
        foreach tile $tiles {
            if { $tile == "" } { continue }
            set T(layer) default
            if [catch {array set T $tile} err] { 
                incr btiles
                continue
            }
            foreach {x0 y0 x1 y1} $T(bb) { break }
            # -- reverse the y coordinate
            if { $invy } { 
                set y0 [expr -1.0*$y0] ; set y1 [expr -1.0*$y1]
            }
            set n  [$cv create rectangle $x0  $y0 $x1 $y1 \
                -tags "tile $T(id)" \
                -outline yellow -width 1 -fill $COLOR($T(layer))]
            unset T
        }
        if { $btiles != 0 } {
            warning bview "$btiles tile(s) could not be loaded"
            warning bview "last error: $err"
        }
        $csel AutoFit
    }

    # -- add a bookmark
    protected method bv_mark_add {bid bbox} {
	set tag mark:$bid
	foreach {x0 y0 x1 y1} $bbox { break }
	$csel World2Canvas $x0 $y0 X0 Y0
	$csel World2Canvas $x1 $y1 X1 Y1
	set col yellow
	set tags "mark:$bid mark"
	$cv create rectangle $X0  $Y0 $X1 $Y1 -tags $tags  -outline $col
	$cv create line      $X0  $Y0 $X1 $Y1 -tags $tags -fill $col
    }

    # -- given an id find tag prefix:*
    protected method bv_tag {tid prefix} {
	foreach tg [$cv gettags $tid] {
	    if { [scan $tg $prefix:%s tag] == 1 } {
		return $tag
	    }
	}
	return ""
    }
    # -- show cached datasets using blue squares
    protected method bv_dset_show {ds} {
	set tag [$ds cget -tag]
	$cv delete dset:$tag
	set bb  [$ds GetAttr bbox]
	if { $bb == "" } { return }
        foreach {x0 y0 x1 y1} $bb { break }
        $csel World2Canvas $x0 $y0 X0 Y0
        $csel World2Canvas $x1 $y1 X1 Y1
        set col yellow
        set tags "dset:$tag mark"
        $cv create rectangle $X0  $Y0 $X1 $Y1 -tags $tags  -fill $col \
	    -stipple @[ade_media gray25.bmp] -outline ""
    }
    # ----------------------------------------------- CallBacks
    # -- a key has been pressed near a bookmark
    public method eKey {key x y} {
	set t [$csel Find $x $y mark]
	if { $t == "" } { return }
	switch -- @$key {
	    @b {
		Event <<bookmark-add>> $VIEW(x0) $VIEW(y0) $VIEW(x1) $VIEW(y1)
	    }
	    @g - @G { 
		set bid  [bv_tag $t mark]
		if { $bid == "" } { return }
		Event <<bookmark-goto>> $bid
	    }
	    @d - @D {
		set bid  [bv_tag $t mark]
		if { $bid == "" } { return }
		BookMarkDelete $bid
		Event <<bookmark-delete>> $bid
	    }
	}
    }
    public method eHit {button x y} {
	set t [$csel Find $x $y mark]
	if { $t == "" } { return }
	set bid  [bv_tag $t mark]
	if { $bid == "" } { return }
	Event <<bookmark-goto>> $bid
    }
    # -------------------------------------------------- Common Initialization
}
