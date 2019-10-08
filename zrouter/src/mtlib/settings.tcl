package provide mtlib 1.0

#       Class: LSettings
# Description: Generic settings dialog
#
# Edit and change settings
#
itcl::class LSettingsDialog {
    inherit LTopLevelEditor LInteractor
    # -------------------------------------------------- Public Variables --
    public variable descr {}      { mk_form $descr }
    public variable balloonhelp 0                    ;# -- enable balloon help?
    # -------------------------------------------------- Public Methods ----
    public method Set {array} {
	$form Set $array
    }
    public method Get {} {
	$form Get
    }
    # -- validate the current settings
    public method Validate {} {
	if ![$this eValidate [$form Get] R] {
	    LMessage $This.msg $R(msg)
	    return 0
	}
	return 1
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable form
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set form [LForm $This.form]
	pack $form -side top -expand 1 -fill both
	LPropagateVar balloonhelp [namespace origin $form]
	$form winconfig -padx 10 -pady 10
    }
    destructor {
	$form delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mk_form {conf} {
	foreach line [split $conf \n] {
	    set line [string trim $line]
	    if { $line == "" } { continue }
	    foreach {field tag type def name help} $line { break }
	    if { $field != "field" } { continue }
	    $form Add $tag $type $def $name $help
	    unset field tag type def name help
	}
    }
    # ----------------------------------------------- Interface Implementation
    # -- validate the new settings
    protected method eValidate {array rr} {
	upvar $rr R
	set R(msg) "settings accepted"
	return 1
    }
    # ----------------------------------------------- CallBacks
    public method vMakeButtons {w} {
        button_add       $w.ok   -text "Accept"  -command "$this Signal 1"
        button_add_right $w.quit -text "Quit"    -command "$this Signal 0"
	set w
    }
    # -------------------------------------------------- Common Initialization
}
