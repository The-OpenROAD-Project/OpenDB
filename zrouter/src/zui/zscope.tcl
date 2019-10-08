package provide ade::ui 2.0

#       Class: ZScope
# Description: Zui microscope
#      Author: Manolis M. Tsangaris
#
itcl::class ZScope {
    inherit LTopLevelEditor LBinder
    # Event <<select>> id
    # Event <<cmd>>  gui cmd
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method ShowSelection {objs} {
	$nb Highlight selection
        $TOOL(selection) Clear
        $TOOL(selection) Show $objs
    }
    public method Show {text} {
	show_to info $text
    }
    public method ShowStats {text} {
	show_to stats $text
    }
    # -- show ade notes
    public method ShowNote {data tag} {
	show_to note $data
	$nb Raise note
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable TOOL
    protected variable nb
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        pack [mktabs $This.tabs] -side top -expand 1 -fill both
        wm title $This "ZUI: INFO"
    }
    destructor {
        foreach t [array names TOOL] {
            $TOOL($t) delete
        }
        $nb delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method mktabs {w} {
        set nb [LSNotebook $w]
        set tools "selection TreeViewer Selection info TextViewer Info"
        lappend tools stats TextViewer Stats
        lappend tools note  NoteViewer Notes
        foreach {tag class tl} $tools {
            set wt [eval $class [$nb Add $tag  $tl].$tag]
            pack $wt  -side top -expand 1 -fill both
            set TOOL($tag) $wt
            $wt Redirect <<select>> $this
            $wt Redirect <<cmd>>    $this
        }
        set w
    }
    protected method show_to {tab text} {
	$nb Highlight $tab
	if { [string match +* $text] } {
	    $TOOL($tab) Show [string range $text 1 end]
	} else {
	    $TOOL($tab) Clear
	    $TOOL($tab) Show $text
	}
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

#
#       Class: TreeViewer
# Description: Dependency viewer
#      Author: Manolis M. Tsangaris
#
itcl::class TreeViewer {
    inherit LFrame LBinder
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Show {objs} {
        foreach obj $objs {
	    set n 0
            array set P "type ? hierarchy ?"
            if [catch {array set P $obj} ] {
		$tree Add /obj-$n -title $obj
	    } else {
if ![info exists P(id)] {
parray P
continue
}
		$tree Add /$P(id) \
		    -title "$P(id) ($P(hierarchy)/$P(type) @ $P(layer))"
	    }
            unset P
	    incr n
        }
        $tree Open /
    }
    public method Clear {} {
        $tree Clear
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable tree
    protected variable linfo
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set tree [LDirTree $This.tree -width 200 -height 200]
        pack $tree -side top -expand 1 -fill both
        $tree Bind <<select>> $this eSelect
    }
    destructor {
        $tree delete
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eSelect {path} {
        if { $path == "" } { return }
        Event <<select>> [file tail $path]
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: TextViewer
# Description: 
#      Author: Manolis M. Tsangaris
#
itcl::class TextViewer {
    inherit LFrame LBinder
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Show {val} {
        $text insert insert $val
    }
    public method Clear {} {
        $text delete 1.0 end
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable text
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set text [LScrollableWidget $This text -width 40 -height 10 -bg white]
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eSelect {path} {
    }
    # -------------------------------------------------- Common Initialization
}
#
#       Class: NoteViewer
# Description: Display notes from the ADE
#      Author: Manolis M. Tsangaris
#
itcl::class NoteViewer {
    inherit LFrame LBinder
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Show {val} {
        $text insert insert $val
    }
    public method Clear {} {
        $text delete 1.0 end
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables
    protected variable text
    protected variable WIN
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set text [LScrollableWidget $This.f text -width 40 -height 10 -bg white]
	pack $This.f -side top -expand 1 -fill both
	set f [frame $This.buttons]
	pack $f -side bottom -fill x

	menu_add $f.op   cmd  {
	    "Show Net"     {show net     $mode {$sel}}
	    "Show Inst"    {show inst    $mode {$sel}}
	    "Select Net"   {select net   $mode {$sel}}
	    "Select Inst"  {select inst  $mode {$sel}}
	}
	button $f.run -text RUN -command "$this eRun" -border 1
	set WIN(run) $f.run
	pack $f.op $f.run -side left
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    protected method menu_add {w tag choices} {
	menubutton $w -menu $w.m -text Command -border 1 -relief raised
	set m [menu $w.m -tearoff 0]
	set var $this.$tag
	foreach {name cmd} $choices {
	    $m add radiobutton -label "$name" \
		-command [list $this eModeSet $name $cmd] \
		-variable $var -value [list $name $cmd]
	}
	set var ""
	set w
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    public method eSelect {path} {
    }
    public method eRun {} {
	upvar #0 $this.cmd var
	if { $var == "" } { bell ; return }
	eval eModeSet $var
    }
    public method eModeSet {name cmd} {
	# -- change the label of the button
	$WIN(run) configure -text $name

	if [catch {$text get sel.first sel.last} sel] {
	    return
	}
	if { $sel == "" } { return }
	set list ""
	set ids 1
	foreach word [split $sel " \t\n"] {
	    set word [string trim $word]
	    if { $word == "" } { continue }
	    if {![regexp {^[0-9]+$} $word] } { set ids 0 }
	    lappend list $word
	}
	if { $ids } {
	    set mode id
	} else {
	    set mode name
	}
	set sel $list
	set cmd [subst $cmd]
	eval Event <<cmd>> $cmd
    }
    # -------------------------------------------------- Common Initialization
}
