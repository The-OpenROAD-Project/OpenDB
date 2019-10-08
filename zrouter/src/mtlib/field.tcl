package provide mtlib 1.0

#
#       Class: LDField
# Description: Generic Field Object
#
itcl::class LDField {
    inherit LFrame LBinder
    # Event <<set>> value
    # -------------------------------------------------- Public Variables --
    public variable name "field"
    public variable desc ""      {field_desc $desc} ;# field description
    # -------------------------------------------------- Public Methods ----
    public method Set {val} {
        set VALUE $val
        $this field_set $val
    }
    public method Get {val} {
        $this field_get $val
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable VALUE
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method field_set {v} {
    }
    protected method field_get {v} {
        set VALUE
    }
    protected method field_desc {d} {
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#       Class: LDFieldChoice
# Description: Choice field
#      Author: Manolis M. Tsangaris  -- mmt@sinastria.gr
#
itcl::class LDFieldChoice {
    inherit LDField
    # -------------------------------------------------- Public Variables --
    public variable maxitems 40               ;# if less than that, use a menu
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable W
    private variable CHOICE                        ;# list of choices
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set W $This.choose
        if { [llength $desc] > $maxitems }  {
            label $W -border 1 -relief raised
            bind $W <1> "$this eChoices $This.choicespopup"
        } else {
            menubutton $W -menu $W.m -text - -border 1 -relief raised
            set m [menu $W.m -tearoff 0]
            foreach {tag name} $desc {
                $m add command -label $name -command [list $this eChoice $tag]
            }
        }
        pack $W -side top -expand 1 -fill both
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method field_set {tag} {
        if ![info exists CHOICE($tag)] {
            $W configure -text "$tag"
        } else {
            $W configure -text $CHOICE($tag)
        }
    }
    protected method field_desc {d} {
        array set CHOICE $d
    }
    # ----------------------------------------------- CallBacks
    # display popup
    public method eChoices {w} {
        set p [LMMPopupChoice $w -choices $desc]
        LPlaceCenter $p
        if [$p Wait] {
            set tag [$p Get]
            $p delete
            Set $tag
            Event <<set>> $tag
        } else {
            $p delete
        }
    }
    public method eChoice {tag} {
        Set $tag
        Event <<set>> $tag
    }
    # -------------------------------------------------- Common Initialization
}

#
# ===========================================================================
#                                                                      POPUPS
# ===========================================================================
#

#       Class: LMMPopup
# Description: Multimodal Popup
#      Author: Manolis M. Tsangaris -- mmt@sinastria.gr
#
itcl::class LMMPopup {
    inherit LTopLevelEditor LInteractor
    # -------------------------------------------------- Public Variables --
    public variable title "CHOICE"
    # -------------------------------------------------- Configuration -----
    public method Wait {} {
        update idletasks
        grab set $This
        set rc [LInteractor::Wait]
        grab release $This
        set rc 
    }
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        # wm overrideredirect $This 1
    }
    destructor {
        grab release $This
    }
    # -------------------------------------------------- Private Methods ---
    # ---------------------------------------------------- Callbacks
    public method vMakeButtons {w} {
        button_add $w.sel -text "≈–…Àœ√«" -command "$this eSelect"
        button_add $w.can -text "¡ ’—Ÿ”«" -command "$this eCancel"
        set w
    } 
    # the user confirmed selection
    public method eSelect {} {
        Signal 1
    }
    # the user confirmed cancelled
    public method eCancel {} {
        Signal 0
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: LMMPopupChoice
# Description: Multimodal Popup for choices
#      Author: Manolis M. Tsangaris -- mmt@sinastria.gr
#
itcl::class LMMPopupChoice {
    inherit LMMPopup
    # -------------------------------------------------- Public Variables --
    public variable choices ""
    # -------------------------------------------------- Public Methods ----
    public method Get {} {
        set VAL
    }
    # -------------------------------------------------- Virtual Methods ---
    # a selectoin has been made (OK or cancel follow)
    public method eSel {index} {
        array set C [$sel Get $index]
        set VAL $C(tag)
    }
    # a choice has been made with double click
    public method eChoice {index} {
        array set C [$sel Get $index]
        set VAL $C(tag)
        eSelect
    }
    # -------------------------------------------------- Protected Variables 
    protected variable sel
    protected variable VAL ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

        set sel [LScrollableTable $This.ch -maxlines 20 -scroll 1]
        pack $This.ch -side top -expand 1 -fill both
        
        $sel AddColumn info "$title" 30
        # the record column (not displayed) holds the matching record
        $sel Bind click        $this eSel
        $sel Bind double-click $this eChoice
        foreach {tag name} $choices {
            $sel Add [list info $name tag $tag]
        }
    }
    destructor {
        $sel delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------------- Callbacks
    # -------------------------------------------------- Common Initialization
}

#       Class: LMMPopupCal
# Description: Multimodal Popup for calendar
#      Author: Manolis M. Tsangaris -- mmt@sinastria.gr
itcl::class LMMPopupCal {
    inherit LMMPopup
    # -------------------------------------------------- Public Variables --
    public variable month 1
    public variable year 1
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    public method eSelect {c} {
        Signal $c/$month/$year
    }
    public method eGotoMonth {step} {
        incr month $step
        if { $month == 0} {
            incr year -1 ; set month 12
        }
        if { $month == 13} {
            incr year 1 ; set month 1
        }
        cal_show $month $year
    }
    # -------------------------------------------------- Protected Variables 
    protected variable sel
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        cal_show $month $year
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    method cal_show {month year} {
        catch {destroy $This.cal}
        set ww [cal_make $This.cal $month $year]
        pack $ww -side top -expand 1 -fill both -ipadx 10 -ipady 10
    }
    private method cal_make {w month year} {
        frame $w -border 1 -bg gray70 -relief groove
        set fp [open "|cal $month $year" r]
        gets $fp title
        set title [string trim $title]

        frame $w.tl
        pack [label $w.tl.back -text "<"] -side left
        pack [label $w.tl.title -text $title -bg gray50\
            -justify c -fg white -anchor c]\
            -side left -expand 1 -fill x
        pack [label $w.tl.next -text ">"] -side right
        grid $w.tl -columnspan 7 -sticky ew
        bind $w.tl.back <ButtonRelease-1> "$this eGotoMonth -1"
        bind $w.tl.next <ButtonRelease-1> "$this eGotoMonth 1"

        gets $fp days
        set N 0
        set wd ""
        foreach c $days {
            set ww $w.dl[incr N]
            label $ww -text $c -width 2 -border 1 -relief groove
            append wd "$ww "
        }
        eval grid $wd
        while { [gets $fp line] != -1 } {
            set wd ""
            foreach c $line {
                set ww $w.dl[incr N]
                label $ww -text $c -width 2 -border 1 -relief groove
                bind $ww <ButtonRelease-1> "$this eSelect $c"
                append wd "$ww "
            }
            if {$wd != "" } { 
                eval grid $wd
            }
        }
        close $fp
        set w
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: LMMPopupTime
# Description: Multimodal Popup for time of day
#      Author: Manolis M. Tsangaris -- mmt@sinastria.gr
#
itcl::class LMMPopupTime {
    inherit LMMPopup
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    public method eSelect {} {
        Signal "$sel_time $sel_period"
    }
    public method eSelectPeriod {w} {
        set sel_period $SEQ([$w cget -text])
        $w config -text $sel_period
    }
    public method eChoose {cv} {
        foreach t [$cv find withtag current] {
            if ![info exists TIME($t)] { return }
            set sel_time $TIME($t)
            return [$this eSelect]
        }
        $this eSelect
    }
    # -------------------------------------------------- Protected Variables 
    protected variable sel_time ""
    protected variable sel_period "AM"
    protected variable TIME
    # -------------------------------------------------- Common Variables --
    common SEQ
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        pack [clock_make $This.cal] -side top -expand 1 -fill both
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    private method clock_make {cv} {
        canvas $cv -bg gray80 -border 1 -relief sunken
        set  R 50
        set x0 [expr $R + 5]
        set y0 [expr $R + 5]
    
        $cv create oval \
            [expr $x0 - $R] [expr $y0 - $R] [expr $x0 + $R] [expr $y0 + $R]
    
        for {set i 0} {$i<=12} {incr i} {
            set th [expr 2 * 3.1428/12.0*$i]
            set x  [expr $x0 + 0.8*$R * sin($th)]
            set y  [expr $y0 - 0.8*$R * cos($th)]
            set x1 [expr $x0 + $R * sin($th)]
            set y1 [expr $y0 - $R * cos($th)]
            set n [$cv create line $x $y $x1 $y1 -width 4\
                 -fill red -tags "tick"]
            set TIME($n) $i
        }
        scan [$cv bbox all] "%f %f %f %f" X0 Y0 X1 Y1
        $cv config  -width [expr $X1 - $X0] -height [expr $Y1 - $Y0]
        label $cv.period -text AM
        $cv create window $x0 $y0 -window $cv.period
        bind $cv.period <ButtonRelease-1> "$this eSelectPeriod $cv.period"
        bind $cv        <ButtonRelease-1> "$this eChoose $cv"
    
        $cv bind tick <ButtonRelease-1>   "$this eChoose $cv"
        set cv
    }
    # -------------------------------------------------- Common Initialization
    array set SEQ {
        "-" AM
        AM PM
        PM Evening
        Evening Morning
        Morning Afternoon
        Afternoon "-"
    }
}
