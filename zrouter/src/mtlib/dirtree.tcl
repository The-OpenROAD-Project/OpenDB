
package require Itcl
package provide mtlib 1.0

#       Class: LDirTree
# Description: Simple directory tree widget
# -- this code is based on R. Hipps code (see note at the end)
#
itcl::class LDirTree {
    inherit LFrame LBinder
    # event
    # Event <<select>> node 
    # Event <<expand>> node 
    # -------------------------------------------------- Public Variables --
    public variable timeout   100             ;# when to repaint
    public variable width    200
    public variable height   100
    # -------------------------------------------------- Public Methods ----

    # Insert an item & set attributes
    #   accepted attributes are:
    #   title: used to display information instead of the node name
    #   icon:  icon used to display near the text
    #   tags:  list of tags associated with this item
    public method Add {v args} {
        if [info exists Tree($v:open)] {return 0}
        eval tree_newitem $tree $v $args
        return 1
    }
    public method Delete {v} {
        tree_delete $v
    }
    public method Clear {} {
        tree_delete /
    }
    # refresh the tree 
    public method Refresh {} {
        tree_draw
    }
    #
    # Change the selection to the indicated item
    #
    public method Select {v} {
      set Tree(selection) $v
      tree_selection_draw $tree
    }
    
    public method Open {dir} {
        tree_open $dir
        tree_draw
    }
    # 
    # Retrieve the current selection
    #
    public method Selection {
      return $Tree(selection)
    }

    public method Info {node} {
        list node $node tags $Tree($node:tags) children $Tree($node:children)
    }

    # configure a node (you may need to refresh, to see changes)
    public method Config {tag args} {
        foreach {op arg} $args {
            set op [string range $op 1 end]
            set Tree($tag:$op) $arg
        }
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable Tree
    protected variable tree
    protected variable IM
    protected variable task ""
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
        tree_make $tree
    }
    destructor {
        foreach im [array names IM] {
            image delete $IM($im)
        }
        catch {after cancel $task}
    }
    # -------------------------------------------------- Private Methods ---
    # Create a new tree widget.
    #
    protected method tree_make {w args} {
        eval $w config -bg white $args
        bind $w <Destroy> "$this eDelitem $w /"

        foreach bm "open closed" {
            set IM($bm) [image create bitmap -data $BM($bm) \
            -maskdata $BM(mask) \
            -foreground black -background white]
        }
        foreach im [array names IMG] {
            set IM($im) [image create photo -data $IMG($im)]
        }

        $w bind x <1>        "$this eSelect $w %x %y"
        $w bind x <Double-1> "$this eExpand $w %x %y"

        tree_node_init $w /
        set Tree(selection) {}
        set Tree(selidx) {}
        tree_draw

        set w
    }

    # Initialize a element of the tree.
    #
    protected method tree_node_init {w v} {
        Config $v -children {} -open 0 -icon {} -tags {}\
            -title {} -titlefg black
    }

    # -- delete a tree
    protected method tree_delete {v} {
        if ![info exists Tree($v:children)] {
            return
        }
        if { $v == "/" } {
            set vv ""
        } else { set vv $v }
        # -- recursively delete all children
        foreach ch $Tree($v:children) {
            tree_delete $vv/$ch
        }
        # -- never actually delete the root
        if { $v == "/" } { return }

        # -- clean up tree attributes
        foreach a [array names Tree $v:*] {
            unset Tree($a)
        }
        # -- remove the entry from the parent children list (if any)
        set p [file dirname $v]
        set d ""
        set a [file tail $v]
        foreach ch $Tree($p:children) {
            if { $ch == $a } {  continue }
            lappend d $ch
        }
        set Tree($p:children) $d
    }

    #
    # Insert a new element $v into the tree $w.
    #
    protected method tree_newitem {w v args} {
        set dir [file dirname $v]
        set n [file tail $v]
        if {![info exists Tree($dir:open)]} {
            error "parent item \"$dir\" is missing"
        }
        set i [lsearch -exact $Tree($dir:children) $n]
        if {$i>=0} {
            error "item \"$v\" already exists"
        }
        lappend Tree($dir:children) $n
        set Tree($dir:children) [lsort $Tree($dir:children)]
        tree_node_init $w $v
        # set the attributes
        eval Config $v $args
        tree_draw 
    }

    
    # -------------------------------------------------------------------

    # -----------------------------------------------------------------
    # Internal use only.
    # Build a single layer of the tree on the canvas.  Indent by $in pixels
    protected method tree_buildlayer {w v in} {
      if {$v=="/"} {
        set vx {}
      } else {
        set vx $v
      }
      set start [expr $Tree(y)-10]
      set y ""
      foreach child $Tree($v:children) {
        set pchild $vx/$child
        set y $Tree(y)
        incr Tree(y) 17
        $w create line $in $y [expr $in+10] $y -fill gray50 
        set icon $Tree($pchild:icon)

        set taglist x
        foreach tag $Tree($pchild:tags) {
          lappend taglist $tag
        }

        set x [expr $in+12]

        if [info exists IM($icon)] {
          set im $IM($icon)
          set taglist1 $taglist ; lappend taglist1 $pchild:icon
          set k [$w create image $x $y -image $im -anchor w -tags $taglist1]
          incr x 20
          set Tree(tag:$k) $pchild
        }

        set taglist1 $taglist ; lappend taglist1 $pchild:text
        set tl $Tree($pchild:title)
        if { $tl == "" } { set tl $child }
        set j [$w create text $x $y -text $tl -font $FONT \
            -anchor w -tags $taglist1 -fill $Tree($pchild:titlefg)]
        set Tree(tag:$j) $pchild
        set Tree($pchild:tag) $j

        if {[string length $Tree($pchild:children)]} {
          if {$Tree($pchild:open)} {
             set j [$w create image $in $y -image $IM(open)]
             $w bind $j <1> [list $this eCloseNode $w $child $vx]
             tree_buildlayer $w $pchild [expr $in+18]
          } else {
             set j [$w create image $in $y -image $IM(closed)]
             $w bind $j <1> [list $this eOpenNode $w $child $vx]
          }
        }
      }
      if { $y != "" } {
        set j [$w create line $in $start $in [expr $y+1] -fill gray50 ]
        $w lower $j
      }
    }

    
    # Open a branch of a tree
    #
    protected method tree_open {v} {
      if {[info exists Tree($v:open)] && ($Tree($v:open)==0)
          && [info exists Tree($v:children)] 
          && [string length $Tree($v:children)]>0} {
        set Tree($v:open) 1
        tree_draw
      }
    }
    
    protected method tree_close { v} {
      if {[info exists Tree($v:open)] && $Tree($v:open)==1} {
        set Tree($v:open) 0
        tree_draw
      }
    }
    
    # Draw the selection highlight
    protected method tree_selection_draw w {
      if {[string length $Tree(selidx)]} {
        $w delete $Tree(selidx)
      }
      set v $Tree(selection)
      if {[string length $v]==0} return
      if {![info exists Tree($v:tag)]} return
      set bbox [$w bbox $Tree($v:tag)]
      if {[llength $bbox]==4} {
        set i [eval $w create rectangle $bbox -fill gray80 -outline {{}}]
        set Tree(selidx) $i
        $w lower $i
      } else {
        set Tree(selidx) {}
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
        if {[info exists Tree(tag:$m)]} {
          return $Tree(tag:$m)
        }
      }
      return ""
    }
    # -------------------------------------------------- 
    # -- EVENT HANDLING
    public method eCloseNode {w c vx} {
        set Tree($vx/$c:open) 0
        tree_draw
    }
    public method eOpenNode {w c vx} {
        set Tree($vx/$c:open) 1
        tree_draw
    }
    public method eSelect {w x y} {
        set lbl [tree_label_at $w $x $y]
        Select $lbl
        Event <<select>> $lbl
    }
    public method eExpand {w x y} {
        set lbl [tree_label_at $w $x $y]
        tree_open  $lbl
        Event <<expand>> $lbl
    }

    # Draw the tree on the canvas (after idle)
    public method eBuild {} {
      set task ""
      $tree delete all
      set Tree(y) 30
      tree_buildlayer $tree / 10
      $tree config -scrollregion [$tree bbox all]
      tree_selection_draw $tree
    }

    # Schedule to build tree when idle
    protected method tree_draw {} {
      if { $task != "" } { return } 
      set task [after $timeout "$this eBuild"]
    }
    
    #
    # Delete element $v from the tree $w.  If $v is /, then the widget is
    # deleted.
    #
    protected method eDelitem {w v} {
      if {![info exists Tree($v:open)]} return
      if {[string compare $v /]==0} {
        # delete the whole widget
        catch {destroy $w}
        foreach t [array names Tree $w:*] {
          unset Tree($t)
        }
      }
      foreach c $Tree($v:children) {
        catch {delitem $w $v/$c}
      }
      unset Tree($v:open)
      unset Tree($v:children)
      unset Tree($v:icon)
      set dir [file dirname $v]
      set n [file tail $v]
      set i [lsearch -exact $Tree($dir:children) $n]
      if {$i>=0} {
        set Tree($dir:children) [lreplace $Tree($dir:children) $i $i]
      }
      tree_draw 
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
