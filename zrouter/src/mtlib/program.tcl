package require Itcl
package provide mtlib 1.0

#       Class: LProgram
# Description: 
#
itcl::class LProgram {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable progname "me"
    public variable argv ""
    public variable usage ""
    public variable rcpath ""              ;# : separated path of rc files
                                           ;# like: ~/.foorc:=lib/foo.rc
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
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable USAGE
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
    # --------------------------------------------------- Protected Methods ---
    # locate rc files in rcpath (: separated path)
    # ~ : home directory on all platforms
    # = : top directory (parent of the directory of the executable)
    # At the end, OPT is loaded with the current default values
    # extracted from the RC files 
    protected method rc_load {rcpath opt} {
        upvar $opt OPT
        global argv0 env
        set home [LPlatform home]
        set top  [LPlatform top]
        foreach f [split $rcpath :] {
            switch -glob -- $f {
                ~/* { set f [file join $home [string range $f 2 end]] }
                =*  { set f [file join $top  [string range $f 1 end]] }
            }
            rc_load_file $f OPT
        }
    }

    # load the defaults from an RC file
    # return 0 if failed to load
    private method rc_load_file {rc opt} {
        if ![file exists $rc] { return 0 }
        upvar $opt OPT
        set fp [open $rc r]
        set def [read $fp]
        close $fp
        set r ""
        foreach l [split $def \n] {
            regsub -all {^#[^$]*$} $l "" l
            append r $l \n
        }
        if [catch {array set OPT $r}] {
            return 0
        } 
        return 1
    }

    # ----------------------------------------------- Interface Implementation
    # -- pass the arguments and run the application
    protected method vRun {opt} {
        upvar $opt OPT
        notice "$progname running"
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
        upvar $usage1 U
        if { $usage != "" } {
            return [usage_set_old $usage U]
        }
        array set U $nusage
    }
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
    
    private method usage_args_parse {av v} {
        upvar $v V
        set n 0
        #
        foreach flag [array names USAGE] {
            array set OPTION $USAGE($flag)
            set var $OPTION(var)
            set V($var) $OPTION(default)
            unset OPTION
        }
        # see if we have to override defaults
        if { $rcpath != "" } {
            rc_load $rcpath V
        }

        while { "@[set flag [lindex $av $n]]" != "@" } {
            if { $flag == "--" } { incr n ; break }
            if [info exists USAGE($flag)] {
                array set OPTION $USAGE($flag)
                set   var $OPTION(var)
                if { $OPTION(arg) != {} } {
                    incr n
                    set V($var) [lindex $av $n]
                } else { incr V($var) }
                unset OPTION
            } else {
                if { [ string first "-" $flag ] != 0 } { break }
                if { "$flag" == "-?"  } { usage     ; return 0 }
                if { "$flag" == "-??" } { usage_rel ; return 0 }
                return [usage_print "unknown option $flag"]
            }
            incr n
        }
        set V(args) [lrange $av $n end]
        return 1
    }
    
    # --------------------------------------------------------- print options
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
    protected method usage_rel {} {
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
