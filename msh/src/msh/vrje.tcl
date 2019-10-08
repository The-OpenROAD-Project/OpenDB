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

package provide MSH 1.0

#
#       Class: VRJE
# Description: Run task on remote slaves
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
# This object simulates an RJE and owns all objects representing remote tasks
#
itcl::class VRJE {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable queue "" ;# task queue
    # -------------------------------------------------- Public Methods ----
    # -- run cmd using stdin and env. Configure vtask object with opt args
    public method Run {tid cmd stdin env {argv ""}} {
        # pass the TASKID
        lappend env MSHTID $tid
        set j [namespace eval :: VTask $tid -tid $tid]
        $j configure -cmd $cmd -env $env -stdin $stdin
	# -- configure the object, if needed
	if { $argv != "" } { 
	    eval $j configure $argv
	}
        $j Bind <<deleted>> $this eVTaskDeleted $j
        $queue Enq $j
        $j NewState QUEUE
        set TID($j) $tid
        set TASK($tid) $j
        set j
    }

    public method TaskStatus {tid} {
        $TASK($tid) Status
    }

    public method Kill {tid} {
        # ---
    }
    public method Tasks {} {
        array names TASK
    }
    # -- display the overall status
    public method SystemStatus {} {
        return "*** RJE OK ([array size TASK] tasks)\n"
    }

    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable TASK
    protected variable TID
    protected variable NTASKS 1000
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        array set TASK {}
        array set TID {}
    }
    destructor {
        # foreach t [array names TID] { $t delete } 
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- Callbacks
    # -- unregister a task
    public method eVTaskDeleted {task tid} {
        unset TID($task)
        unset TASK($tid)
    }
    # -------------------------------------------------- Common Initialization
}
