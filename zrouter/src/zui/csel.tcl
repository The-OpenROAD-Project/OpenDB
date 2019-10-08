package provide ade::ui 2.0

#
#       Class: CSelector
# Description: Canvas Selector & scale keeper
#      Author: Manolis M. Tsangaris
#
itcl::class CSelector {
    inherit LBinder
    # Event <<selecting-region>> x0 y0 x1 y1      ;# while region is selected
    # Event <<selected-region>> x0 y0 x1 y1 reason ;# when region is finalized
    # Event <<escape>>                            ;# user interrupt
    # Event <<key>> key x y                       ;# key hit
    # Event <<cursor>> x y                        ;# mouse coordinates
    # -------------------------------------------------- Public Variables --
    public variable canvas ""  { set cv $canvas }
    public variable active 1   ; # is selection active ?
    public variable aspect 1   ; # should we respect aspect ratio?
    public variable invy 0     ; # invert y axis
    public variable selcolor red  ;# rubber band selection color
    public variable deltax 1   ; # selection increment in pixels in x direction
    public variable deltay 1   ; # selection increment in pixels in y direction
    public variable keepwbb 0  ; # should we keep world BB on autofit?
    public variable zoomf   10 ; # percentage of zoom to do
    # -------------------------------------------------- Public Methods ----
    # set the bounding box of all objects in canvas in world coordinates
    # this does not affect drawing, as that uses the coordinates of items
    # actually in canvas. BB is only used for clipping
    public method SetBB {bb} {
        bbox_set $bb
    }

    # reset scaling xforms
    public method Reset {} {
        array set S [list x0 0 y0 0 xx 0 yy 0 sx 1.0 sy 1.0]
    }
    public method Clear {} {
        $cv delete box
        Reset
    }
    public method AutoFit {{keep ""}} {
        if { $keep == "" } {
            set keep $keepwbb
        }
        autofit $keep
    }
    # -- convert canvas to world coordinates
    public method Unscale {X0 Y0 X1 Y1} {
        canvas2world $X0 $Y0 x0 y0 
        canvas2world $X1 $Y1 x1 y1 
        list $x0 $y0 $x1 $y1
    }
    public method SelectionSet {x0 y0 x1 y1} {
        sel_set $x0 $y0 $x1 $y1 
    }
    # -- clip x, y around the bbox, with respect to optional point X0,Y0
    public method Clip {x y {x0 ""} {y0 ""}} {
        sel_clip $x $y $x0 $y0
    } 

    # -- set the rubber band selection box
    public method SetSelection {x0 y0 x1 y1} {
        set X0 $x0
        set Y0 $y0
        set X1 $x1
        set Y1 $y1
    }
    # -- return the pixel grid size in world coordinates
    # -- rows and columns: size of the pixel grid
    public method PixelGrid {rows cols x0 y0 x1 y1} {
        set width   [winfo width $cv] ; set height  [winfo height $cv]
        set dx [expr abs($x1-$x0)] ; set dy [expr abs($y1-$y0)]
        list [expr $rows*$dx/$width] [expr $cols*$dy/$height]
    }

    # -- select the entire canvas
    public method SelectAll {} {
        $this eStart 0 0
        $this eMotion [winfo reqwidth $cv] [winfo reqheight $cv]
        $this eStop
    }

    # -- world2canvas
    public method World2Canvas {x y XX YY} {
	upvar $XX X ; upvar $YY Y
	world2canvas $x $y X Y
    }

    # -- find an object close
    public method Find {x y tag} {
	csel_find $x $y $tag
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable BB ; # bbox as an array
    protected variable cv
    protected variable X0 0
    protected variable Y0 0
    protected variable X1 0
    protected variable Y1 0
    protected variable task ""
    protected variable selecting 0
    protected variable S
    protected variable ZOOM
    protected variable REASON ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        bind $cv <Shift-B1-Motion>        "$this eMotion %x %y"
        bind $cv <Motion>                 "$this eCursor %x %y"
        bind $cv <Shift-ButtonRelease-1>  "$this eStop"
        bind $cv <KeyPress-Left>          "$this eMove %K"
        bind $cv <KeyPress-Right>         "$this eMove %K"
        bind $cv <KeyPress-Up>            "$this eMove %K"
        bind $cv <KeyPress-Down>          "$this eMove %K"
        bind $cv <KeyPress-space>         "$this eZoom in"
        bind $cv <KeyPress-Escape>        "$this Event <<escape>>"
        bind $cv <Control-KeyPress-space> "$this eZoom out"
        bind $cv <Enter>                  "focus $cv"
        bind $cv <Shift-3>                "$this eShow %x %y"

        bind $cv <Key>                    "$this Event <<key>> {%K} %x %y"
        bind $cv <1>                      "$this Event <<hit>> 1 %x %y"
        Clear

        set ZOOM(dx) [expr int($zoomf/100.0*[winfo reqwidth  $cv])]
        set ZOOM(dy) [expr int($zoomf/100.0*[winfo reqheight $cv])]
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method clear {} {
        catch {after cancel $task}
        $cv delete all
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- Event
    public method eStart {x y} {
        if {! $active } { return }
        catch {after cancel $task}
        set X0 $x 
        set Y0 $y 
        set X1 $x 
        set Y1 $y 
        set selecting 1
        $cv delete box
    }
    # -- Report cursor position
    public method eCursor {x y} {
        if {! $active } { return }
        canvas2world $x $y X Y
	Event <<cursor>> $x $y $X $Y
    }
    public method eMotion {x y} {
        if {! $active } { return }
        set REASON "area selection"
        catch {after cancel $task}
        # is it the first time we run after last selection?
        if { !$selecting } { eStart $x $y ; return }
        box_draw $X0 $Y0 $x $y
        set X1 $x
        set Y1 $y
        eval Event <<selecting-region>> [Unscale $X0 $Y0 $X1 $Y1]

	# -- report on cursor coordinates
        eCursor $x $y
    }
    public method eStop {} {
        catch {after cancel $task ; set task ""}
        if {! $active } { return }
        if { $X0 == "" }  { return }
        if { ($X0 == $X1) && ($Y0 == $Y1) } {
            return
        }
        # mark selection as invalid
        set selecting 0

        set bb [Unscale $X0 $Y0 $X1 $Y1]
        lappend bb $REASON
        set REASON ""
        eval Event <<selected-region>> $bb
    }
    # --
    # -- process the relative move keypresses
    # -- but do not actually redraw, until the user has stopped moving
    # --
    public method eMove {k} {
        if {! $active } { return }
        set REASON "panning"
        catch {after cancel $task}
        set dx 0 ; set dy 0
        switch $k {
            Left  { set dx [expr - $deltax] }
            Right { set dx $deltax }
            Up    { set dy [expr - $deltay] }
            Down  { set dy $deltay }
        }
        set X0 [expr $X0 + $dx] ; set Y0 [expr $Y0 + $dy]
        set X1 [expr $X1 + $dx] ; set Y1 [expr $Y1 + $dy]
        $cv delete box
        $cv create rectangle $X0 $Y0 $X1 $Y1 -tags box -outline $selcolor\
             -width 2
        eval Event <<selecting-region>> [Unscale $X0 $Y0 $X1 $Y1]
        set task [after 1000 "$this eStop"]
    }
    # perform zoom in/out
    public method eZoom {dzoom} {
        if {! $active } { return }
        set REASON "zoom $dzoom"
        catch {after cancel $task ; set task ""}
        set dx 0 ; set dy 0
        switch $dzoom {
            out { set dx $ZOOM(dx)         ; set dy $ZOOM(dy)         }
            in  { set dx [expr -$ZOOM(dx)] ; set dy [expr -$ZOOM(dy)] }
        }
        set X0 [expr $X0 - $dx] ; set Y0 [expr $Y0 - $dy]
        set X1 [expr $X1 + $dx] ; set Y1 [expr $Y1 + $dy]

        $cv delete box
        $cv create rectangle $X0 $Y0 $X1 $Y1 -tags box -outline $selcolor\
            -width 2
        eval Event <<selecting-region>> [Unscale $X0 $Y0 $X1 $Y1]

	if { $task == "" } {
	    set task [after 1000 "$this eStop"]
	}
    }
    public method eShow {X Y} {
        canvas2world $X $Y x y
    }
    # -------------------------------------------------------- SCALING 
    # --- convert world to canvas coordinates
    protected method world2canvas {x y XX YY} {
        upvar $XX X
        upvar $YY Y

        # X = (x - x0) * sx
        set X [expr ($x - $S(x0))* $S(sx)]

        # Y = (y - y0) * sy
        if { $invy } { set y [expr -1.0 * $y] }
        set Y [expr ($y - $S(y0))* $S(sy)]
    }
    # -- convert canvas coordinates to problem coordinates
    protected method canvas2world {X Y xx yy} {
        upvar $xx x
        upvar $yy y

        # X = (x - x0) * sx => x = X/sx + x0
        set x [expr $X /$S(sx) + $S(x0)]
    
        # Y = (y + y0) * sy => y = Y/sy + y0
        set y [expr $Y /$S(sy) + $S(y0)]

        if { $invy } { set y [expr -1.0 * $y] } 
    }


    # -- rescale canvas to fit all data in it
    # -- if keepwbb is set, the bounding box will be used to 
    # -- scale objects. Otherwise use the total object bbox
    protected method autofit {keepwbb} {
        # reset scaling coefficients
        Reset
        set width   [winfo width $cv] ; set height  [winfo height $cv]

        # respect the bounding box, if any
        # and if there is a previous scaling in effect
        if { [array size BB] && $keepwbb } {
            if { $invy } {
                set y0 [expr  -1 * $BB(y0)]
                set y1 [expr  -1 * $BB(y1)]
                # fix y0 y1 ...
                if { $y0 > $y1 } { swap y0 y1 }
            } else {
                set y0 $BB(y0)
                set y1 $BB(y1)
            }
            set bb [list $BB(x0) $y0 $BB(x1) $y1]
            debug "autofit: used existing bb: $bb"
        } else {
            set bb [$cv bbox all]
            debug "autofit: object bb: '$bb'"
            if { $bb != "" } {
                # give 1/30 breathing room around each side
                scan $bb "%f %f %f %f" x0 y0 x1 y1
                set dx [expr abs($x0-$x1)/30.]
                set dy [expr abs($y0-$y1)/30.]
                set bb [list \
                    [expr $x0 - $dx] [expr $y0 - $dy] \
                    [expr $x1 + $dx] [expr $y1 + $dy] \
                ]
                debug "autofit: adjusted object bb: $bb"
            }
        }

        # any objects?
        if { $bb == "" } { return }

        # collect the current bounding box, in world
        # coordinates, since no scaling has happened yet
        scan $bb "%f %f %f %f" x0 y0 x1 y1

        # first move objects to their origin
        set S(x0) $x0 ; set S(y0) $y0
        set dx [expr  - $x0]; set dy [expr  - $y0]
        $cv move all $dx $dy

        # compute scale to achieve auto-fit
        set sx [expr 1.0*($width )/($x1 - $x0)]
        set sy [expr 1.0*($height)/($y1 - $y0)]

        # -- pick the lowest scale, if we are to respect aspect ratio
        if { $aspect } {
            if { $sx > $sy } { set sx $sy }
            set sy $sx
        }
        set S(sx) $sx ; set S(sy) $sy
        $canvas scale all 0 0 $sx $sy
        debug "autofit mapped  $bb -> ${width}x$height"
        debug "autofit scaling $S(x0) $S(y0) $S(sx) $S(sy)"
    }
    protected method sel_set {x0 y0 x1 y1} {
        world2canvas $x0 $y0 X0 Y0
        world2canvas $x1 $y1 X1 Y1
        box_draw $X0 $Y0 $X1 $Y1
    }

    # -- draw the selection box
    protected method box_draw {x0 y0 x1 y1} {
        $cv delete box
        $cv create rectangle $x0 $y0 $x1 $y1 -tags box -outline $selcolor \
            -width 2
    }
    # -- make a note of our bbox
    # -- keep it in world coordinates 
    protected method bbox_set {bb} {
        catch {unset BB}
        if { $bb == "" } {
            return
        }
        if {[scan $bb "%f %f %f %f" x0 y0 x1 y1] != 4 } {
            return
        }
        set BB(x0) $x0 ; set BB(x1) $x1 
        set BB(y0) $y0 ; set BB(y1) $y1 
    }
    # draw bbox for the purposes of debugging
    protected method bbox_draw {tag} {
        $cv delete $tag
        world2canvas $BB(x0) $BB(y0) X0 Y0
        world2canvas $BB(x1) $BB(y1) X1 Y1
        $cv create rectangle $X0 $Y0 $X1 $Y1 -outline green -width 2 -tags $tag
    }
    # -- clip point x,y with respect with current BBOX
    # -- in the future will do the correct clipping based on xp, yp
    protected method sel_clip {x y xp yp} {
        if { $x < $BB(x0) }  { set x $BB(x0) }
        if { $y < $BB(y0) }  { set y $BB(y0) }
        if { $x > $BB(x1) }  { set x $BB(x1) }
        if { $y > $BB(y1) }  { set y $BB(y1) }
        list $x $y
    } 
    protected method bb_print {msg} {
        set res ""
        foreach a {x0 y0 x1 y1} {
            append res [format "%5d $BB($a)]
        }
        puts "$msg: $res"
    }
    protected method bbdiff {bb1 bb2} {
        foreach {a1 a2 a3 a4} $bb1 { break }
        foreach {b1 b2 b3 b4} $bb2 { break }
        set res ""
        foreach t {1 2 3 4} {
            lappend res [expr [set a$t]-[set b$t]]
        }
        return $res
    }
    protected method bb_delta_print {msg bb} {
        set res ""
        set N 0
        foreach a {x0 y0 x1 y1} {
            append res [format "%5d " [expr int($BB($a) - [lindex $bb $N])]]
            incr N
        }
        puts "$msg: $bb (delta $res)"
    }

    # -- find the closest object of tag tag
    protected method csel_find {x y tag} {
        set t [$cv find closest $x $y]
	set last ""
        while { $last != $t } {
            foreach tg [$cv gettags $t] {
                if { $tg == $tag } {
                    return $t
                }
            }
	    set last $t
	    set t [$cv find closest $x $y $t]
        }
        return 0
    }
    # -------------------------------------------------- 
    protected method debug {msg} {
        LLog csel debug $msg
    }
    # -------------------------------------------------- Common Initialization
}
