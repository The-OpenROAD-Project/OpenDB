package provide mtlib 1.0
#
#
# Interesting, program development tools
#

# use vi to edit a file, source it again, or to edit a function

proc vi {{what ""}} {
	global env 
	global _vi_last
	if { $what == "" } {
		if ![info exists _vi_last] { return }
		set what $_vi_last
	} else {
		set _vi_last $what
	}
	if [info exists env(DISPLAY)] {
		set cmd "xterm -e vi "
	} else {
		set cmd "vi </dev/tty >/dev/tty 2>/dev/tty "
	}
	if [file exists $what] { 
		set file $what
		set mtime [file mtime $file]
		if [catch {eval exec "$cmd $file"} rc] {
			puts "vi $file: $rc" 
			return
		}
		# any modifications?
		if { [file mtime $file] == $mtime } { return }
		# is it a tcl file?
		if {![string match *.tcl $file]} { return }
		uplevel #0 source $what
		return
	}
	error "vi: I do not know how to edit '$what'"
}

#
# Description: Simple debugger
#
namespace eval LDebug {

variable SLEVEL                ;# STARTING LEVEL OF DEBUGGING
variable MAXLEVEL  100000      ;# MAXIMUM LEVEL TO TRACE
variable NEXTLEVEL 0           ;# STOP TRACING UNTIL YOU ENCOUNTER THIS LEVEL
variable PROMPT  "dbg> "
variable DONTTRACE 0           ;# temporarily do not trace
                               ;# used when we execute debugger tasks
variable BREAKP                ;# breakpoints to stop

namespace export dbg

proc init {} {
}

#
#  dbg: start a debugging sesion
# args: the command to debug
#
proc dbg {args}  {
	set main [lindex $args 0]
	if { $main == "" } {
		error "usage: dbg cmd args ..."
	}

	variable SLEVEL
	set SLEVEL [expr [info level] -1]  ;# note the level of debugging
	variable BREAKP
	catch {unset BREAKP}

	trace add execution $main enter     "[namespace code eEnter]"
	trace add execution $main leave     "[namespace code eLeave]"
	trace add execution $main enterstep "[namespace code eEnterStep]"
	trace add execution $main leavestep "[namespace code eLeaveStep]"
	
	set n [catch {uplevel $args} rc]

	trace remove execution $main enter     "[namespace code eEnter]"
	trace remove execution $main leave     "[namespace code eLeave]"
	trace remove execution $main enterstep "[namespace code eEnterStep]"
	trace remove execution $main leavestep "[namespace code eLeaveStep]"

	if { $n } {
		error "dbg $args: $rc"
	}
	set rc
}

# -------------------------------------------------------------------------
# printed a tabbed line (based on level)
proc tab {label fmt args} {
	set l [info level] ; incr l -2
	set tab [string repeat ">" $l]
	puts [format "%s %8s: %s" $tab $label [eval format [list $fmt] $args]]
}

proc slimit {string len} {
	if { [string len $string] <= $len } { return $string }
	return "[string range $string 0 [incr len -2]].."
}
# -- print a cmd and args
proc scmd {cmd len} {
	if { [string len $cmd] <= $len } { return $cmd }
	set out "[lindex $cmd 0]"
	foreach arg [lrange $cmd 1 end] {
		if { [string length $arg] > 10 } {
			append out [string range $arg 0 8] "..  "
		} else {
			append out $arg " "
		}
	}
	return $out
}

# -------------------------------------------------------- TRACE EVENTS HANDLERS
# we enter a command being traced
proc eEnter {cmdstring _} {
	variable DONTTRACE ; if {$DONTTRACE} return
	tab entering "- %s" $cmdstring
	set level [expr [info level] -2]
	prompt $level
}

# we leave a command being traced
proc eLeave {cmdstring code result _} {
	variable DONTTRACE ; if {$DONTTRACE} return
	set level [expr [info level] -2]
	if { $code == 0 } {
		set code ""
	} else {
		set code " (ERROR)"
	}
	tab leaving {[%s] = %s%s} [scmd $cmdstring 30] [slimit $result 30] $code
	prompt $level
}

# we enter a command below the top traced command
proc eEnterStep {cmdstring _} {
	variable DONTTRACE ; if {$DONTTRACE} return

	set level [expr [info level] -2]

	set r [stop_in [lindex $cmdstring 0] $level]
	if { $r == "" } { return }

	tab breakp  "%s" $r
	tab calling "%s" [scmd $cmdstring 30]
	prompt $level
}


# we leave a command below the traced command
proc eLeaveStep {cmdstring code result _} {
	variable DONTTRACE ; if {$DONTTRACE} return
	variable MAXLEVEL 

	set level [expr [info level] -2]

	if { $level > $MAXLEVEL } { return }

	if { $code == 0 } {
		set code ""
	} else {
		set code " (ERROR)"
	}
	tab returning {[%s] = %s%s} [scmd $cmdstring 30]\
		[slimit $result 30] $code
	# prompt $level
}
# ---------------------------------------------------------- UI
# ---------------------------------------------------------- INSPECT STATE
proc print_context {level} {
	variable DONTTRACE
	puts "level: $level"
    set cmd [lindex [info level $level] 0]
	if { $cmd == "" } {
		puts "warning:no proc at level $level"
		return
	}
	# are we within an object context?
	set mdef [find_method $level]
	if { $mdef != "" } {
		puts $mdef
		return
	}
	if {[catch {info args $cmd} args]} { 
		puts "$cmd: builtin (probably)"
		return
	}
	puts "proc $cmd {$args} {[info body $cmd]}"
}

proc find_method {level} { 
	if { [catch {uplevel #$level set this} this] } { 
		return ""
	}
	set method [lindex [info level $level] 0]
	set mdef ""
	append mdef "$this ([$this info class])"
	append mdef "	[$this info function $method -protection] method $method"
	append mdef " {[$this info function $method -args]}"
	append mdef " {[$this info function $method -body]}"
	return $mdef
}

proc print_vars {level} {
	foreach v [uplevel #$level info vars] {
		puts "$v [uplevel #$level set $v]"
	}
}

# print stack from the given level up to the top
proc print_stack {level} {
    set number 1
    while { $level > 0 } {
       puts [format "%2d:%d: %s" $number $level [info level $level]]
       incr number
       incr level  -1
    }
}

proc do_cmd {level cmd} {
	puts $level:$cmd
	if { [catch {uplevel #$level $cmd} rc] } {
		puts "error: $rc"
	} else {
		puts "$rc"
	}
}

proc do_breakp {arg} {
	set arg [lrange $arg 1 end]
	variable BREAKP
	if { $arg == "" } {
		foreach f [lsort [array names BREAKP]] {
			puts "$f"
		}
		return
	}
	foreach a $arg {
		set BREAKP($a) 0
	}
}

# clear breakpoints -- accept regular (glob) expressions
proc do_clearbp {arg} {
	set arg [lrange $arg 1 end]
	variable BREAKP
	foreach ap $arg {
		foreach a [array names BREAKP $ap] {
			catch {unset BREAKP($a)}
		}
	}
}

# should we stop in this function?
# are there breakpoints?
# Return the reason
proc stop_in {cmd level} {
	variable BREAKP
	variable MAXLEVEL 

	if [info exists BREAKP($cmd)] {
		return "in function $cmd"
	}
	# is this an object?
	if { [catch {$cmd info heritage} heritage] } { return "" }

	set obj [namespace tail $cmd]
	if [info exists BREAKP($obj)] {
		return "in object $obj"
	}

	# handle breaking inside a class hierarchy
	foreach class $heritage {
		set class [namespace tail $class]
		if [info exists BREAKP($class)] {
			return "in class $class (of $obj)"
		}
	}

	# if we are deeper than needed, no debug
	if { $level > $MAXLEVEL } { return "" }

	return "at level $level"
}


proc prompt {level} {
	variable MAXLEVEL
	variable PROMPT
	variable DONTTRACE

	# disable tracing ... just in case
	set nt $DONTTRACE
	set DONTTRACE 1

	while 1 {
		puts -nonewline "$PROMPT" ; flush stdout
		if { [gets stdin line] == -1 } {
			# do not trace anymore
			return
		}
		set line [string trim $line]
		switch -glob -- $line {
			s - '' { set MAXLEVEL  10000  ; break }
			n      { set MAXLEVEL  $level ; break }
			c      { set MAXLEVEL  0      ; break }
			w {
				print_stack $level
			}
			l { 
				print_context $level
			}
			v { print_vars $level }
			!!* { do_cmd [info level] [string range $line 2 end] } 
			!0* { do_cmd 0            [string range $line 2 end] } 
			!1* { do_cmd 1            [string range $line 2 end] } 
			!2* { do_cmd 2            [string range $line 2 end] } 
			!3* { do_cmd 3            [string range $line 2 end] } 
			!4* { do_cmd 4            [string range $line 2 end] } 
			!*  { do_cmd $level       [string range $line 1 end] } 
			b*  { do_breakp $line }
			B*  { do_clearbp $line }
			* { usage }
		}
	}
	set DONTTRACE $nt
}

proc usage {} {
	puts {
s             : to step
l             : list current execution context
v             : print current variables
n             : to step and skip a call
w             : print the stack
!             : run a command in the current context
!!            : run a command in the dbg context
!0            : run a command in the toplevel context
!n            : run a command in the n-th level (n=0-4) context
b function(s) : break in function, class, or object
b             : list breakpoints
B function(s) : clear breakpoints
}
}

}

