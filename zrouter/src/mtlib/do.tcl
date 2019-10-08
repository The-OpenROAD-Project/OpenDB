#
# simple flow tool
#
#
package provide mtlib 1.0

set gCMDS ""

proc do {args} {
    global gCMDS
    set out ""

    # -- list all commands
    if { $args == "" } {
        append out Commands:\n
        foreach {name cmd} $gCMDS {
            append out [string trim $name] \n
        }
        return $out
    }

    # -- get help on commands
    set ncmd [lindex $args 0]
    switch -glob -- @$ncmd {
	@-v { 
	    global gVDO
	    package require Tk
	    wm withdraw .
	    if ![info exists gVDO] {
		set gVDO [LVisualDo .vdo#auto -title "Visual Do"]
	    } else {
		$gVDO UnHide
	    }
	    $gVDO configure -cmds $gCMDS
	}
        @- - @-h* { 
            append out {usage:
        do: to see all possible commands
    do cmd: to execute a command
    do -v : to invoke the visual do menus
do {
    cmdname "command1 arg1 ..."
    cmdname "command2 arg1 ..."
}            to define named commands
}
            return $out
        }
    }

    # -- execute a command
    foreach {name cmd} $gCMDS {
        if { $ncmd == $name } {
            puts do:$cmd
            uplevel 1 $cmd
            return
        }
    }
    set def [lindex $args 0]
    # -- define commands
    if ![catch {llength $def} err] {
        foreach {name cmd} $def {
            lappend gCMDS $name [string trim $cmd]
        }
        return
    }
    error "$ncmd: command not found"
}


#
#       Class: VDO: visual do for ADE
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class LVisualDo {
    inherit LTopLevel
    # -------------------------------------------------- Public Variables --
    public variable cmds "" { vd_setup $cmds }
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable menus ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set menus $This.menus
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method vd_setup {cmds} {
	catch {destroy $menus}
	frame $menus
	foreach {name cmd} $cmds {
	    vd_add $menus $name $cmd
	}
	vd_add $menus Quit quit
	pack $menus -side top -expand 1 -fill both
    }
    protected method vd_add {w name cmd} {
	set N [llength [winfo children $w]]
	set b $w.b[incr N]
	button $b -text $name -command [list $this eCmd $b $cmd]
	pack $b -side top -fill x
    }
    # ----------------------------------------------- CallBacks
    public method eCmd {w cmd} {
	if [catch {uplevel #0 $cmd} rc] {
	    $w configure -bg red -activebackground orange
	    error $rc
	} else {
	    $w configure -bg seagreen -activebackground green
	}
    }
    # -------------------------------------------------- Common Initialization
}
