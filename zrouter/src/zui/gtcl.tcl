package provide ade::ui 2.0

#
#       Class: GTCL
# Description: Gui TCL language
#      Author: Manolis M. Tsangaris
#
# Interpet user plain commands as zroute commands. Visualize output.
#
# GTCL works using a slave safe interpeter bound to some of its own
# public methods.
#
itcl::class GTCL {
    inherit LObject LBinder
    # Event <<result>>    value         ;# value to be visualized
    # Event <<command>>   cmd args      ;# command to run in the backend
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # show any type of object
    # to be more specific, the user can enter a hint (like -net or -name)
    public method show {args} {
        eval command user_find_show = $args
    }
    public method +show {args} {
        eval command user_find_show + $args
    }

    public method inspect {obj} {
        show_object $obj
        # -- now inspect it
        result [command inspect $obj]
    }
    # -- select an object
    public method select {args} {
        eval command user_find_select = $args
    }
    public method +select {args} {
        eval command user_find_select + $args
    }

    # allow the user to change the bbox
    public method bbox {args} {
	if [string match sel* $args] {
	    eval command view_set_selection user
	} else {
	    eval command view_set $args user
	}
    }

    # -- set the grid fatcor (i.e. how many pixels to aggregate/sample)
    public method gfactor {gf} {
        command gfactor $gf
    }

    # -- source
    public method source {file} {
	if { $fp != "" } { 
	    error "there is another source pending"
	}
	set fp [open $file r]
	$this eSourcedCommand
    }

    # -- help
    public method help {args} {
	Event <<command>> help /ucmds
    }

    # ------------------------------------------------------------------------
    public method Run {args} {
	set cmd [lindex $args 0]
	# -- is it a method ?
	if [$interp Exists $cmd] {
	    $interp Run $args
	} else {
	    error "command not found: $args"
	}
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable interp
    protected variable fp ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set interp [LInterp $this.interp]
        set cmds "show +show inspect select +select bbox gfactor source help"
        foreach cmd $cmds {
            $interp Bind $cmd "$this $cmd"
        }
    }
    destructor {
        $interp delete
    }
    # -------------------------------------------------- Private Methods ---
    private method result {res} {
        Event <<result>> $res
    }
    private method command {args} {
        eval Event <<command>> $args
    }
    # ----------------------------------------------- Interface Implementation
    # the user has asked us to show a chip object name or chip object id
    protected method show_object {obj type} {
        if { $type == "" } {
            set type [type_of $obj]
        }
        switch $type {
            id      {
                # is the object there already? just select it
                if { [result [command select $obj]] } {
                    return ""
                }
                # -- center the object
                result [command center $obj]

                # -- select it
                result [command select $obj]
            }
            default {
                error "cannot show `$obj' yet (of type `$type')"
            }
        }
        return ""
    }
    # -- in general, chip object ids have that form
    protected method type_of {name} {
        if { [regexp {[a-zA-Z][0-9]+\.[a-zA-Z][0-9]+.*} $name] } {
            return id
        }
        return "unknown"
    }

    # find objects and make the results a selection
    protected method gt_select {arg} {
        set matches [obj_find select $arg]
        set res "" 
        foreach obj $matches {
            array set P $obj
            lappend res $P(id)
        }
        result [list type selection obj $res]
    }

    # find objects and display the results 
    protected method gt_show {arg} {
        set res [obj_find select $arg]
        if { $res == "" } { bell ; return }
        array set A [lindex $res 0]
        result [list type inspection obj $A(id) \
            info set/group inspection $res bbox ""]
    }

    # find an object
    protected method obj_find {name arg} {
        if { ( $arg == "" ) } { 
            error [find_usage $name]
        }
        foreach {a b c} $arg { break }
        if { ($a == "") || ($b == "") || ($c == "") } {
            error [find_usage $name]
        }
        command find $a $b $c
    }
    protected method find_usage {name} {
        set list {net name "net by name" net id "net by id"}
        set msg "Usage:\n"
        append msg "  gui-id (to $name any gui object)\n" 
        foreach {a b c} $list {
            append msg "  $name $a $b string (to $name $c)\n" 
        }
        set msg
    }
    # ----------------------------------------------- CallBacks
    # -- read the next line and evaluate
    public method eSourcedCommand {} {
	while { ! [eof $fp] } { 
	    set cmd ""
	    while { [gets $fp line] != -1 } {
		append cmd $line \n
		if [info complete $cmd] { break }
	    }
	    if { $cmd == "" } {continue}
	    eval $cmd
	    update
	}
	close $fp
	set fp ""
    }
    # -- called when a command is completed
    public method eCommandDone {} {
	if { $fp == "" } { return }
    }
    # -------------------------------------------------- Common Initialization
}
