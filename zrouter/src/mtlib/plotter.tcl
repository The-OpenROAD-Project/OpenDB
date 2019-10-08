package provide mtlib 1.0

#
#       Class: LPlotter
# Description: Simple Scolling Plotter
#
itcl::class LPlotter {
    inherit LFrame LBinder
    # Event <<close>>
    # Event <<select-canvas>> x y info
    # -------------------------------------------------- Public Variables --
    public variable window 60    ; # how many points to keep -- 0 no scroll
    public variable scrolltime idle
    public variable scroll 1     ; # should we scroll?
    public variable graphopts  "" ;# bargraph opts
    public variable xticks    "auto"
    public variable yticks    "auto"

    public variable pwidth  400
    public variable pheight 200
    # -------------------------------------------------- Public Methods ----
    method Plot {tag x y} {
        set vx $Vx($tag) ; set vy $Vy($tag)
        set ${vx}(++end) $x
        set ${vy}(++end) $y
        set lastx $x
        if { ($task == "") } {
            set task [after $scrolltime "$this eUpdated"]
        }
    }
    method AddSet {tag label} {
        set n [array size Vx]
        set col  $COLOR([expr $n % 5])
        set sym $SYMBOL([expr $n / 5])
        set vx ::$This.x$tag
        set vy ::$This.y$tag
        set Vx($tag) [blt::vector create $vx]
        set Vy($tag) [blt::vector create $vy]

        $plotter element create $tag -xdata $vx -ydata $vy -symbol $sym
        $plotter element configure $tag -fill $col -label "$label"
    }
    method RemoveSet {tag} {
	$plotter element configure $tag -label ""
	update idletasks
	$plotter element delete $tag
	blt::vector destroy $Vx($tag)
	blt::vector destroy $Vy($tag)
	unset Vx($tag)
	unset Vy($tag)
    }
    method Clear {} {
        foreach tag [array names Vx] {
            $plotter element delete $tag
            blt::vector destroy $Vx($tag)
            blt::vector destroy $Vy($tag)
            unset Vx($tag)
            unset Vy($tag)
        }
        catch {after cancel $task}
    }
    method Configure {args} {
        eval $plotter $args
    }
    method ScrollTo {x} {
        set lastx $x
        if { $task == "" } {
            set task [after $scrolltime "$this eUpdated"]
        }
    }
    method SetPointer {x y} {
        scan [$plotter transform $x $y] "%d %d" X Y
        $plotter crosshairs config -position @$X,$Y
    }
    method InvTransform {x y} {
        $plotter invtransform $x $y
    }
    method Transform {x y} {
        $plotter transform $x $y
    }

    method Show {msg} {
        $winfo config -text $msg
    }
    # -------------------------------------------------- Virtual Methods ---
    protected variable scrollthresh 0
    # -- the data has possibly changed ...
    method eUpdated {} {
        set task ""

        # -- keep lastx near the end of the graph
        set max [$plotter axis cget x -max]
        if { $lastx < $scrollthresh } {
            return
        }

	if { $scroll } {
	    set max [expr $lastx + $window/2]
	    set min [expr $max - $window]
	    set scrollthresh [expr $max - $window/20]
	    $plotter axis config x -min $min -max $max

	    # -- prune data sets that do not fit in the window
	    foreach v [array names Vx] {
		set vx $Vx($v) ; set vy $Vy($v)
		while { ([$vx length] > 0) && ([$vx index  0] < $min) } {
		    $vx delete 0
		    $vy delete 0
		}
	    }
	    # no need to prune elements > max
	    SetPointer $lastx 0
	}
	if { $xticks == "auto" } {
	    auto_ticks x 10
	}
	if { $xticks == "auto" } {
	    auto_ticks y 10
	}
        update idletasks
    }
    # -- called when a button is pressed
    public method eSelect {x y} {
        $plotter element closest $x $y V
        Event <<select-canvas>> $x $y [array get V]
    }

    # -------------------------------------------------- Protected Variables 
    protected variable plotter
    protected variable Vx
    protected variable Vy
    protected variable task ""
    protected variable lastx 0
    protected variable winfo
    # -------------------------------------------------- Common Variables --
    common COLOR
    common SYMBOL
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	package require BLT

        pack [mk_plotter $This.plotter] -side top -expand 1 -fill both
        pack [mk_info $This.info] -side bottom -fill x

        $plotter crosshairs on
        $plotter axis config x -min "" -max ""
        $plotter axis config y -min "" -max ""

	$plotter axis config x -majorticks ""
	$plotter axis config x -minorticks "0.0"
	$plotter axis config y -majorticks ""
	$plotter axis config y -minorticks "0.0"

	$plotter configure -barmode aligned
	LBridge pwidth  "$plotter configure -width"
	LBridge pheight "$plotter configure -height"

        bind $plotter <ButtonPress-1> "$this eSelect %x %y"
    }
    destructor {
        foreach v [array names Vx] { blt::vector destroy $Vx($v) }
        foreach v [array names Vy] { blt::vector destroy $Vy($v) }
        catch {after cancel $task}
    }
    # -------------------------------------------------- Private Methods ---
    method mk_plotter {w} {
        set plotter [eval blt::graph $w $graphopts]
        set w
    }
    method mk_info {w} {
        set winfo [label $w -border 1 -relief sunken -bg gray70]
        set w
    }
    # -- compute auto_ticks, if necessary
    protected method auto_ticks {axis steps} {
	set min [$plotter axis cget $axis -min]
	if { $min == "" } { return }
	set max [$plotter axis cget $axis -max]
	if { $max == "" } { return }
	set stepsize [expr ($max - $min) / $steps]
	$plotter axis config $axis -stepsize $stepsize
    }
    # -------------------------------------------------- Common Initialization
    array set COLOR {
        0 springgreen 1 blue 2 coral
        3 cyan 4 sienna 5 red 6 yellow 7 orange
    }
    array set SYMBOL\
       {0 diamond 1 plus 2 square 3 splus 4 cross 5 circle 6 scross 7 triangle}
    # -------------------------------------------------- TESTS
}
