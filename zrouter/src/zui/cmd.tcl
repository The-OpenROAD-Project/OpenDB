package provide ade::ui 2.0

#
#       Class: ZCMD
# Description: ZUI command handling
#      Author: Manolis M. Tsangaris
#
itcl::class ZCMD {
    inherit LObject LBinder
    # Event <<result>> value ;# value to be visualized
    # Event <<adapter-event>> args ;# when remote adapter posts events
    # -------------------------------------------------- Public Variables --
    public variable stream 1                 ;# should we stream?
    public variable loader ""                ;# loader
    public variable rpc ""                   ;# remote connection
    public variable opts "" { array set OPT $opts }
    public variable zhist ""                 ;# session history
    public variable zhelp ""                 ;# helper
    public variable dsmgr ""                 ;# data set manager
    public variable bmgr  ""                 ;# bookmark manager

    public variable pprefix zroute ;# zroute session info directory prefix
    # -------------------------------------------------- Public Methods ----
    public method result {args} {
        Event <<result>> $args
    }
    # -------------------------------------------------- USER COMMANDS
    # -- center around this object
    public method center {obj} {
        cmd_center $obj
    }
    # -------------------------------------------------- public commands
    # --- initialize
    public method init {} {
        set s $OPT(server)
        if { $s == "" } {
            set s [session_collect]
            if { $s != "" } {
                result type sessionselect sessions $s
                return
            }
            set s 9768@localhost
        }
        connect $s
    }

    # -- connect to a remote server, or reconnect
    public method connect {{s "-"}} {
        if { $s == "-" } {
            set s [$zdm Get server]
        }
        $zdm Set server $s
	$zdm Set crawler ""   ;# to force a choice of a crawler
        hpush "connect $s"
    }

    # -- collect all available sessions to connect to
    public method zsessions {} {
        result type sessionselect sessions [session_collect]
    }

    # -- force cmd to refresh the GUI
    public method refresh {} {
	# -- update objects in the display list
	foreach {dset result} $DLIST {
	    $dsmgr Refresh $dset
	    eval result $result
	}
    }

    # -- set filters
    public method filters {f} {
        $zdm Set filters $f
        hpush "filtering"
    }

    # --- select a set of objects in the GUI
    # --- add to selection if mode is "+"
    # --- by default, replace selection
    public method select {objs {mode "="}} {
	if { $mode == "=" } {
	    set sel $objs
	} else {
	    set sel [concat [$zdm Get selection] $objs]
	}
        $zdm Set selection $sel
        hpush "select objects"
    }

    # -- return object definition. Use the cache if you can
    public method objdef {obj} {
	if { $obj == "" } { return "" }
	$dsmgr ObjDef $obj
    }

    # -- show a group of objects
    public method group {obj} {
        set dset [$dsmgr GetGroup [$zdm Get chip] $obj [$zdm Get bbox]]
        dset_result type group dset $dset obj $obj
    }

    # -- show the entire chip (apply filters, if present)
    public method fullchip {{filters ""}} {
        $zdm Set start /
        $zdm Set mode all
	set f [mk_filters $ACHIP(filters) $filters]
	$zdm Set filters $f

	# -- update the filters
	result type filters filters $f

	# -- normalize and set the view
	result type view bbox  $ACHIP(bb) reason fullchip
	hpush "fullchip"
    }
    # -- enter chip mode again
    # -- keep the rest of the parameters the same
    public method chipmode {} {
	set obb [$zdm Get obbox]
        $zdm Set start /
        $zdm Set mode all
	$zdm Set bbox $obb
	hpush "chipmode"
    }

    # -- set streaming
    public method stream {mode} {
	configure -stream $mode
    }

    # -- perform an inspection query -- show choices if necessary
    public method inspect {obj {info ""}} {
        if { $info == "" } {
            set choices [$loader ObjInspectChoices [$zdm Get chip] $obj]
            result type choose chip [$zdm Get chip] \
		obj $obj choices $choices
            return
        }
        $zdm Set start $obj 
	if { $info != "self" } {
	    $zdm Set mode $info
	    $zdm Set bbox ""
	}
        hpush "inspect $obj $info"
    }

    # -- change the bounding box to view
    public method view_set {x0 y0 x1 y1 reason} {
        if { $x0 > $x1 } { swap x0 x1 } 
        if { $y0 > $y1 } { swap y0 y1 } 
        $zdm Set bbox "$x0 $y0 $x1 $y1"
        hpush "$reason"
    }

    # -- set the current view to the bbox of the selection
    public method view_set_selection {reason} {
	set sel [$zdm Get selection]
	# -- is there a selection?
	if { $sel == "" } { return }

	set BB ""
	foreach obj $sel {
	    set info [objdef $obj]
	    if { $info == "" } { return }
	    array set A $info
	    set BB [bbox_merge $BB  $A(bb)]
	}
	# -- no data points
	if { $BB == "" } { return }
	# -- cause the gui to do the rest of the work (like norm the bbox)
	result type view bbox [bbox_pad $BB 0.1 0.1] reason $reason
    }
    # -- merge current bbox bb with newbb
    protected method bbox_merge {abox bbox} {
	if { $abox == "" } {
	    set abox $bbox ; return $abox
	}
	if { $bbox == "" } { return $abox }
	foreach {ax0 ay0 ax1 ay1} $abox { break }
	foreach {bx0 by0 bx1 by1} $bbox { break }
	if { $bx0 < $ax0 } { set ax0 $bx0 }
	if { $by0 < $ay0 } { set ay0 $by0 }
	if { $ax1 < $bx1 } { set ax1 $bx1 }
	if { $ay1 < $by1 } { set ay1 $by1 }
	list $ax0 $ay0 $ax1 $ay1
    }
    # -- enlarge the bbox by sx*DX and sy*DX (DX, DY the bbox size)
    protected method bbox_pad {bb sx sy} {

	foreach {ax0 ay0 ax1 ay1} $bb { break }
	set dx  [expr $sx * ($ax1 - $ax0)]
	set dy  [expr $sy * ($ay1 - $ay0)]

	# -- keep the maximum
	if { $dx < $dy } { set dx $dy }

	set ax0 [expr 1.0*$ax0 - $dx]
	set ay0 [expr 1.0*$ay0 - $dx]
	set ax1 [expr 1.0*$ax1 + $dx]
	set ay1 [expr 1.0*$ay1 + $dx]
	list $ax0 $ay0 $ax1 $ay1
    }

    # -- notify on the new pixel grid sizes
    public method grid_set {grid} {
        foreach {gx gy} $grid { break }
        set GX $gx ; set GY $gy
        egrid_set
    }

    # -- change gfactor, i.e. how many pixels we will use to
    # -- aggregate/sample data
    public method gfactor {gf} {
        set gfactor $gf
        egrid_set
        hpush "gfactor"
    }

    # make a note of the new obbox -- do not produce events
    # obbox is the real bbox (based on the actual objects drawn on canvas)
    public method obbox_set {bb} {
        $zdm Set! obbox $bb
    }

    # -- return id information from the backend
    public method whoami {} {
        result type info whoami [$loader Whoami]
    }

    # -- handle touching an object, depending on the mode
    # -- bb is the cursor bounding box
    public method touch {bb {mode select}} {
        cmd_touch $bb $mode
    }

    # -- handle mouse over an object
    public method peek {bb} {
        cmd_peek $bb
    }


    # -- history handling
    public method history {do} {
       switch -- $do {
            back {
                if ![$zhist Back P]  { bell ; return }
            }
            next {
                if ![$zhist Next P]  { bell ; return }
            }
            clear {
                $zhist Clear
                return
            }
        }
        # -- P now is the new state to go to
	eval $zdm Set [array get P]
    }


    # -- handle mouseover (balloon help) calls
    # -- 
    public method flash {X Y} {
    }

    # ------------------------------------------------------------- find 
    # -- called by gtcl as a response to the user select command
    public method user_find_select {mode args} {
	# -- unselect ?
	if { ($args == "") || ($args == "{}") } { 
	    select ""
	    return
	}
        set objs [obj_find select [$zdm Get bbox] $args]
        if { $objs == "" } { 
            return
        }
        set res ""
        foreach obj $objs {
            array set P $obj
	    $dsmgr AddObjCache $P(id) [array get P]
            lappend res $P(id) ; unset P
        }
	# -- this select is always additive
        select $res $mode
    }

    # -- called by user to find and display objects
    # -- if mode is additive (+), just append to the existing data set
    public method user_find_show {mode args} {
	if { $mode == "=" } {
	    $zdm Set start $args
	    $zdm Set mode find
	    $zdm Set bbox ""
	    hpush "find & show"
	    return
	}
	error "unsupported mode for show ($mode)"
    }

    # ------------------------------------------------------------- help
    public method help {topic} {
        $zhelp UnHide
        $zhelp Show $topic
    }

    # ------------------------------------------------------------- crawler
    # -- generate the list of crawlers
    public method crawlers {} {
        result type crawler_choices crawlers [$loader Crawlers]
    }
    # -- use a new crawler
    public method crawler_set {cr} {
        $loader CrawlerSet $cr
        $zdm Set crawler $cr
        hpush "crawler set to $cr"
    }
    # ------------------------------------------------------------- bookmarks
    # -- add a bookmark
    public method addbookmark {x0 y0 x1 y1 {descr ""}} {
	set bid [$bmgr Add $x0 $y0 $x1 $y1 $descr]
	result type bookmark bid $bid bbox [list $x0 $y0 $x1 $y1]
    }
    public method gotobookmark {bid} {
	array set B [$bmgr Get $bid]
	view_set $B(x0) $B(y0) $B(x1) $B(y1) bookmark
    }
    public method deletebookmark {bid} {
	$bmgr Remove $bid
    }
    # -------------------------------------------------- Protected Variables 
    protected variable oldfilters "" ;# previously used filters
    protected variable ACHIP         ;# chip attributes
    protected variable OPT           ;# options
    protected variable gfactor  4    ;# pixel lines to sample
    protected variable GX  ""        ;#
    protected variable GY  ""        ;#
    protected variable zdm           ;# dependency manager
    protected variable ADE_ATTR      ;# ade attributes
    protected variable DLIST ""      ;# display list of data sets
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        set OPT(maxobjects) 1 ;# i.e. all objects
        eval configure $args
	$loader Bind <<ade-event>> $this eADEEvent
	zdm_setup
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method zdm_setup {} {
        set zdm [LSDM $this.dm -debug 0]

        # -- setup the dependency manager
        $zdm Depend server
        $zdm Bind   server   "$this eMake server"

        $zdm Depend crawler  server

        $zdm Depend view  crawler start mode bbox filters
        $zdm Bind   view  "$this eMake view"

        $zdm Depend selection  view
        $zdm Bind   selection  "$this eMake selection"

        $zdm Depend overview    server crawler
        $zdm Bind overview     "$this eMake overview"

        $zdm Depend filters  overview
        $zdm Bind   filters  "$this eMake filters"

        #$zdm Depend overview/sel overview bbox
        #$zdm Bind overview/sel "$this eMake overview/sel"

        $zdm Set! server  9768@localhost ;# default server
        $zdm Set! crawler ""         ;# default crawler
        $zdm Set! chip all           ;# the name of the chip
        $zdm Set! mode all           ;# navigation mode
        $zdm Set! start /            ;# start of navigation
        $zdm Set! bbox ""            ;# bbox being used
        $zdm Set! filters ""         ;# filters used 

        $zdm Set! selection ""       ;# list of selected objects

	$zdm Set! cbbox ""           ;# last used cursor bbox
        $zdm Set! gfactor 1          ;# grid factor (for aggragation)
        $zdm Set! obbox ""           ;# the real bbox (based on drawn objs)
        $zdm Set! gx ""
	$zdm Set! gy ""              ;# pixel grid size

        $zdm Reset                  ;# cancel any activity due to init

        $zdm Propagate on-idle       ;# propagation mode
    }

    # ----------------------------------------------- Interface Implementation
    # -- connect to a server and check if it is ok to use it
    # -- set A to the remote ADE attributes
    protected method conn_check {server aa} {
	upvar $aa A
        if [catch {$rpc Connect $server} err] {
            return 0
        }
        if [catch  {$rpc Run zroute::whoami} res ] {
            $rpc Disconnect
            return 0
        }
        $rpc Disconnect
        array set S $res
        set user [LPlatform user]

	# -- is this the right user?
        if { $S(user) != "$user"  } {
            return 0
        }
	# -- assemble the remote ADE attributes
	catch {unset A}
	array set A $res
        return 1
    }
    # return all possible sessions
    protected method session_collect {} {
        set base [::LPrefs::PrefDir $pprefix]
        set res ""
        foreach file [glob -nocomplain [file join $base *]] {
            if ![file isfile $file] { continue }
            set pref [LPrefs $this.pref -file $file]
            catch {unset S}
            array set S [$pref Get]
            $pref delete
            # is it junk?
            if ![info exists S(pid)] {
                file delete $file
                continue
            }
            lappend res [array get S]
        }
        set res
    }
    # -- save current state in history
    protected method hpush {reason} {
        set state [$zdm GetAll]
        lappend state reason $reason
        $zhist Push $state
    }

    # -- generate a dataset result, and keep it in the cache
    # -- just like "method result"
    protected method dset_result {args} {
	array set A $args
	set DLIST [list $A(dset) $args]
        Event <<result>> $args
    }
    # -------------------------------------------------------- center object
    protected method cmd_center {obj} {
        set info [$loader ObjInfo [$zdm Get chip] $obj]
        if { $info == "" } {
            result type error error "$obj: does not exist"
            return
        }
        array set A $info
        # -- get the bbox of the object
        foreach {cx0 cy0 cx1 cy1} $A(bb) { break }

        # -- compute centroid
        set cxx [expr ($cx0+$cx1)/2.0] ; set cyy [expr ($cy0+$cy1)/2.0]

        # -- use the current bbox, or the chip bbox if none
        set bb [$zdm Get bbox] 
        if { $bb == "" } { set bb $ACHIP(bb) } 
        # any bbox after all?
        if { $bb == "" } { return }

        # -- compute the bbox size
        foreach {x0 y0 x1 y1} $bb { break }
        set cx [expr abs($x1 - $x0)/2.0] ; set cy [expr abs($y1 - $y0)/2.0]

        # change bbox to center around the selected object
        bbox [expr $cxx-$cx] [expr $cyy-$cy] [expr $cxx+$cx] [expr $cyy+$cy]
    }
    # --------------------------------------------------------------- touch
    protected method cmd_touch {bb mode} {
        # make a note of the cursor bbox (no need to cause an update)
        $zdm Set! cbbox $bb

        # find all objects touched by the mouse
        set objects [cmd_cursor_objects $bb]

        # -- tell the gui about the hit list
        # result type hitlist hitlist $objects

        if { $objects == "" } { return }

	top_object $objects S

        # return the right value, based on the type of operation
        switch $mode {
            select   {
                $zdm Set selection $S(id)
                hpush "select $S(id)"
            }
            inspect {
                $zdm Set selection $S(id)
                hpush "select to inspect $S(id)"
                inspect $S(id)
            }
	    default {
		puts "*****=cmd_touch/$mode"
	    }
        }
    }
    # -- locate the top most object from the list
    # -- return attributes to S
    # -- update the cache
    protected method top_object {objects ss} {
	upvar $ss S
        # -- select the highest layer object
        set layer -1
	set ss ""
        foreach obj $objects {
            array set P $obj
            if {$P(layer) > $layer} {
                set ss [array get P]
		set layer $P(layer)
            }
	    $dsmgr AddObjCache $P(id) [array get P]
            unset P
        }
	array set S $ss
	array size S
    }

    # -- locate objects below the cursor and use that information
    # -- to display a balloon help
    protected method cmd_peek {bb} {
        # find all objects touched by the mouse
        set objects [cmd_cursor_objects $bb]

	if [top_object $objects S] {
            set info [$loader ObjPeek \
		[$zdm Get chip] $bb [$zdm Get filters] $S(id)]
	} else {
	    set info ""
	    set S(id) ""
	}
	result type peek obj $objects info $info top $S(id)
    }

    # -- set the real grid size, based on gfactor
    protected method egrid_set {} {
        if { ($GX == "") || ($gfactor == "") } { 
            $zdm Set gx ""
            $zdm Set gy ""
        } else {
            $zdm Set gx [expr $GX * $gfactor]
            $zdm Set gy [expr $GY * $gfactor]
        }
    }

    # -- convert chip_get filters into usable filter value
    # -- apply the initial filter values on top of the defaults
    protected method mk_filters {afilters init} {
	array set INIT $init
	array set F {}
	foreach {filter _ descr} $afilters {
	    array set F1 {}
	    foreach {f _ val} $descr {
		set F1($f) $val
	    }
	    if [info exists INIT($filter)] {
		array set F1 $INIT($filter)
	    }
	    set F($filter) [array get F1]
	    unset F1
	}
	array get F
    }

    # --------------------------------------------------------------- FIND
    # -- find an object "name" -- return a list of objects
    protected method obj_find {name bb arg} {
        if { ( $arg == "" ) } {
            error [find_usage $name]
        }
	# -- parse the three additional parameters
        foreach {a b c} $arg { break }
        switch [llength $arg] {
            1 { return [$dsmgr ObjInfo [$zdm Get chip] $a] }
        }
        if { ($a == "") || ($b == "") || ($c == "") } {
            error [find_usage $name]
        }
        $loader Find [$zdm Get chip] $a $b $c $bb [$zdm Get filters]
    }
    # -- find an object "name" -- return a data set
    protected method obj_find_dset {name bb arg} {
        if { ( $arg == "" ) } {
            error [find_usage $name]
        }
	# -- parse the three additional parameters
        foreach {a b c} $arg { break }
        switch [llength $arg] {
            1 { return [$dsmgr ObjInfo [$zdm Get chip] $a] }
        }
        if { ($a == "") || ($b == "") || ($c == "") } {
            error [find_usage $name]
        }
        set dset [$dsmgr Find [$zdm Get chip] $a $b $c $bb [$zdm Get filters]]
	set dset
    }
    # -- display usage of find
    protected method find_usage {name} {
        set list {net name "net by name" net id "net by id"}
        set msg "Usage:\n"
        append msg "  gui-id (to $name any gui object)\n"
        foreach {a b c} $list {
            append msg "  $name $a $b string (to $name $c)\n"
        }
        set msg
    }
    # ------------------------------------------------------------- Rendering
    # -- try to connect to a remote server
    # -- return 0 if it has failed
    protected method nconnect {server} {
        $loader Connected 0
        if [catch {$rpc Connect $server} err] {
            $loader Connected 0
            result type warning info "connect: $server: $err"
            return 0
        }
        $loader Connected 1
	# -- initialize connection
	$loader Init

	# -- obtain ADE attributes
	catch {unset ADE_ATTR}
	array set ADE_ATTR [$loader Whoami]

	result type connection server $server attrs [array get ADE_ATTR]

        return 1
    }

    # -- draw the chip overview 
    protected method chip_overview_draw {} {
        array set ACHIP   [$loader GetAttr [$zdm Get chip]]
        set ACHIP(bb)     [string trim $ACHIP(bb)]
        set ACHIP(server) [$zdm Get server]
        foreach {x0 y0 x1 y1} $ACHIP(bb) { break }
        set ACHIP(width)  [expr 1.0*abs($x1-$x0)]
        set ACHIP(height) [expr 1.0*abs($y1-$y0)]
        set ACHIP(area)   [expr 1.0*abs($x1-$x0) * abs($y1-$y0)]

	# -- move Blocks to the filters section
        if [info exists ACHIP(Blocks)] {
            lappend ACHIP(filters) block "Blocks" $ACHIP(Blocks)
            unset ACHIP(Blocks)
        }

        if { $ACHIP(bb) == "" } {
            result type error error "[$zdm Get chip] bbox is empty"
        } else {
	    # -- ask the GUI to draw the entire chip
            result type overview chip [$zdm Get chip] attrs [array get ACHIP]
        }

        # -- now show the tiles (if any)
        result type tiles tiles [$loader GetTiles [$zdm Get chip]]
    }
    # -- compute and draw a subset of the chip
    protected method chip_draw {} {
        set bb [$zdm Get bbox]
        if { $bb == "" } { return }
	set str [cmd_streamable $bb] 

	# -- request the data set
	set dset [$dsmgr Get [$zdm Get chip] $bb [$zdm Get filters] \
		$OPT(maxobjects) [$zdm Get gx] [$zdm Get gy] $str]

	dset_result type objects bbox $bb dset $dset
    }

    # --- compute and draw a set of objects related to start
    protected method inspect_draw {} {
        set dset [$dsmgr ObjInspect [$zdm Get chip] \
            [$zdm Get start] [$zdm Get mode ]  \
            [$zdm Get bbox]  [$zdm Get filters] ]

	set obj [$zdm Get start] 

        dset_result type inspection info [$zdm Get mode] obj $obj\
            dset $dset bbox [$zdm Get bbox]
    }

    # -- Compute & draw a selection
    # -- if only one object is selected, ask the backend for its extended
    # -- information.
    protected method selection_draw {objs} {
        switch [llength $objs] {
            0 {
                result type selection info "" obj ""
            }

            1 {
                # choose the first object to return
                set sel [lindex $objs 0]
                set info [$loader ObjMessage \
                    [$zdm Get chip]  [$zdm Get cbbox] [$zdm Get filters] $sel]
		# -- collect the object attributes
                result type selection info $info obj $sel
            }

            default {
                result type mselection set $objs
            }
        }
    }

    # -- find objects and draw them on canvas
    # -- if bb is set, objects will be clipped there
    protected method find_draw {{bb ""}} {
        set dset [obj_find_dset show $bb [$zdm Get start]]
        if { $dset == "" } {
            bell
        }
        dset_result type objects bbox $bb dset $dset
    }

    # -- see if this command is worth streaming
    protected method cmd_streamable {bb} {
	if { $OPT(nostreaming) } { return 0 }
	if { ! $stream         } { return 0 }
	# -- take into account if the platform can support streaming
	set ::platform::STREAMING
    }
    # ------------------------------------------------------------- cursor
    # depending on the context mode, find objects under the cursor (bb)
    private method cmd_cursor_objects {bb} {
        switch [$zdm Get mode] {
            all     { 
                # -- global chip mode
                $loader GetCursor [$zdm Get chip] $bb [$zdm Get filters]
            }
            find     { 
		# -- find mode
                obj_find cursor $bb [$zdm Get start]
            }
            default { 
                # -- inspect mode
                $loader ObjInspectCursor [$zdm Get chip] \
                    [$zdm Get start] [$zdm Get mode]  \
                    $bb [$zdm Get filters]
            }
        }
    }

    # -- callback to provide actions when a node has changed
    # -- or when its dependants have changed

    public method eMake {node} {
	# -- get old & new values of dependencies
	$zdm GetDepValues $node OLD VAL
	set changed [$zdm GetChanged $node]
	switch $node {
	    server {
		# -- server has changed
		if ![nconnect $VAL(server)] {
		    $zdm Set! server ""
		}
		$zdm Set! selection ""
	    }
	    overview {
		# -- need to update overview
		if { $VAL(server) == "" } { return }
		chip_overview_draw
	    }
	    selection {
		# -- need to update selection
		selection_draw $VAL(selection)
	    }
	    view {
		# -- need to refresh the view
		switch $VAL(mode) {
		    all     { chip_draw }
		    find    { find_draw $VAL(bbox) }
		    default { inspect_draw }
		}
	    }
	    filters {
		# -- update filters in the GUI
		if { $VAL(filters) == "" } { return }
		result type filters filters $VAL(filters)
	    }
	    default {
		puts ">>> invalid $node (due to $changed)"
	    }
	}
    }
    # -- an ADE event has been received
    public method eADEEvent {event args} {
	switch $event {
	    "<<plot>>" { 
		result type plot data $args
	    }
	    "<<note>>" {
		result type note data $args
	    }
	    "<<uiadapter-post>>" {
		# -- adapter events
		if [catch {eval Event <<adapter-event>> $args} err] {
		    global errorInfo
		    puts ----------
		    puts $errorInfo
		}
	    }
	    default {
		puts "**** ADE event $event $args"
	    }
	}
    }

    # -------------------------------------------------- Common Initialization
}
