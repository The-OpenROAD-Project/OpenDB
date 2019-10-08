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

#       Class: RTIC
# Description: RTI client
#      Author: Manolis M. Tsangaris
#
# RTIC implements a simple controller for a remotely running job
# RTIC will notify the server about interesting events
#
itcl::class RTIC {
    # Event <<rtool-output>> $lines
    # Event <<rtool-exit>>    exit $rc msg $msg
    inherit LObject LInteractor
    # -------------------------------------------------- Public Variables --
    public variable opts "" { array set OPT $opts }
    public variable timeout 100              ;# collect 100ms worth of output
    # -------------------------------------------------- Public Methods ----
    # -- run a remote command
    public method Run {cmd} {
	if { $fp == "" } { 
	    error "remote command exited"
	}
	if [catch {puts $fp $cmd} rc] {
	    error "$cmd: failed to send: $rc"
	}
	catch {flush $fp}
	# -- the command output will come back to us the normal way
	return ""
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable OPT
    protected variable srv ""
    protected variable me ""            ;# my inbound connection
    protected variable fp ""            ;# my command file channel
    protected variable task ""        
    protected variable OUT ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	# -- cleanup env
	global env
	catch {unset env(ENV)}
	if { $OPT(args) == "" } { 
	    puts stderr "rti: no command was supplied"
	    cleanup 1
	    return
	}

	set cmd ""
	foreach a $OPT(args) {
	    set cmd [concat $cmd $a]
	}

        set me  [LRPCServer $this.conn -address 7841- -debug 0]
	$me Bind Run $this Run
        set srv [LRPCClient $this.srv]
        $srv Connect $OPT(address)
        set reg ""
        lappend reg host   [info hostname]  pid [pid] conn [$me Address]
        lappend reg cmd $cmd
        $srv Run register $reg
	# -- now run the command
	rtic_run $cmd
    }
    destructor {
	# -- we should probably killtree here
	if { $fp == "" } { catch {close $fp} }
        $srv Run unregister
        $srv delete
        $me delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method rtic_run {cmd} {
	set cmd "/bin/sh -c $cmd"
	set fp [open "| /bin/sh" r+]
	fconfigure $fp -buffering line
	puts $fp "exec 2>&1"
	puts $fp "$cmd ; echo @@@ \$? ; exit 0"
	fileevent $fp r "$this eResponse $fp"
    }
    # ----------------------------------------------- Interface Implementation
    protected method cleanup {rc} {
	if { $task != "" } { eSend }
	Signal $rc
    }
    # ----------------------------------------------- CallBacks
    # -- process output lines
    public method eResponse {fp} {
	if { [gets $fp line] == -1 } {
	    $srv Run post <<rtool-exit>> exit 0 msg ""
	    close $fp
	    cleanup 0
	}
	append OUT $line \n
	if { $task == "" } { set task [after $timeout "$this eSend"] }
    }
    # -- send collected output
    public method eSend {} {
	set task ""
	if { $OUT == "" } { return }
        $srv Run post <<rtool-output>> $OUT
	set OUT ""
    }
    # -------------------------------------------------- Common Initialization
}
