#
# Description: Miscalleneous useful tcl routines
#      Author: Manolis M. Tsangaris -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 1995-2004
#
package provide mtlib 1.0

set progname [file tail $argv0]

proc error1 {msg} {
	global errorInfo
	puts stderr $msg
	set N 10
	foreach line [split $errorInfo \n] {
		if {![incr N -1]} { break }
		puts stderr $line
	}
	if { !$N } {
		puts stderr ...
	}
}

# swap to variables
proc swap {x y} {
    upvar $x X
    upvar $y Y
    set n $X
    set X $Y
    set Y $n
}

# -- approximate formatting
proc time_afmt {secs} {
    if { $secs < 60    } { return "${secs} secs" }
    if { $secs < 3600  } { return "[expr $secs /60] mins" }
    if { $secs < 86400 } { return "[expr $secs /3600] hours" }
    return "[expr $secs /86400] days"
}

# -- simple comment remover
# -- it should be smarter and not remove quoted comments
proc LRemoveComments {string} {
    regsub -all {#[^\n]*\n} $string "\n" string
    set string
}

proc LPrintStack {} {
    set level [info level]
    while { $level >= 0 } {
	puts [format "%3d:%s\n" $level [info level $level]]
    }
}

# -- trim a string, add ".." if too large
proc LTrim {string length} {
    set n [string length $string]
    if { $n <= $length } { return $string }
    set n [expr $length - 2]
    return "[string range $string 0 $n].."
}
