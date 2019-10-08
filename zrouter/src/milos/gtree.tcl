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

## gtree.tcl (Tree widget)
## Library module for Milos
## Author: Mattias Hembruch


package require Itcl
package require Iwidgets
package require BLT
package provide ade::milos::gtree 0.1


itcl::class GTree {

    private variable top; #my top in the hierarchy
    private variable tv;
    private variable treelist;
    private variable tlsize 0;
    private variable myframe;

    private variable cur_tree { };
    private variable cur_idx 0;

    private common TREELIST;
    private common TREECNT 0;

    private common VTREE;
    private common VSIZE;
    private common VCNT 0;
    private common LINKED;

    private common COLMENUIDX 0;

    private variable vidx;
    private variable bindproc;
    private variable allbindproc "";
    private variable vsb "";
    private variable hsb "";

    private variable mytop "";
    private variable mymain;
    private variable mytitle "Root";

    private variable bindidx 0;

    private variable sortDir;
    private variable colActive;

    private variable mymenu;
    private variable mymenuidx;

    private variable colList;

    private variable _findWindow;

    private method select { } {
        ## if bindproc not set, don't bother looking it up
        if { [$tv selection present] == 1 } {
            set id [$tv curselection]
            set name [$tv get -full $id]
            set tag [lrange [$tv tag names $id] 1 end]
        } else {
            return
        }
        if { $allbindproc != "" } {
            $allbindproc $id $name $tag
        } elseif { [info exists bindproc($cur_tree)] && $bindproc($cur_tree) != 0 } {
            $bindproc($cur_tree) $id $name $tag
        }
    }

    private method get_vidx { } {
        return $vidx
    }
    private method get_curtree { } {
        return $cur_tree
    }



    ###################### Public interface ########################
    public method constructor { start_top } {
        if { [info command table] ne "table" } {
            namespace eval :: {namespace import ::blt::*}
        }
        #namespace eval :: { namespace import blt::* }
        #namespace eval :: { namespace import -force blt::tile::* }
        set vidx $VCNT
        set VSIZE($vidx) 0
        set LINKED($vidx) ""
        incr VCNT
        set mytop $start_top

        set myframe [frame $mytop.f$vidx]
        ::pack $myframe -fill both -expand 1
        set top $myframe

        set _findWindow [frame ${top}.find ]

        treeview ${top}.t
        set tv "${top}.t"

        new_tree
        #set cur_tree [tree create]
        #set treelist(0) $cur_tree
        #$tv configure -tree $cur_tree

        $tv configure -selectcommand "[itcl::code $this select]"
        set mymain [join [$tv column names]]

        _initMenu
        buildFind $_findWindow


        ## remove some default bindings we don't like..
        ::bind TreeView <B2-Motion> ""
        ::bind TreeView <Button-2> ""
        ::bind TreeView <ButtonRelease-2> ""

        ::bind $tv <Enter> "focus $tv"
        ::bind $tv <Leave> "focus $mytop"
        ::bind $tv <Control-f> "[itcl::code $this _showFind]"

        ## initialize main column
        bindCol -1 [itcl::code $this _colMoveStartCB] <ButtonPress-2>
        bindCol -1 "[itcl::code $this _colMoveCB] %x %y" <B2-Motion>
        bindCol -1 [itcl::code $this _colMoveDoneCB] <ButtonRelease-2>

        set colList(-1,label) ""
        set colList(-1,hidden) 1
        set colList(-1,moveable) 1
        set colList(-1,sortOptions) ""

        array set sortDir {}
        array set colList {}

        setVsb 1
        setHsb 1
        pack
    }


    ### GTREE related methods ###
    public method share_tree { gtree {link 0} } {
        set vidx [$gtree get_vidx]
        set cur_tree [$gtree get_curtree]
        for { set i 0 } { $i < $VSIZE($vidx) } { incr i } {
            set bindproc($VTREE($vidx,$i)) 0
        }
        $tv delete
        #puts "cur_tree, shared; $cur_tree"
        set self 0
        set other 0
        if { $link == 1 } {
            foreach arg $LINKED($vidx) {
                if { $arg == $gtree } { set other 1 }
                if { $arg == $this } { set self 1 }
            }
            if { $other == 0 } { lappend LINKED($vidx) $gtree }
            if { $self == 0 } { lappend LINKED($vidx) $this }
        }
        select_tree $link
    }

    public method setSize { w h } {
        $tv configure -width $w -height $h
    }

    private method setVsb { {on 1} } {
        if { $vsb == "" } {
            set vsb [scrollbar $top.vs -orient vertical \
                -command "$tv yview"]
            $tv configure -yscrollcommand "$vsb set"
        }
        pack
    }

    private method setHsb { {on 1} } {
        if { $hsb == "" } {
            set hsb [scrollbar $top.hs -orient horizontal \
                -command "$tv xview"]
            $tv configure -xscrollcommand "$hsb set"
            }
        pack
    }



    public method new_tree { } {
        set cid $TREECNT
        set TREELIST($cid) [tree create]
        incr TREECNT

        set nid $VSIZE($vidx)
        set VTREE($vidx,$nid) $TREELIST($cid)
        set cur_tree $TREELIST($cid)
        $tv configure -tree $cur_tree

        incr VSIZE($vidx)
        set bindproc($cur_tree) 0

        return $nid
    }
    private method myselect { idx } {
        if { $idx >= 0 && $idx < $VSIZE($vidx) } {
            set cur_tree $VTREE($vidx,$idx)
            $tv configure -tree $cur_tree
            return $idx
        }
    }

    public method cur_tree { } { return $cur_tree }
    public method select_tree { idx } {
        if { $idx >= 0 && $idx < $VSIZE($vidx) } {
            set cur_tree $VTREE($vidx,$idx)
            $tv configure -tree $cur_tree
            foreach arg $LINKED($vidx) {
                if { $arg == $this } { continue }
                $arg myselect $idx
            }
            return $idx
        }
        return -1
    }

    ### ENTRY related methods ###

    public method _showFind { args } {
        ## toggle the "find" window
        #puts "showFind: $args"
        if { $findActive == 0 } {
            set findActive 1
            buildColList
        } else {
            set findActive 0
            set findLboxSel [$findLbox index [$findLbox get]]
            table forget $_findWindow
        }
        pack
    }

    public method hideFind { } {
        puts "Hide Find!"
    }

    public method see { idx } {
        $tv see $idx 
    }

    public method clear { } {
        $cur_tree delete root
    }

    public method setData { idx data } {
        foreach {col val} $data {
            $tv entry configure $idx -data [list $col $val]
        }
    }

    public method insert { lab tag {dat {}} } {
        set id [$tv insert end $lab]
        foreach {col val} $dat {
            #puts "Adding data field: $col, value $val"
            $tv entry configure $id -data [list $col $val]
        }
        $tv tag add $tag $id
        return $id
    }

    public method bind { bp {tree 0} } {
        if { $tree eq "all" } {
            set allbindproc $bp
        } else {
            set bindproc($cur_tree) $bp
        }
    }

    public method separator { char } {
        $tv configure -separator $char -autocreate yes
    }

    ### COLUMN related methods ###
    public method colNames { } {
        set nl [$tv column names]
        set fl {}
        foreach name $nl {
            if { $name ne $mymain } {
                lappend fl $name
            } else {
                lappend fl -1
            }
        }
        return $fl
    }

    public method colTitles { } {
        set nl [$tv column names]
        set fl {}
        foreach name $nl {
            lappend fl [$tv column cget $name -title]
        }
        return $fl
    }

    private method colNameFromTitle { title } {
        set idx 0
        foreach name [colTitles] {
            if { $name eq $title } {
                return [colAt $idx]
            }
            incr idx
        }
    }

    public method addCol { name label {pos "end"} } {
        set name [string trim $name]
        set label [string trim $label]
        set idx [$tv column insert $pos $name]
        $tv column configure $name -title $label
        set colList($name,label) $label
        set colList($name,hidden) 1
        set colList($name,moveable) 1
        set colList($name,sortOptions) ""
        bindCol $name [itcl::code $this _colMoveStartCB] <ButtonPress-2>
        bindCol $name "[itcl::code $this _colMoveCB] %x %y" <B2-Motion>
        bindCol $name [itcl::code $this _colMoveDoneCB] <ButtonRelease-2>
        hideCol $name
    }

    public method setMoveable { name on_or_off } {
        set colList($name,moveable) $on_of_off
    }

    public method colPos { name } {
        if { $name eq -1 } {
            set name $mymain
        }
        set idx 0
        foreach col [$tv column names] {
            if { $name eq $col } {
                return $idx
            }
            incr idx
        }
    }

    public method bindCol { name callback event {tag ""} } {
        if { ($name eq "") || ($callback eq "") } { 
            return
        }

        if { $tag ne "" } {
            #let user specify tag
            set bt $tag
        } else {
            #generate unique tag
            set bt "btag$bindidx"
            incr bindidx
        }
        set bact "$callback $this $name"
        #puts "bact: $bact"

        if { $name == "-1" } {
            #check for main column
            set name $mymain
        }

        set ot [$tv column cget $name  -bindtags ]
        set flag 0
        foreach tag $ot {
            if { $tag eq $bt } {
                ## this column already has the tag
                set flag 1
                break;
            }
        }
        if { $flag == 0 } {
            lappend ot $bt
            #puts "ntag: $ntag"
            $tv column configure $name -bindtags $ot
            #puts "tags: [$tv column cget $name -bindtags]"
        }
        $tv column bind $bt $event $bact
        return $bt
    }

    public method setColSort { name {button 1} } {
        if { [info exists sortDir($name)] == 0 } {
            set sortDir($name) ""
        }

        set bb "<ButtonPress-$button>"
        bindCol $name [itcl::code $this _colSortCB] $bb
    }

    private variable moveCol ""
    private variable movePos 0
    private variable moveBackground ""
    private variable moveTB ""
    private variable moveATB ""
    private variable moveDest 0
    private variable dummyCols ""
    private method _colMoveStartCB { widget col } {
        if { $col == -1 } {
            set col $mymain
        }
        set moveCol $col
        set movePos [colPos $col]
        set moveDest -2
        set cl [llength [colNames]]
        #puts "MovePos: $movePos"
        #puts "CL: $cl"

        set moveTB [$tv column cget $moveCol -titlebackground]
        set moveATB [$tv column cget $moveCol -activetitlebackground]

        $tv column configure $moveCol -titlebackground lightblue
        $tv column configure $moveCol -activetitlebackground lightblue


        if { $movePos <= $cl } {
            $tv column insert end EDummy
            $tv column configure EDummy -title "@"
            append dummyCols "EDummy"
            $tv column configure EDummy -width 6
        }
    }

    private method _colMoveCB { x y widget col } {
        set nc [$tv column nearest $x $y]
        if { $nc ne "" } {
            if { $nc ne $moveCol } {
                set npos [colPos $nc]
                if { $npos > $movePos } {
                    set npos [expr $npos - 1]
                }
                if { $movePos != $npos } {
                    #puts "Move $moveCol from pos $movePos to pos $nc [colPos $nc]"
                    $tv column configure $nc -activetitlebackground red
                    $tv column activate $nc
                    set moveDest $npos
                    #puts "set moveDest: $moveDest"
                    return
                }
            }
            #puts "Reset moveDest!"
            set moveDest -2
        }
    }

    private method moveCol { name new } {
        #puts "Move: $name/$old to $new!"
        set olab [$tv column cget $name -title]
        set btags [$tv column cget $name -bindtags]
        $tv column delete $name
        $tv column insert $new $name
        $tv column configure $name -title $olab
        $tv column configure $name -bindtags $btags
    }

    private method _colMoveDoneCB { widget col } {
        foreach name $dummyCols {
            $tv column delete $name
        }
        set dummyCols ""
        foreach name [$tv column names] {
            $tv column configure $name -activetitlebackground $moveATB
        }
        $tv column configure $moveCol -titlebackground $moveTB
        #puts "End: $moveDest"
        if { $moveDest >= 0 } {
            if { $moveCol ne $mymain } {
                moveCol $moveCol $moveDest
            } else {
                ## move all other columns into the right positions..
                if { $moveDest > $movePos } { 
                    for { set i $movePos } { $i < $moveDest } { incr i } {
                        set rneighbor [colAt [expr $i + 1]]
                        moveCol $rneighbor $i
                    }

                } else {
                    ##
                    for { set i $movePos } { $i > $moveDest } { incr i -1 } {
                        set lneighbor [colAt [expr $i - 1]]
                        moveCol $lneighbor $i
                    }
                }

            }
        }
    }

    public method colAt { idx } {
        return [lindex [colNames] $idx]
    }

    private method _colSortCB { widget col } {
        if { $sortDir($col) eq "" } {
            set sortDir($col) "-decreasing"
            sortCol $col "-decreasing $colList($col,sortOptions)"
        } else {
            set sortDir($col) ""
            sortCol $col "$colList($col,sortOptions)"
        }
    }

    private method _initMenu { } {
        set mymenuidx $COLMENUIDX
        set mymenu [menu .m$mymenuidx]
        $mymenu configure -tearoff 0
        incr COLMENUIDX
        set pr [itcl::code $this _colActivateMenu]
        set yr "if { \[$pr %W %x %y] != 0 } { break }"
        ::bind $mymenu <ButtonRelease-1> $yr
    }

    private method _colActivateMenu { widget x y } {

        set h [expr [winfo height $widget] -1 ]
        set w [expr [winfo width $widget] -1 ]

        #puts "CAM!! $widget $x,$h $y,$w [$widget entryconfigure @$y]"
        if { ($x < 1)  || ($x > $w) } {
            return 0
        }
        if { ($y < 1) || ($y > $h) } {
            return 0
        }
        $widget invoke @$y
        return 1
    }

    private method _colActivateCB { col args } {
        set acnt 0
        set aidx -1
        foreach {name val} [array get colActive] {
            if { $val == 1 } {
                incr acnt
                set aidx $name
            }
        }
        if { $acnt == 0 } {
            ## cannot toggle!!
            set colActive($col) 1
        } else {
            _toggleShowCol $col
        }
    }

    private method _colMenuCB { x y args } {
        if { $dummyCols ne "" } {
            #puts "Ignoring activation due to move!"
            return
        }
        _buildMenu
        tk_popup $mymenu $x $y
    }

    private method _buildMenu { } {
        array set colActive {}
        $mymenu delete 0 last
        set idx 0
        set cnl [colNames]
        foreach name [colTitles] {
            set cn [lindex $cnl $idx]
            if { $cn == -1 } {
                set cn $mymain
            }
            set hidden [$tv column cget $cn -hide]
            #puts "Name: $name, cn: $cn, hidden: $hidden"
            set cn [lindex $cnl $idx]
            if { $hidden == 0 } {
                set colActive($cn) 1
            } else {
                set colActive($cn) 0
            }
            $mymenu add checkbutton -label $name -variable [itcl::scope colActive($cn)]  -command "[itcl::code $this _colActivateCB] $cn"
            incr idx
        }
    }

    public method setActivateCol { on_off } {
        if { $on_off eq 1 } {
            bindCol -1 "[itcl::code $this _colMenuCB] %X %Y" <ButtonPress-3> all
        } else {
            $tv column bind all <ButtonPress-3> {}
        }
    }

    public method sortOptions { col args } {
        set colList($col,sortOptions) "$args"
    }

    public method sortCol { col args } {
        if { $col == -1 } {
            set cmd "$cur_tree sort 0 -reorder [join $args]"
        } else {
            set cmd "$cur_tree sort 0 -key $col -reorder [join $args]"
        }
        eval $cmd
    }


    public method _toggleShowCol { name } {
        if { $name == "-1" } {
            set name $mymain
        }
        if { [$tv column cget $name -hide] == 0 } {
            $tv column configure $name -hide 1
            set colList($name,hidden) 1
        } else {
            $tv column configure $name -hide 0
            set colList($name,hidden) 0
        }
    }


    public method hideCol { args } {
        foreach name $args {
            if { $name == "-1" } {
                set name $mymain
            }
            $tv column configure $name -hide 1
            set colList($name,hidden) 1
        }
    }

    public method showCol { args } {
        foreach name $args {
            if { $name == "-1" } {
                set name $mymain
            }
            $tv column configure $name -hide 0
            set colList($name,hidden) 0
        }
    }



    public method title { title } {
        $tv column configure treeView -text $title
        set $mytitle $title
    }

    ### FIND related methods
    public method pack { } {
        set row1 0
        set row2 1
        set cmd "table $top "
        if { $findActive == 1 } {
            incr row1
            incr row2
            append cmd "0,0 $_findWindow -fill x -cspan 2 "
        }
        append cmd "$row1,0 $tv -fill both"
        if { $vsb != "" } {
            append cmd " $row1,1 $vsb -fill y"
        }
        if { $hsb != "" } {
            append cmd " $row2,0 $hsb -fill x"
        }
        #puts "cmd: $cmd"
        eval $cmd

        ## HSB row
        table configure $top "r$row2" -resize none

        ## VSB row
        table configure $top "r$row1" c1 -resize none

        ## tree row
        table configure $top "r$row1" -resize both

        if { $findActive == 1 } {
            ## find row
            table configure $top r0 -resize none
        }
    }


    private method buildColList { } {
        $findLbox delete 0 end
        foreach name [colTitles] {

            $findLbox insert end $name
        }
        $findLbox insert end "All Columns"
        $findLbox select $findLboxSel
    }

    private variable findChoice 1
    private variable findEntry ""
    private variable findLbox ""
    private variable findActive 0
    private variable findLboxSel 0



    private method buildFind { ftop } {
        set row 0
        set l1 [label ${ftop}.l1 -text "Column:"]
        set l2 [label ${ftop}.l2 -text "Filter:"]
        set b1 [button ${ftop}.b1 -text "Reset" -command [itcl::code $this _findResetCB] -width 2]
        table $ftop $row,0 $l1 $row,1 $l2 $row,2 $b1

        set row 1
        set findLbox [iwidgets::optionmenu ${ftop}.l]

        set findEntry [entry ${ftop}.e -width 10]
        set b [button ${ftop}.b -text "Apply" -width 2 -command [itcl::code $this _findSetCB] ]
        table $ftop $row,0 $findLbox $row,1 $findEntry $row,2 $b
        table configure $ftop $findEntry -fill x

        set row 2
        set r1 [radiobutton ${ftop}.rb1 -text "Match" -value 1 -variable [itcl::scope findChoice]]
        set r2 [radiobutton ${ftop}.rb2 -text "No Match" -value 2 -variable [itcl::scope findChoice]]
        set r3 [radiobutton ${ftop}.rb3 -text "Highlight" -value 3 -variable [itcl::scope findChoice]]
        table $ftop $row,0 $r1 $row,1 $r2 $row,2 $r3 

        #table configure $ftop c* -resize expand
        #table configure $ftop r* -resize expand
        buildColList
        ::bind $ftop <Enter> "focus $ftop"
        ::bind $ftop <Leave> "focus $mytop"
        ::bind $ftop <Control-f> "[itcl::code $this _showFind]"
        ::bind $ftop <KeyPress-Return> "$b invoke"
        ::bind $findEntry <KeyPress-Return> "$b invoke"
        #set findActive 1
    }

    private method _findResetCB { } {
        #puts "RESET FIND!"
        foreach ent [$cur_tree find 0] {
            $tv entry configure $ent -fg black
        }
        $tv show all
        $findEntry delete 0 end
    }

    private method _findSetCB { } {
        #puts "FIND: $findChoice, [$findLbox get] [$findEntry get]"
        set filter [$findEntry get]
        if { $filter eq "" } {
            _findResetCB
        }
        set col [colNameFromTitle [$findLbox get]]
        _findSetFilter $filter $col $findChoice
    }

    public method _findGetEntries { filter col } {
        #puts "FGE: $filter, $col"
        if { $col ne "" } {
            set cmd "$cur_tree find 0"
            if { $col ne "-1"} {
                append cmd " -key $col"
            }
            append cmd " -glob \"$filter\""
            #puts "cmd: $cmd"
            set elist [eval $cmd]
            #puts "Matching for $filter, $col: $elist"
            return $elist
        } else {
            set tl {}
            foreach col [colNames] {
                #puts "Loop: $col"
                set tl [concat $tl [_findGetEntries $filter $col]]
            }
            #puts "Matching for $filter, $col: $tl"
            return $tl
        }
    }

    private method _findSetFilter { filter col visual } {
        set el [_findGetEntries $filter $col]
        switch $visual {
            1 {
                ## show only these
                foreach ent [$cur_tree find 0] {
                    $tv entry configure $ent -fg black
                }
                $tv hide all
                foreach ent $el {
                    $tv open $ent
                    $tv show $ent
                }
            }
            2 {
                ## show all others
                foreach ent [$cur_tree find 0] {
                    $tv entry configure $ent -fg black
                }
                $tv show all
                foreach ent $el {
                    $tv hide $ent
                }
            }
            3 {
                ## highlight
                $tv show all
                foreach ent $el {
                    $tv open $ent
                    $tv entry configure $ent -fg red
                }
            }
        }
    }
}

