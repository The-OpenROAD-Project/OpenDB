package provide mtlib 1.0

#
#       Class: LSTree
# Description: Simple Tree Widget
#
itcl::class LSTree {
    inherit LFrame LBinder
    # Event <<selected>> "tags"
    # -------------------------------------------------- Public Variables --
    public variable timeout 500 ;# how long to wait before you use the selection
    public variable terminals 0 ;# if true, only terminal nodes are returned
                                ;# as part of the selection
    public variable colors "" { catch {unset COLOR} ; array set COLOR $colors } 
    # -------------------------------------------------- Public Methods ----
    public method Add {path name {attrs ""}} {
        node_add $path $name $attrs
    }
    # -- unselect everything  -- no events
    public method Reset! {} {
        foreach tag [array names SELECTED] {
            node_select $tag 0
        }
    }
    public method Clear {} {
    }

    # select a node
    public method Select {tags {mode 1}} {
        foreach tag $tags {
            node_select $tag $mode
        }
    }
    # select all children
    public method Select* {tag {mode 1}} {
        node_select $tag $mode
        foreach ch $CHILDREN($tag) {
            Select* $tag/$ch $mode
        }
    }
    # return selection
    public method Selection {} {
        array names SELECTED
    }
    # Select quietly (i.e. no event)
    public method Select! {tags {mode 1}} {
        foreach tag $tags {
            node_select $tag $mode
        }
    }

    # return the current settings as boolean values
    public method Get {} {
        foreach node [array names WIN] {
            if [info exists SELECTED($node)] {
                set A($node) 1
            } else {
                set A($node) 0
            }
            # eliminate non-terminals
            if { $terminals && ($CHILDREN($node) != "") } {
                set A($node) 0
            }
        }
        array get A
    }
    public method Schedule {} {
        if { $task == "" } {
            set task [after $timeout "$this eSelectionSend"]
        }
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable BG
    protected variable SELECTED
    protected variable task ""
    protected variable WIN 
    protected variable CHILDREN
    protected variable COLOR
    protected variable N 0
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        pack [mknode $This.top . "."] -side top -expand 1 -fill both
        set BG(selected) "#898989"
        set BG(normal)   "#d9d9d9"
        array set SELECTED {}
    }
    protected method mknode {w node title} {
        frame $w -border 1 -relief sunken
        if { $node != "." } {
            label $w.title -text $title -justify left -anchor w -bg $BG(normal)
            pack $w.title -side left -fill both  -expand 1
            # pack [frame $w.spacer -width 20] -side left -expand 1 -fill y
            bind $w.title <Control-1> "$this eSelectSet* $node"
            bind $w.title <1>         "$this eSelectToggle* $node"
            bind $w.title <Motion>    "$this eReschedule"
            bind $w.title <Leave>     "$this eReschedule"
            bind $w.title <Enter>     "$this eReschedule"
        }
        set CHILDREN($node) ""
        set WIN($node) $w
        set w
    }
    protected method node_select {node {select 1}} {
        if ![info exists WIN($node)] {
            puts stderr "*** $This: $node: does not exist"
            return
        }
        # no need to update the root node
        if { $node == "." } { return }

        if { $select } {
            if [info exists COLOR($node)] {
                set bg $COLOR($node)
            } else {
                set bg $BG(selected)
            }
            $WIN($node).title configure -bg $bg
            set SELECTED($node) $node
        } else {
            $WIN($node).title configure -bg $BG(normal)
            catch {unset SELECTED($node)}
        }
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- CallBacks
    # -- in response to a user selection
    public method eSelectToggle* {node} {
        if [info exists SELECTED($node)] {
            Select* $node 0
        } else {
            Select* $node 1
        }
        Schedule
    }
    # -- in response to a user exclusive selection
    public method eSelectSet* {node} {
	catch {unset SELECTED}
	Select* $node 1
        Schedule
    }
    # after a timeout, when selection is finalized
    public method eSelectionSend {} {
        set task ""
        Event <<selected>> [array names SELECTED]
    }
    # reschedule a selection send
    # done to delay forming the selection
    public method eReschedule {} {
        if { $task != "" } {
            after cancel $task
            set task [after $timeout "$this eSelectionSend"]
        }
    }
    # -------------------------------------------------- 
    protected method node_add {path name attrs} {
        set parent [file dirname $path]
        if { $parent == "/" } { set parent "." }
        if ![info exists WIN($parent)] {
            puts stderr "*** $This: $parent: does not exist"
            return
        }
        set node [file tail $path]
        set w $WIN($parent).c[incr N]
        lappend CHILDREN($parent) $node
        set w [mknode $w $path $name]
        pack $w -side top -expand 1 -fill both
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
