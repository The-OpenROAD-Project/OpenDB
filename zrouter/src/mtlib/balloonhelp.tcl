package provide mtlib 1.0

#
# Object Name: LBalloonHelp
# Description: Balloon help support
# Heavily based on tix
#
itcl::class LBalloonHelp {
    inherit LTopLevel LBinder
    # Event popup w    -> ePopupWin $w
    # Event popupdown  -> ePopdown
    # Event Help  w    -> eHelp $w
    # -------------------------------------------------- Public Variables --
    public variable active 1              ;# is help active?
    public variable timeout 1000          ;# how long to wait?
    public variable dx 10 {} ;# where to place help window
    public variable dy 10 {} 
    # -------------------------------------------------- Protected Variables 
    protected variable Help
    protected variable task ""
    protected variable UPCALL
    protected variable LAST ""            ;# last window to activate help
    protected variable ACTIVE             ;# remember if popup should be active
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    # -------------------------------------------------- Public Methods ----
    # add balloon help to a window
    public method Add {w msg {initWait 0}} { 
	help_bind $w
	set ACTIVE($w) 0
        set Help($w) $msg
    }

    # -- notify me with an upcall, instead of displaying static text
    # -- the call will receive two extra arguments (x y coordinates)
    public method Notify {w call} {
	help_bind $w
	set UPCALL($w) $call
	set ACTIVE($w) 0
    }

    method Delete {tag} {
	if [info exists Help($tag)] {
	    unset Help($tag)
	}
	if [info exists UPCALL($tag)] {
	    unset UPCALL($tag)
	}
	if [info exists ACTIVE($tag)] {
	    unset ACTIVE($tag)
	}
    }

    # -- process a motion event just in case the windows bindings
    # -- make the normal binding of <motion> not to work
    public method Motion {w} {
	eMotion $w
    }

    # -- popup a window 
    public method Message {msg} {
	# -- in case no window has caused activation
	set x [winfo pointerx $This]
	set y [winfo pointery $This]
	show_info $msg $x $y
    }
    # -- in case the bindings are taken
    public method Leave {w} {
	$this eEnd $w
    }
    public method Enter {w} {
	$this eStart $w
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Private Methods ---
    constructor {args} {
        eval configure $args
        ::option add *LBalloonHelp.Message.background #ffff60
        ::option add *LBalloonHelp.Message.foreground black
        ::option add *LBalloonHelp.Message.width 180

        message $This.msg
        pack $This.msg -expand 1 -fill both
        wm overrideredirect $This 1
        wm withdraw $This

        Bind popup     $this ePopupWin
        Bind popupdown $this ePopdown
    }
    protected method help_bind {w} {
        set class [winfo class $w]
        bind $w <Enter>  +[bind $class <Enter>]
        bind $w <Enter>  "+$this eStart $w"
        bind $w <Leave>  +[bind $class <Leave>]
        bind $w <Leave>  "+$this eEnd $w"
        bind $w <Motion> +[bind $class <Motion>]
        bind $w <Motion> "+$this eMotion $w"
        bind $w <Destroy>  +[bind $class <Destroy>]
        bind $w <Destroy> "+catch {$this eDeleted $w}"
    }
    # -- show a text on a popup window
    public method show_info {hlp x y} {
        $This.msg config -text $hlp
	set xx [expr  $dx + $x]
	set yy [expr  $dy + $y]
        wm geometry $This +$xx+$yy
        wm deiconify $This
        raise $This
	update idletasks
    }
    # -------------------------------------------------- Upcalls

    public method eDeleted {w} {
        catch {unset Help($w)}
    }
    
    public method eStart {w} {
        if !$active { return }
	set ACTIVE($w) 1
    }
    
    # -- we are out of the selected Help window
    public method eEnd {w} {
	set ACTIVE($w) 0
        catch {after cancel $task}
        $this ePopdown
    }
    
    public method eMotion {w} {
        if !$active { return }
	if { !$ACTIVE($w) } { return }
        $this ePopdown
        catch {after cancel $task}
        set task [after $timeout $this eActivate $w]
    }
    
    public method eActivate {w} {
	set LAST $w
	if !$active { return }
        ePopupWin $w
    }
    
    public method ePopupWin {w} {
        set x [winfo pointerx $w]
        set y [winfo pointery $w]
        $this ePopup $w $x $y
    }

    # -- time to popup a help window
    public method ePopup {w x y} {
	# -- cancel any pending popup
        catch {after cancel $task}

	set hlp ""
	# -- if an upcall exists, do not popup anything
	# -- unless there is something returned
	if [info exists UPCALL($w)] {
	    set hlp [uplevel #0 $UPCALL($w) $x $y]
	    if { $hlp == "" } { return }
	} elseif [info exists Help($w)] {
	    set hlp $Help($w)
	    if { $hlp == "-" } {
		set hlp [Event Help $w]
	    }
	}
	# -- anything to display?
	if { $hlp == ""  } { return }
	show_info $hlp $x $y
    }
        
    public method ePopdown {} {
        wm withdraw $This
	set LAST ""
	update idletasks
    }
    # -------------------------------------------------- Common Initialization
}
