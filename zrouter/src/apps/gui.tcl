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

package require Tk

namespace eval history {
    proc add? {w} {
    variable $w
    variable n$w
    upvar 0 $w hist
    set s [set ::[$w cget -textvariable]]
    if {$s == ""} return
    if [string compare $s [lindex $hist end]] {
        lappend hist $s
        set n$w [llength $hist]
    }
    }
    proc move {w where} {
    variable $w
    variable n$w
    upvar 0 $w hist
    incr n$w $where
    if {[set n$w]<0} {set n$w 0}
    if {[set n$w]>=[llength $hist]+1} {
        set n$w [llength $hist]
    }
    set ::[$w cget -textvar] [lindex $hist [set n$w]]
    }
    proc for {type name args} {
    switch -- $type {
        entry {
        uplevel $type $name $args
        bind $name <Up> {history::move %W -1}
        bind $name <Down> {history::move %W 1}
        bind $name <Next> {history::move %W 99999}
        bind $name <Return> {history::add? %W}
        variable $name {}
        variable n$name 0
        }
        default {error "usage: history::for entry <w> <args>"}
    }
    }
 }



text .zgt -wrap word -relief flat -state disabled
set zgtprompt "ade>"

proc zgtinsert { w text } {
    $w configure -state normal
    $w insert end $text
    $w configure -state disabled
    $w see end
}

proc zgtprocess { w cmd} {
    global zgtprompt
    global cmpcmd
    #zgtinsert $w "$zgtprompt $cmd\n"
    if { $cmpcmd == "" } {
        if { [info complete $cmd] } {
            ::zr.cli runcmd $cmd
        } else {
            zgtinsert $w $cmd
            append cmpcmd $cmd
        }
    } else {
        append cmpcmd $cmd
        if { [info complete $cmpcmd] } {
            ::zr.cli runcmd $cmpcmd
            set cmpcmd ""
        } else {
            zgtinsert $w $cmd
        }
    }

}

set cmpcmd ""
frame .zgf 
label .zgf.l -text $zgtprompt
history::for entry .zgf.e -textvar zgtcmd -relief flat
bind .zgf.e <Return> {+ zgtprocess .zgt $zgtcmd; set zgtcmd ""}
pack .zgf.l -side left
pack .zgf.e -side left -fill x -expand 1

eval pack [winfo children .] -fill both -expand 1
focus .zgf.e

rename exec ::tcl_exec

proc exec { cmd } {
    set rc [ catch { ::tcl_exec $cmd } out ]
    puts $out
    return $rc
}

::zr.zlog configure -guiwidget .zgt -guiinsert "zgtinsert"
