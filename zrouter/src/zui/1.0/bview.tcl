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
#       Class: BView
# Description: Bird's eye view
#      Author: Manolis M. Tsangaris
#
itcl::class BView {
    inherit LFrame LBinder
    # Event <<select-region>> x0 y0 x1 y1
    # -------------------------------------------------- Public Variables --
    public variable width  "200"
    public variable height "200"
    public variable bbox    ""  { bbox_set $bbox } 
    public variable invy    1   ; # invert y axis?
    # -------------------------------------------------- Public Methods ----
    public method LoadTiles {tiles} {
        clear
        tiles_load $tiles
    }
    public method SelectionSet {x0 y0 x1 y1} {
        $csel SelectionSet $x0 $y0 $x1 $y1 
    }
    public method Colors { } {
        array get COLOR
    }
    public method SelectAll {} {
        $csel SelectAll
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable cv
    protected variable csel
    protected variable X0 ; protected variable X1
    protected variable Y0 ; protected variable Y1
    protected variable COLOR ; # -- per layer color
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set cv [canvas $This.cv -border 1 -relief sunken -bg gray50 ]
        pack $cv -side top -expand 1 -fill both 
        LBridge width  "$cv configure -width"
        LBridge height "$cv configure -height"

        set csel [CSelector $this.csel -canvas $cv -invy $invy -selcolor black] 
        # -- redirect events to us
        $csel Redirect <<select-region>> $this
        array set COLOR {
            default indianred
            0 forestgreen
            1 indianred
            2 darkseagreen
            3 blue
            4 lightblue
            5 orange
            6 gray
            7 purple
            8 orange
            15 indianred
        }
    }
    destructor {
        $csel delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method clear {} {
        catch {after cancel $task}
        $cv delete all
        $csel Reset
    }
    # ----------------------------------------------- Interface Implementation
    protected method bbox_set {bb} {
        foreach {X0 Y0 X1 Y1} $bb { break }
        $csel SetBB $bb
    }
    # ----------------------------------------------- Event
    protected method tiles_load {tiles} {
        # make sure that our window stabilizes ...
        update idletasks ; update

        $cv delete all
        # obtain the chip bbox
        if { $bbox == "" } { 
            # -- nothing loaded -- give up!
            return
        }
        if { $tiles == "" } {
            set tiles [list [list type tile id t1 bb $bbox]]
        }
        set btiles 0
        set err ""
        foreach tile $tiles {
            if { $tile == "" } { continue }
            set T(layer) default
            if [catch {array set T $tile} err] { 
                incr btiles
                continue
            }
            foreach {x0 y0 x1 y1} $T(bb) { break }
            # -- reverse the y coordinate
            if { $invy } { 
                set y0 [expr -1.0*$y0] ; set y1 [expr -1.0*$y1]
            }
            set n  [$cv create rectangle $x0  $y0 $x1 $y1 \
                -tags "tile $T(id)" \
                -outline yellow -width 1 -fill $COLOR($T(layer))]
            unset T
        }
        if { $btiles != 0 } {
            warning bview "$btiles tile(s) could not be loaded"
            warning bview "last error: $err"
        }
        $csel AutoFit
    }
    # -------------------------------------------------- Common Initialization
}
