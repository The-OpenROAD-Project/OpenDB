#
package provide mtlib 1.0

# Object Name: LDottyMaker
# Description: Class for creating dotty graphs
#
itcl_class LDottyMaker {
    # ---------------------------------------------------- Public Variables ---
    public file "" { open_file $file }
    public rankdir LR
    # ---------------------------------------------------- Protected Variables 
    protected NodeView
    protected EdgeView
    protected node_def "label {} color {} style {}"
    protected edge_def "label {} color {}"
    protected fp ""
    protected Node
    # ---------------------------------------------------- Common Variables ---
    # ---------------------------------------------------- Configuration ------
    constructor {config} { init }
    destructor { cleanup }
    method config {config} { }
    # ---------------------------------------------------- Public Methods ------
    method NodeView {class view} {
        set NodeView($class) [view2dotty $view]
    }
    method EdgeView {class view} {
        set EdgeView($class) [view2dotty $view]
    }
    method AddNode {id args} {
        set A(class) default
        array set A $args
        add_node $id A
    }
    method AddEdge {from to args} {
        set A(class) default
        array set A $args
        add_edge $from $to A
    }
    method NodeExists {n} { info exists Node($n) }

    method Subgraph {name nodes} {
        puts $fp "subgraph $name \{"
        foreach n $nodes {
            puts $fp "  \"$n\";"
        }
        puts $fp "\}"
    }
    method Flush {} {
        close_file
    }
    # --------------------------------------------------- Virtual Methods -----
    # --------------------------------------------------- Private Methods -----
    method init { } {
        NodeView default $node_def
        EdgeView default $edge_def
    }
    method cleanup { } {
        catch {close_file}
    }
    method open_file {file} {
        if { $file == "-" } {
            set fp stdout
        } else {
            set fp [open $file w]
        }
        puts $fp "digraph G \{"
        puts $fp "rankdir=$rankdir"
    }
    method close_file {} {
        if { $fp == "" } { return }
        puts $fp "\}"
        if { $file == "-" } { return }
        close $fp
    }
    # ---------------------------------------------------- 
    method add_node {id a} {
        upvar $a A
        set Node($id) 0
        puts -nonewline $fp "\"$id\""
        puts $fp " \["
        if [info exists NodeView($A(class))] {
            puts -nonewline $fp "$NodeView($A(class))"
        }
        foreach n [array names A] {
            if { $A($n) == "" } { continue }
            puts $fp "  $n = \"$A($n)\""
        }
        puts $fp "\]"
    }
    method add_edge {from to a} {
        upvar $a A
        puts -nonewline $fp "\"$from\" -> \"$to\" "
        puts $fp " \["
        if [info exists EdgeView($A(class))] {
            puts -nonewline $fp "   $EdgeView($A(class))"
        }
        foreach n [array names A] {
            if { $A($n) == "" } { continue }
            puts $fp "  $n = \"$A($n)\""
        }
        puts $fp "\];"
    }
    method view2dotty {view} {
        array set A $view
        set l ""
        foreach n [array names A] {
            if { $A($n) == "" } { continue }
            append l "  $n = \"$A($n)\"\n"
        }
        set l
    }
    # ---------------------------------------------------- 
    method test {} {
        NodeView people "shape circle style filled color lightblue"
        NodeView dept   "shape ellipse style filled color yellow"

        AddNode mt class people
        AddNode ek class people
        AddNode 1124 class dept
        AddEdge 1124 mt
        AddEdge 1124 ek

        Flush
    }
}

# Class:       LDotty
# Description: spawn and control a dotty process
#              At some point, we will also capture its window
#
itcl_class LDotty {
    # -------------------------------------------------- Public Variables --
    public cmd dotty
    # -------------------------------------------------- Protected Variables 
    protected pid ""
    protected file ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {config} { init }
    destructor { cleanup }
    method config {config} { }
    # -------------------------------------------------- Public Methods ----
    method ShowGraph {f} {
        show_graph $f
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Private Methods ---
    method init { } {
    }
    method cleanup { } {
        if { $pid != "" } { catch {exec kill -TERM $pid } }
    }
    method show_graph {file} {
        if { $pid != "" } { exec kill $pid }
        set pid [exec $cmd $file &]
    }
    # -------------------------------------------------- Common Initialization
}
