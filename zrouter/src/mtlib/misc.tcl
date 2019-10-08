
package provide mtlib 1.0
#
# Description: Miscalleneous useful tcl routines
#

global argv0
global progname
if [info exists argv0] {
    set progname [file tail $argv0]
}

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


proc memusage { {pid 0} } {
    if { $pid == 0 } {
        set pid [pid]
    }

    set rc [catch {set size [exec ps -p $pid -o rss,vsz | tail -1] } out]

    if { $rc == 0 } {
        return $size
    } else {
        return ""
    }
}

proc showmem { } {
    set a [memusage]
    notice "Memory usage - Rss: [lindex $a 0] Vsz: [lindex $a 1]"
}

# -- trim a string, add ".." if too large
proc LTrim {string length} {
    set n [string length $string]
    if { $n <= $length } { return $string }
    set n [expr $length - 2]
    return "[string range $string 0 $n].."
}

# -- simple comment remover
# -- it should be smarter and not remove quoted comments
proc LRemoveComments {string} {
    regsub -all {#[^\n]*\n} $string "\n" string
    set string
}

# -- like catch, but return the actual return code
proc LCatch {cmd _rc} {
    upvar $_rc rc
    set rt [uplevel [list catch $cmd $_rc]]
    if { $rt == 0 } { return 0 }
    global errorCode
    lindex $errorCode end
}
