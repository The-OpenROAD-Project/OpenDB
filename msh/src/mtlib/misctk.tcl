#      Author: Manolis M. Tsangaris -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 1995-2004

package provide mtlib 1.0

#
#
#
proc LScrollableWidget {p widget args} {
    if ![winfo exists $p] { frame $p }
    set w $p.[string tolower $widget]
	eval $widget $w $args
    scrollbar $p.vs -relief sunken \
        -command "$w yview" -width 10
    scrollbar $p.hs -relief sunken -orient horizontal  \
        -command "$w xview" -width 10
    $w  config -xscrollc "$p.hs set" -yscrollc "$p.vs set"
    grid columnconfig $p 0 -weight 1
    grid rowconfig    $p 0 -weight 1
    grid $w $p.vs -sticky ns
    grid $w -sticky nsew
    grid $p.hs -sticky ew
    set w
}

proc LNewWin {prefix} {
	set n 1
	while { [winfo exists [set w $prefix-$n]] } {
		incr n
	}
	set w
}


proc LRaiseAll {w} {
	if ![catch {grid slaves $w} children] {
		foreach p $children {
			LRaiseAll $p
		}
	}
	if ![catch {pack slaves $w} children] {
		foreach p $children {
			LRaiseAll $p
		}
	}
	raise $w
}
#
# Miscalleneous functrions
#
#
# center a toplevel window
#
proc LCenter { w {cx ""} {cy ""}} {
    wm withdraw $w
	update idletasks
	if { $cx == "" } {
		set cx [expr [winfo screenwidth $w]/2]
		set cy [expr [winfo screenheight $w]/2]
	}
    set x [expr $cx - [winfo reqwidth $w]/2 - [winfo vrootx [winfo parent $w]]]
    set y [expr $cy - [winfo reqheight $w]/2 - [winfo vrooty [winfo parent $w]]]
    wm geom $w +$x+$y
    wm deiconify $w
	update idletasks
}

#
# Place w in the middle of the current pointer position
#
proc LPlaceCenter {w args} {
	set p [lindex $args 1]
	wm withdraw $w
	update idletasks
	set wx [winfo reqwidth  $w] ; set wy [winfo reqheight $w]
	switch -- [lindex $args 0] {
		-s  {
    		set  X [winfo rootx $p] ; set Y [winfo rooty $p]
			set px [winfo reqwidth  $p] ; set py [winfo reqheight $p]
    		set  x [expr $X  + $px - ($wx /2) ] 
    		set  y [expr $Y  + $py + ($wy /2) ]
		}
		default {
    		set X [winfo pointerx $w] ; set Y [winfo pointery $w]
    		set x [expr $X  - ($wx / 2)] 
    		set y [expr $Y  - ($wy / 2)]
		}
	}
    wm geom $w +$x+$y
	wm deiconify $w
}

#
# give emacs bindings to an entry widget
#
proc LBindEmacs {w} {
   bind $w <KeyPress-Delete> "
      if \[catch {$w delete sel.first sel.last}\] {
		$w delete \[expr \[$w index insert\]-1\]
	}
   "
   bind $w <Enter> "focus $w"
   bind $w <Leave> "focus ."
   bind $w <Control-KeyPress-d> "$w delete \[$w index insert\]"
   bind $w <Control-KeyPress-k> "$w delete \[$w index insert\] end"
   bind $w <Control-KeyPress-f> "$w icursor \[expr \[$w index insert\]+1\]"
   bind $w <Control-KeyPress-b> "$w icursor \[expr \[$w index insert\]-1\]"
   bind $w <Control-KeyPress-a> "$w icursor 0"
   bind $w <Control-KeyPress-e> "$w icursor end"
   bind $w <Control-KeyPress-u> "$w delete 0 end"
 
   # mac like
   bind $w <Double-Button-1> "$w select from 0; $w select to end"
} 

proc LForallPacked {root var Body {level 1}} {
    uplevel $level set $var $root
    uplevel $level $Body
    incr level
    foreach wch [pack slaves $root] {
        LForallPacked $wch $var $Body $level
    }
}

#
# Traverse the packing hierarchy of window w up,
# executing on each level a Tcl command
# (var is bound to each consecutive parent)
#
proc LForeachPackingParent {w var Body {level 1}} {
	while  { $w != "" } {
		set lastw $w
		uplevel $level set $var $w
		uplevel $level $Body

		## puts "looking for parent of $w"
		# Find its packing parent (if packer was used)
		if ![catch {set parent [pack info slaves $w]} ] {
			set w [lindex $parent 1]
		} else {
			## puts "looking inside tables"
			# Find its packing parent (if grid was used)
			if [catch {set p [grid slaves $w]} ] {
				return
			}
			set p ""
			## puts "looking inside masters=[blt_table masters *]"
			foreach parent [blt_table masters *] {
				## puts "checking masters=$parent"
				if { [blt_table slaves $parent $w] == $w } {
					set p $parent
					break
				}
			}
			if { $p == "" } { return }
			set w $p
			if { $lastw == $w } {
				error "oops! parent($lastw)=$lastw !"
				return
			}
		}
	}
	return
}

