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
set ::qtupdate 0
set elabel ""


## BBox values
set bx0 0
set by0 0
set bw 0
set bh 0

## Widget values
set wx0 0
set wy0 0
set ww 0
set wh 0

## Scroll offsets
set scrollx 0
set scrolly 0

## Running Scale Factor
set rsf 1.0
## Current Scale Factor
set sf 0.1

## Oversample Factor
set os 1.0

## Scroll amount
set scroll 50

## For now
set layers 8


frame .fr  -bg red  -width 100 -height 100
set frw [winfo id .fr]
label .elabel -textvariable elabel
pack .elabel

load tkqt.so
::tkqt::newParent $frw
::tkqt::setLayers $layers
::tkqt::show

set boxcnt 0


## QT widget frame
pack .fr   -fill both -expand 1


## Toggle Frame
frame .tf
for { set i 0 } { $i < $layers } { incr i } {
    set layer($i) 1
    set cmd "checkbutton .tf.b$i -text \"Toggle $i\" -variable layer($i) "
    append cmd "-command { uplayer $i }"
    eval $cmd
    pack .tf.b$i  -side left
}
pack .tf


## Bottom Frame
frame .botf
set b [frame .botf.buttonf]
button $b.c -text "Quit" -command { exit }
button $b.d -text "Add 10000" -command { addboxes 10000 }
button $b.d1 -text "Special" -command { special }
button $b.e -text "Update" -command { upwidget }
pack $b.c $b.d $b.d1 $b.e
pack $b -side left

set b [frame .botf.scalef]
label $b.l -text "Scale Factor"
entry $b.e -textvariable sf
button $b.zi -text "Zoom In" -command { upscale 2}
button $b.zo -text "Zoom Out" -command { upscale 1}

label $b.l2 -text "Oversample"
entry $b.e2 -textvariable os
button $b.set -text "Set Oversample" -command { setos }
pack $b.l $b.e -side left -side top
pack $b.zi  -side left -side top
pack $b.zo -side right -side top
pack $b.set -side left -side bottom
pack $b.e2 -side left -side bottom
pack $b.l2 -side left -side bottom
pack $b -side left -fill x -expand 1

set a [frame .botf.scroll]
set b [frame $a.1]
label $b.l -text "Scroll Amount"
entry $b.e -textvariable scroll
pack $b.l -side top -side left
pack $b.e -side top -side left
set b [frame $a.2]
button $b.ml -text "Scroll Left" -command { scroll 2}
button $b.mr -text "Scroll Right" -command { scroll 1}
button $b.mu -text "Scroll Up" -command { scroll 4}
button $b.md -text "Scroll Down" -command { scroll 3}
pack $b.mr -side top -side right
pack $b.ml -side top -side left
pack $b.md -side left -side bottom
pack $b.mu -side bottom -side right

pack $a.1 -side top
pack $a.2 -side bottom

pack $a

pack .botf -fill x

wm geometry . 800x662
set ww [winfo width .fr] 
set wn [winfo height .fr]

bind . <Configure> { resized }

proc upwidget { } {
    ::tkqt::up
}

proc setos { } {
    global os
    ::tkqt::setOS $os
}

proc special { } {
    ::tkqt::box 300 200 200 100 3
    ::tkqt::box 500 200 200 150 2
    upwidget
}

proc scroll { dir } {
    global scroll
    global scrollx scrolly
    global bx0 by0

    set sx 0
    set sy 0
    if { $dir == 1 } {
        set sx [expr 0 - $scroll]
    } elseif { $dir == 2 } {
        set sx $scroll
    } elseif { $dir == 3 } {
        set sy [expr 0 - $scroll] 
    } elseif { $dir == 4 } {
        set sy $scroll
    }
    ::tkqt::scroll $sx $sy
    set scrollx [expr $scrollx + $sx]
    set scrolly [expr $scrolly + $sy]
    #set bx0 [expr $bx0 - $sx]
    #set by0 [expr $by0 - $sy]
    puts "AScroll: $bx0 $by0"
    upwidget
}

proc upscale { inout } {
    global sf rsf 

    if { $inout == 1 } {
        set asf [expr {1+$sf}]
    } elseif { $inout == 2 } {
        set asf [expr {1/(1+$sf)}]
    }
    set rsf [expr $rsf * $asf]
    puts "SF/RSF: $asf / $rsf"

    upbbox
}


proc upbbox { } {
    global rsf bw bh bx0 by0
    global ww wh wx0 wy0
    global scrollx scrolly

    puts "Before: $bx0 $by0 $bw $bh"
    set xmid [expr $ww/2 + $wx0]
    set bw [expr {int($ww*$rsf)}]
    #set bx0 [expr {int($xmid-($ww/2))}]

    set ymid [expr $wh/2 + $wy0]
    set bh [expr {int($wh*$rsf)}]
    #set by0 [expr {int($ymid-($wh/2))}]
    puts "After: $bx0 $by0 $bw $bh"

    ::tkqt::setBBox $bx0 $by0 $bw $bh
    upwidget
}


proc resized { } {
    global boxcnt bh bw
    global ww wh wx0 wy0

    #puts "main geom: [winfo geometry .]"
    #puts "fr geom: [winfo width .fr] [winfo height .fr]"
    set wn [winfo width .fr] 
    set hn [winfo height .fr]


    if { ($wn != $ww) || ($hn != $wh) } {
        puts "Resize!"
        set ww $wn
        set wh $hn
        ::tkqt::resize $ww $wh
        upbbox
        upwidget
    }
}


proc uplayer { l } {
    global layer
    ::tkqt::setLayer $l $layer($l)
    upwidget
}

proc addboxes { n } {
    global boxcnt bw bh os

    set maxw [expr [winfo width .fr] * $os]
    set maxh [expr [winfo height .fr] * $os]

    for { set i 0} { $i < $n } {incr i } {
        set x0 [expr int(rand()*$maxw)]
        set y0 [expr int(rand()*$maxh)]
        set l [expr int(rand()*40)+1]
        set h [expr int(rand()*10)+1]
        set layer [expr int(rand()*8)]

        #puts "::tkqt::box $x0 $y0 [expr $x0 + $l] [expr $y0 + $h] $layer"
        ::tkqt::box $x0 $y0 $l $h $layer
    }
    incr boxcnt $n
    #::tkqt::setBBox 0 0 [winfo width .fr] [winfo height .fr]
    upwidget
}


