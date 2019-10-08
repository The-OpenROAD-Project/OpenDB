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

package provide ads::rti 1.0

#       Class: RTINS
# Description: RTI nameserver
#      Author: Manolis M. Tsangaris
#
itcl::class RTINS {
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable opts "" { array set OPT $opts }
    # -------------------------------------------------- Public Methods ----
    public method List {} {
        array names CLIENT
    }
    public method Info {id} {
	if ![info exists CLIENT($id)] { return "" }
        set CLIENT($id)
    }
    public method State {id} {
	if ![info exists STATE($id)] { return "" }
        set STATE($id)
    }
    public method Address {} {
        $srv Address
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable OPT
    protected variable srv ""
    protected variable CLIENT
    protected variable STATE
    protected variable VIEWER
    protected variable INSPECT              ;# Record the interest of a viewer
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set srv [LRPCServer $this.srv -address $OPT(address)]

	# -- server to remote rti API
        $srv Bind register   $this eRegister
        $srv Bind vregister  $this eRegisterViewer
        $srv Bind unregister $this eUnRegister
        $srv Bind rerror     $this eReportError
        $srv Bind post       $this ePost
	# -- UI API
        $srv Bind inspect    $this eInspect start
        $srv Bind uninspect  $this eInspect stop
        $srv Bind event  $this eEcho

        $srv Bind <<logout>> $this eLogout

        $srv Bind List       $this List
        $srv Bind Info       $this Info
        $srv Bind State      $this State

        array set VIEWER {}
        array set INSPECT {}
    }
    destructor {
        $srv delete
    }
    # -------------------------------------------------- Private Methods ---
    # -- send an event to all connected clients
    protected method send_all {event args} {
        foreach viewer [array names VIEWER] {
            eval $srv SendEvent $viewer $event $args 
        }
    }

    # -- send those events only to subscribers
    protected method send_subs {toolid event args} {
	# -- inspect has keys of the form viewer/toolid
        foreach viewer [array names VIEWER] {
            if { [array names INSPECT $viewer/$toolid] != "" } {
                eval $srv SendEvent $viewer $event $args 
            }
        }
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eRegister {argv} {
        upvar peer peer
        puts "registered: $peer"

        set CLIENT($peer) $argv
        set STATE($peer) [list state unknown]
        send_all <<rtool-new-state>> $peer start
    }
    # register a viewer
    public method eRegisterViewer {argv} {
        upvar peer peer
        set VIEWER($peer) $argv
    }
    public method eUnRegister {} {
        upvar peer peer
        puts "unregister $peer"
        send_all <<rtool-new-state>> $peer exit
    }
    public method eLogout {peer} {
        puts "disconnect: $peer"
	# -- is this a CLIENT?
        if [info exists CLIENT($peer)] {
            unset CLIENT($peer)
            unset STATE($peer)
            send_subs $peer <<rtool-new-state>> $peer disconnect
        }
	# -- is it a viewer?
        if [info exists VIEWER($peer)] {
            unset VIEWER($peer)
        }

        # clean up inspections
        foreach a [array names INSPECT $peer/*] {
            unset INSPECT($a)
        }
        foreach a [array names INSPECT */$peer] {
            unset INSPECT($a)
        }
    }
    public method eReportError {error info} {
        upvar peer peer
        puts "REPORTERROR: $peer $error" 
        set STATE($peer) [list state error error $error info $info]
        # notify all connected viewers on the error condition
        send_subs $peer <<rtool-new-state>> $peer error
    }

    # rti client has an event to post 
    public method ePost {event args} {
        upvar peer peer
        # clean up new lines:
        # regsub -all \n $args \\n args
        eval send_subs $peer $event $args
    }

    # the viewer wants to inspect a remote tool 
    public method eInspect {mode id} {
        upvar peer peer
        if {$mode == "start"} {
            set INSPECT($peer/$id) ""
        } else {
            foreach a [array names INSPECT $peer/$id] {
                unset INSPECT($a)
            }
        }
    }
    public method eEcho {args} {
        puts events:$args
    }
    # -------------------------------------------------- Common Initialization
}
