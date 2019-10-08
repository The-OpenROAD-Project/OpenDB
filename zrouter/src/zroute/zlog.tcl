#
# BSD 3-Clause License
#
# Copyright (c) 2019, Nefelus Inc
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package require Itcl
package require mtlib 1.0

package provide ade::main 1.0

itcl::class ZLog {
    public variable guiwidget ""
    public variable guiinsert ""
    public variable fulldir ""; ## Used to override timestamp generation
    public variable logroot "logs"
    public variable loglink "" {
        set dir [file dirname $loglink]
        set loglink [file join $dir [file_expand $loglink] ]
        file delete $loglink
        path_link $loglink $logdir
    }

    public variable enable_verbose 1 {
        ::sys::setVerbose $enable_verbose
    }

    public variable enabled 1
    public variable sinit 0


    public variable userlogfile "" { 
        if { $logfile != "" } {
            if { $oldlogfile != "" } {
                if { [file isfile $oldlogfile] == 1 } {
                    file delete $oldlogfile
                }
            }

            set dir [file dirname $userlogfile]
            set userlogfile [file join $dir [file_expand $userlogfile] ]

            if { [file isdirectory $userlogfile] } {
                $proc_puts "\n\nWarning: cannot create log file $userlogfile - is a directory.\n\n"
                return
            }

            if { [file isfile $userlogfile] == 1 } {
                file delete $userlogfile
            }

            set rl [path_link $userlogfile $logfile]
            if { [lindex $rl 0] != 0 } {
                $proc_puts "\n\nWarning: cannot create logfile $userlogfile."
                $proc_puts "Error message: [lindex $rl 1]."
                $proc_puts "Please verify the filename.\n\n"
                incr log_err
            }

            set oldlogfile $userlogfile
        }
    };

    public variable evmgr ""  {
        set bind "$this logit"
        foreach arg $EVENTS {
            $evmgr Bind $arg $bind
        }
    }

    public method reset_bindings { } {
        $cli reset_bindings
    }

    public variable cli "" { 
        #$cli Bind <<eof>> $this onexit
    } ;
    #public variable debug 0;
    public variable log_debug 0;
    public variable timestamp 0;
    public variable app;
    public variable keeplog "1d";

    public variable return_notice 0 {
        if { $return_notice == 1 } {
            set last_notice ""
        }
    }

    protected variable dup_logfile "";
    protected variable log "";
    protected variable log_verbose "";
    protected variable logfile "";
    protected variable logdir "";
    protected variable oldlogfile "";
    protected variable log_warn 0;
    protected variable log_err 0;
    protected variable ps1;

    protected variable errcnt 0;
    protected variable warncnt 0;

    protected variable last_notice "";

    protected variable debug_enable;
    protected variable debug_except;

    protected common EVENTS {cmdi cmdo prompt notice debug clierror warning error info verbose}; 
    protected common proc_puts "::__puts"
    protected common proc_exit "::__exit"


    # -- if log file contains a %n, generate a new file using %n
    # -- as a placeholder for sequence number
    public method file_expand {pattern} {
        if ![string match *%n* $pattern] { return $pattern }

        set dir [file dirname [file normalize $pattern]]
        set fp [file tail $pattern]

        set cwd [pwd]

        cd $dir

        regsub %n $fp "*" gp
        
        set fl { }
        set max 0

        set rc [catch { set fl [glob "$gp"] } out]
        if { $rc == 0 } {
            regsub %n $fp "/" gp
            set fs [file split $gp]
            foreach {pre suf} $fs { break; }


            foreach arg $fl {
                regsub $pre $arg "" arg
                regsub $suf $arg "" arg

                if { [string is integer $arg] } {
                    if { $arg > $max } {
                        set max $arg
                    }
                }
            }


            incr max
        }
        regsub %n $fp $max gp

        cd $cwd
        return $gp

    }

    public method prunelogs { } {
        set kl [string trim $keeplog]
        set base [string index $kl end]

        ## default to days
        set multiplier 1440
        if { [string is integer $base] } {
            set multiplier 1440
        } else {
            if { $base == "d" } {
                set multiplier 1440
            } elseif { $base == "h" } {
                set multiplier 60
            } elseif { $base == "m" } {
                set multiplier 1
            } else {
                ## unknown - assume days
                set multiplier 1440
            }
            set cnt [scan $kl "%d" num]
            if { $cnt != 1 } {
                ## error
                warning "Cannot interpret keeplogs value: $base"
                return
            }
            set base $num
        }

        set tsback [expr $base * $multiplier]
        set rmts "run_[timediff $timestamp $tsback]"

        set cwd [pwd]
        if { [file isdirectory $logdir] } {
            cd $logdir
            cd ..
            catch { set fl [glob "run_\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]\[0-9]"]}

            set fl [lsort $fl]
            set ll [llength $fl]
            for { set i 0 } { $i < $ll } { incr i } {
                set arg [lindex $fl $i]
                if { $arg > $rmts } { 
                    break
                }
            }

            set idx [expr $i - 1]
            set oldl [lrange $fl 0 $idx]


            foreach arg $oldl {
                $proc_puts "Remove logs directory: $arg"
                set rc [catch { file delete -force $arg } output]
                if { $rc != 0 } {
                    puts "Error: could not remove directory [file normalize $arg]."
                    puts "Error was: $output"
                }
            }
            cd $cwd
        }

    }

    public method set_dup_file { {filename ""} {w_or_a w} } {
        if { $dup_logfile != "" } {
            flush $dup_logfile
            close $dup_logfile
            set dup_logfile ""
        }

        if { $filename != "" } {
            set dup_logfile [open $filename $w_or_a]
        }
        return ""
    }

    public method getLogdir { } {
        return $logdir
    }

    public method tolog { outmsg } {
        if { $log != "" } {
            $proc_puts $log $outmsg
            flush $log
        }
        if { $dup_logfile != "" } {
            $proc_puts $dup_logfile $outmsg
        }
        verbose $outmsg
    }

    public method tolognnl { outmsg } {
        if { $log != "" } {
            $proc_puts -nonewline $log $outmsg
            flush $log
        }
        if { $dup_logfile != "" } {
            $proc_puts -nonewline $dup_logfile $outmsg
        }
        verbosennl $outmsg
    }

    private method init_puts { } {
        if { [info commands $proc_puts] == "" } {
            rename ::puts $proc_puts
        }
        set cmd "
            proc ::puts { str args } {
                #$proc_puts \"In global proc puts: \$str / \$args\"
                set len \[llength \$args]
                if { \$len == 0 } {
                    $this putl \$str
                } elseif { \$len == 1 } {
                    $this putl \$str \[lindex \$args 0]
                } elseif { \$len == 2 } {
                    $this putl \$str \[lindex \$args 0] \[lindex \$args 1]
                }
            }"
        eval $cmd

        if { [info commands $proc_exit] == "" } {
            rename ::exit $proc_exit
        }
        set cmd "
            proc ::exit { args } {
                $this onexit
                set cmd \"exit [concat \$args]\"
                eval \$cmd
            }"
        eval $cmd

        set cmd "
            proc ::parray {a {pattern *}} {
                upvar 1 \$a array
                if {!\[array exists array]} {
                    $this putl \"\\\"\$a\\\" isn't an array\"
                }
                set maxl 0
                foreach name \[lsort \[array names array \$pattern]] {
                if {\[string length \$name] > \$maxl} {
                    set maxl \[string length \$name]
                }
                }
                set maxl \[expr {\$maxl + \[string length \$a] + 2}]
                foreach name \[lsort \[array names array \$pattern]] {
                set nameString \[format %s(%s) \$a \$name]
                $this putl \[format \"%-*s = %s\" \$maxl \$nameString \$array(\$name)]
                }
            }"

        eval $cmd

    }


    public method constructor { args } {
        init_puts
        eval configure $args

        set debug_enable("") { }
        set debug_except("") { }

        start_log
    }

    private method start_log { } {
        global env
        set bind "$this logit"

        if { $fulldir == "" } {

            if { $timestamp == 0 } { 
                set timestamp [timestamp]
            }

            set logdir [file join $logroot $app "run_$timestamp"]

            ## for slave propagation
            set env(ZR_LOGDIR) $logdir

            set rc [catch { file mkdir $logdir } out ]
            if { $rc != 0 } {
                $proc_puts "\n\nWarning: could not create log directory $logdir."
                $proc_puts "Error was: $out"
                $proc_puts "No file logging will occur.\n\n"
                incr log_err
                set log ""
                set logfile ""
                return
            }
        } else {
            set logdir $fulldir
        }

        set logfile [file join $logdir "$app.log"]
        set rc [catch {set log [open $logfile "w"]} out ]
        if { $rc != 0 } {
            $proc_puts "\n\n Warning: could not create log file $logfile."
            $proc_puts "Error was: $out"
            $proc_puts "No file logging will occur.\n\n"
            incr log_err
            set log ""
            set logfile ""
        } else {
            fconfigure $log -buffering line
        }

        set verbose_name [file join $logdir "${app}_debug.log"]
        set rc [catch {set log_verbose [open $verbose_name "w"]} out ]
        if { $rc != 0 } {
            $proc_puts "\n\n Warning: could not create debug log file $verbose_name."
        } else {
            fconfigure $log_verbose -buffering line
        }

    }

    public method restart { lf } {
        dellog
        set timestamp 0
        start_log 
        if { $lf == "" } {
            configure -userlogfile $oldlogfile
        } else {
            configure -userlogfile $lf
        }
    }

    public method destructor { } {
        onexit
    }

    public method onexit { } {
        if { [info commands $evmgr] != "" } {
            foreach arg $EVENTS {
                $evmgr UnBind $arg 
            }
        }
        summary
        rename ::puts ""
        rename $proc_puts ::puts

        rename ::exit ""
        rename $proc_exit ::exit
        dellog
    }

    public method delete { } {
        itcl::delete object $this
    }

    public method summary { } {
        putl "\nExecution Summary:\n"
        set msg "Warnings: $warncnt"
        if { $log_warn > 0 } {
            append msg "  ($log_warn log warning(s))"
        }
        puts $msg
        set msg "Errors: $errcnt"
        if { $log_err > 0 } {
            append msg "  ($log_err log error(s))"
        }
        puts $msg

        return
    }

    private method dellog { } {
        if { $log != "" } {
            close $log
        }
    }

    public method putl { str args } {

        ## don't do ANYTHING for slave init
        if { $sinit == 1 } { return }

        set cnt [llength $args]
        if { $cnt == 2 } {
            set fp [lindex $args 0]
            set outmsg [lindex $args 1]
        } elseif {$cnt == 1} {
            set outmsg [lindex $args 0]
        } else {
            set outmsg $str
        }

        if { $str == "-nonewline" } {
            if { $log_debug > 2 } {
                $proc_puts "putl: nnl"
            }
            #set outmsg [stripcr $outmsg]
            if { $cnt == 2 } {
                $proc_puts -nonewline $fp $outmsg
            } else {
                ## Log only if NOT going to a file
                if { $guiwidget != "" } {
                    $guiinsert $guiwidget "$outmsg"
                } else {
                    $proc_puts -nonewline $outmsg
                }
                if { $enabled == 1 } {
                    if { $log_debug > 2 } {
                        #$log Lognl "" "notice" "YYY($cnt): $outmsg"
                        tolognnl "YYY($cnt): $outmsg"
                    } else {
                        #$log Lognl "" "notice" "$outmsg"
                        tolognnl "$outmsg"
                    }
                }
            }

        } else {
            #$proc_puts stderr "putl: fp = $fp"
            if { $log_debug > 2 } {
                $proc_puts "putl: nl"
            }
            if { $cnt == 1 } {
                if { $log_debug > 2 } {
                    $proc_puts "putl: case 1 ($cnt): $args, [llength $args]"
                }
                set fp $str
                if { ($fp == "stdout" || $fp == "stderr") && ($guiwidget != "") } {
                    $guiinsert $guiwidget "$outmsg\n"
                } else {
                    $proc_puts $fp $outmsg
                }
            } else {
                if { $log_debug > 2 } {
                    $proc_puts "putl: case 2 ($cnt)"
                }
                if { $guiwidget != "" } {
                    $guiinsert $guiwidget "$outmsg\n"
                } else {
                    $proc_puts $outmsg
                }
                ## Log only if NOT going to a file
                if { $enabled == 1 } {
                    if { $log_debug > 2 } {
                        #$log Log "" "notice" "ZZZ($cnt): $outmsg"
                        tolog "ZZZ($cnt): $outmsg"
                    } else {
                        #$log Log "" "notice" "$outmsg"
                        tolog "$outmsg"
                    }
                }
            }
        }
    }

    #private method logcr { } {
        #$log Log "" "notice" "\n"
    #}

    private proc stripcr { str } {
        set str [string trimright $str]
        set a [string index $str end ]
        if { $a == "\n" } {
            set str [string range $str 0 end-1]
        }
        return $str
    }

    private proc stripcr_notrim { str } {
        set a [string index $str end ]
        if { $a == "\n" } {
            set str [string range $str 0 end-1]
        }
        return $str
    }

    private proc stripq { str } {
        set id1 [string first "\"" $str]
        set id2 [string last "\"" $str]
        if { ($id1 >= 0) && ($id2 >=0) } {
            if { $id1 <= $id2 } {
                set str [string range $str $id1 $id2]
                if { [string length $str] <= 2 } {
                    return ""
                }
                return [string range $str 1 end-1]
            }
            return $str
        }
        return $str
    }
    private proc stripb { str } {
        set id1 [string first "{" $str]
        set id2 [string last "}" $str]
        if { ($id1 >= 0) && ($id2 >=0) } {
            if { $id1 <= $id2 } {
                set str [string range $str $id1 $id2]
                if { [string length $str] <= 2 } {
                    return ""
                }
                return [string range $str 1 end-1]
            }
            return $str
        }
        return $str
    }

    public method logit { event args } {

        if { $log_debug >= 2 } {
            putl "\nLog Debug."
            putl "event: $event"
            putl "args: $args"
            putl "Whole thing: $event $args"
            putl "End of Log Debug.\n"
        }

        if { $event == "clierror" } {
            incr errcnt
            #set outmsg "error: [stripb [stripb $args]]"
            set outmsg "[stripb [stripb $args]]"
            #$log Log "" "$event" $outmsg
            tolog $outmsg

        } elseif { $event == "prompt" } {
            #$log Lognl "" "prompt" "[string trim [stripcr [stripb $args]]] "
            set outmsg "[string trim [stripcr [stripb $args]]] "
            tolognnl $outmsg
            set ps1 "$outmsg"

        } elseif { $event == "cmdi" } {
            set outmsg "[string trim [stripcr [stripb $args]]]"
            if { $outmsg != "::zr.zlog onexit" } {
                #$log Log "" "cmdi" $outmsg
                if { $guiwidget != "" } {
                    $guiinsert $guiwidget "$ps1$outmsg\n"
                }
                tolog $outmsg
            }

        } elseif { $event == "cmdo" } {
            set outmsg "[stripb [stripb $args]]"
            tolog "$outmsg"

        } elseif { $event == "warning" } {
            ## Try to parse warning
            incr warncnt
            set outmsg "Warning: [stripq [stripb [stripb $args]]]"

            putl $outmsg
            
        } elseif { $event == "info" } {
            set outmsg [stripq [stripb [stripb $args]]]
            putl -nonewline $outmsg

        } elseif { $event == "notice" } {

            #puts "nt: original arg: $args"
            set outmsg [stripq [stripb [stripb $args]]]
            #puts "msg: $outmsg"
            if { $return_notice == 0 } {
                putl -nonewline "$outmsg"
            } else {
                append last_notice [stripcr_notrim $outmsg]
            }
            #putl $outmsg
        } elseif { $event == "debug" } {
            #putl "DEBUG: $args"
            check_debug $args
        } elseif { $event == "error" } {
            incr errcnt
            #puts "ERRDEB: $args"
            set outmsg "[stripb [stripb [stripb $args]]]"
            #puts "ERRDEB2: $outmsg"
            error $outmsg
            flush stdout
        } elseif { $event == "verbose" } {
            set outmsg [stripq [stripb [stripb $args]]]
            verbosennl $outmsg
        }

    }

    private method verbose { msg } {
        if { $enable_verbose == 1 } {
            $proc_puts $log_verbose $msg
        }
    }
    private method verbosennl { msg } {
        if { $enable_verbose == 1 } {
            $proc_puts -nonewline $log_verbose $msg
        }
    }

    public method getLastNotice { } {
        return $last_notice 
    }

    private method check_debug { args } {
        #set a [stripq [stripb [stripb $args]]]
        set a [stripb $args]
        #putl "debug args 1: $args - $a"
        set a [stripb $a]
        #putl "debug args 2: $args - $a"

        ## not needed?
        #set a [stripq $a]
        #putl "debug args 3: $args - $a"

        set idx [expr [string first "\{" $a]  - 1]
        set module [string range $a 0 $idx]
        set module [string trim $module]
        set idx [string last ":" $module]
        set tag [string range $module [expr $idx + 1] end]
        set module [string range $module 0 [expr $idx - 1]]
        #putl "debug module: $module - $tag"
        if { [info exists debug_enable($module)] } {
            #putl "Debug exists for $module!"

            set enl [lindex $debug_enable($module) end]
            set exl [lindex $debug_except($module) end]

            #putl "Looking for tag: $tag"
            if { [string length $enl] > 0 } {
                ## see if enabled
                foreach t [split $tag ""] {
                    if { [string first $t $enl] >= 0 } {
                        set outmsg "[stripb $a]"
                        putl -nonewline $outmsg
                        break;
                    }
                }
            } else {
                ## see if in "except" list
                set flag 1
                foreach t [split $tag ""] {
                    if { [string first $t $exl] >= 0 } {
                        set flag 0
                        break;
                    }
                }
                if { $flag == 1 } {
                    set outmsg "[stripb $a]"
                    putl -nonewline $outmsg
                }
            }
        } else {
            #putl "Module $module not found for debugging!"
        }

    }

    public method dump_debug { } {
        parray debug_enable
        parray debug_except
    }

    public method enable_debug_log { args } {
        set mod ""
        set only ""
        set except ""
        foreach {arg1 arg2} $args {
            switch [string range $arg1 1 end] {
                "module" { set mod [string trim $arg2]}
                "only" { set only [string trim $arg2]}
                "except" { set except [string trim $arg2]}
                default { putl "Invalid argument: $arg1"; return 1}
            }
        }

        if { $mod eq "" } {
            return 1
        }

        if { [info exists debug_enable($mod)] } {
            if { [string length $only] > 0 } {
                set ndebug [lreplace $debug_enable($mod) end end $only]
                set debug_enable($mod) $ndebug
            }
            if { [string length $except] > 0 } {
                set ndebug [lreplace $debug_except($mod) end end $except]
                set debug_except($mod) $ndebug
            }
            ::sys::setDebug $mod $debug_enable($mod) $debug_except($mod)
            return 0
        } else {
            set debug_enable($mod) $only
            set debug_except($mod) $except
            ::sys::setDebug $mod $debug_enable($mod) $debug_except($mod)
            return 0
        }
        return 1
    }

    public method save_debug_log_config { args } {
        set mod ""
        foreach {arg1 arg2} $args {
            switch [string range $arg1 1 end] {
                "module" { set mod [string trim $arg2]}
                default { putl "Invalid argument: $arg1"; return 1}
            }
        }
        if { $mod eq "" } {
            return 1
        }

        if { [info exists debug_enable($mod)] } {
            lappend debug_enable($mod) [lindex $debug_enable($mod) end]
            lappend debug_except($mod) [lindex $debug_except($mod) end]
            return 0
        }
        return 1

    }

    public method restore_debug_log_config { args } {
        set mod ""
        foreach {arg1 arg2} $args {
            switch [string range $arg1 1 end] {
                "module" { set mod [string trim $arg2]}
                default { putl "Invalid argument: $arg1"; return 1}
            }
        }
        if { $mod eq "" } {
            return 1
        }

        if { [info exists debug_enable($mod)] } {
            set debug_enable($mod) [lreplace $debug_enable($mod) end end]
            set debug_except($mod) [lreplace $debug_except($mod) end end]
            set flag 0
            if { [llength $debug_enable($mod)] == 0 } {
                array unset debug_enable $mod
                set flag 1
            }
            if { [llength $debug_except($mod)] == 0 } {
                array unset debug_except $mod
                set flag 1
            }
            if { $flag == 0 } {
                ::sys::setDebug $mod $debug_enable($mod) $debug_except($mod)
            } else {
                ::sys::setDebug $mod "" "*"
            }
            return $flag
        } else {
            return 1
        }

    }

    public method clear_debug_log_config { args } {
        set mod ""
        foreach {arg1 arg2} $args {
            switch [string range $arg1 1 end] {
                "module" { set mod [string trim $arg2]}
                default { putl "Invalid argument: $arg1"; return 1}
            }
        }
        if { $mod eq "" } {
            return 1
        }
        array unset debug_enable $mod
        array unset debug_except $mod
        ::sys::setDebug $mod "" ""
        return 0
    }

    public method get_debug_log_config { args } {
        set mod ""
        set only ""
        set except ""
        foreach {arg1 arg2} $args {
            switch [string range $arg1 1 end] {
                "module" { set mod [string trim $arg2]}
                "only=" { set only [string trim $arg2]}
                "except=" { set except [string trim $arg2]}
                default { putl "Invalid argument: $arg1"; return 1}
            }
        }

        if { $mod eq "" } {
            return 0
        }

        if { ![info exists debug_enable($mod)] } {
            return 0
        } else {
            if { $only != "" } {
                upvar $only ol
                set ol [lindex $debug_enable($mod) end]
            }
            if { $except != "" } {
                upvar $except ex
                set ex [lindex $debug_except($mod) end]
            }
            return 1
        }

    }

}
