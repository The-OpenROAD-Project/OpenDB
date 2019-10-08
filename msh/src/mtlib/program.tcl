package require Itcl
package provide mtlib 1.0

#       Class: LProgram
# Description: 
#      Author: Manolis M. Tsangaris  -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LProgram {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable progname "me"
    public variable argv ""
    public variable usage ""
    # -------------------------------------------------- Public Methods ----
    public method Run {} {
        usage_set $usage $nusage USAGE
        if [usage_args_parse $argv V] {
            $this vRun V
        } else {
            Signal -1
        }
    }
    public method Perror {msg} {
        puts stderr "error: $progname: $msg"
    }
    public method Warning {msg} {
        puts stderr "warning: $progname: $msg"
    }
    public method Die {msg} {
        Perror $msg
        Signal -1
    }
    public method Usage {{msg ""}} {
        usage_print $msg
    }
    public method Exit {code} {
        Signal $code
    }
    # -- set default values (has priority over option command)
    public method SetDefault {args} {
	array set DEFAULT $args
    }
    # -- was option set by the user?
    public method OptionSet {option} {
	set OPTIONSET($option)
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable OPTIONSET             ;# remember if option was set by user
    private variable USAGE
    private variable DEFAULT               ;# user defined default values
    private variable nusage ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        global argv0
        set progname [file tail $argv0]
        set argv     [uplevel #0 set argv]
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # add an option
    protected method option {var flag arg {desc "\0"} {default "\0"}} {
	set OPTIONSET($var) 0
        set opt [list var $var flag $flag arg {} desc {}]

        switch -glob -- "$desc-$default" {
            "\0-\0" {
                # unary argument
                lappend opt desc $arg default 0
            }
            "*-\0" {
                # binary argument no default (defaults to {} :-)
                lappend opt arg $arg desc $desc default {}
            }
            * {
                lappend opt arg $arg desc $desc default $default
            }
        }
        lappend nusage $flag $opt
    }
    # ----------------------------------------------- Interface Implementation
    # -- pass the arguments and run the application
    protected method vRun {opt} {
        upvar $opt OPT
        puts "$progname running"
        if { [info exists OPT(help)] && $OPT(help) } {
            Usage
        }
        return 0
    }
    # -------------------------------------------------- 
    # Arguments are described as the example next:
    # var   flag [arg] descr [default]
    # ftest -a  file "file to test" 
    # fkeep -b  file "file to keep" /dev/null
    # dont  -c  "do not actually do it"
    #

    private method usage_set {usage nusage usage1} {
        upvar $usage1 USAGE
        if { $usage != "" } {
            return [usage_set_old $usage USAGE]
        }
        array set USAGE $nusage
    }
    # -- parse the old usage string (set by the user with "set usage ..."
    private method usage_set_old {descr usage} {
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
                    Warning "invalid argument description:\n'$descr'"
                    return 0
                }
            }
            array set A $n
            set USAGE($flag) [array get A]
        }
        return 1
    }
    
    # -- parse the argv based on the usage
    private method usage_args_parse {av v} {
        upvar $v V
        set n 0
        #
        foreach flag [array names USAGE] {
            array set OPTION $USAGE($flag)
            set var $OPTION(var)
            set V($var) $OPTION(default)
	    if [info exists DEFAULT($var)] {
		set V($var) $DEFAULT($var)
	    }
            unset OPTION
        }
        while { "@[set flag [lindex $av $n]]" != "@" } {
            if { $flag == "--" } { incr n ; break }
            if [info exists USAGE($flag)] {
                array set OPTION $USAGE($flag)
                set   var $OPTION(var)
		incr OPTIONSET($var)
                if { $OPTION(arg) != {} } {
		    # -- binary option?
                    incr n
                    set V($var) [lindex $av $n]
                } else {
		    # -- boolean option?
		    incr V($var)
		}
                unset OPTION
            } else {
                if { [ string first "-" $flag ] != 0 } { break }
                if { "$flag" == "-?"  } { usage_print     ; return 0 }
                if { "$flag" == "-??" } { usage_print_rel ; return 0 }
                return [usage_print "unknown option $flag"]
            }
            incr n
        }
        set V(args) [lrange $av $n end]
        return 1
    }
    
    # --------------------------------------------------------- print options
    # -- print usage
    private method usage_print {{msg ""}} {
        global argv0
        set me [file tail [file root $argv0]]
        if { $msg != "" } { Perror $msg }
        puts stderr "$progname: usage: "
        foreach flag [lsort [array names USAGE]] {
            array set OPTION $USAGE($flag)
            if { "@$OPTION(default)" == "@" } {
                set def ""
            } else {
                set def "($OPTION(default))"
            }
            if { ($OPTION(arg) == "") && ($OPTION(default)==0)} {
                set def ""
            }
	    set var $OPTION(var)
	    if [info exists DEFAULT($var)] { set def "($DEFAULT($var))" }
            puts stderr [usage_print_opt $flag $OPTION(arg) $OPTION(desc) $def]
            unset OPTION
        }
        return 0
    }

    # -- print the option usage
    private method usage_print_opt {flag arg desc def} {
        if { [string length $arg] > 10 } {
            return [format "%12s %-10s \n%12s: %s %s" $flag $arg "" $desc $def]
        }
        format "%12s %-10s : %s %s" $flag $arg $desc $def
    }

    # -- print usage in a relational form -- for grsh to pickup
    protected method usage_print_rel {} {
        puts "option|param|descr|default"
        foreach flag [lsort [array names ARG]] {
            array set DESCR $ARG($flag)
            if { "@$DESCR(default)" == "@" } {
                set def ""
            } else {
                set def "$DESCR(default)"
            }
            if { ($DESCR(arg) == "") && ($DESCR(default)==0)} {
                set def ""
            }
	    set var $OPTION(var)
	    if [info exists DEFAULT($var)] { set def $DEFAULT($var) }
            puts "[string range $flag 1 end]|$DESCR(arg)|$DESCR(desc)|$def"
            unset DESCR
        }
        return 0
    }
    
    # ---------------------------------------------------------------------
    
    protected method Test {} {
        global argv
        set usage {
            son   -n "switch on" 
            ftest -a file "file to test" 
            fkeep -b file "file to keep" /dev/null
            dont  -c "do not actually do it"
        }
        Run
    }
    # -------------------------------------------------- Common Initialization
}
