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

package provide ade::sui 1.0

package require mtlib 1.0

#
# SIMPLE EMBEDDABLE ZUI for ZROUTE
#
# zui start  : to start the gui
# zui_example: for an example
#

#
#       Class: MZUI
# Description: Simple Embeddable ZUI
#      Author: Manolis M. Tsangaris
#
itcl::class MZUI {
    inherit LTopLevelEditor
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method clear {}  {
        $cv Clear
    }
    public method line {args}  {
        $cv Line $args
    }
    public method text {args}  {
        $cv Text $args
    }
    public method box {args}  {
        $cv Box $args
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable cv ""
    protected variable hist ""
    protected variable con ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set cv [MCanvas $This.cv -width 640 -height 480]
        pack $cv -side top -expand 1 -fill both
        configure -title "zroute:mzui"
        set cv [namespace code $cv]

        set cmd  [entry $This.e -bg gray50]
        set hist [LConsoleHistory $this.hist]
        set con [LConsole $This.con -hist $hist -width 40 -height 2 -out 0]
        pack $con -side top -expand 1 -fill x
        $con Bind <<cmd>> $this eRunCmd
    }

    destructor {
        $cv delete
        $hist delete
        $con delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    public method vMakeButtons {w} {
        button_add $w.q -text Clear     -command "$cv Reset"
        button_add $w.f -text Refresh   -command "$cv Refresh"
    }
    # ----------------------------------------------- CallBacks
    public method eRunCmd {cmd} {
        $con Push $cmd
        puts "zroute> $cmd"
        if [catch {uplevel #0 $cmd} rc] {
            $con ReShow err $rc
            puts err:$rc
        } else {
            $con ReShow out $rc
            puts $rc
        }
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: MCanvas
# Description: Simple Canvas
#      Author: Manolis M. Tsangaris
#
itcl::class MCanvas {
    inherit LFrame
    # -------------------------------------------------- Public Variables --
    public variable width  640
    public variable height 480
    # -------------------------------------------------- Public Methods ----
    public method Clear {} {
        $cv delete all
        catch {unset TAG}
    }
    public method Refresh {} {
        Clear
        Redraw
        $csel AutoFit
    }
    public method Line {argv} {
        dl mk_line $argv
    }
    public method Text {argv} {
        dl mk_text $argv
    }
    public method Box {argv} {
        dl mk_box $argv
    }
    public method Redraw {} {
        foreach cmd $DLIST {
            eval $cmd
        }
    }
    public method Reset {} {
        Clear
        set DLIST ""
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable cv ""
    protected variable csel ""
    protected variable TAG
    protected variable task ""
    protected variable DLIST ""        ;# DISPLAY LIST
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set cv [canvas $This.cv -bg black]
        set csel [CSelector $this.csel -canvas $cv -invy 0 -selcolor red]
        pack $cv -side top -expand 1 -fill both
        LBridge width  "$cv configure -width"
        LBridge height "$cv configure -height"
    }
    destructor {
        $csel delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mk_coords {x y x1 y1} {
        set y  [expr -1.0 * $y ]
        set y1 [expr -1.0 * $y ]
        list $x $y $x1 $y1
    }
    protected method mk_line {argv} {
        # set coords [mk_coords [lrange $argv 0 1]]
        eval $cv create line $argv
    }
    protected method mk_box {argv} {
        eval $cv create rectangle $argv -fill red
    }
    protected method mk_text {argv} {
        set args [lrange $argv 3 end]
        foreach {x y text} $argv {break}
        eval $cv create text $x $y -text [list $text] -fill yellow $args
    }
    protected method dl {args} {
        lappend DLIST $args
        if { $task == "" } { set task [after 100 "$this eRefresh"] }
        return ""
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eRefresh {} {
        set task ""
        Refresh
    }
    # -------------------------------------------------- Common Initialization
}

set gZUI ""
proc zui {cmd args} {
    package require Tk
    package require Itcl
    wm withdraw .
    global gZUI
    switch -- $cmd {
        start { catch { $gZUI delete } ; set gZUI [eval MZUI .z#auto $args] }
        clear { $gZUI clear }
        line  { eval $gZUI line $args }
        text  { eval $gZUI text $args }
        box   { eval $gZUI box $args }
        default {
            puts stderr "unknown command: $cmd\n"
            puts stderr "Usage"
            puts stderr "mzui start: to initialize the GUI" 
            puts stderr "mzui clear: to clear the screen"
            puts stderr "mzui line x0 y0 x1 y1 ... xN yN"
            puts stderr "mzui text x0 y0 text"
        }
    }
}

proc zui_example {} {
    set cmd {

    # start the GUI
    zui start
    # clear canvas
    zui clear
    # draw a green box
    zui box 100 100 200 200 -fill green
    # draw a blue line
    zui line 0 0 90 100 90 150 200 200 -tags t1 -fill blue
    # attach text
    zui text 0 0 X1 -fill green
    # attach another text
    zui text 100 100 X2 -fill red

    # from within your C++ code use:
    # Tcl_Eval(interp,"zui text 0 0 {X1} -fill green",0);
    # or:
    # char cmd[1024]
    # sprintf(cmd,"zui text %f %f %f %f {%s}",x0,y0,x1,y1,text);
    # Tcl_Eval(interp,cmd,0);
    }
    puts $cmd
    eval $cmd
}
