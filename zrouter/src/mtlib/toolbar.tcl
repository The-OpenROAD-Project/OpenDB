package provide mtlib 1.0
#
#       Class: LLinearToolBar
# Description: Simple toolbar
#
itcl::class LLinearToolBar {
    inherit LFrame
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Add {tag name {group ""}} {
        if { $group == "" } { set group $tag }
        set w $This
        label $w.$tag -text $name -border 1 -relief raised
        bind $w.$tag <1> "$this eButton $tag"
        pack $w.$tag -side left -expand 1 -fill both
        lappend MEMBERS($group) $tag
        set SELECTED($group) ""
        set WIN($tag) $w.$tag
        set GROUP($tag) $group
    }
    public method Get {} {
        array get SELECTED
    }
    public method Set {tag {mode 0}} {
        eButton $tag $mode
    }
    # -------------------------------------------------- Virtual Methods ---
    public method eButton {tag {mode ""}} {
        set group $GROUP($tag)
        if {$mode == ""} {  
            set mode [$WIN($tag) cget -relief]
        }
        foreach b $MEMBERS($group) {
            $WIN($b) configure -relief raised
        }
        switch $mode {
            raised - 1 {
                $WIN($tag) configure -relief sunken
                set SELECTED($group) $tag
            }
            sunken - 0 {
                $WIN($tag) configure -relief raised
                set SELECTED($group) ""
            }
        }
    }
    # -------------------------------------------------- Protected Variables 
    protected variable MEMBERS
    protected variable WIN
    protected variable SELECTED
    protected variable GROUP
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        array set SELECTED {}
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LLinearToolBarMS
# Description: Toolbar with mulitstate buttons
#
itcl::class LLinearToolBarMS {
    inherit LFrame LBinder
    # Event <<update>>   ;# a hint that buttons have been updated
    # -------------------------------------------------- Public Variables --
    public variable timeout 300 ;# milliseconds to wait for buttons to settle
    # -------------------------------------------------- Public Methods ----
    # -- add a button which belongs to a group and has $states (on off)
    # -- buttons that belong on the same group are exclusive
    # -- multistate buttons will circle through ...2,1,0,1,1 ..
    public method Add {tag name {group ""} {states 2}} {
        if { $group == "" } { set group $tag }
        set w $This
        label $w.$tag -text $name -border 1 -relief raised
        bind $w.$tag <1> "$this eButton $tag +"
        pack $w.$tag -side left -expand 1 -fill both
        lappend MEMBERS($group) $tag
        set SELECTED($group) ""
        set WIN($tag) $w.$tag
        set GROUP($tag) $group
        set STATE($tag) 0 
        set STATES($tag) $states
    }
    # get the current state of the buttons
    public method Get {} {
        set res ""
        foreach b [array names STATE] {
            lappend res $b $STATE($b)
        }
        return $res
    }
    public method Set {tag {mode 0}} {
        eButton $tag $mode
    }
    # set with no update
    public method Set! {tag {mode 0}} {
        eButton $tag $mode
        update_cancel
    }
    # clear all buttons
    public method Reset {} {
        foreach tag [array names STATE] {
            eButton $tag 0
        }
        update_cancel
    }
    # remove all buttons
    public method Clear {} {
        foreach tag [array names STATE] {
            destroy $WIN($tag) 
        }
        catch {unset {MEMBERS}}
        catch {unset {WIN}}
        catch {unset {SELECTED}}
        catch {unset {GROUP}}
        catch {unset {STATE}}
        catch {unset {STATES}}
    }
    # -------------------------------------------------- Virtual Methods ---
    # the button has been touched and we need to act
    # + indicates that we would like to circle through all values
    public method eButton {tag {mode "+"}} {
        # find the next state
        if { $mode == "+" } {
            set next [expr ($STATE($tag) + 1) % $STATES($tag)]
        } else {
            set next $mode
        }
        button_set $tag $next

        # now we need to turn off anything else that might be 
        # in the same group as this button

        set group $GROUP($tag)
        # if this button was not selected in this group, deselect the others
        if { ($SELECTED($group) != $tag) && ($SELECTED($group) != "")} {
            button_set $SELECTED($group) 0
        }
        set SELECTED($group) $tag
    }
    # change the appearence of the button, depending on the state
    # 0: normal raised 1
    # 1: normal sunken 1
    # 2: gray   sunken 2
    protected method button_set {tag state} {
        set w $WIN($tag)
        set bd [expr $state * 2]
        switch $state {
            0       { $w configure -relief raised -border 1    -bg "#d9d9d9" }
            default { $w configure -relief sunken -border $bd -bg gray70 }
        }
        # indicate that values have been updated
        update_schedule
        set STATE($tag) $state
    }
    # -------------------------------------------------- Protected Variables 
    protected variable MEMBERS
    protected variable WIN
    protected variable SELECTED
    protected variable GROUP
    protected variable STATE
    protected variable STATES
    protected variable task ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        array set SELECTED {}
    }
    destructor {
        if { $task != "" } { after cancel $task }
    }
    # -------------------------------------------------- Private Methods ---
    # schedule a delayed update event
    protected method update_schedule {} {
        update_cancel
        set task [after $timeout "$this eUpdate"]
    }
    public method eUpdate {} {
        Event <<update>>
        set task ""
    }
    protected method update_cancel {} {
        if { $task != "" } { after cancel $task }
        set task ""
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
