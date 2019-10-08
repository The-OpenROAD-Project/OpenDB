package provide mtlib 1.0
#
#       Class: LMenu
# Description: Popup a menu and return the choice
#
itcl::class LMenu {
    inherit LTopLevel LInteractor
    # -------------------------------------------------- Public Variables --
    public variable title "Menu"
    public variable style "menu"   ;# menu or list
    public variable maxelements 10 ;# will use a list then
    public variable width   0      ;# 0 to autosize
    public variable height 10
    # -------------------------------------------------- Public Methods ----
    public method Add {choice name} {
        set m $This.choice[llength [winfo children $This]]
        # label or separator
        if { $choice != "-" } {
            label $m -text $name -justify left -anchor w
            bind $m <ButtonPress-1>   "$this eChoosing $m"
            bind $m <ButtonPress-3>   "$this eChoosing $m"
            bind $m <ButtonRelease-1> "$this eSelect $m $choice"
            bind $m <ButtonRelease-3> "$this eSelect $m $choice"
        } else {
            label $m -text "$name" -justify left -anchor w
        }
        pack $m -side top -fill x
    }
    public method AddChoice {choice name} {
	set id [$wchoices index end]
	$wchoices insert end "$name"
	if { $choice != "-" } { 
	    set CHOICE($id) $choice
	}
    }
    public method Make {descr} {
	set choose ""
	set n [expr $maxelements * 2]
	if { $style != "menu" || ([llength $descr] > $n) } {
	    catch {destroy $wchoices}
	    catch {unset CHOICE}
	    #wm overrideredirect $This 0
	    set wchoices [LScrollableWidget $This.ch listbox\
		-width $width -height $height]
	    pack $This.ch -side top -expand 1 -fill both
	    bind $wchoices <1> "$this eChoiceSelected %x %y"
	    bind $wchoices <3> "$this eChoiceSelected %x %y"
	    button $This.ch.b1 -text Cancel -command "$this Signal 0" 
	    grid $This.ch.b1 -columnspan 2 -sticky ew
	    set n 0
	    foreach {choice name} $descr {
		AddChoice $choice $name
		set n1 [string length $name]
		if { $n1 > $n } {set n $n1}
	    }
	    # -- autosize if the width is 0
	    if { $width == 0 } { 
		$wchoices configure -width $n
	    }
	} else {
	    foreach {choice name} $descr {
		Add $choice $name
	    }
	}
        Post
    }
    public method Select {descr} {
        Make $descr
        Wait
        update idle
        set choose
    }
    public method Post {} {
        set x [winfo pointerx $This] ; incr x -5
        set y [winfo pointery $This] ; incr y -5
        wm  geometry $This +$x+$y
    }
    # -------------------------------------------------- Virtual Methods ---
    public method eChoosing {w} {
        $w configure -relief sunken
    }
    public method eSelect {w choice} {
        $w configure -relief flat
        set choose $choice
        wm withdraw $This
        Signal 1
    }
    # -- are we out of the menu?
    public method eLeave {} {
	# -- no need to do that if in list mode
	if { $wchoices != "" } { return }
        set win [winfo containing -displayof $This \
            [winfo pointerx $This] [winfo pointery $This]]
        if [string match $This* $win] { return }
        # -- since we have left the menu, just cancel
        set choose ""
        wm withdraw $This
        Signal 0
    }
    # -- called when the listbox is selected
    public method eChoiceSelected {x y} {
	set id [$wchoices index @$x,$y]
	if { $id == "" } { Signal 0 ; return }
	if ![info exists CHOICE($id)] { bell; return }
	set choose $CHOICE($id)
	Signal 1
    }
    # -------------------------------------------------- Protected Variables 
    protected variable w
    protected variable choose ""
    protected variable wchoices ""
    protected variable CHOICE             ;# map names to choices
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        winconfig -border 1 -relief groove
        set w(title) [label $This.title -bg gray60]
        pack $w(title) -side top -expand 1 -fill x
        LBridge title "$w(title) configure -text"
        wm overrideredirect $This 1
        bind $This <Leave> "$this eLeave"
    }
    destructor {}
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
