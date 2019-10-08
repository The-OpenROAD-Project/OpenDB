package provide mtlib 1.0

#       Class: LArgs
# Description: argument processing facility
#

namespace eval LArgs {
# -------------------------------------------------- ARG PARSING PROCS
proc Parse {argv opt descr} {
    upvar $opt OPT
    usage_make $descr USAGE
    parse_all $argv OPT USAGE
}

proc usage_make {descr usage} {
    upvar $usage USAGE
    foreach d [split $descr \n] {
        set d [string trim $d]
        if { $d == "" } { continue }
        if [regexp \# $d ] { continue }
        set var  [lindex $d 0]
        set flag [lindex $d 1]
        set d    [lrange $d 1 end]
        array set A [list flag $flag default 0 var $var]
        set n ""
        switch [llength $d] {
            1 { set n [list arg {}] }
            2 { set n [list arg {} desc [lindex $d 1]] }
            3 { set n [list arg [lindex $d 1] desc [lindex $d 2] default {}]
            }
            4 { set n [list arg [lindex $d 1] desc [lindex $d 2]  \
                default [lindex $d 3]]
            }
            default {
                error "invalid argument description:\n'$descr'"
                return 0
            }
        }
        array set A $n
        set USAGE($flag) [array get A]
    }
    return 1
}
    
proc parse_all {argv opt usage} {
    upvar $opt V
    upvar $usage USAGE
    set n 0
    #
    foreach flag [array names USAGE] {
        array set OPTION $USAGE($flag)
        set var $OPTION(var)
        set V($var) $OPTION(default)
        unset OPTION
    }
    while { "@[set flag [lindex $argv $n]]" != "@" } {
        if { $flag == "--" } { incr n ; break }
        if [info exists USAGE($flag)] {
            array set OPTION $USAGE($flag)
            set   var $OPTION(var)
            if { $OPTION(arg) != {} } {
                incr n
                set V($var) [lindex $argv $n]
            } else { incr V($var) }
            unset OPTION
        } else {
            if { [ string first "-" $flag ] != 0 } { break }
            if { "$flag" == "-?"  } { usage     ; return 0 }
            error [usage_print USAGE "unknown option $flag"]
        }
        incr n
    }
    set V(args) [lrange $argv $n end]
    return 1
}

# --------------------------------------------------------- print options
proc usage_print {usage {msg ""}} {
    upvar $usage USAGE
    set out ""

    if { $msg != "" } { append out $msg \n }
    if [info exists USAGE(args)] {
        set flag args
        array set OPTION $USAGE(args)
        append out {usage: command args [options]} \n
        append out [usage_print_opt $flag $OPTION(arg) \
            $OPTION(desc) $OPTION(default)] \n
    } else {
        append out {usage: command [options]} \n
    }
    foreach flag [lsort [array names USAGE]] {
        if { $flag == "args" } { continue }
        array set OPTION $USAGE($flag)
        if { "@$OPTION(default)" == "@" } {
            set def ""
        } else {
            set def "($OPTION(default))"
        }
        if { ($OPTION(arg) == "") && ($OPTION(default)==0)} {
            set def ""
        }
        append out [usage_print_opt $flag $OPTION(arg) $OPTION(desc) $def] \n
        unset OPTION
    }
    set out
}

# -- print the option usage
proc usage_print_opt {flag arg desc def} {
    if { [string length $arg] > 10 } {
        return [format "%12s %-10s \n%12s: %s %s" $flag $arg "" $desc $def]
    }
    format "%12s %-10s : %s %s" $flag $arg $desc $def
}

# --------------------------------------------------------- testing
proc Test {} {
    global argv
    set usage {
        files args "" "files to process" ""
        son   -n "switch on" 
        ftest -a file "file to test" 
        fkeep -b file "file to keep" /dev/null
        dont  -c "do not actually do it"
    }
    if [Parse $argv OPT $usage] {
        parray OPT
    }
}
}
