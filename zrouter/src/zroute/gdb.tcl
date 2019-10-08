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

package provide ade::gdb 1.0

#
# gdb debug utilities for ade
#

namespace eval ade::gdb {
    variable dbpid { 0 0 }
    variable zr                                 ;# name of the executable
    namespace export dbkill gdb xxgdb ddd

proc init {} {
    variable zr
    set zr [info nameofexecutable]
}

# -- kill debugger
proc dbkill { } {
    variable dbpid
    foreach { sig pid } $dbpid { break; }
    if { $pid != 0 } {
        switch { $sig } {
            -INT { exec kill -INT $pid }
            -HUP { exec kill -HUP $pid }
            default { exec kill -INT $pid }
        }
        set dbpid { 0 0 }
    }
}

# -- run gdb on this ade session
proc gdb { {display ""} args } {
    variable zr
    variable dbpid
    set pid [pid]
    set dbpid -INT
    if { $display == "" } {
        set cmd "exec xterm -e gdb $zr $pid $args &"
    } else {
        if { $display == 0 } {
            set cmd "exec xterm -e gdb $zr $pid $args &"
        } else {
            set cmd "exec xterm -display $display -e gdb $zr $pid $args &"
        }
    }
    lappend dpid [eval $cmd]
}

# -- run xxgdb on this session
proc xxgdb { {display ""} } {
    variable dbpid
    variable zr 

    set pid [pid]
    set dbpid -HUP

    if { $display == "" } {
        lappend dbpid [exec xxgdb $zr $pid &]
    } else {
        lappend dbpid [exec xxgdb -display $display $zr $pid &]
    }
}

# -- run ddd (?)
proc ddd { {display ""} args } {
    variable zr
    variable dbpid

    set dbpid -HUP
    set pid [pid]

    if { $display == "" } {
        lappend dbpid [exec ddd $zr $pid $args &]"
    } else {
        lappend dbpid [exec ddd -display $display $zr $pid $args &]"
    }
}

}
