
package require Itcl
package provide mtlib 1.0

#       Class: LDirTree
# Description: Simple directory tree widget
# -- this code is based on R. Hipps code (see note at the end)
#      Author: Manolis M. Tsangaris  -- mmt@performance.enet.gr
#   Copyright: Manolis M. Tsangaris, Sinastria, 2002
#
itcl::class LDirTree {
    inherit LFrame LBinder
    # event
    # Event <<select>> node                ;# B1-Press
    # Event <<expand>> node                ;# Double-B1-Press
    # Event <<command>> node               ;# B3-Press
    # -------------------------------------------------- Public Variables --
    public variable timeout   100             ;# when to repaint
    public variable width    200
    public variable height   100
    public variable bg white
    public variable fg gray50
    public variable selbg gray80
    public variable sortedchildren 1           ;# sort the children by name
    public variable sep "/"                    ;# path separator
    # -------------------------------------------------- Public Methods ----

    # Insert an item & set attributes
    #   accepted attributes are:
    #   title: used to display information instead of the node name
    #   icon:  icon used to display near the text
    #   tags:  list of tags associated with this item
    #   titlefg:  foreground of the title
    #   icon:     node icon
    #
    public method Add {v args} {
        # -- if the node is already there, just reconfigure
        if [info exists TreeOpen($v)] {
            tree_config $v $args
            return 0
        }
        tree_newitem $tree $v $args
        return 1
    }
    # like add
    public method AddNode {path attrs} {
	set _cmd [list Add $path]
	foreach {a v} $attrs {
	    lappend _cmd -$a $v
	}
	eval $_cmd
    }

    # -- clear the entire tree
    public method Clear {} {
        tree_delete [froot]
        tree_draw
    }
    public method Delete {v} {
        tree_delete $v
        tree_draw
    }
    # refresh the tree 
    public method Refresh {} {
        tree_draw
    }
    #
    # Change the selection to the indicated item
    #
    public method Select {v} {
      set SELECTION $v
      tree_selection_draw $tree
    }
    
    public method Open {dir} {
        if ![info exists TreeOpen($dir)] { bell ; return }
        set TreeOpen($dir) 1
        tree_draw
    }
    # -- Retrieve the current selection
    public method Selection {
	return $SELECTION
    }

    public method Info {node} {
        list node $node\
	    tags $TreeAttr([list $node tags])\
	    children $TreeChildren($node)
    }

    public method Get {node attr} {
	switch $attr {
	    children {
		set TreeChildren($node)
	    }
	    default { set TreeAttr([list $node $attr])}
	}
    }

    # configure a node (you may need to refresh, to see changes)
    public method Config {tag args} {
	tree_config $tag $args
    }

    # -- return a unique id
    # -- useful to generate a unique path component
    public method Key {tag} {
	if ![info exists KEY($tag)] {
	    set KEY($tag) [array size KEY]
	}
	set KEY($tag)
    }

    # -- make sure an item is visible (within scrolling area)
    public method See {path} {
	if ![info exists NodeTag($path)] { return }
	# -- move scrollbars, so the node is visible
	set t $NodeTag($path)
	foreach {x0 y0 x1 y1} [$tree bbox $t] { break }
	foreach {X0 Y0 X1 Y1} [$tree cget -scrollregion] { break }
	set x $x0
	set y $y0
	set rx [expr 1.0*($x-$X0)/($X1-$X0)]
	set ry [expr 1.0*($y-$Y0)/($Y1-$Y0)]
	$tree yview moveto $ry
	# -- do this in case of horizontal direction of the tree
	# $tree xview moveto $rx
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable tree             ;# the canvas for the tree
    protected variable IM               ;# images
    protected variable TreeY 0
    protected variable TagNode          ;# the Node of a tag
    protected variable NodeTag          ;# the tag of a node
    protected variable task ""
    protected variable TreeOpen         ;# should the node be drawn open?
    protected variable TreeChildren     ;# all children of a node
    protected variable TreeAttr         ;# node attributes index by {node attr}
    protected variable SELIDX ""
    protected variable SELECTION ""
    protected variable KEY
    # -------------------------------------------------- Common Variables --
    common BM
    common IMG
    common FONT
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        option add LDirTree*highlightThickness 0
        set tree [LScrollableWidget $This canvas]
        LBridge width  "$tree configure -width"
        LBridge height "$tree configure -height"
        LBridge bg "$tree configure -bg"
        tree_make $tree
    }
    destructor {
        foreach im [array names IM] {
            image delete $IM($im)
        }
        catch {after cancel $task}
    }
    # -------------------------------------------------- Private Methods ---
    # -- configure a tree node
    protected method tree_config {tag argv} {
        foreach {op arg} $argv {
            set op [string range $op 1 end]
            set TreeAttr([list $tag $op]) $arg
        }
        # redraw the tree
        tree_draw
    }
    # Create a new tree widget.
    #
    protected method tree_make {w args} {
        eval $w config $args
	bind $w <1> "$this eSelect $w %x %y"

        foreach bm "open closed" {
            set IM($bm) [image create bitmap -data $BM($bm) \
            -maskdata $BM(mask) \
            -foreground $fg -background $bg]
        }
        foreach im [array names IMG] {
            set IM($im) [image create photo -data $IMG($im)]
        }

        $w bind x <1>        "$this eSelect $w %x %y"
        $w bind x <Double-1> "$this eExpand $w %x %y"
        $w bind x <3>        "$this eCommand $w %x %y"

        tree_node_init $w [froot]
        set SELECTION {}
        set SELIDX {}
        tree_draw

        set w
    }

    # Initialize a element of the tree.
    #
    protected method tree_node_init {w v} {
        Config $v -icon {} -tags {} -title {} -titlefg black
	set TreeChildren($v) ""
	set TreeOpen($v) 0
    }

    # -- delete a tree based on its tag
    protected method tree_delete {v} {
        # -- if the node does not exist ...
        if ![info exists TreeOpen($v)] { return }

        # -- delete all children, if any
        foreach ch $TreeChildren($v) {
            tree_delete [fjoin $v $ch]
        }

	# -- do not delete the root node
	if { $v != [froot] } {
	    unset TreeOpen($v)
	    unset TreeChildren($v)
	    set attrs [array names TreeAttr [list $v *]]
	    foreach a $attrs {
		unset TreeAttr($a)
	    }

	    # -- delete any tags
	    if [info exists NodeTag($v)] {
		set j $NodeTag($v)
		unset TagNode($j)
		unset NodeTag($v)
	    }
	}

	# -- fix the parent children
	set p [fparent $v]
	set d ""
	set a [ftail $v]
	foreach ch $TreeChildren($p) {
	    if { $ch == $a } {  continue }
	    lappend d $ch
	}
	set TreeChildren($p) $d
    }

    #
    # Insert a new element $v into the tree $w.
    #
    protected method tree_newitem {w v argv} {
        set parent [fparent $v]
        set node [ftail $v]
        if {![info exists TreeOpen($parent)]} {
            error "parent item \"$parent\" is missing"
        }
        set i [lsearch -exact $TreeChildren($parent) $node]
        if {$i>=0} {
            error "item \"$v\" already exists"
        }
        lappend TreeChildren($parent) $node
	if { $sortedchildren } {
	    set TreeChildren($parent) [lsort $TreeChildren($parent)]
	}
        tree_node_init $w $v
        # set the attributes
        tree_config $v $argv
        tree_draw 
    }

    
    # -------------------------------------------------------------------
    # Internal use only.
    # Build a single layer of the tree on the canvas.  Indent by $in pixels
    protected method tree_buildlayer {w v in} {
      set start [expr $TreeY-10]
      set y ""
      foreach child $TreeChildren($v) {
        set pchild [fjoin $v $child]
        set y $TreeY
        incr TreeY 17
        $w create line $in $y [expr $in+10] $y -fill $fg
        set icon $TreeAttr([list $pchild icon])

        set taglist x
        foreach tag $TreeAttr([list $pchild tags]) {
          lappend taglist $tag
        }

        set x [expr $in+12]

        if [info exists IM($icon)] {
          set im $IM($icon)
          set taglist1 $taglist ; lappend taglist1 $pchild:icon
          set k [$w create image $x $y -image $im -anchor w -tags $taglist1]
          incr x 20
          set TagNode($k) $pchild
        }

        set taglist1 $taglist ; lappend taglist1 $pchild:text
        set tl $TreeAttr([list $pchild title])
        if { $tl == "" } { set tl $child }
        set j [$w create text $x $y -text $tl -font $FONT \
            -anchor w -tags $taglist1 \
            -fill $TreeAttr([list $pchild titlefg])]
        set TagNode($j) $pchild
        set NodeTag($pchild) $j

        if {[string length $TreeChildren($pchild)]} {
          if {$TreeOpen($pchild)} {
             set j [$w create image $in $y -image $IM(open)]
             $w bind $j <1> [list $this eCloseNode $w $child $v]
             tree_buildlayer $w $pchild [expr $in+18]
          } else {
             set j [$w create image $in $y -image $IM(closed)]
             $w bind $j <1> [list $this eOpenNode $w $child $v]
          }
        }
      }
      if { $y != "" } {
        set j [$w create line $in $start $in [expr $y+1] -fill $fg ]
        $w lower $j
      }
    }

    
    # Open a branch of a tree
    #
    protected method tree_open {v} {
      if {[info exists TreeOpen($v)] && ($TreeOpen($v)==0)
          && [info exists TreeChildren($v)] 
          && [string length $TreeChildren($v)]>0} {
        set TreeOpen($v) 1
        tree_draw
      }
    }
    
    protected method tree_close { v} {
      if {[info exists TreeOpen($v)] && $TreeOpen($v)==1} {
        set TreeOpen($v) 0
        tree_draw
      }
    }
    
    # Draw the selection highlight
    protected method tree_selection_draw w {
      if {[string length $SELIDX]} {
        $w delete $SELIDX
      }
      set v $SELECTION
      if {[string length $v]==0} return
      if {![info exists NodeTag($v)]} return
      set bbox [$w bbox $NodeTag($v)]
      if {[llength $bbox]==4} {
        set i [eval $w create rectangle $bbox -fill $selbg -outline {{}}]
        set SELIDX $i
        $w lower $i
      } else {
        set SELIDX {}
      }
    }
    
    #
    # Return the full pathname of the label for widget $w that is located
    # at real coordinates $x, $y
    #
    protected method tree_label_at {w x y} {
      set x [$w canvasx $x]
      set y [$w canvasy $y]
      foreach m [$w find overlapping $x $y $x $y] {
        if {[info exists TagNode($m)]} {
          return $TagNode($m)
        }
      }
      return ""
    }
    # -------------------------------------------------- 
    # -- EVENT HANDLING
    public method eCloseNode {w c vx} {
        set TreeOpen([fjoin $vx $c]) 0
        tree_draw
    }
    public method eOpenNode {w c vx} {
        set TreeOpen([fjoin $vx $c]) 1
        tree_draw
    }
    # -- B1
    public method eSelect {w x y} {
        set lbl [tree_label_at $w $x $y]
        Select $lbl
        Event <<select>> $lbl
    }
    # -- Double B1
    public method eExpand {w x y} {
        set lbl [tree_label_at $w $x $y]
        tree_open  $lbl
        Event <<expand>> $lbl
    }
    # -- B3 has been pressed
    public method eCommand {w x y} {
        set lbl [tree_label_at $w $x $y]
        tree_open  $lbl
        Event <<command>> $lbl
    }

    # Draw the tree on the canvas (after idle)
    public method eBuild {} {
      set task ""
      $tree delete all
      set TreeY 30
      tree_buildlayer $tree [froot] 10
      $tree config -scrollregion [$tree bbox all]
      tree_selection_draw $tree
    }

    # Schedule to build tree when idle
    protected method tree_draw {} {
      if { $task != "" } { return } 
      set task [after $timeout "$this eBuild"]
    }
    # ---------------------------------------------------------------
    # -- manipulate paths (which can have arbitrary separators)
    # -- sep can either be / or anything else
    protected method ftail {path} {
	if { $sep == "/" } { return [file tail $path] }
	lindex $path end
    }
    # -- is this path root?
    protected method fisroot {path} {
	if { $path == $sep } { return  1}
	return 0
    }
    # -- return the parent
    protected method fparent {path} {
	if { $sep == "/" } { return [file dirname $path] }
	lrange $path 0 end-1
    }
    # -- join paths
    protected method fjoin {path file} {
	if { $sep == "/" } { return [file join $path $file] }
	lappend path $file
	set path
    }
    # -- return the root node
    protected method froot {} {
	if { $sep == "/" } { return / }
	return ""
    }
    # -------------------------------------------------- Common Initialization
    switch $tcl_platform(platform) {
        unix - default {
            set FONT \
            -adobe-helvetica-medium-r-normal-*-11-80-100-100-p-56-iso8859-1
        }
        windows {
            set FONT \
            -adobe-helvetica-medium-r-normal-*-14-100-100-100-p-76-iso8859-1
        }
    }
    #
    # Bitmaps used to show which parts of the tree can be opened.
    #
    set maskdata "#define solid_width 9\n#define solid_height 9"
    append maskdata {
      static unsigned char solid_bits[] = {
       0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01,
       0xff, 0x01, 0xff, 0x01, 0xff, 0x01
      };
    }
    set BM(mask) $maskdata
    set data "#define open_width 9\n#define open_height 9"
    append data {
      static unsigned char open_bits[] = {
       0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x7d, 0x01, 0x01, 0x01,
       0x01, 0x01, 0x01, 0x01, 0xff, 0x01
      };
    }
    set BM(open) $data
    set data "#define closed_width 9\n#define closed_height 9"
    append data {
      static unsigned char closed_bits[] = {
       0xff, 0x01, 0x01, 0x01, 0x11, 0x01, 0x11, 0x01, 0x7d, 0x01, 0x11, 0x01,
       0x11, 0x01, 0x01, 0x01, 0xff, 0x01
      };
    }
    set BM(closed) $data

    array set IMG {
    idir {
        R0lGODdhEAAQAPIAAAAAAHh4eLi4uPj4APj4+P///wAAAAAAACwAAAAAEAAQAAADPVi63P4w
        LkKCtTTnUsXwQqBtAfh910UU4ugGAEucpgnLNY3Gop7folwNOBOeiEYQ0acDpp6pGAFArVqt
        hQQAO///
    }
    ifile {
        R0lGODdhEAAQAPIAAAAAAHh4eLi4uPj4+P///wAAAAAAAAAAACwAAAAAEAAQAAADPkixzPOD
        yADrWE8qC8WN0+BZAmBq1GMOqwigXFXCrGk/cxjjr27fLtout6n9eMIYMTXsFZsogXRKJf6u
        P0kCADv/
    }
    }
}

#
# I am D. Richard Hipp, the author of this code.  I hereby
# disavow all claims to copyright on this program and release
# it into the public domain. 
#
#                     D. Richard Hipp
#                     January 31, 2001
#
# As an historical record, the original copyright notice is
# reproduced below:
#
# Copyright (C) 1997,1998 D. Richard Hipp
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
# 
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA  02111-1307, USA.
#
# Author contact information:
#   drh@acm.org
#   http://www.hwaci.com/drh/
#
