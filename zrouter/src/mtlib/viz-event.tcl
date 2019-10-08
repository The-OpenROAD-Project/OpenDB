package provide mtlib 1.0

#
#       Class: LVizEvent
# Description: Simple Event visualizer
#
itcl::class LVizEvent {
    inherit LFrame LBinder
    # Event <<select>> set state time
    # -------------------------------------------------- Public Variables --
    public variable xtitle "" { $graph yaxis configure -title $xtitle }
    public variable ytitle "" { $graph xaxis configure -title $ytitle }
    public variable gtitle "" { $graph configure -title $gtitle }
    public variable bg     "black"
    # -------------------------------------------------- Public Methods ----
    public method Clear {} {
        clear
        reset
    }
    # -- add a new system
    public method Add {tag {name ""}} {
        if { $name == "" } { set name $tag }
        lappend Tags $tag
        set Rank($tag) [incr NSETS]
        set TagOf($Rank($tag)) $tag
        set Name($tag) $name
        set State($tag) ""
    }
    # -- system state change
    public method NewState {tag state time} {
        lappend State($tag) $state $time
    }
    public method Detail {tag at detail} {
        lappend Detail($tag) [list $at $detail]
    }
    # -- define State View
    public method StateView {state color} {
        set StateColor($state) $color
    }
    public method Refresh {} {
        foreach  tag $Tags {
            show_state $tag
        }
    }
    # -------------------------------------------------- Protected Variables 
    protected variable graph
    protected variable StateColor
    protected variable State
    protected variable Rank
    protected variable NSETS 0                    ;# to generate Rank(set)

    protected variable Detail
    protected variable Name
    protected variable Tags {}
    protected variable TagOf
    protected variable COLORS ""     ;# preallocated colors
    protected variable NCOLOR 0      ;# color counter
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        package require BLT
        pack [mkgraph $This.g] -side top -expand 1 -fill both
        set COLORS "orange indianred yellow brown darkseagreen lightblue"
	LBridge bg [list $graph configure -plotbackground]
    }
    destructor { cleanup }
    # -------------------------------------------------- Private Methods ---
    private method mkgraph {w} {
        set graph [blt::graph $w -relief sunken -border 1]
        $graph configure -plotbackground black
        $graph xaxis configure -command "$this eLabelX"
        $graph yaxis configure -command "$this eLabelY"
        $graph crosshairs configure -color white -hide no
        reset
        set w
    }
    protected method reset {} {
        catch {unset Rank}
        catch {unset TagOf}
        catch {unset Name}
        array set Rank {}
        array set TagOf {}
        array set Name {}
        catch {unset State}
        set Tags ""
        catch {$graph element delete *}
        catch {$graph pen delete *}
    }
    protected method cleanup { } {
    }
    # -------------------------------------------------- 
    # -- we generate stacked bargraphs, but apparently
    # -- we need to do them in reverse
    protected method show_state {set} {
        set n 0
        set last ""
        set lastt 0
        # -- delete any previous elements & pens
        foreach {state start}  $State($set) {
	    if { $last != "" } {
		mkelem $set:[incr n] $set $state $lastt $start $Rank($set)
	    }
            set last $state
            set lastt $start
        }
        $graph legend deactivate *
        $graph axis configure y -max 0 -min [expr -1*$NSETS -1]
    }
    protected method mkelem {elem set state x0 x1 y} {
        set st $set/$state
        set color ""
        foreach spat [array names StateColor] {
            if [string match $spat $st] {
                set color $StateColor($spat)
            }
        }
        if { $color == "" } {
            if [info exists StateColor($state)] {
                set color $StateColor($state)
            }
        }
        if { $color == "" } {
            set color [lindex $COLORS $NCOLOR]
            incr NCOLOR
            if { $color == "" } {
                set NCOLOR 0
                set color [lindex $COLORS $NCOLOR]
                incr NCOLOR
            }
            set StateColor($state) $color
        }
        # -- this state is a a final one
        if { $color == "-" } { return }
        set y [expr -1 * $y]
        set data "$x0 $y $x1 $y"
        catch {$graph pen delete p-$elem}
        $graph pen create p-$elem -linewidth 20 -symbol "" \
            -color $color -outline white

        catch {$graph element delete $elem}
        $graph element create $elem  -label ""\
            -fill $color -data $data -pen p-$elem 
        $graph element bind $elem <ButtonPress-1> \
            [list $this eSelect %x %y $set $state]
        $graph element bind $elem <ButtonRelease-1> \
            [list $this eRelease]
	$graph pen delete p-$elem
    } 
    # -------------------------------------------------- 
    public method eLabelY {w y} {
        set y [expr -1*$y ]
        if [info exists TagOf($y)] {
            return $Name($TagOf($y))
        }
        return ""
    }
    public method eLabelX {w x} { set x }

    # Clear the graph completely
    protected method clear {} {
        # foreach t [$graph element names] { $graph tag delete $t }
        foreach n [$graph element names] {
            $graph element delete $n
        }
    }
    # -------------------------------------------------- Callbacks
    public method eSelect {x y set state} {
        set time [$graph axis invtransform x $x]
        $graph crosshairs config -position @$x,$y -hide no
        Event <<select>> $set $state $time
    }
    public method eRelease {} {
        $graph crosshairs config -hide yes
    }
    # -------------------------------------------------- 
    protected method test {} {
        configure -xtitle "Time (min:sec)"
        configure -ytitle "State"

        ShowState none   gray
        ShowState active indianred
        ShowState paused blue    
        ShowState inuse  red     
        reset

        Add   s1 "Contact #1"
        Add   s2 "Inquiry #1"
        Add   s3 "Inquiry #2"

        State s1 active 0
        State s1 none   100

        State s2 active 10
        State s2 paused 20
        State s2 active 80
        State s2 none   100

        State s3 active 20
        State s3 paused 80

        Draw
    }
    # -------------------------------------------------- Common Initialization
    option add *StateMonitor*background gray70
    option add *StateMonitor*plotBackground gray90
}

