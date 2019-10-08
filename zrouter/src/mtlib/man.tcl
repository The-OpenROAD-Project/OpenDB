package provide mtlib 1.0


#
# simple documentation
#
namespace eval LMAN {

variable MAN  ;# where to store documentation

variable SECTIONS { 
	NAME SYNOPSIS OPTIONS DESCRIPTION OPTIONS BUGS SEEALSO FILES AUTHOR
}


namespace export man

# will export the documentation commands at the top level
proc export {} {
    namespace eval :: namespace import [namespace current]::*
}

proc man {args} {
	variable MAN
	switch [llength $args] {
		1 {
			man_show [lindex $args 0]
		}
		2 {
			man_set [lindex $args 0] [lindex $args 1]
		}
		default {
			set res "" 
			foreach cmd [array names MAN] {
				lappend res [namespace tail $cmd]
			}
			lsort $res
		}
	}
}

proc man_show {topic} {
	variable MAN
	foreach t "$topic ::$topic" { 
		if [info exists MAN($t)] {
			return [man_render $topic $MAN($t)]
		}
	}
	return "man: no information for $topic"
}

proc man_set {topic info} {
	variable MAN
	set ns [uplevel 2 namespace current]
	array set P $info
	foreach a [array names P] {
		set P($a) [trim_tab [split $P($a) \n]]
	}
	set MAN($topic) [array get P]
}

#
# ---------------------------------------------------------------------
#
# remove minimum leading tab (if any)
proc trim_tab {lines} {
		set tab "                            "
		foreach l $lines {
			if { [string trim $l] == "" } { continue }
			if ![regexp {(^[	]+)} $l _ s] { set tab "" ; break }
			if { [string length $s] < [string length $tab] } {
				set tab $s
			}
			unset s
		}
		set out ""
		set n [string length $tab]
		foreach l $lines {
			set l [string range $l $n end]
			regsub -all {^[	]+} $l "    " l
			append out $l \n
		}
		set out
}

proc man_render {topic info} {
	variable SECTIONS
	if [catch {array set M $info}] {
		return "$topic: (** free text ***)\n$info"
	}
	set out ""
	foreach section $SECTIONS {
		set section [string tolower $section]
		if ![info exists M($section)] { continue }
		append out "[string toupper $section]\n"
		set n 0
		foreach line [split $M($section) \n] {
			if { !$n && ($line == "") } { incr n ; continue }
			append out "   $line\n"
			incr n
		}
	}
	set out
}
}

# -- example follows
if 0 {
man Show {
NAME { Show -- show builtin documentation }
SYNOPSIS {
Show topic
}
DESCRIPTION {
The purpose of Show is to display documentation about a particular
tcl function available to the user.
The documentation is entered using the Set command.
}
OPTIONS {
None
}

}
man Set {
NAME { Show -- show builtin documentation }
SYNOPSIS {
	Set function-name documentation
}
DESCRIPTION {
The purpose of Set is to define the documentation for a particular
tcl function available to the user.
The format of the documentation is directly taken out of the unix
man command, and it is simply a set of value attribute pairs like the
example below:

	NAME { foo -- the most common name of programmers }
	SYNOPSIS { foo }
	DESCRIPTION {
		The origin of foo is lost in the past, and it is probably related to
		the old typewriters
	}
	OPTIONS {
		none
	}
	BUGS { none }
	AUTHOR {Mr. Donald Foo}
	
	SEEALSO { see also bar }
	}

	OPTIONS {
	None
	}

}

puts [LMAN::man LMAN::Show]
puts [LMAN::man LMAN::Set]
}
