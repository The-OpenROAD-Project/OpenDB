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

package provide LMSH 1.0

namespace eval msh {

variable MSHC ""      ;# MSH controller object
variable MDS ""       ;# MDS interface object
variable PROGRAM      ;# program name (argv0)
variable OPT          ;# program options, set by ade (correspond to argv)
variable STARTTIME 0  ;# session start time
variable MAGIC1 "<START>" ;# start and stop markers for remote job output
variable MAGIC2 "<END>"

namespace export spawn monitor status msh respawn

proc init {} {
    variable MSHC
    variable PROGRAM
    variable OPT
    global argv0 env

    set PROGRAM [find_link $argv0]
# Hack to remove binary specific path from PATH if environment MPT_ARCH is set.
if { [info exists ::env(MPT_ARCH)] } {
    set fastid $::env(PATH)
    set fastid1 [split $fastid :]
    set counfastid [lreplace $fastid1 0 1]
    set newpath [join $counfastid :]
    set ::env(PATH) $newpath
# Strip path information from variable argv0 to enable executing from wrapper tree.
    set PROG1 [split $PROGRAM /]
    set scount [expr [llength $PROG1] - 4]
    set lcount [expr $scount + 1]
    set NPROG1 [lreplace $PROG1 $scount $lcount]
    set PROG4 [join $NPROG1 /]
    set PROGRAM $PROG4
} else {
    puts "No Environment Variable MPT_ARCH detected"
    set np [file split $PROGRAM]
    set np [lreplace $np end-3 end-2]
    set cmd "set np \[file join $np]"
    eval $cmd
    if { [file exists $np] == 1 } {
        puts "$np found!"
        puts "Using $np as remote executable!"
        set PROGRAM $np
    } else {
        puts "$np not found!"
        puts "Using $PROGRAM as remote executable!"
    }
}
    # -- are we a remote instance of a master?
    # -- then do not bother starting msh or making any commands available
    if ![info exists env(MSHMASTER)] {
        set env(MSHMASTER) [info hostname]
        set MSHC [MSHC mshc#auto -options [array get OPT]]
        set MSHC [namespace current]::$MSHC
        namespace eval :: [list LExportMethods $MSHC \
                "Jobs JobStatus JobTasks TaskStatus TasksStatus"]

        namespace eval :: namespace import [namespace current]::*
    }
    if ![info exists env(MDS)] {
        error "unable to contact MDS: no env(MDS)"
    }
    # -- initialize MDS
    mds_init $env(MDS)

    # -- initialize enhanced logger
    if { [info command el] != "el" } {
        ELogger el
    }
    return ""
}

# -- provide configuration for some mds parameters
proc configure {args} {
    variable PROGRAM
    foreach {var val} $args {
    switch @$var {
        @-program {
        if ![file exists $val] {
            error "configure $var $val: file does not exist"
        }
        if ![file executable $val] {
            error "configure $var $val: not an executable file"
        }
        # -- we hope this is a proper ADE file
        set PROGRAM $val
        }
        default {
        error {usage: configure [-program progname]}
        }
    }
    }
}

# -- find the link of a given executable
proc find_link {f} {
    set f [file normalize $f]
    set d [file dirname $f]
    if ![catch {file readlink $f} l] {
        set l [file join $d $l]
    } else {
        set l $f
    }
    set l
}

# -- set options (the ones of ade argv)
proc SetOpt {opt} {
    variable OPT
    variable STARTTIME 
    array set OPT $opt
    # -- note the starting time in seconds
    set STARTTIME $OPT(sstarttime)
}

proc Analysis {tasks rr} {
    upvar $rr R
    array set S   "QUEUE 0 DEF 0 DISP 0"
    array set MAX "QUEUE 0 DEF 0 DISP 0"
    variable STARTTIME
    variable OPT

    set NTASKS 0
    set timeline ""
    foreach t $tasks {
        array set Q [$t Status]
        array set QS $Q(stats)
        foreach state [array names S] {
            if [info exists QS($state)] {
                incr S($state) $QS($state)
                if { $MAX($state) < $QS($state) } {
                    set MAX($state)  $QS($state)
                }
            }
        }
        set RT($t) [list $QS(QUEUE) $QS(DISP)]
        append timeline [task_timeline $t $STARTTIME Q ~]
        incr NTASKS
    }
    array set R [list TQUEUE $S(QUEUE) MQUEUE $MAX(QUEUE)]
    array set R [list AQUEUE [expr $S(QUEUE)/$NTASKS]]
    array set R [list TDISP $S(DISP) MDISP $MAX(DISP)]
    array set R [list ADISP [expr $S(DISP)/$NTASKS]]
    set R(tasks) [array get RT]

    if {$OPT(mark) } {
        set R(TIMELINE) $timeline
    }
    set R(NTASKS) $NTASKS
}

proc PrintAnalysis {rr} {
    upvar $rr R

    el notice "Task Analysis: $R(NTASKS) tasks"
    if { $R(NTASKS) == 0 } {  return }

    el notice [format "%15s: %6.1f tot %6.1f avg %6.1f max" \
        "Queue Time" $R(TQUEUE) $R(AQUEUE) $R(MQUEUE)]

    el notice [format "%15s: %6.1f tot %6.1f avg %6.1f max" \
        "Run Time"   $R(TDISP) $R(ADISP) $R(MDISP)]
    if [info exists R(tasks)] { 
        array set T $R(tasks)
        el notice [format "%18s %5s %5s" Task Queue Run]
        foreach task [lsort [array names T]] {
            foreach {qt rt} $T($task) {break}
            el notice [format "%18s %5d %5d" $task $qt $rt]
        }
    }

    if [info exists R(TIMELINE)] {
        el notice "$R(TIMELINE)"
    }
}

proc cleanup {} {
    variable MSHC
    variable MDS
    $MSHC delete ; set MSHC ""
    $MDS delete ; set MDS ""
}


# initialize MDS
proc mds_init {address} {
    variable MDS
    set MDS  [namespace code [ZMDSClient #auto -address $address]]
    namespace eval ::\
    [list LExportMethods $MDS \
    "Publish Subscribe Wait mstatus NextVersion Unpublish"]
}

# -------------------------------------------------------------- new api
proc msh {args} {
    variable MSHC
    eval $MSHC $args
}

# -- spawn a tcl task running tcl 'command' -- configure object with args
proc spawn {task command args} {
    variable MSHC
    variable PROGRAM
    variable MAGIC1
    variable MAGIC2

    # construct a command to run the remote user command
    set fcmd {
        fconfigure stdout -buffering line
        if { [catch {%s} rc] } {
            global errorInfo
            puts "%s[list error \$rc \$errorInfo]%s"
        } else {
            puts "%s[list return \$rc]%s"
        }
        flush stdout
    }
    set stdin [format $fcmd $command $MAGIC1 $MAGIC2 $MAGIC1 $MAGIC2]
    set tj [$MSHC Spawn $PROGRAM $task $stdin $args]
}

# -- resubmit an old task optionally create a new one
proc respawn {oldtask {newtask ""}} {
    variable MSHC
    set stdin [$oldtask cget -stdin]
    set cmd [$oldtask cget -cmd]
    set timeout [$oldtask cget -timeout]
    set serial [$oldtask cget -serial]
    incr serial
    if {$newtask == "" } {
	$oldtask delete
	set newtask $oldtask
    }
    set argv [list -serial $serial -timeout $timeout]
    set tj [$MSHC Spawn $cmd $newtask $stdin $argv]
    set tj
}

# -- monitor a bunch of tasks
proc monitor {tasks {body ""}} {
    variable MSHC

    set now [clock seconds]

    set rtasks ""
    foreach t $tasks {
        if [catch {$t cget -tid} rc] {
            LLog monitor warning "task $t: cannot monitor: $rc"
            continue
        }
        set TASK($rc) $t
        lappend rtasks $t
    }

    # any tasks to monitor?
    if { $rtasks == "" } {
        return 
    }

    mark main/monitor-start

    array set B $body

    # clear up the task variable
    catch {uplevel unset task}

    set sema [namespace current]::[LSemaphore #auto]

    array set STAT  "DONE 0 DEF 0 QUEUE 0 DISP 0 now 10000"
    array set LSTAT "DONE 0 DEF 0 QUEUE 0 DISP 0 now 0"

    set STAT(total) [array size TASK]
    set MAXT 15                         ;# track status every so often (15 sec)
    set MAXD [expr $STAT(total) / 10]   ;# state change tracking (10% done)

    while { [set ntasks [array names TASK]] != "" } {
        update idletasks
        # get the task status in bulk
        array set T [$MSHC TasksStatus $ntasks]
        foreach t $ntasks {
            array set S $T($t)
            set S(task) $TASK($t)

            switch $S(state) {
                DONE  { incr STAT(DONE) }
            }
    
            set STAT(now) [clock seconds]
            state_show STAT LSTAT $MAXD $MAXT

            if { $S(state) == "DONE" } {
                if [info exists B(done)] {
                    parse_output S $S(output)

		    # -- make semaphore available so they can "break"
		    set S(sema) $sema

                    uplevel [list array set task [array get S]]
                    if { [catch {uplevel $B(done)} rc]} {
                        LLog monitor warning "error:monitor done: $B(done): $rc"
                    }
                }
                unset TASK($t)
            }
            if [info exists B(check)] {
		# -- make semaphore available so they can "break"
		set S(sema) $sema
		uplevel [list array set task [array get S]]
		if { [catch {uplevel $B(check)} rc]} {
		    LLog monitor warning "error:monitor check: $B(check): $rc"
		}
            }
            unset S
        }
        update idletasks
        $sema Wait 2000
    }
    $sema delete
    # force last statistics
    state_show STAT LSTAT
    Analysis $rtasks R
    #PrintAnalysis R
    mark main/monitor-end
}

# -- return msh status
proc status {} {
    variable MSHC
    $MSHC Info
}

# -- print statistics, if they have changed, or if long time has
# -- passed
# --   maxd: state change threshold
# --   maxt: time change threshold
proc state_show {now last {maxd -1} {maxt -1}} {
    upvar $now STAT
    upvar $last LSTAT
    if { [expr $STAT(now) - $LSTAT(now)] > $maxt } {
        state_print STAT
        array set LSTAT [array get STAT]
        return
    }
    # compute changes of statistics
    set dt 0
    # collect changes of states
    foreach state [array names STAT {[A-Z]*} ]  {
        incr dt [expr abs($STAT($state) - $LSTAT($state))]
        if { $dt > $maxd } {
            state_print STAT
            array set LSTAT [array get STAT]
            return
        }
    }
}

# -- parse output and extract return values
# -- place them in the task array
proc parse_output {ss out} {
    upvar $ss S
    variable MAGIC1
    variable MAGIC2
    regsub "^.*$MAGIC1" $out "" out
    regsub "$MAGIC2.*\$" $out "" out
    array set S {error {} return {} errorInfo {}}
    # -- just in case $out is not a proper list
    if { [catch {lindex $out 0} cmd] } {
	set S(error) "abnormal termination of remote task ($cmd)"
	return
    }
    switch $cmd {
        return {
            set S(return)    [lindex $out 1]
        }
        error {
            set S(error)     [lindex $out 1]
            set S(errorInfo) [lindex $out 2]
        }
        default {
            set S(error)     "abnormal termination of remote task"
        }
    }
}
proc state_print {stat} {
    upvar $stat STAT
    set out "$STAT(total) tasks"
    foreach state "DONE" {
        if { $STAT($state) != 0 } {
            append out ", $STAT($state) $state"
        }
    }
    el notice "monitor: $out"
}

# ---------------------------------------------------------------
# -- Return the task timeline
# --
# --       prefix: string prefix for the status output
# --         task: the name of the task
# -- sessionstart: the session start time
# --           ss: is the state of th task
proc task_timeline {task sessionstart ss {prefix ""}} {
    upvar $ss S
    array set ST $S(stats)
    set last [expr $S(start) - $sessionstart]
    set out "# $task timeline\n"
    foreach state "DEF QUEUE DISP" {
        append out  "$prefix$last $task/$state\n"
        set last [expr $last + $ST($state)]
    }
    append out "$prefix$last $task/EXIT\n"
    set out
}


# -- END OF NAMESPACE msh
}
