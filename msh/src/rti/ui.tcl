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

package provide ads::rti 1.0

#
#       Class: URTI
# Description: RTI user interface
#      Author: Manolis M. Tsangaris
#
itcl::class URTI {
    inherit LTopLevelEditor LInteractor
    # -------------------------------------------------- Public Variables --
    public variable opts "" { array set OPT $opts }
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable OPT
    protected variable hist
    protected variable cons
    protected variable sb
    protected variable nav
    protected variable W
    protected variable srv
    protected variable NODE ""                        ;# node under inspection
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        urti_css_setup
        pack [mkpanel $This.ui] -side top -expand 1 -fill both

        cRefresh
    }
    destructor {
        #$sb   delete
        $hist delete
        $nav  delete
        $cons delete
        $srv  delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mkpanel {w} {
        frame $w 
        configure -title "Remote Tool Inspector"
        set srv [LRPCClient $this.srv -debug 0]

        $srv Connect $OPT(address)
        $srv Bind <<rtool-new-state>> $this eToolNewState
        $srv Run vregister ""
        $srv Bind <<rtool-error>>      $this eToolError
        $srv Bind <<rtool-cmd>>        $this eToolCmd
        $srv Bind <<rtool-output>>     $this eToolOutput
        $srv Bind <<rtool-exit>>       $this eRtoolExit

        set nav  [CNavigator $This.nav -rpc $srv]
        set hist [LConsoleHistory $this.hist]
        set cons [TConsole $This.con -width 80 -height 15  -hist $hist]
        #set sb   [ScriptBrowser $This.sb -width 40 -height 15]

        $nav Bind <<select>>     $this eSelectNode
        $nav Bind <<connect>>    $this cConnect

        pack $nav  -side left -expand 1 -fill both
        #pack $sb   -side left -expand 1 -fill both
        pack $cons -side left -expand 1 -fill both
        set w
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method vMakeButtons {w} {
        button_add $w.q -text Quit -command "$this Signal 0"
        button_add $w.r -text Refresh -command "$this cRefresh"
        set w
    }
    public method cRefresh {} {
        $nav Refresh
    }
    # -- connect to a remote tool
    public method cConnect {{tag ""}} {
	if { $tag != "" } { set NODE $tag }
        if {$NODE == "" } { 
            bell ; return
        }
        # stop all other inspections
        $srv Run uninspect *
        # start inspecting this node
        $srv Run inspect $NODE

        array set P [$srv Run Info $NODE]
        $cons Connect [array get P]
        #$sb configure -title $P(host)
        $cons Clear
        #$sb Clear

        array set S [$srv Run State $NODE]
        if { $S(state) == "error" } {
            $cons Show err "$S(error) $S(info)"
        }
    }
    public method eSelectNode {node} {
        set NODE $node
    } 

    public method eToolNewState {tag state} {
	# update the brwoser
        $nav Update $tag $state
	if { $tag != $NODE } { return }
	switch $state {
	    disconnect {
		set NODE ""
		$cons Disconnect
	    }
	    start { }
	}
    }

    public method eToolError {msg} {
        $cons Show  err $msg
    }

    public method eToolCmd {cmd rc} {
        $cons Show cmd [fix $cmd]
        set rc [string trim [fix $rc]]
        if { $rc != "" } { 
            $cons Show out $rc
        }
    }
    protected method fix {s} {
        regsub -all {\\n} $s \n s
        regsub -all \n\$ $s "" s
        set s
    }

    public method eToolOutput {out} {
            $cons Show out $out
    }

    # the inspected tool has exited
    # pass exit and msg attributes
    public method eRtoolExit {args} {
	array set P $args
	$cons Show out "---\nExited ($P(exit) $P(msg))" 
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: CNavigator
# Description: Connection navigator
#      Author: Manolis M. Tsangaris
#
itcl::class CNavigator {
    inherit LFrame LBinder
    # Event <<select>>     tag
    # Event <<connect>>    tag
    # Event <<rtool-exit>> node
    # -------------------------------------------------- Public Variables --
    public variable title   "tools"
    public variable rpc      ""                  
    # -------------------------------------------------- Public Methods ----
    public method Refresh {} {
        refresh
    }
    public method Update {tag state} {
        rtool_update $tag $state
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable tree
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        winconfig -border 1 -relief groove
        pack [label $This.title -text $title] -side top -fill x
        LBridge title "$This.title configure -text"
        set tree [LDirTree $This.tree -width 50]
        pack $tree -side top -expand 1 -fill both
        $tree Bind <<select>> $this eSelect
        $tree Bind <<expand>> $this eExpand
        $tree configure -width 300
    }
    destructor {
        $tree delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method refresh {} {
        # $tree Clear
        $tree Add /tools -title TOOLS -type none
        foreach client [$rpc Run List] {
            rtool_add $client
        }
        update idletasks
        $tree Open /tools
        $tree Refresh
    }
    protected method rtool_add {tag} {
        array set P [$rpc Run Info $tag]
        array set S [$rpc Run State $tag]
	# -- not there?
	if ![array size S] { return }
        set prog $P(cmd)
        set tl  "$prog ($P(pid)@[lindex [split $P(host) .] 0])"
        $tree Add /tools/$tag -title $tl -type tool -tag $tag
        if { $S(state) == "error"  } {
            $tree Config /tools/$tag -titlefg red
        } else {
            $tree Config /tools/$tag -titlefg black
        }
    }
    protected method rtool_update {tag state} {
        switch $state {
            start { }
            error { bell }
            exit { }
            disconnect {
                $tree Delete /tools/$tag
                Event <<rtool-exit>> /tools/$tag
            }
        }
        refresh
    }
    # ----------------------------------------------- CallBacks
    public method eSelect {path} {
	if { $path == "" } { Event <<select>> "" ; return }
	switch [$tree Get $path type] {
	    node {
		Event <<select>> [$tree Get $path tag]
	    }
	    default { Event <<select>> "" }
	}
    }
    public method eExpand {path} {
	if { $path == "" } { return }
	switch [$tree Get $path type] {
	    tool {
		Event <<connect>> [$tree Get $path tag]
	    }
	    default { Event <<select>> "" }
	}
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: TConsole
# Description: Remote Console
#      Author: Manolis M. Tsangaris
#
itcl::class TConsole {
    inherit LFrame
    # -------------------------------------------------- Public Variables --
    public variable title "console"
    public variable width  40
    public variable height 20
    public variable hist   ""
    # -------------------------------------------------- Public Methods ----
    public method Connect {alist} {
        array set P $alist
        con_connect P
    }
    public method Disconnect {} {
        $rtool Disconnect
        configure -title "Disconnected"
    }
    public method Show {type info} {
        $cons Show $type "$info"
    }
    public method Clear {} {
        $cons Clear
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable cons
    protected variable rtool    ""     ;# remote tool connection
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        winconfig -border 1 -relief groove
        pack [label $This.title -text $title] -side top -fill x
        LBridge title "$This.title configure -text"
        set cons [LConsole $This.cons -width $width -height $height \
	    -autoscroll 1 -hist $hist]
        $cons configure -append 1
        pack $cons -side top -expand 1 -fill both
        foreach v "width height hist" {
            LBridge $v  "[namespace code $cons] configure -$v"
        }
        $cons Bind <<cmd>> $this eRunCmd
 
        set rtool [LRPCClient $this.peer]
    }
    destructor {
        $cons delete
        $rtool delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method con_connect {pp} {
        upvar $pp P
        $rtool Disconnect
        $rtool Connect $P(conn)
        configure -title "$P(cmd) ($P(host))"
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eRunCmd {cmd} {
        $cons Show cmd $cmd
        # is it a local command?
        set cm [list uplevel #0 [list $rtool Run Run $cmd]]
        if { [catch $cm rc] } {
	    set mode err
        } else {
	    set mode out
        }
	if { $rc != "" } { $cons Show $mode $rc }
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: ScriptBrowser
# Description: Simple Script Browser
#      Author: Manolis M. Tsangaris
#
itcl::class ScriptBrowser {
    inherit LFrame
    # -------------------------------------------------- Public Variables --
    public variable title "script"
    public variable width  40
    public variable height 20
    # -------------------------------------------------- Public Methods ----
    public method Clear {} {
        $cons delete 0.0 end
    }
    public method Add {line} {
        $cons insert end $line \n
	$cons see end
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable cons ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        winconfig -border 1 -relief groove
        pack [label $This.title -text $title] -side top -fill x
        LBridge title "$This.title configure -text"
        set cons [LScrollableWidget $This.ws text -bg gray80]
        pack $This.ws -side top -expand 1 -fill both

        foreach v "width height" {
            LBridge $v  "$cons configure -$v"
        }
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
