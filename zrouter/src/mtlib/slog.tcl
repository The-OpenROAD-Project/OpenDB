# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB
package provide mtlib 1.0

#
#       Class: LLogger
# Description: Simple Logging & debugging facility
#
itcl::class LLogger {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable logfile   "-" { log_open $logfile }
    public variable format   ""   { log_format $format }
    # -------------------------------------------------- Public Methods ----
    # -- enable a level of logging
    public method Enable {levels} {
        foreach level $levels {
            if ![::info exists ENABLE($level)] {
                set ENABLE($level) 0
            }
            incr ENABLE($level)
        }
    }
    public method Log {fac level msg} {
        log_add $fac $level $msg
    }
    public method warning {fac msg} {
        Log $fac warning $msg
    }
    public method notice {fac args} {
        Log $fac notice $args
    }
    # export some Log methods as flat commands
    public method Export {} {
        LExportMethods $this "Enable Config" LLog
        LExportMethods $this Log             L

        set al [interp aliases]

        set not 0
        set warn 0
        foreach arg $al {
            if { $arg == "notice" } {
                incr not
            } elseif { $arg == "warning" } {
                incr warn
            }
        }
        if { $not == 0 } {
            LExportMethods $this notice 
        }
        if { $warn == 0 } {
            LExportMethods $this warning 
        }
    }

    # create automatic log proc bindings
    public proc Auto {args} {
        set log [eval LLogger #auto $args]
        $log Enable "warning error notice"
        $log Export
        set log
    }
    public method Config {args} {
        eval configure $args
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable ENABLE
    protected variable fp ""
    protected variable PROG ""
    protected variable CMD ""       ;# log command to print
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        if { $fp == "" } {
            configure -logfile $logfile
        }
        global argv0
        set PROG [file tail [file rootname $argv0]]
        set CMD {{%s %s %s } $fac $level $msg}
        if { $format == "" } {
            configure -format "fac level msg"
        } else {
            configure -format $format
        }
    }
    destructor {
        if { $fp != "" } { close $fp }
    }
    # -------------------------------------------------- Private Methods ---
    protected method log_add {fac level msg} {
        if {![::info exists ENABLE($level)] && ![::info exists ENABLE(*)]} {
            return
        }
        set output [subst $CMD]
        if { $logfile == "-" } {
            set mgr [ itcl::find objects "::zr.evmgr"]
            if { $mgr != "" } {
                ::zr.evmgr Post "$level {[subst $CMD]\n}"
            } else {
                puts $fp [subst $CMD]
                flush $fp
            }
        } else {
            puts $fp [subst $CMD]
            flush $fp
        }
    }
    protected method log_format {format} {
        set fmt ""
        set params ""
        foreach f $format {
            switch -glob $f {
                date  {
                    append fmt   "%s "
                    append params {[log_date] }
                }
                prog* {
                    append fmt   "%-5s "
                    append params {[string range $PROG 0 4]}
                }
                fac   {
                    append fmt   "%8s:"
                    append params {[string range $fac 0 7]}
                }
                msg   {
                    append fmt   "%s "
                    append params {$msg}
                }
                level   {
                    append fmt   "%6s:"
                    append params {[string range $level 0 5]}
                }
                default {
                    puts stderr "warning: LLog:skipping unkown format '$f'"
                    continue
                }
            }
            append params " "
        }
        set fmt [string trimright $fmt]
        set CMD "\[format {$fmt} $params\]"
    }
    protected method log_date {} {
        #clock format [clock seconds] -format "%Y%m%d %H%M%S "
        clock format [clock seconds] -format "%Y%m%d"
    }
    # ----------------------------------------------- Interface Implementation
    protected method log_open {file} {
        if { $fp != "" } { close $fp }
        if { $file == "-" } {
            set fp stderr
        } else {
            set file [log_expand $file]
            set fp [open $file a+]
        }
        fconfigure $fp -buffering line
    }
    # -- if log file contains a %n, generate a new file using %n
    # -- as a placeholder for sequence number
    protected method log_expand {pattern} {
        if ![string match *%n* $pattern] { return $pattern }
        set n 1
        # -- do not try hard (100 log files are enough!)
        while {$n < 100} {
            set f $pattern
            regsub %n $f $n f
            if ![file exists $f] { return $f }
            incr n
        } 
        regsub %n $pattern "" pattern
        set pattern
    }
    # -------------------------------------------------- Common Initialization
}

# -- using this log function, will cause LLog::Auto to be invoked
# -- to install the regular logging
proc LLog {args} {
    #puts stderr "warning: enabling standard log"
    #puts stderr "warning: use LLogger::Auto to control logging"

    # remove the definition of this proc
    rename LLog ""

    # create the logger object
    LLogger::Auto
    uplevel #0 LLog $args
}

#
#       Class: LSlog
# Description: Simple class to inherit a logging behavior
#      Author: Manolis M. Tsangaris
#
# In case interception of logging is needed, logger should
# be changed to point to an appropriate object
#
itcl::class LSLog {
    # -------------------------------------------------- Public Variables --
    public variable logger ""               ;# where to log
    # -------------------------------------------------- Public Methods ----
    public method notice {args} {
        $logger LLog $module notice [eval format $args]
    }
    public method debug {args} {
        $logger LLog $module info [eval format $args]
    }
    public method warning {args} {
        $logger LLog $module warning [eval format $args]
    }
    public method logname {name} {
        set module $name
    }
    public method LLog {mod level msg} {
        ::LLog $mod $level $msg
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    private variable module ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set module [string tolower [namespace tail [$this info class]]]
        if { $logger == "" } { set logger $this }
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