#
# Traverse all nodes of the subtree rooted at root
# formed by the packing/blt_table master/slave hierarchy
#
proc LForeachPacked {root var Body {level 1}} {
    uplevel $level set $var $root
    uplevel $level $Body
    incr level

    foreach wch [pack slaves $root] {
        LForeachPacked $wch $var $Body $level
    }

   if ![catch {blt_table masters $root}] {
   	foreach wch [blt_table slaves $root] {
       		LForeachPacked $wch $var $Body $level
  	}
   }
}

proc LTraverseWin {w var BODY {level 1}} {
    uplevel $level set $var $w
    uplevel $level $BODY
    incr level
	
	if ![catch {grid slaves $w} children] {
		foreach p $children {
			LTraverseWin $p $var $BODY $level
		}
	}
	if ![catch {pack slaves $w} children] {
		foreach p $children {
			LTraverseWin $p $var $BODY $level
		}
	}
}
#
# Run a command while events on $w  are blocked...
#
proc LWithBusy {w Body} {
    set wcmd $w
    if { [info command $w-win-] != "" } {
        set wcmd $w-win-
    }
    set cur [$wcmd cget -cursor]
    $wcmd configure -cursor watch
    grab set $w
    update idletasks
    set n [catch {uplevel $Body} err]
    $wcmd configure -cursor $cur
    grab release $w
    update idletasks
    if $n {
        error $err
    }
    return $err
}


# Load an Image from an external file
#
proc LLoadImage {w image {opts ""}} {
	catch {destroy $w}
	frame $w   -bg red
	frame $w.f -bg green -width 0 -height 0
	place  $w.f -in $w -relx 0 -rely 0  -width 0 -height 0
	update idletasks
	set cmd "exec xli -verbose -windowid [winfo id $w.f] $opts $image"
	# puts cmd=$cmd
	set s [eval $cmd]
	update idletasks

	# the xli output is like :
	# image is a 93x100 GIF87a image with 256 colors
	scan $s "%*s %*s %*s %dx%d %s %*s %*s %d" x y type colors
	# puts "<$s>"
	# puts "x=$x y=$y type=$type colors=$colors"

	place  $w.f -in $w -relx 0.0 -rely 0.0 -width $x -height $y
	$w config -width ${x} -height $y
	return $w.f
}

#
# execute two commands with a delay, so that animation like
# effect can be produced.
#
proc LAnimateCmd {n timeout BODY1 BODY2} {
    set sema [Semaphore #auto]
    while { [incr n -1] > 0 } {
        $sema Wait $timeout
        uplevel $BODY1
        $sema Wait $timeout
        uplevel $BODY2
    }
    $sema delete
}

# configure a window, in case it exists
proc ifWinConfig {win args} {
	if [winfo exists $win] {
		eval "$win config $args"
	}
}

# Evaluate a statement, if a window exists
proc ifWin {win args} {
	if [winfo exists $win] {
		eval $args
	}
}

# Query all properties of a window
proc LGetXProps {wid a} {
	upvar $a A
	if { $wid == "/" } { set wid [xaccess xwinroot -widget .] }
	foreach p [xaccess propget -windowid $wid] {
		if [catch {set r [xaccess propget -windowid $wid -property $p]}] {
			continue
		}
		set A([lindex $r 0]) [lrange $r 2 end]
	}
}

# Query a window property
proc LGetXProp {wid prop a} {
	upvar $a A
	if { $wid == "/" } { set wid [xaccess xwinroot -widget .] }
	foreach p [xaccess propget -windowid $wid] {
		if [catch {set r [xaccess propget -windowid $wid -property $p]}] {
			continue
		}
		set pname [lindex $r 0]
		if { $pname == $prop } { 
			set A(name)  $pname
			set A(type)  [lindex $r 1]
			set A(value) [lrange $r 2 end]
			return 1
		}
	}
	return 0
}

#
# Class:        LWindowDumper
# Description:  Simple Xwindow dumper
#      Author: Manolis M. Tsangaris -- mtlib@sinastria.gr
#   Copyright: Manolis M. Tsangaris, 1995-2004
#
itcl::class LWindowDumper {
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
		eval configure $args
	}
    destructor {}
    # -------------------------------------------------- Public Methods ----
	method Dump {w file} {
		update idletasks ; update
		set id [winfo id $w]
		exec xwd -id $id | xwdtopnm | ppmtogif > $file 2>/dev/null
	}
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- Common Initialization
}

proc LScrollableText {w args} {
	frame $w -class Text
	set box [listbox $w.box -relief sunken -border 1]
	if { $args != "" } {
		eval $box config $args
	}
	scrollbar $w.vsb -relief sunken -border 1 -width 10
	pack $w.box -side left -expand 1 -fill both
	pack $w.vsb -side left -fill y

	$w.vsb config -command "$w.box yview"
	$w.box config -yscroll "$w.vsb set"
	set box
}

# Locate the position of MTLIB
foreach p $auto_path {
	if [string match */mtlib $p] {
		set gMTLIB $p
	}
}
proc LBitmap {path} {
	global gMTLIB
	return "@$gMTLIB/bm/$path"
}
