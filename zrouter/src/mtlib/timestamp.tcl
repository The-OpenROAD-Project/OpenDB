package provide mtlib 1.0
#Year 2000, Jan 1
set _tsmin 946713600

proc timestamp { } {
    set secs [clock seconds]
    set ms [clock clicks -milliseconds]


    set base [expr { $secs * 1000 }]
    set fract [expr { $ms - $base }]
    if { $fract > 1000 } {
        set diff [expr { $fract / 1000 }]
        incr secs $diff
        incr fract [expr { -1000 * $diff }]
    }
    return "[clock format $secs -format "%y%m%d%H%M%S"][format %03d $fract]"
}

proc seconds { timestamp } {
    global _tsmin
    set date "[string range $timestamp 2 3]/[string range $timestamp 4 5]/[string range $timestamp 0 1]"
    set hms "[string range $timestamp 6 7]:[string range $timestamp 8 9]:[string range $timestamp 10 11]"
    set cs "$date $hms"

    set secs [clock scan $cs]
    if { $secs < $_tsmin } {
        set secs $_tsmin
    }

    return $secs
}


proc timediff { timestamp diff} {
    global _tsmin

    set date "[string range $timestamp 2 3]/[string range $timestamp 4 5]/[string range $timestamp 0 1]"
    set hms "[string range $timestamp 6 7]:[string range $timestamp 8 9]:[string range $timestamp 10 11]"
    set cs "$date $hms"
    set secs [clock scan $cs]
    set ms [string range $timestamp 12 14]

    set secs [expr $secs - ($diff * 60)]

    if { $secs < $_tsmin } {
        set secs $_tsmin
    }


    return "[clock format $secs -format "%y%m%d%H%M%S"]$ms"
}

