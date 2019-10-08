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

#       Class: VTask
# Description:
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
# Represent and manage a running task
#
itcl::class VTask {
    inherit LObject LBinder
    # Event <<deleted>>
    # -------------------------------------------------- Public Variables --
    public variable class default          ;# task class
    public variable timeout 0              ;# do timeout
    public variable cmd ""                 ;# the command to run
    public variable env ""                 ;# environment to pass
    public variable tid 0                  ;# task ID
    public variable output ""              ;# current output
    public variable stdin ""               ;# stdin given
    public variable host ""                ;# where it runs
    public variable serial 1               ;# task serial number (for resub)
    # -------------------------------------------------- Public Methods ----
    public method Info {} {
	set cmd
    }
    public method NewState {state} {
        task_record $state
    }
    public method Stats {ss} {
        upvar $ss S
        array set S [task_stats]
    }
    public method PrintStats {} {
        format "%5s %4s sec queue %4s sec disp" $tid $STAT(QUEUE) $STAT(DISP)
    }
    public method Status {} {
        list state $LASTSTATE stats [task_stats] output $output \
            host $host tid $tid start $STARTTIME class $class
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable STARTTIME
    protected variable STAT
    protected variable LASTSTATE  ""
    protected variable LASTTIME ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set STARTTIME [clock seconds]
        array set STAT "DEF 0 QUEUE 0 DISP 0"
        NewState DEF
    }
    destructor {
        Event <<deleted>> $tid
    }
    # -------------------------------------------------- Private Methods ---
    protected method task_record {state} {
        set now [clock seconds]
        # -- charge time to the last state
        if { $LASTSTATE != "" } {
            set STAT($LASTSTATE) [expr $now - $LASTTIME]
        }
        set LASTSTATE $state
        set LASTTIME $now
    }
    # give us the time (in sec) on each state
    protected method task_stats {} {
        # update the time on the current state
        switch $LASTSTATE {
            DONE - "" { }
            default {
                set now [clock seconds]
                set STAT($LASTSTATE) [expr $now - $LASTTIME]
            }
        }
        array get STAT
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
