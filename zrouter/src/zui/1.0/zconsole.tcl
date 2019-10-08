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

#       Class: ZConsole
# Description: Console to talk to the zroute
#      Author: Manolis M. Tsangaris
#
itcl::class ZConsole {
	inherit LTopLevelEditor
    # -------------------------------------------------- Public Variables --
	public variable rpc ""       ;# server to connect to
	public variable hfile "hist" ;# suffix of the history file
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
	protected variable hist ""
	protected variable con  ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
		eval configure $args
		configure -title "zroute console"
		set hist [LConsoleHistory $this.hist]
		set con  [LConsole $This.con -hist $hist -width 80 -height 15]
		$con Bind <<cmd>> $this eRunCmd
		pack $con -side top -fill both -expand 1
	}
    destructor {
		$hist delete
	}
    # -------------------------------------------------- Private Methods ---
	# ----------------------------------------------- Interface Implementation
	public method vMakeButtons {w} {
		button_add $w.hide  -text Hide  -command "$this Hide"
		button_add $w.clear -text Clear -command "[namespace code $con] Clear"
	}
    # -------------------------------------------------- CallBacks       ---
	public method eRunCmd {cmd} {
		$con Show cmd $cmd
		# is it a local command?
		if { [string match !* $cmd] } {
			set cm "uplevel #0 [string range $cmd 1 end]"
		} else {
			set cm "uplevel #0 $rpc Run $cmd"
		}
		if { [catch $cm rc] } {
			$con Show err $rc
		} else {
			$con Show out $rc
		}
	}
    # -------------------------------------------------- Common Initialization
}
