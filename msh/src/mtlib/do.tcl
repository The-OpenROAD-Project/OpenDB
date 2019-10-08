#
# simple flow tool
#
#

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
		@- - @-h* { 
			append out {usage:
        do: to see all possible commands
    do cmd: to execute a command
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
