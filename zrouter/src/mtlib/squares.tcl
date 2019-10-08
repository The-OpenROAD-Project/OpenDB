package provide mtlib 1.0
#
#       Class: LSquares
# Description:
#
itcl::class LSquares {
    inherit LFrame LBinder
    # Event <<click1>> tag
    # Event <<click3>> tag
    # Event <<enter>>  tag
    # Event <<leave>>  tag
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method ItemState {item {state ""}} {
        item_state $item $state
    }

    # -- establish a mapping between the item state and the color
    # -- to display it
    public method ItemColors {{st ""}} {
        array set COLOR $st
    }
    
    # -- clear
    public method Clear {} {
        item_clear
    }

    # -- return the currently selected item 
    public method Current {} {
        set n [$cv find withtag current]
        if { $n == "" } { return ""}
        set n [lindex $n 0]
        if [info exists ITEM($n)] {
            return $ITEM($n)
        }
        return ""
    }
    # -- connect the two items
    public method ClearConnect {} {
        $cv delete connections
    }
    public method Connect {a b} {
        item_connect $a $b
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable cv
    protected variable COLOR
    protected variable posx
    protected variable posy
    protected variable SIZE
    protected variable ITEM
    protected variable rescale ""
    protected variable task ""   ;# idletask configure
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

        array set posx "next 0 d 20 max 300"
        array set posy "next 0 d 20 max 300"

        set w $This
        winconfig -border 1 -relief sunken
        set cv [canvas $w.cv -width $posx(max) -height $posy(max)]
        $cv config -bg black
        pack $cv -side top -fill both
        bind $cv <Configure> "$this eScheduleConfig $This"
        $cv bind item <ButtonPress-1> "$this eSelect <<click1>>"
        $cv bind item <ButtonPress-3> "$this eSelect <<click3>>"
        $cv bind item <Enter>         "$this eSelect <<enter>>"
        $cv bind item <Leave>         "$this eSelect <<leave>>"

        LBridge width  "$w.cv configure -width"
        LBridge height "$w.cv configure -height"


    # -- an item has changed state
    # -- if the item does not exist create it
    }
    destructor { }
    public method delete {} { itcl::delete object $this }
    # -------------------------------------------------- Private Methods ---
    protected method item_state {item state} {

        if { [$cv find withtag j:$item] == "" } {
            item_add $item
        }
        if { $state == "" } { return }
        if [catch {set COLOR($state)} cl] {
            puts stderr "warning: cannot map $item/$state to color"
            set cl white
        }
        $cv itemconfig j:$item -fill $cl
    }

    protected method item_add {j {fill gray}} {
        set x1 [expr $posx(next)+$posx(d)]
        set y1 [expr $posy(next)+$posy(d)] 
        set t  [$cv create rectangle $posx(next) $posy(next) $x1 $y1 \
            -tags "item j:$j" -fill $fill -outline black]
        set ITEM($t) $j

        # -- update the next position to plot
        set posx(next) $x1
        if { $x1 >= $posx(max) } {
            set posx(next) 0
            set posy(next) [expr $posy(next) + $posy(d)]
        }

        # -- schedule a rescale when done, just in case we are out
        # -- of the visible canvas
        if { $rescale == "" } {
            set rescale [after idle $this eRescale]
        }
    }

    protected method items_redraw {width height} {
        set items [$cv find withtag item]
        set nitems [llength $items]
        if { $nitems == 0 } {
            return
        }
        # -- initialize
        set posx(next) 0 ; set posy(next) 0

        # -- the total area is ... (90% of the actural)
        set area [expr 0.9*$width * $height]
        # -- the block area is ...
        set sarea [expr $area / $nitems]
        # -- the width of each block
        set dx    [expr int(sqrt($sarea))]

        # -- do not accept a very large block size
        set mdx [expr $width / 10.0]
        if { $dx > $mdx } { set dx $mdx }

        set posx(d) $dx
        set posy(d) $dx

        # -- the number of squares per row and column
        set nx    [expr int(1.0*$width/$dx)]
        set ny    [expr int(1.0*$height/$dx)]

        # -- the width of each row
        set posx(max) [expr $nx * $dx]
        # -- the height of each column
        set posy(max) [expr $ny* $dx]

        # puts "$dx $posx(max) $posy(max)"
        # -- sanity check: 
        if { ($posx(max) > $width) || ($posy(max) > $height) } {
            puts stderr "bbox $posx(x) $pos(y) not fit window ($width $height)"
        }

        foreach t $items {
            set j $ITEM($t)
            set fill [$cv itemcget j:$j -fill]
            $cv delete j:$j
            unset ITEM($t)
            item_add $j $fill
        }
        # -- make sure we do not cause another redraw
        catch {after cancel $rescale}
        set rescale ""
    }

    protected method item_clear {} {
        $cv delete all
        catch {unset ITEM}
        set posx(next) 0 ; set posy(next) 0
        set posx(d) 20 ; set posy(d) 20
    }
    protected method item_connect {a b} {

        if { [scan [$cv bbox j:$a] "%f %f %f %f" x0 y0 x1 y1] != 4 } {
            return 0
        }
        set X1 [expr ($x1 + $x0)/2.0]
        set Y1 [expr ($y1 + $y0)/2.0]

        if { [scan [$cv bbox j:$b] "%f %f %f %f" x0 y0 x1 y1] != 4 } {
            return 0
        }
        set X2 [expr ($x1 + $x0)/2.0]
        set Y2 [expr ($y1 + $y0)/2.0]
        $cv create line $X1 $Y1 $X2 $Y2 -arrow last \
            -fill greenyellow -tags connections
        return 1
    }
    # -----------------------------------------------------------
    # -- EVENT HANDLERS
    # check if the size of the canvas has changed
    public method eConfig {w} {
        # -- ignore the first time we are called
        set x [winfo reqwidth $w]
        set y [winfo reqheight $w]
        if ![info exists SIZE(w)] {
            set SIZE(w) $x
            set SIZE(h) $y
            return
        }
        items_redraw $x $y
        set task ""
    }
    public method eScheduleConfig {w} {
        if { $task != "" } { return }
        set task [after idle "$this eConfig $w"]
    }
    # -- rescale if necessary
    public method eRescale {} {

        set rescale ""

        if ![winfo exists $cv] { return }

        scan  [$cv bbox all] "%f %f %f %f" x0 y0 x1 y1
        set width  [$cv cget -width]
        set height [$cv cget -height]
        set dx     [expr $x1 - $x0]
        set dy     [expr $y1 - $y0]

        # -- no need to rescale
        if { ($dx <= $width) && ($dy <= $height) } {
            return
        }
        items_redraw $width $height
    }
    public method eSelect {mode} {
        set j $ITEM([$cv find withtag current])
        Event $mode $j
    }
    # -----------------------------------------------------------
    public method test {} {
        ItemColors {Q gray R yellow S green F red}

        for {set j 1} { $j < 1000} {incr j} {
            ItemState $j Q
        }

        foreach n "2 3 4 5" {
            after [expr $n * 1000] "$this test_set 1 [expr $n*27]"
            update idletasks ; update
        }
    }
    public method test_set {a b} {
        ItemState $a R
        ItemState $b F
        Connect   $a $b
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