#       Class: LEventParser
# Description: Simple Event Parser
#
itcl::class LEventParser {
    inherit LObject LBinder
    # Event <<eof>>
    # -------------------------------------------------- Public Variables --
    public variable file "" { file_open $file }
    public variable prefix "" ;# only look for lines stating with this
    public variable opts  ""  {array set OPT $opts}
    # -------------------------------------------------- Public Methods ----
    public method State {} {
        list seq [array get STATE] sets $SETS state [array get COLOR]
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable OPT
    protected variable fp
    protected variable RANK
    protected variable NRANK 0
    protected variable COLOR
    protected variable STATE
    protected variable SETS ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set prefix $OPT(prefix)
        if { $OPT(style) != "" } {
            style_parse $style
        }
    }
    destructor {
        if { $fp != "" } {
            close $fp
        }
        array set STATE {}
        array set RANK {}
        array set STATE {}
        set SETS {}
    }
    # -------------------------------------------------- Private Methods ---
    protected method file_open {file} {
        set fp [open $file r]
        fileevent $fp r "$this eLine $fp"
    }
    protected method parse_line {line} {
        set line [string trim $line]
        switch -glob -- $line {
            "#*"   {  return }
            set*  {
                set_add [lindex $line 1]
            }
            state* {
                set COLOR([lindex $line 1]) [lindex $line 2]
                Event <<state>> [lindex $line 1] [lindex $line 2]
            }
            [0-9]* {
                eval state_add $line
            }
        }
    }
    protected method state_add {time state} {
        set st    [split $state /]
        set set   [lindex $st 0]
        set state [lrange $st 1 end]
        if ![info exists STATE($set)] {
            set_add $set
        }
        lappend STATE($set) [list $time $state]
    }
    protected method set_add {set} {
        lappend SETS $set
        set RANK($set) [incr NRANK]
        set STATE($set) ""
        Event <<set>> $set
    }
    protected method style_parse {file} {
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eLine {fp} {
        if { [gets $fp line] == -1 } {
            close $fp
            set fp ""
            Event <<eof>>
            return
        }
        if { $prefix == "" } { 
            parse_line $line
        } else {
            if ![string match $prefix* $line] { return }
            set line [string range $line [llength $prefix] end]
            parse_line $line
        }
    }
    # -------------------------------------------------- Common Initialization
}
