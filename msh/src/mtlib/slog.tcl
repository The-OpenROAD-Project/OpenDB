# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB
package provide mtlib 1.0

#
#       Class: LLogger
# Description: Simple Logging & debugging facility
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LLogger {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable logfile   "-" { log_open $logfile }
    public variable format   ""   { log_format $format }
    # -------------------------------------------------- Public Methods ----
    # -- enable a level of logging
    public method Enable {levels} {
        if { [string match *:* $levels] } {
            set levels [split $levels :]
        }
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
    public method notice {fac msg} {
        Log $fac notice $msg
    }
    public method debug {fac msg} {
        Log $fac debug $msg
    }
    # export some Log methods as flat commands
    public method Export {} {
        LExportMethods $this "Enable Config" LLog
        LExportMethods $this Log             L
        LExportMethods $this warning 
        LExportMethods $this notice 
        LExportMethods $this debug
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
        puts $fp [subst $CMD]
        flush $fp
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
            set fp [open $logfile a+]
        }
        fconfigure $fp -buffering line
    }
    # -------------------------------------------------- Common Initialization
}

# -- using this log function, will cause LLog::Auto to be invoked
# -- to install the regular logging
proc LLog {args} {
    puts stderr "warning: enabling standard log"
    puts stderr "warning: use LLogger::Auto to control logging"

    # remove the definition of this proc
    rename LLog ""

    # create the logger object
    LLogger::Auto
    uplevel #0 LLog $args
}
