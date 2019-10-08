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

package provide ade::main 1.0

#       Class: ZEventMGR
# Description: Zroute event handler
#      Author: Manolis M. Tsangaris
#
itcl::class ZEventMGR {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Post {event} {
        ev_dispatch $event
    }
    public method Bind {event cmd} {
        set HANDLER($event) $cmd
    }
    public method UnBind {event} {
        if ![info exists HANDLER($event)] { return 0 }
        unset HANDLER($event)
        return 1
    }
    # return all known events
    public method KnownEvents {} {
        array get KNOWN_EVENT
    }

    public method Bindings {} {
        array get HANDLER
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable HANDLER
    protected variable KNOWN_EVENT         ;# keep count of known events
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method ev_dispatch {ev} {

        ## Can't use debug in here - debug uses this!

        if { [info command ::zr.zlog] eq "::zr.zlog" } {
            if { [::zr.zlog get_debug_log_config -module ZEV] == 1 } {
                puts "ZEV:E Event: $ev"
            }
        }

        set idx [string first " " $ev]
        set event [string trim [string range $ev 0  [expr $idx - 1]]]
        set vector [string range $ev $idx end]
        if { [::zr.zlog get_debug_log_config -module ZEV] == 1 } {
            puts "ZEV:E Event: $event"
            puts "ZEV:E Vector: $vector"
        }

        if ![info exists KNOWN_EVENT($event)] {
            set KNOWN_EVENT($event) 0
        }
        incr KNOWN_EVENT($event)
        set h ""
        foreach ev "$event * **" { 
            if [info exists HANDLER($ev)] {
                set h $HANDLER($ev)
                break
            }
        }


        if { $h == "" } {
            if { ($event != "cmdi") && ($event != "cmdo") 
              && ($event != "prompt") } {
                puts "**    $event $vector"
            }
            return
        }
        if { $event != "error" } {
            lappend h $event $vector
        } else {
            #puts "ZEV:E EI:: $::errorInfo"
            #puts "ZEV:E EC:: $::errorCode"
            #puts "ZEV:E Handler: $h"
            #puts "ZEV:E Event: $event"
            set vl [split $vector " "]
            #foreach arg $vl {
            #    puts "ZEV:E Vec: $arg"
            #}
            lappend h $event [join [lrange $vl 0 end-2]]
        }
        uplevel #0 $h
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: ZDBGUI
# Description: Simple user interface for ZDBG
#      Author: Manolis M. Tsangaris
#
itcl::class ZDBGUI {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable evmgr ""         ;# event manager
    public variable prompt "zdb> "
    public variable nostop 0        ;# if set, we do not stop on bp
    public variable rcpath  ".zdbrc ~/.zdbrc" ;# path of init files to read
    # -------------------------------------------------- Public Methods ----
    # Run a command in debug mode
    public method Run {args} {
        rc_read $rcpath
        set nostop 0
        catch {unset LAST}
        set LAST(cmd) $args
        cli_start
        Wait
        uplevel $zdbg Run $args
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable zdbg ""
    protected variable LAST                         ;# last breakpoint info
    protected variable RCFILE ""                    ;# rc file to use
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set zdbg [ZDBG $this.zdbg -evmgr $evmgr]
        $zdbg Bind <<error>> $this eError
        $zdbg Bind <<stop>>  $this eStopped
        $zdbg Bind <<event>> $this eEvent
    }
    destructor {
        $zdbg delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method cli_start {} {
        fileevent stdin r "$this eCommand stdin"
        prompt
    }
    protected method prompt {} {
        puts -nonewline stdout $prompt 
        flush stdout
    }
    # evaluate a command, print output
    # return 0 to continue execution, 1 for more prompts
    protected method cli_eval {cmd} {
        set res ""
        switch -glob [lindex $cmd 0] {
            c {
                return 0
            }
            s {
                $zdbg TraceMode stop-next
                return 0
            }
            C {
                $zdbg TraceMode skip
                return 0
            }
            b* {
                set res [cli_breakpoint [lrange $cmd 1 end]]
            }
            e {
                set res [cli_knownev]
            }
            w {
                set res [cli_where]
            }
            h*  { set res [cli_help [lrange $cmd 1 end]] }
            !*  { set res [cli_run $cmd] }
            default { 
                set res "$cmd: bad command. Use h for help"
            }
        }
        if { $res != "" } {
            puts $res
        }
        return 1
    }
    protected method cli_help {{topic ""}} {
        set s ""
        switch -glob -- $topic {
            b* {
                pr s {zdbg breakpoint examples:}
                pr s {b myevent { $a > 10 }}
                pr s {   stop in myevent when attribute a is > 10}
                pr s {b myevent { ($a > 10) && ($b < 20) } }
                pr s {   stop in myevent when a is > 10 and b < 20}
                pr s {b myevent { $a == "xyz" } }
                pr s {   stop in myevent when a is 'xyz'}
                pr s ""
            }
            zdbrc* {
                pr s {./.zdbrc and ~/.zdbrc are consulted to initialize}
                pr s {the debugger, the first one will be read.}
                pr s {Any legal zdb command can be executed.}
                pr s ""
            }
            "" - default {
                pr s "zdbg commands: "
                pr s "s  : continue to the next event"
                pr s "C  : skip over all breakpoints-run to finish"
                pr s "c  : continue running. stop at the next breakpoint"
                pr s "^D : resume processing, do not stop"
                pr s "b  : list or set breakpoints"
                pr s "e  : list all known events"
                pr s "help breakpoints : breakpoint examples"
                pr s "help zdbrc : profile examples"
            }
        }
        set s
    }
    protected proc pr {var msg} {
        upvar $var V
        append V $msg \n
    }
    protected method cli_breakpoint {arg} {
        if { $arg == "" } {
            return [$zdbg Breakpoints]
        }
        set expr [lrange $arg 1 end]
        set ev    [lindex $arg 0] 
        $zdbg Break $ev $expr
        if { $expr != "" } {
            return ""
        } 
        return ""
    }
    protected method cli_knownev {} {
        set res "Known Events:\n"
        foreach {ev count} [$evmgr KnownEvents] {
            append res [format "%10s %4d time(s)\n" $ev $count]
        }
        set res
    }
    # run a tcl command
    protected method cli_run {cmd} {
        set cmd [string range $cmd 1 end]
        if { [catch {uplevel #0 $cmd} rc] } {
            return "error:$rc"
        }
        return $rc
    }
    # show the last breakpoint
    protected method cli_where {} {
        if ![info exists LAST(event)] { return "started: $LAST(cmd)" }
        return "$LAST(event) $LAST(vector)"
    }
    # read the profiles, if any
    protected method rc_read {files} {
        foreach p $files {
            set f [glob -nocomplain $p]
            if { $f == "" } { continue }
            if ![file exists $f]  { continue }
            puts "zdb: sourcing $f"
            zsource $f
            break
        }
    }
    # read a profile and execute the commands in it
    protected method zsource {f} {
        set fp [open $f r]
        set cmd ""
        while { [gets $fp line] != -1 } {
            append cmd $line \n
            if ![info complete $cmd] { continue }
            set cmd [string trim $cmd] 
            if { $cmd == "" } { continue }
            if [catch {cli_eval $cmd} rc] {
                puts "error: $rc"
                close $fp
                return
            }
            set cmd ""
        }
        close $fp
        # remember the RCFILE
        set RCFILE $f
    }
    # ----------------------------------------------- Interface Implementation
    public method eStopped {event expr vector} {
        set LAST(event) $event 
        set LAST(expr) $expr 
        set LAST(vector) $vector 
        $this eEvent $event $vector
        if { $nostop } {
            return
        }
        if {$expr == ""} {
            puts "stopped:"
        } else {
            puts "stopped: due to: $expr"
        }
        cli_start
        Wait
    }
    public method eError {args} {
        puts "Breakpoint error: $args"
    }
    public method eCommand {fp} {
        if { [gets $fp line] == -1 } {
            fileevent $fp r ""
            set nostop 1
            Signal 0
            return
        }
        set cmd [string trim $line]
        if { [catch {cli_eval $cmd} rc] } {
            puts stderr "error: $rc"
        } elseif { $rc == 0 } {
            # continue execution
            fileevent $fp r ""
            Signal 0
            return
        } 
        prompt
    }
    public method eEvent {ev vector} {
        puts "**    $ev $vector"
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#       Class: ZDBG
# Description: Zroute debugger
#      Author: Manolis M. Tsangaris
#
itcl::class ZDBG {
    inherit LObject LBinder
    # Event <<stop>>
    # -------------------------------------------------- Public Variables --
    public variable evmgr "" ;# event manager 
    # -------------------------------------------------- Public Methods ----
    public method Run {cmd} {
        if [catch {uplevel $cmd} rc] {
            uplevel [list error $rc]
        } else {
            return $rc
        }
    }
    # Control Tracing/stopping
    # stop-next: stop on the next breakpoint
    #      skip: skip all events
    #    normal: report all breakpoints
    public method TraceMode {mode} {
        set TRACEMODE $mode
        if { $mode == "stop-next" } {
            Break ** ""
        }
    }
    # define a breakpoint on an event
    public method Break {event expr} {
        set BREAK($event) $expr
        $evmgr Bind $event [list $this eBreakpoint $expr]
    }

    # list all break points
    public method Breakpoints {} {
        set res ""
        set fmt    "%15s %s"
        append res [format $fmt EVENT CONDITION]
        foreach ev [lsort [array names BREAK]] {
            append res "\n" [format $fmt $ev $BREAK($ev)]
        }
        set res
    }
    # -------------------------------------------------- Virtual Methods ---
    # called for every breakpoint we want to evaluate
    public method eBreakpoint {expr event arg} {
        # if we need to stop to the next event ...
        switch $TRACEMODE {
            stop-next {
                set TRACEMODE normal
                if [$evmgr UnBind **] {
                    unset BREAK(**)
                }
                Event <<stop>> $event step $arg
                return
            }
            skip {
                Event <<event>> $event $arg
                return
            }
        }
        # User defined breakpoint

        # does the predicate match?
        if [catch {stop_in $arg $expr} rc] {
            Event <<error>> bpeval $event $rc
        } elseif { $rc } {
            # report stopping
            Event <<stop>> $event $expr $arg
        } else {
            # just report the event
            Event <<event>> $event $arg
        }
    }
    # -------------------------------------------------- Protected Variables 
    protected variable BREAK
    protected variable TRACEMODE normal
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected proc stop_in {_vector _expr} {
        if { $_expr == "" } { return 1 } 
        foreach {_a _v} $_vector {
            set $_a $_v
        }
        subst -nocommands $_expr
        eval expr $_expr
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
