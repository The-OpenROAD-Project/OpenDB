package provide mtlib 1.0

#       Class: LPopupTopLevel
# Description: Popup a window and manage it (not by window manager)
# TODO: If we move its parent, we move this window as well
#
itcl::class LPopupTopLevel {
    inherit LTopLevel LBinder
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Post {{x ""} {y ""} } {
        if { $x == "" } {
            set x [winfo pointerx $This]
            set y [winfo pointery $This]
        }
        incr y -5
        incr x -5
        wm  geometry $This +$x+$y
    }
    # -------------------------------------------------- Virtual Methods ---
    # -- are we out of the menu?
    public method eLeave {} {
        set win [winfo containing -displayof $This \
            [winfo pointerx $This] [winfo pointery $This]]
        if [string match $This* $win] { return }
        Event <<leave>>
    }
    # -------------------------------------------------- Protected Variables 
    protected variable wtitle
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        winconfig -border 1 -relief groove
        #wm overrideredirect $This 1
        wm transient $This
        bind $This <Leave> "$this eLeave"
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}


#       Class: LPopupText
# Description: Popup text
#
itcl::class LPopupText {
    inherit LPopupTopLevel
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Print {msg} {
        $text insert end $msg
    }
    public method Clear {} {
        $text delete 0.0 end
    }
    public method Config {args} {
        eval $text configure $args
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable text
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set text [text $This.text]
        pack $text -side top -expand 1 -fill both
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
