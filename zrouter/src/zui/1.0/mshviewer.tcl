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

package provide ade::ui 1.0

#
# Simple MSH viewer
#

#       Class: MSHViewer
# Description: view msh
#      Author: Manolis M. Tsangaris
#
itcl::class MSHViewer {
    inherit LFrame LBinder
    # Event <<task-info>> task attributes ;# task info requested
    # -------------------------------------------------- Public Variables --
    public variable rpc ""
    # -------------------------------------------------- Public Methods ----
    public method Monitor {{t 5000}} {
        Clear
        set job *
        eMonitor $t
    }
    public method Clear {} {
        $p1 Clear
        $p2 Clear
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable p1
    protected variable p2
    protected variable l1
    protected variable l2
    protected variable job "*"
    protected variable d1
    protected variable d2
    protected variable task ""
    protected variable tl
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        mkpanel $This.panel
    }
    destructor {
        $p2 delete
        $p1 delete
        catch {after cancel $task}
    }
    # -------------------------------------------------- Private Methods ---
    protected method mkpanel {w} {
        frame $w
        set tl [label $w.tl -text "Multiprocessing Status"]
        grid $w.tl -columnspan 2
        bind $w <Enter> "$this eMonitor"

        set l1 [label $w.l1 -text "" -relief sunken -border 1]
        set l2 [label $w.l2 -text "" -relief sunken -border 1]
        grid $l1 $l2 -sticky ew
        bind $l1 <1> "$this eJobSelect *"

        set p1 [LSquares $w.p1 -width 90 -height 30]
        set p2 [LSquares $w.p2 -width 90 -height 30]
        grid $w.p1 $w.p2
        pack $w -side top -expand 1 -fill both

        $p1 Bind <<click1>>  $this eJobSelect
        $p1 Bind <<enter>>   $this eJobView
        $p2 Bind <<enter>>   $this eTaskView show
        $p2 Bind <<click1>>  $this eTaskView examine
        $p2 Bind <<click3>>  $this eTaskOptions

        set d1 [label $w.d1 -text "" -relief sunken -border 1]
        set d2 [label $w.d2 -text "" -relief sunken -border 1]
        grid $d1 $d2 -sticky ew

        $p2 Bind <<leave>>  $this eClear $d2
        $p1 Bind <<leave>>  $this eClear $d1

        set cdef {
            defined wheat queued orange running darkseagreen done lightblue
            DEF wheat QUEUE orange DISP darkseagreen DONE lightblue
        }
        $p1 ItemColors $cdef
        $p2 ItemColors $cdef
    }
    # ----------------------------------------------- Callbacks
    public method eMonitor {{t 5000}} {
        catch {after cancel $task}
        $tl configure -bg darkseagreen ; update
        if [update_status] {
            $tl configure -bg white
            set task [after $t "$this eMonitor $t"]
        } else {
            # monitor less frequently, due to inactivity
            set job *                                    ;# monitor all jobs 
            set task [after 120000 "$this eMonitor $t"]
            $tl configure -bg "#d9d9d9"
        }
        return ""
    }
    public method eJobSelect {_job} {
        set job $_job
        update_status
    }
    public method eJobView {j} {
        array set S [$rpc Run JobStatus $j]
        $d1 configure -text "[nameit $j] $S(state)"
    }
    public method eTaskView {mode tid} {
        array set S [$rpc Run TaskStatus $tid]
        switch $S(state) {
            DISP { set t "running" }
            DONE { set t "done" }
            default { set t $S(state) }
        }
        set S(vstate) $t
        $d2 configure -text "[nameit $tid]: $t"
        if {$mode == "examine"} {
            Event <<task-info>> [array get S]
        }
    }
    public method eClear {d _} {
        $d configure -text ""
    }
    public method eTaskOptions {tag} {
        if [catch {$rpc Run slaves $tag} rc] {
            puts "trouble connecting to backend ($rc)"
            return
        }
        array set S $rc
        if ![info exists S($tag)] {
            puts "$tag: does not exist"
            return
        }
        # 
        if { [$rpc Run Break $tag] == 1 } {
            notice mshview "$tag: task not ready to be inspected yet"
            return
        }
        array set T $S($tag)
        puts "task: $tag at $T(port)@$T(host)"
        exec zui -s $T(port)@$T(host) &
    }
    # ----------------------------------------------- Interface Implementation
    # -- obtain multiprocessing status - update panels
    protected method update_status {} {
        if [catch {$rpc Run Jobs} jobs] {
            return 0
        }
        $l1 configure -text "[llength $jobs] job(s)"
        set tasks 0
        set mj ""
        $p1 Clear
        $p2 Clear
        set more 0
        foreach j $jobs {
            array set S [$rpc Run JobStatus $j]
            $p1 ItemState $j $S(state)
            if { $S(state) != "DONE" } { incr more }
            if { [string match $job $j] } {
                incr tasks [update_tasks $j]
                lappend mj [nameit $j]
            }
            unset S
        }
        if { $tasks } {
            $l2 configure -text "$tasks $mj task(s)"
        }
        set more
    }
    protected method update_tasks {job} {
        set tasks [lsort [$rpc Run JobTasks $job]]
        $l2 configure -text "[llength $tasks] [nameit $job] task(s)"
        set n 0
        foreach t $tasks {
            array set S [$rpc Run TaskStatus $t]
            $p2 ItemState $t $S(state)
            incr n
        }
        set n
    }
    protected method nameit {nm} {
        namespace tail $nm
    }
    # -------------------------------------------------- Common Initialization
}

