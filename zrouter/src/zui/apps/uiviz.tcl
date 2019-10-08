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

package provide ade::ui::apps 1.0

#
#       Class: UIViz
# Description: UI visualization abstraction
#      Author: Manolis M. Tsangaris
#
itcl::class UIViz {
    inherit LObject LBinder
    # -------------------------------------------------- Public Variables --
    public variable stylesheet "" ;# stylesheet for visualizaton
    # -------------------------------------------------- Public Methods ----
    # -- new states
    public method NewStates {argv} {
	$this uiv_new_states $argv
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    protected method uiv_new_states {states} {
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#
#       Class: UIViz_milos
# Description: UI visualization abstraction
#      Author: Manolis M. Tsangaris
#
itcl::class UIViz_milos {
    inherit UIViz
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable top ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	set top [VSystem .milos -title "ADE:Milos Visualization"]

	set css {
	    nodes {
		A "Cloned Node" {}
		B "Setup Node"  {}
		C "Hold Node"   {}
		main   "Main Node" {
		    state State
		    opts  Opts
		    undos Undos
		    tim_updates      "Tim. Updates"
		    - -
		    -     "Setup Slacks"
		   total.setup.slack "Total"
		   worst.setup.slack "Worst"
		    -     "Hold Slacks"
		   total.hold.slack  "Total"
		   worst.hold.slack  "Worst"
		    -     -
		}
	    }
	    layout {
		A 1 1 
		B 1 2 
		C 1 3 
		main   2 2
	    }
	    colors {
		idle         gray
		init         lightblue
		opt-setup    orange1
                sending-db   orange2
		merge        green1

		eco          brown2
                sending-eco  brown2

                extraction   green2
                slack.update darkolivegreen
                merging-remote-timing green

		optimize blue
	    }
	    state-graph {
		status
	    }
	    plot { 
		sets { 
		    main.opts  "optimizations"
		    main.undos "undo"
		}
		title  "Convergence Process"
		xtitle "time (sec)"
		ytitle "count"
	    }
	}
	if { $stylesheet == "" } { set stylesheet $css }
	uiv_render $stylesheet
    }
    destructor {
	$top delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method uiv_new_states {states} {
	foreach {attr value}  $states {
	    if ![regexp {^([^\.]+)\.(.*)$} $attr _ prefix suffix] {
		continue
	    }
	    $top SetState $prefix $suffix $value
	    unset prefix
	    unset suffix
	}
	$top Refresh
    }
    # -- render the stylesheet
    protected method uiv_render {css} {
	set css [LRemoveComments $css]
	foreach {section def} $css {
	    switch $section {
		nodes {
		    foreach {node descr attrs} $def {
			$top AddNode $node $descr $attrs
		    }
		}
		layout {
		    $top Layout $def
		}
		attributes {
		    foreach {attr info} $def {
			$top AddAttribute $attr $info
		    }
		}
		colors {
		    $top configure -colormap $def
		}
		state-graph {
		    # -- make sure the status variable is a list
		    $top configure -state_attribute [lindex $def 0]
		}
		plot {
		    $top configure -plotter $def
		}
	    }
	}
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#       Class: VSystem
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class VSystem {
    inherit LTopLevelEditor
    # -------------------------------------------------- Public Variables --
    public variable colormap "" {
	if { $wgraph == "" } { return }
        foreach {state color} $colormap {
            $wgraph StateView $state $color
        }
	foreach tag [array names WIN] {
	    $WIN($tag) configure -colormap $colormap
	}
    }
    public variable plotter "" { ;# plotter configuration
	if { $wplotter == "" } { return }
	vs_plotter_config $plotter
    }
    public variable state_attribute "status" { 
	# attribute to plot in state graph
	foreach tag [array names WIN] {
	    $WIN($tag) configure -state_attribute $state_attribute
	}
    }
    # -------------------------------------------------- Public Methods ----
    public method AddNode {tag name attrs} {
	vs_add_node $tag $name $attrs
    }
    # -- record the optional layout
    public method Layout {lay} {
	set layout $lay
	vs_do_layout $layout
    }
    public method SetState {tag attr val} {
	if ![winfo ismapped $This] { 
	    $this UnHide
	}
	set now [expr [clock seconds] - $epoch]
	# -- update the main view
	$WIN($tag) Set $attr $val
	# -- update the state graph, if needed

	if { $attr == $state_attribute } {
	    $wgraph NewState $tag $val $now
	}
	if [info exists PLOT($tag.$attr)] {
	    $wplotter Plot $tag.$attr $now $val
	}
    }
    public method Refresh {} {
	$wgraph Refresh
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable wnodes ""
    protected variable wgraph ""
    protected variable wplotter ""
    protected variable winfo ""       ;# info window
    protected variable WIN            ;# windows of attributes
    protected variable CF             ;# stylesheet
    protected variable nb             ;# notebook
    protected variable epoch 0        ;# start of time
    protected variable PLOT           ;# plotter attributes
    protected variable layout ""      ;# layout
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

	set epoch [clock seconds]

	winconfig -border 1 -relief sunken
        set nb [LSNotebook $This.nb]
	pack $nb -side top -expand 1 -fill both

 	set wnodes [$nb Add system System]
	$wnodes configure -bg gray40 -border 1 -relief sunken

 	set wgraph [LVizEvent [$nb Add graph "Time Graph"].viz]
	$wgraph configure -bg gray40
	$wgraph Bind <<select>> $this eVizEventSelect

	pack $wgraph -side top -expand 1 -fill both

 	set wplotter [LPlotter [$nb Add plotter "Plotter"].plotter]
	$wplotter configure -scroll 0
	pack $wplotter -side top -expand 1 -fill both
	$wplotter Bind <<select-canvas>> $this ePlotterSelect

	if { $colormap != "" } { 
	    configure -colormap $colormap
	}
	if { $plotter != "" } { 
	    configure -plotter $plotter
	}
	set winfo [entry $This.info -bg black -fg red]
	pack $winfo -side bottom -fill x
	
    }
    destructor {
	foreach tag [array names WIN] { $WIN($tag) delete }
	$nb delete
	$wgraph delete
	$wplotter delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method vs_add_node {tag name attrs} {
	set w [VComputeNode $wnodes.n$tag -title $name -colormap $colormap \
	    -state_attribute $state_attribute]
	set WIN($tag) $w

	foreach {attr info} $attrs {
	    $WIN($tag) Define $attr $info
	}

	# -- add a system on the time graph
	$wgraph Add $tag $name
    }
    protected method vs_do_layout {layout} {
	foreach {tag row column} $layout {
	    if [info exists WIN($tag)] {
		grid $WIN($tag) -row $row -column $column -padx 2 -pady 2
	    }
	}
    }
    protected method vs_plotter_config {descr} {
	$wplotter Clear
	foreach {a val} $descr {
	    switch $a {
		title  { $wplotter Configure configure -title $val }
		xtitle { $wplotter Configure axis config x -title $val }
		ytitle { $wplotter Configure axis config y -title $val }
		sets {
		    foreach {tag name} $val {
			set PLOT($tag) 1
			$wplotter AddSet $tag $name
		    }
		}
	    }
	}
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method vMakeButtons {w} {
	button_add_right $w.hide -text Hide -command "$this Hide"
	set w
    }
    public method eVizEventSelect {node state time} {
	$winfo delete 0 end
	$winfo insert 0 [format "%20s: %s (at %.0f sec)" $node $state $time]
    }
    public method ePlotterSelect {x y info} {
	array set V $info
	$winfo delete 0 end
	if [info exists V(name)] {
	    $winfo insert 0 "$V(name) at $x $y"
	}
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: VComputeNode
# Description:
#      Author: Manolis M. Tsangaris
#
itcl::class VComputeNode {
    inherit LFrame
    # -------------------------------------------------- Public Variables --
    public variable title "node"
    public variable colormap "" { array set COLOR $colormap }
    public variable state_attribute "status"
    # -------------------------------------------------- Public Methods ----
    public method Set {attr val} {
	mn_set $attr $val
    }
    public method Define {attr info} {
	mn_define $attr $info
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable WIN
    protected variable COLOR
    protected variable N 0
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
	winconfig -border 1 -relief raised -padx 1 -pady 1
	label $This.title -text $title
	grid $This.title -columnspan 2 -sticky ew
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method mkattr {attr info} {
	incr N
	if {$attr != "-" } {
	    set wl [label $This.lattr$N -text $info: -anchor e]
	    set wv [label $This.vattr$N -text "" -anchor w\
		-width 20 -bg white -border 1 -relief sunken -padx 10]
	    grid $wl $wv
	    grid configure $wv -sticky ew
	    set WIN($attr) $wv
	} elseif {$info == "-" } {
	    set wl [frame $This.lattr$N -height 2 -border 1 -relief groove]
	    grid $wl -columnspan 2
	} else {
	    set wl [label $This.lattr$N -text " -- $info --" -anchor c]
	    grid $wl -columnspan 2 -sticky ew
	}
	grid configure $wl -sticky ew
    }
    # ----------------------------------------------- Interface Implementation
    protected method mn_define {attr info} {
	mkattr $attr $info
    }
    protected method mn_set {attr val} {
	if { $attr == $state_attribute } {
	    if [info exists COLOR($val)] {
		$This.title configure -bg $COLOR($val)
	    } else {
		$This.title configure -bg yellow
	    }
	} else {
	    # -- in case the attribute is missing, just add it there
	    if ![info exists WIN($attr)] {
		mkattr $attr $attr
	    }
	    $WIN($attr) configure -text $val
	}
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
