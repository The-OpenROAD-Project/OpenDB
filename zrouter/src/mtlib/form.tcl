package provide mtlib 1.0

#       Class: LForm
# Description: Form with fields
#
itcl::class LForm {
    inherit LFrame LBinder
    # Event <<modified>>
    # -------------------------------------------------- Public Variables --
    public variable balloonhelp 0          ;# enable ballon help
    # -------------------------------------------------- Public Methods ----
    # -- show a record
    public method Show {rr} {
        upvar $rr R
        form_clear
        form_show R
    }
    # -- add a field
    public method Add {tag type def name {help ""}} {
        form_add $tag $type $def $name $help
    }
    # -- get (all) field values
    public method Get {{attr ""}} {
        if { $attr == "" } { return [array get VAL ] }
        set VAL($attr)
    }
    # -- set all fields
    public method Set {val} {
        foreach {a v} $val {
            field_set $a $v
        }
    }
    # -- clear form
    public method Clear {} {
        form_clear
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable MAXF 8
    protected variable NF 0
    protected variable TYPE
    protected variable WIN
    protected variable WBASE
    protected variable VAL
    protected variable CHOICE
    protected variable READONLY                      ;# is the field disabled?
    protected variable OBJ                           ;# keep track of objects
    protected variable BH ""                         ;# balloon help
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        winconfig -border 1 -relief sunken -pady 10 -padx 10
        pack [mk_form $This.f] -side top -expand 1 -fill both
        set WBASE $This.f
	set BH [LBalloonHelp $This.bh]
	LBridge balloonhelp [list [namespace origin $BH] configure -active]
    }
    destructor {
        foreach o [array names OBJ] {
            $o delete
        }
	$BH delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mk_form {w} {
        frame $w
        set w
    }
    protected method form_clear {} {
    }
    protected method form_show {rr} {
        upvar $rr R
        foreach tag [lsort [array names R]] {
            field_set $tag $R($tag)
        }
    }
    protected method form_add {tag type def name help} {
        set w $WBASE.f$NF
        set WIN($tag) $w-e
        set TYPE($tag) $type
        set READONLY($tag) 0
	set bw "" ;# -- balloon help windows
        switch $type {
            choice {
                label $w-l -text "$name:" -justify right
                set choices [eval $def]
                set m  $w-e
                LDFieldChoice $m -desc $choices
                $m Bind <<set>> $this eChoiceSet $tag
                set OBJ($m) ""
		set bw "$m $w-l"
            }
            button {
                button $w-e -text $name -command "$this Event <<goto>> $tag"
		set bw "$w-e"
            }
            note {
                label $w-l -text "$name:" -justify right
                text  $w-e -width 20 -height 3
                bind $w-e <Leave>  "$this eValidate $tag"
                bind $w-e <Enter>  "focus $w-e"
		set bw "$w-l $w-e"
            }
            default {
                label $w-l -text "$name:" -justify right
                entry $w-e 
                bind $w-e <Enter>  "focus $w-e"
                bind $w-e <Leave>  "$this eValidate $tag"
                bind $w-e <Return> "$this eValidate $tag"
                if { $def == "const" } { $w-e configure -state disabled }
		set bw "$w-l $w-e"
            }
        }
	if { $help != "" } { 
	    foreach wh $bw {
		$BH Add $wh $help
	    }
	}
        if { $def == "const" } { set READONLY($tag) 1 }

        set VAL($tag) ""
        if [winfo exists $w-l] {
            grid  $w-l -column [expr 2*($NF/$MAXF)+0] -row [expr $NF % $MAXF]\
                -sticky ens
        }
        grid  $w-e -column [expr 2*($NF/$MAXF)+1] -row [expr $NF % $MAXF]\
            -sticky ew
        incr NF
    }
    protected method field_set {tag value} {
	# -- ignore undefined fields
        if ![info exists WIN($tag)] {
	    set VAL($tag) $value
            return
        }
        switch $TYPE($tag) {
            choice {
                $WIN($tag) Set $value
            }
            date {
                set value [lindex $value 0]
                $WIN($tag) delete 0 end
                $WIN($tag) insert 0 $value
            }
            button {
            }
            note {
                $WIN($tag) delete 0.0 end
                $WIN($tag) insert 0.0 $value
            }
            default {
                $WIN($tag) configure -state normal
                $WIN($tag) delete 0 end
                $WIN($tag) insert 0 $value
                if { $READONLY($tag) } { $WIN($tag) configure -state disabled }
            }
        }
        set VAL($tag) $value
    }
    # -------------------------------------------------------------- VALIDATION
    # ----------------------------------------------- Interface Implementation
    public  proc validate_date {str} {
	if { ([scan $str "%d/%d/%d" y m d] == 3) ||\
	    ([scan $str "%d-%d-%d" y m d ]== 3)} {
	    set dt "$y-$m-$d"
	} elseif {([scan $str "%d/%d" m d] == 2) ||\
	    ([scan $str "%d-%d" m d]== 2) } {
	    set y [clock format [clock seconds] -format %Y]
	    set dt "$y-$m-$d"
	} elseif {([scan $str "%d" d] == 1) || ([scan $str "%d" d]== 1) } {
	    set y [clock format [clock seconds] -format %Y]
	    set m [clock format [clock seconds] -format %m]
	    set dt "$y-$m-$d"
	} else {
	    return ""
	}
    }
    # ----------------------------------------------- CallBacks
    # a choice on a choice-field has been made
    public method eChoiceSet {tag ctag} {
        field_set $tag $ctag
        Event <<modified>> $tag
    }

    # -- after leaving a field validate it
    public method eValidate {tag} {
        set last $VAL($tag)
        switch $TYPE($tag) {
            choice { }
            date {
                set dt [string trim [$WIN($tag) get]]
                if { $dt == "" } {
                    set VAL($tag) $dt
                    return
                }
                set dt [validate_date $dt]
                if { $dt == "" } {
                    bell ; return
                }
                field_set $tag $dt
            }
            note {
                set val [string trim [$WIN($tag) get 0.0 end]]
                field_set $tag $val
            }
            default {
                set val [string trim [$WIN($tag) get]]
                field_set $tag $val
            }
        }
        if { $last != $VAL($tag) } {
            Event <<modified>> $tag
        }
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: LArrayEditor
# Description: Edit an Array
#      Author: Manolis M. Tsangaris  -- mmt@sinastria.gr
#
itcl::class LArrayEditor {
    inherit LApplication
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Virtual Methods ---
    public method Edit {vv {mask *} {vartype ""}} {
        upvar $vv V
        array set TYPE $vartype
        foreach f [lsort [array names V $mask]] {
            if [info exists TYPE($f)] {
                set type $TYPE($f)
            } else {
                set type text
            }
            $form Add $f $type - [string toupper $f]
            $form Set [list $f $V($f)]
        }
        if ![Wait] { return 0 }
        array set V [$form Get]
        return 1
    }
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable form
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set form [LForm $This.form]
        pack $form -side top -expand 1 -fill both
    }
    destructor {
        $form delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method mSave {} {
        Signal 1
    }
    public method mCancel {} {
        Signal 0
    }

    public method vMakeButtons {w} {
        button_add $w.s       -text ACCEPT  -command "$this mSave"
        button_add_right $w.q -text CANCEL  -command "$this mCancel"
        set w
    }
    # -------------------------------------------------- Common Initialization
}
