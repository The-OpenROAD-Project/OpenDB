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
#       Class: CCanvas
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class CCanvas {
    inherit LFrame LBinder
    # event 
    # Event <<touch>> button x y
    # Event <<progress>> percent
    # Event <<select-region>> x0 y0 x1 y1
    # -------------------------------------------------- Public Variables --
    public variable colorize type
    public variable layers ""
    public variable colors "" { array set COLOR $colors }
    public variable bbox ""   { $csel SetBB $bbox }
    public variable filters "" ; # filters used for this data set
    public variable invy 0
    public variable deltax 1 ;# selection increment - x dir
    public variable deltay 1 ;# selection increment - y dir
    public variable selcolor yellow
    public variable keepwbb 0   ;# should we keep world bb on autofit?
    # -------------------------------------------------- Public Methods ----
    public method Show {} {
        update idletasks
        $csel AutoFit
        cv_colorize
    }
    public method Clear {} {
        $cv delete all
        set SELECTED ""
        $csel Clear
        catch {unset ID}
        catch {unset LAYER}
        catch {unset BG}
        catch {unset MARK}
        array set LAYER {}
    }
    # select an object : return 1 if the object exists
    public method Select {tag} {
        cv_select $tag
    }
    public method SelectionClear {} {
        selection_clear
    }
    # -- apply filters
    # -- it is possible that our dataset does not have all the objects
    # -- filtered. In that case, we will return a list of missing object types
    public method Visible {nfilters} {
        array set F $nfilters
        array set F0 $filters
        set ON "" ; set OFF ""
        # -- collect visible and invisible tags
        foreach attr [array names F] {
            foreach {value visibility} $F($attr) {
                if { $visibility } {
                    lappend ON $attr:$value
                } else {
                    lappend OFF $attr:$value
                }
            }
        }
        # -- apply visible
        foreach tag $ON  { $cv itemconfig $tag -state normal }
        # -- apply not visible
        foreach tag $OFF { $cv itemconfig $tag -state hidden }
    }
    # -- return all marks found
    public method Marks {} {
        array get MARK
    }
    # -- show objects based on their marks
    # -- if Mark=0 do not show
    # -- if Mark=1 show normal
    # -- if Mark=2 show highlighted
    public method ViewMarks {mm} {
        marks_view $mm
    }

    public method AddObj {instance ss} {
        upvar $ss S
        cv_add_obj $instance S
    }
    public method Group {start list} {
        cv_group $start $list
    }

    # return the cursor region at x y, in real coordinates
    public method CursorRegion {x y} {
        $csel Unscale [expr $x - 1] [expr $y - 1] [expr $x + 1] [expr $y + 1]
    }

    # return the pixel grid size in world coordinates
    # rows and columns represent how many pixels we want to consider 
    # for the grid
    public method PixelGrid {rows cols x0 y0 x1 y1} {
        $csel PixelGrid $rows $cols $x0 $y0 $x1 $y1
    }

    # -- se if object $id exists in canvas
    public method Exists {id} {
        llength [$cv find withtag id:$id]
    }

    # -- define the rubber band selection box
    public method SetSelectionBox {x0 y0 x1 y1} {
        $csel SetSelection $x0 $y0 $x1 $y1
    }

    # -- return the real bbox of the objects drawn
    public method GetBBox {{tag all}} {
        set bb [string trim [$cv bbox $tag]]
        if { $bb == "" } { return "" }
        eval $csel Unscale $bb
    }
    public method Dump {} {
        set im [image create photo]
        update idletasks ; update
        $im configure -format window -data $cv
        set im
    } 
    public method Zoom {mode} {
        $csel eZoom $mode
    }
    # autofit all objects in canvas
    public method AutoFit {} {
        $csel AutoFit 0
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable cv
    protected variable cmd
    protected variable COLOR
    protected variable N 0
    protected variable ID
    protected variable LAYER
    protected variable MARK
    protected variable csel

    # -- background of selected items
    protected variable BG

    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        pack [cv_make $This.cv] -expand 1 -fill both
        set csel [CSelector $this.sel -canvas $cv -invy $invy -keepwbb $keepwbb]
        $csel Redirect <<select-region>>    $this
        $csel Redirect <<selecting-region>> $this
        $csel Redirect <<escape>>           $this
        LBridge deltax "$csel configure -deltax"
        LBridge deltay "$csel configure -deltay"
        array set LAYER {}
    }
    destructor {
        $csel delete
    }
    # -------------------------------------------------- Private Methods ---
    private method cv_add_obj {id ss} {
        upvar $ss S
        set tags ""
        set bb "" ; set dd ""
        if [info exists S(bb)] { set bb $S(bb) ; unset S(bb) }
        if [info exists S(dd)] { set dd $S(dd) ; unset S(dd) }

        # collect all marks
        if [info exists S(mark)] {
            foreach m $S(mark) {
                set MARK($m) $m
                lappend tags mark:$m
            }
            unset S(mark)
        }

        foreach t [array names S] {
            lappend tags $t:$S($t)
        }

        if { $bb != "" } {
            foreach {x0 y0 x1 y1} $bb { break }
            set y0 [expr -$y0] ; set y1 [expr -$y1]
            # via has an additional diagonal line
            switch $S(type) {
                via {
                    set n  [$cv create rectangle $x0 $y0 $x1 $y1 \
                        -outline gray50]
                    set n1 [$cv create line $x0 $y0 $x1 $y1 -fill gray50]
                    set ID($n1) $id
                }
                default {
                    set n [$cv create rectangle $x0 $y0 $x1 $y1 -outline ""]
                }
            }
        }
        # -- draw a closed polygon
        if { $dd != "" } {
            set points ""
            foreach {x0 y0} $dd {
                lappend points $x0 [expr -$y0]
            }
            lappend points [lindex $dd 0] [expr -[lindex $dd 1]]
            set n [eval $cv create polygon $points]
        }
        $cv itemconfigure $n -tags $tags
        set ID($n) $id
        if ![info exists S($colorize)] { set S($colorize) default }
        # -- keep track of layers
        set LAYER($S(layer)) ""
    }

    private method cv_colorize {} {
        foreach n [array names LAYER] {
            $cv itemconfigure $colorize:$n  -fill [cv_color $n]
        }
    }
    private method cv_color {class} {
        if ![info exists COLOR($class)] {
            return $COLOR(default)
        }
        return $COLOR($class)
    }
    private method cv_make {w} {
        set cv [canvas $This.cv -border 1 -relief sunken -bg black]
        # just absorb the SHIFT+B1,2
        bind $cv <Shift-ButtonPress-1> "info level"
        bind $cv <Shift-ButtonPress-3> "info level"

        bind $cv <ButtonPress-1> "$this eTouch 1 %x %y"
        bind $cv <ButtonPress-3> "$this eTouch 3 %x %y"

        bind $cv <ButtonPress-3> "$this eTouch 3 %x %y"

        set w
    }

    public method cv_find {tag pp} {
        upvar $pp P
        set t [$cv find withtag $tag]
        set P(nid) $t
        set P(id) $ID($t)
        foreach tg [$cv gettags $t] {
            if { [scan $tg type:%s P(type)] == 1 } { 
                scan [$cv bbox $t] "%s %s" P(x) P(y)
                return 1
            }
        }
        return 0
    }
    # -------------------------------------------------------------------------
    protected method cv_select {tag} {
        if { $tag == "" } { return 0}
        set tag id:$tag
        set n 0
        foreach t [$cv find withtag $tag] {
            incr n
            # is it already selected?
            if [info exists BG($t)] { continue }
            $cv addtag selected withtag $t
            if { [scan [$cv bbox $t] "%f %f %f %f" x0 y0 x1 y1] != 4 } {
                continue
            }
            set s "$t:sel selection"
            if 0 {
                $cv create line $x0 $y0 $x1 $y1 -tags $s -fill red
                $cv create line $x0 $y1 $x1 $y0 -tags $s -fill red
            } else {
                $cv create polygon $x0 $y0 $x0 $y1 $x1 $y1 $x1 $y0 $x0 $y0\
                    -tags $s -outline $selcolor -fill ""
            }

            set BG($t) [$cv itemcget $t -fill]
            # $cv itemconfig $t -fill yellow
        }
        return $n
    }
    protected method selection_clear {} {
        foreach t [$cv find withtag selected] {
            $cv delete $t:sel
            $cv dtag $t selected 
            if [info exists BG($t)] {
                $cv itemconfig $t -fill $BG($t)
                unset BG($t)
            }
        }
        $cv delete selection
    }
    # -------------------------------------------------- 
    # -- locate the object closest to x y but visible
    public method cv_locate {pp x y} {
        upvar $pp P
        set t [$cv find closest $x $y 0]
        if { $t == "" } { return 0 }
        set nt $t
        while 1 {
            if { [$cv itemcget $nt -state] != "hidden" } {
                set sel 0
                foreach tg [$cv gettags $nt] {
                    if { $tg == "selection" } {
                        incr sel ; break
                    }
                }
                if { !$sel } { break }
            }
            set nt [$cv find closest $x $y 0 $nt]
            if { $nt == $t } { return 0 }
        }
        set P(nid) $nt
        set P(id) $ID($nt)
        foreach tg [$cv gettags $nt] {
            if { [scan $tg type:%s P(type)] == 1 } { 
                scan [$cv coords $t] "%s %s %s %s" P(x0) P(y0) P(x1) P(y1)
                return 1
            }
        }
        return 0
    }
    protected method cv_group {a objs {max 100}} {
        if { $objs == "" } {
            return 0
        }
        set children 0
        set not 0
        if ![obj_centroid id:$a x0 y0] {
            warning "main object $a not on canvas"
            incr not
            set x0 0 ; set y0 0
        }
        foreach OBJ $objs { 
            array set S $OBJ
            if ![obj_centroid id:$S(id) x1 y1] {
                incr not
                foreach {px0 py0 px1 py1} $S(bb) { break }
                if { $px0 == "" } { continue }
                foreach {x1 y1} [$csel Clip \
                    [expr ($px0+$px1)/2] [expr ($py0+$py1)/2]] { break }
                if { $x1 == "" } { continue }
                unset px0 ; unset px1; unset py0 ; unset py1
            }
            incr children
            $cv create line $x0 $y0 $x1 $y1 \
                -tags  group -fill gold -width 2 -arrow last
            if { ![incr max -1]  } { break }
        }
        if { $not } {
            warning "$not objects not on canvas"
        }
        if {!$max} {
            warning "> $children objects in the group. Skipping"
        }
        set children
    }
    protected method obj_centroid {a xx yy} {
        upvar $xx X
        upvar $yy Y
        if { [scan [$cv coords $a] "%f %f %f %f" x0 y0 x1 y1] != 4 } { 
            return 0
        }
        set X [expr ($x0+$x1)/2.0]
        set Y [expr ($y0+$y1)/2.0]
        return 1
    }
    # ----------------------------------------------------------------
    public method eTouch {n x y} {
        Event <<touch>> $n $x $y
    }
    public method eDrag {w} {
        if [cv_find current p] {
            DnD::Value [array get p]
        }
    }
    public method eDrop {w} {
        array set d [DnD::Detail]
        status_show $d(value)
    }
    # ---------------------------------------------------------------- MARKS
    public method marks_view {mm} {
        array set M $mm
        # collect all invisible tags
        set HI "" ; set OFF "" ; set ON ""
        foreach mark [array names M] {
            switch $M($mark) {
                0 { lappend OFF $mark }
                1 { lappend ON  $mark }
                2 { lappend HI  $mark }
            }
        }
        # -- apply visible
        foreach tag $ON  {
            $cv itemconfig mark:$tag -state normal -outline ""
        }
        # -- apply not visible
        foreach tag $OFF {
            $cv itemconfig mark:$tag -state hidden
        }
        # -- apply highlighted
        foreach tag $HI  { 
            $cv itemconfig mark:$tag -outline yellow
        }
    }
    # --------------------------------------------------------------- MESSAGES
    proc warning {msg} {
        ::warning canvas $msg
    }
    proc notice {msg} {
        ::notice canvas $msg
    }
    # -------------------------------------------------- Common Initialization
}
