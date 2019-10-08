package provide ade::ui 2.0

#
# The main ZUI window class
#

#       Class: LZUI
# Description: Athena ZUI 
#      Author: Manolis M. Tsangaris
#
itcl::class LZUI {
    inherit LApplication LBinder
    # Event <<command>> cmd ?arg? ?arg?       ;# gui command
    # Event <<user-command>> cmd args         ;# user command
    # Event cmd args ;# for every ui command
    # -------------------------------------------------- Public Variables --
    public variable rpc   ""              ;# pass this to mshviewer
    public variable opts "" { array set OPT $opts }
    public variable bhelp ""              ;# balloon help
    # -------------------------------------------------- Public Methods ----
    # -- vizualize a value produced by CMD
    public method Viz {arg} {
        visualize $arg
    }

    # post an event based on the desired work ZCMD has to do
    public method CMD {cmd args} {
        eval Event <<command>> $cmd $args
    }
    # same as CMD, but after the idle loop
    public method BCMD {cmd args} {
        after idle $this Event <<command>> $cmd $args
    }

    # -- show a set of objects
    public method ShowObjects {objects {bb ""}} {
        show_objects $objects $bb
    }
    # -- show a stream of objects
    public method ShowObjectStream {cmd {bb ""}} {
        show_objects "-" $bb $cmd
    }

    public method SelectionClear {} {
        $cv SelectionClear
    }

    public method SelectionC2 {} {
        puts "zui SelectionClear"
        set SELECTED ""
        $cv SelectionClear
        CMD select { } ""
        InfoShow "--- no selection  ---"
    }

    # select an object -- return true if it was selected
    public method Select {obj attrs} {

        if ![$cv Select $obj $attrs] {
            return 0
        }
        clipboard clear
        clipboard append $obj
        lappend SELECTED $obj
        return 1
    }
    # -- Show a Group of objects
    public method Group {obj group} {
        $cv Group $obj $group
    }
    # clear canvas
    public method Clear {} {
        set SELECTED ""
        $cv Clear
        $MTB Clear
    }
    public method Quit {} {
        Event <<quit>>
    }
    public method InfoShow {msg} {
        cli_show $msg
    }
    # -- monitor multiprocessing
    public method Monitor { } {
        $cpanel Monitor
    }

    # -- Show object information -- return the object id 
    public method ObjInfo {obj attrs} {
        foreach a {hierarchy type id layer bb} {
            set S($a) "$a?"
        }
        if [catch {array set S $attrs} rc] {
            warning "Object Info: '$attrs'"
            warning "$rc"
        }
        if { [info exists S(msg)] } {
            InfoShow $S(msg)
        } else {
            InfoShow "--- no information on $obj ---"
            #InfoShow [format "%s : %s @ %s (%s)" \
                $S(hierarchy).$S(type) $S(id) $S(layer) $S(bb)]
        }
    }

    public method Zoom {mode} {
	$cv Zoom $mode
    }

    public method ZoomToSelection {} {
	CMD view_set_selection user
    }

    # -- autofit all objects to canvas
    public method AutoFit {} {
	set bb [$cv FullBBox]
	if { $bb == "" } { bell ; return }
	eval eSelectedRegion $bb [list "autofit"]
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- CallBacks
    # an object has been touched
    public method eTouch {button x y} {
        set bb [$cv CursorRegion $x $y]

        switch -glob $button {
            1      { CMD touch $bb select }
            2      { CMD touch $bb group }
            3      { CMD touch $bb inspect }
            default  { bell }
        }
    }

    # -- a user has selected a filter
    public method eSelectFilter {_filters} {
        set filters $_filters
        $cv Visible $filters
        CMD filters $filters
    }
    # -- a user has asked to set all filter fields
    public method eGlobalSetFilter {_filters} {
        set filters $_filters
        $cv Visible $filters
        CMD filters $filters
    }

    # -- the user has selected a region on bird's eye view window
    public method eSelectedRegion {x0 y0 x1 y1 reason} {
        bbox_norm x0 y0 x1 y1
        CMD view_set $x0 $y0 $x1 $y1 "$reason (overview)"
        CMD grid_set [$cv PixelGrid 1 1 $x0 $y0 $x1 $y1]
    }

    # -- the user has selected a region on the main canvas
    # -- cmd will actually update the bbox of the bview
    public method eSelectedRegionCV {x0 y0 x1 y1 reason} {
        bbox_norm x0 y0 x1 y1
        CMD view_set $x0 $y0 $x1 $y1 "$reason (main)"
        CMD grid_set [$cv PixelGrid 1 1 $x0 $y0 $x1 $y1]
    }

    # -- the user is selecting a region on the main canvas
    # -- update birds'eye view
    public method eSelectingRegionCV {x0 y0 x1 y1 reason} {
        $cpanel SelectionSet $x0 $y0 $x1 $y1
    }

    # -- since a gesture starts, we turn off selections
    # -- disable balloon help
    public method eGestureStart {gesture} {
	# $cv Select "" ""
	eNewSettings [list balloon 0]
    }
    # -- gesture ends -- enable balloon help (if it was set)
    public method eGestureEnd {gesture} {
	eNewSettings [list balloon $CONF(balloon)]
    }

    # go back to history
    public method eBack {} {
        CMD history back
    }
    public method eNext {} {
        CMD history next
    }

    # -- view may have changed
    public method eUpdateView {} {
        $cv ViewMarks [$MTB Get]
    }

    public method eInspect {w} {
        set obj [string trim [$w cget -text]]
        if { $obj != "" } {
            CMD inspect $obj
        }
    }

    # -- task information was requested by the msh viewer
    public method eTaskInfo {arg} {
        array set S $arg
        array set ST "DISP 0 QUEUE 0 DEF 0"
        array set ST $S(stats)
        if { $S(host) != "" } {
            set h "([lindex [split $S(host) .] 0])"
        } else {
            set h ""
        }
        set st [format {Task %s: %s [Q: %5s, Run: %5s] %s} \
            $S(tid) $S(vstate) \
            [time_afmt $ST(QUEUE)] [time_afmt $ST(DISP)] $h]
        
        $scope Show "$st\n$S(output)\n--"
        InfoShow "$st"
    }
    # ------------------------------------------------------------- Selection
    # show selection in the info box
    public method mClipboardShow {} {
        $scope Show "CLIPBOARD:\n[array names CLIPBOARD]"
        InfoShow "[array size CLIPBOARD] object(s) in the clipboard"
        $cv SelectionClear

	# -- skip the selection for now
	return
        foreach obj [array names CLIPBOARD] {
            $cv Select $obj [CMD objdef $obj]
        }
    }

    # clear selection
    public method mClipboardClear {} {
        catch {unset CLIPBOARD}
        InfoShow "[array size CLIPBOARD] object(s) in selection"
    }

    # add an object to the selection
    public method mClipboardAdd {obj} {
        set CLIPBOARD($obj) ""
    }

    # remove an object from selection
    public method mClipboardRemove {obj} {
        catch {unset CLIPBOARD($obj)}
    }
    # -------------------------------------------------- 
    # -- toolbar
    public method vMakeToolbar {w} {
        toolbutton_add $w.conn -text "Connect"  -command "$this CMD zsessions"
        toolbutton_add $w.refr -text "Refresh"  -command "$this CMD refresh"
        toolbutton_add $w.snap -text "Snap"     -command "$this mSnap"
        toolbutton_add $w.over -text "AllChip" -command "$this mAllChip"
        toolbutton_add $w.chip -text "ChipMode"  -command "$this mChipMode"

        toolbutton_add_right $w.next -text "Next"  -command "$this eNext"
        toolbutton_add_right $w.back -text "Back"  -command "$this eBack"


        #set TB [LLinearToolBar $w.tb]
        #$TB winconfig -border 2 -relief sunken
        #$TB Add select  Select click1
        #$TB Add group   Group  click1
        #$TB Set select 1
        #pack $w.tb -side right


        # - the mark toolbar
        set MTB [LLinearToolBarMS $w.mtb]
        $MTB Bind <<update>> $this eUpdateView
        pack $w.mtb -side right

	# -- settings toolbar
	set STB [LLinearToolBarMS $w.stb]
	$STB Add  balloon  balloon "" 2
        $STB Bind <<update>> $this eSTBSettingsApply
        pack $w.stb -side right -fill y -padx 3 -pady 3

        # add inspect button
        label $w.sel -text "" -width 10 -border 1 -relief sunken -bg gray70
        set winspect $w.sel
        bind $w.sel <1> "$this eInspect $w.sel"
        bind $w.sel <3> "$this eInspect $w.sel"
        pack $w.sel -side left -fill y -pady 2

	# -- apply default settings now. At some point, we should
	# -- read them from ~/.zuirc
	eNewSettings [list stream 1 balloon 0]

        set w
    }

    public method vMakeButtons {w} {
        button_add_right $w.quit -text "Quit"     -command "$this Quit"
	set lights [LLights $w.lights]
	$lights Add cache Cache "hit green miss yellow"
	pack $lights -side left
	set w
    }
    # -------------------------------------------------- Protected Variables 
    protected variable cv
    protected variable cmd ""
    protected variable console ""     ;# zui command console
    protected variable winspect ""    ;# object inspection window
    protected variable wplotter ""    ;# plotter
    protected variable hist ""
    protected variable cpanel
    protected variable filters ""     ; # currently selected filters
    protected variable SELECTED ""    ;# Holds the last selected object
    protected variable CLIPBOARD      ;# Holds all objects in CLIPBOARD
    protected variable TB             ;# toolbar
    protected variable MTB            ;# Mark toolbar
    protected variable STB            ;# Settings toolbar
    protected variable OPT            ;# options
    protected variable CONF           ;# zui configuration variables
    protected variable DUMPFILE ""    ;# prefix to dump screen
    protected variable DUMPFILENO 1   ;# sequence number
    protected variable CANCEL 0       ;# true, if user wants to cancel
    protected variable scope          ;# scope
    protected variable lights ""      ;# lights
    protected variable dslist ""      ;# data set list
    protected variable album ""       ;# album maker
    protected variable ADE_ATTR       ;# server attributes

    protected variable CURSOR         ;# cursor information
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        wm geometry $This 900x700
	configure -title "ADE/UI"
        css_setup
        pack [zui_setup $This.a] -side top -expand 1 -fill both
	set CURSOR(X) 0
	set CURSOR(Y) 0
	set CURSOR(info) ""
    }
    destructor {
        $cv delete
        #$TB delete
        $console delete
        $hist delete
        $scope delete
	$lights delete
	$dslist delete
	# -- add a final page in the album, if any
	if { [$album cget -pages] > 0 } {
	    $album AddPage {
		title "The End" 
		body "page intentionally left blank :-)"
	    }
	    $album Render
	}
	$album delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method zui_setup {w} {
	frame $w
        set cpanel [CPanel $w.cpanel -rpc $rpc]

        $cpanel Bind <<selected-region>> $this eSelectedRegion
        $cpanel Bind <<select-filter>>   $this eSelectFilter
        $cpanel Bind <<global-set-filter>>   $this eGlobalSetFilter
        $cpanel Bind <<cursor>>          $this eCursorPos

	# -- bookmarks
        $cpanel Bind <<bookmark-add>>      $this mBookMarkAdd
        $cpanel Bind <<bookmark-goto>>     $this mBookMarkGoto
        $cpanel Bind <<bookmark-delete>>   $this mBookMarkDelete

        # -- task info event from msh viewer
        $cpanel Bind <<task-info>>     $this eTaskInfo

        set cv [CCanvas $w.cv -invy 1 -deltax 5 -deltay 5 \
	    -keepwbb 1 -opts [array get OPT]]

        $cv Bind <<key>>              $this eCanvasKey
        $cv Bind <<touch>>            $this eTouch
        $cv Bind <<selected-region>>  $this eSelectedRegionCV
        $cv Bind <<selecting-region>> $this eSelectingRegionCV
        $cv Bind <<message>>          $this eCanvasMessage
        $cv Bind <<gesture-start>>    $this eGestureStart
        $cv Bind <<gesture-end>>      $this eGestureEnd
        $cv Bind <<cursor>>           $this eCursorPos

        set hist     [LConsoleHistory $this.hist]
        set console  [console_make $w.wcon $hist]

	# -- arrange them around
	grid $w.cpanel $w.cv
	grid ^         $console
	grid configure $w.cpanel -sticky nsew -rowspan 2
	grid configure $w.cv     -sticky ewns
	grid configure $console  -sticky ew
	grid columnconfigure $w 0 -weight 2
	grid columnconfigure $w 1 -weight 2
	grid rowconfig       $w 0 -weight 1

        $cv configure -colorize layer
        $cv configure -colors {
            layer:0 mediumseagreen
            layer:1 lawngreen
            layer:2 magenta
            layer:3 lightskyblue
            layer:4 orange
            layer:5 indianred
            layer:6 orangered
            layer:7 mediumslateblue
            layer:8 magenta3
            layer:9 khaki3
        }
        $cpanel configure -colors [$cv cget -colors]

        set scope [ZScope $w.scope]
        $scope configure -geometry -0+0
        $scope Bind     <<select>> $this eScopeSelect
        $scope Bind     <<cmd>>    $this eScopeCommand

	set dslist [DSList $w.dslist]
	$dslist configure -geometry -0+300

	set album [LAlbum $this.album -dir ade-album]
	# -- configure Album session variables
	$album configure -attributes {
	    title "ADE: Session History Album (Athena Design Systems)"
	}

	# -- setup the baloon help
	$bhelp Notify [$cv CanvasWindow] "$this eBalloonHelp"
	$cv Bind <<enter>> $bhelp Enter [$cv CanvasWindow]
	$cv Bind <<leave>> $bhelp Leave [$cv CanvasWindow]

	# -- create the plotter
	if { [catch {ZPlotter $this.pl} rc] } {
	    warning "Failed to instantiate Plotter: plotting is disabled"
	    warning "Error: $rc"
	    set wplotter $rc
	    catch {$wplotter delete}
	    set wplotter ""
	} else {
	    set wplotter $rc
	    $wplotter configure -geometry 500x300+0-300
	    $wplotter Hide
	}
	set w
    }
    public method vMakeMenus {w} {
        set m [menu_add $w.f -text File]
            $m add command -label "Open"     -command "$this mOpen"
            $m add command -label "Connect"  -command "$this CMD zsessions"
            $m add command -label "Snap"     -command "$this mSnap"
            $m add command -label "Select Album"  -command "$this mDumpSel"
            $m add command -label "Settings" -command "$this mChangeSettings"
            $m add command -label "Quit"     -command "$this Quit"

        set m [menu_add $w.e -text Clipboard]
            $m add command -label "Clear"    -command "$this mClipboardClear"
            $m add command -label "Show"     -command "$this mClipboardShow"

        set m [menu_add $w.v -text View]
            $m add command -label "BookMark" -command "$this mBookMarkAdd"
            $m add command -label "AllChip"  -command "$this mAllChip"
            $m add command -label "ChipMode" -command "$this mChipMode"
            $m add command -label "Crawlers ... "  -command "$this CMD crawlers"
            $m add separator
            $m add command -label "Zoom in"     -command "$this Zoom in"
            $m add command -label "Zoom out"    -command "$this Zoom out"
            $m add command -label "Zoom to Sel" -command "$this ZoomToSelection"
            $m add command -label "Auto Fit"   -command "$this AutoFit"
            $m add command -label "Show Console" \
                -command "$this Event <<show-console>>"
            $m add command -label "Refresh"    -command "$this CMD refresh"
            $m add command -label "clear "     -command "$this Clear"
            $m add separator
            $m add command -label "Album"      -command "$this mAlbumView"

        set m [menu_add $w.hist -text History]
            $m add command -label "Next"  -command "$this CMD history next"
            $m add command -label "Back"  -command "$this CMD history back"
            $m add command -label "Clear" -command "$this CMD history clear"

        set m [menu_add_right $w.h -text Help]
            $m add command -label "General"    -command "$this CMD help /"
            $m add command -label "ADE/UI News" \
                -command "$this CMD help /news"
            $m add separator
            $m add command -label "UI Commands" \
                -command "$this CMD help /gcmds"
            $m add command -label "UI User Commands" \
                -command "$this CMD help /ucmds"
            $m add command -label "ADE/ZDB" \
                -command "$this CMD help /zdb"
            $m add separator
            $m add command -label "Send Error Report" \
                -command "$this mSendErrorReport"

    }
    # --------------------------------------------------------- 
    # -- set the window title
    protected method connection_show {aa} {
        upvar $aa A
	global ADE_UI_INFO
        configure -title "ADE: $A(port)@$A(host) ($ADE_UI_INFO(version))"
    }

    # -- Draw a number of objects
    protected method show_objects {objects bb {stream_cmd ""}} {
        $cv Clear
        set CANCEL 0
        set auto_mark $OPT(automark)
        $cv configure -bbox $bb
	# -- is this a stream?
	if { $objects == "-" } {
	    set lcmd [list [namespace current]::$cv AddObjects set1 -]
	    set rcmd [concat $stream_cmd [list $lcmd]]
	    if [catch {eval $rcmd} rc] {
		warning "streaming failed: $rc"
		return
	    }
	} else {
	    $cv AddObjects set1 $objects
	}
        $cv Visible $filters
	# -- no bbox was set?
	if { $bb == "" } {
	    # -- obtain the bbox of the loaded data sets
	    set bb [$cv FullBBox]
	    foreach {x0 y0 x1 y1} $bb { break }
	    if [info exists x0] {
		# -- normalize bbox
		bbox_norm x0 y0 x1 y1
		$cv configure -bbox [list $x0 $y0 $x1 $y1]
	    }
	}

        # -- add tristate buttons for all marks present in the dataset
        array set M [$cv Marks set1]
        $MTB Clear
        foreach mark [lsort [array names M]] {
            $MTB Add $mark  $M($mark) "" 2
            $MTB Set! $mark 1
        }
        # update the backend on the real bbox
        CMD obbox_set [$cv GetBBox]
    }
    # -- Show a dataset
    protected method show_dset {dset bb} {
        $cv Clear
        set CANCEL 0
        set auto_mark $OPT(automark)
        $cv configure -bbox $bb
	# -- add data set to the canvas
	$cv AddDSet $dset
        $cv Visible $filters

	# -- no bbox was set?
	if { $bb == "" } {
	    # -- obtain the bbox of the loaded data sets
	    set bb [$cv FullBBox]
	    foreach {x0 y0 x1 y1} $bb { break }
	    if [info exists x0] {
		# -- normalize bbox
		bbox_norm x0 y0 x1 y1
		$cv configure -bbox [list $x0 $y0 $x1 $y1]
	    }
	}

        # -- add tristate buttons for all marks present in the dataset
	foreach m [$dset Marks] {
	   set M($m) $m
	}
        $MTB Clear
        foreach mark [lsort [array names M]] {
            $MTB Add $mark  $M($mark) "" 2
            $MTB Set! $mark 1
        }
        # update the backend on the real bbox
        CMD obbox_set [$cv GetBBox]
    }
    # -- time a command: N is the var set to count
    protected method timeit {nn name BODY} {
        upvar $nn N
        set N 0
        set now [clock seconds]
        uplevel $BODY
        set dt [expr [clock seconds] - $now + 1]
        set msg "$N $name in $dt seconds [expr $N / $dt] $name/sec"
        if { $CANCEL } {
            append msg " (** interrupted **)"
        }
        InfoShow $msg
    }
    # --------------------------------------------------------- event handlers
    # send a user command out to be executed 
    public method eConsoleCommand {cmd} {
        $console Show cmd $cmd
        eval Event <<user-command>> $cmd
    }
    # -- open a file
    public method mOpen {} {
        set ftypes {
            {{All Files} *           }
        }
        set f [file_open_dialog -filetypes $ftypes -title "Select a chip" ]
        if { $f != "" } {
            CMD load $f
        }
    }
    # -- show the portion of the chip on the current bbox & filters
    public method mChipMode {} {
        # $cpanel SelectAll
        CMD chipmode
    }
    # -- show the entire chip (instances only)
    public method mAllChip {} {
        CMD fullchip [list type [list inst/bbox 1]]
    }
    # -- snap the current view, save it into an album
    public method mSnap {{d ""}} {
	# -- do we need the opening page?
	if { [$album cget -pages] == 0 } {
	    set pg [list title "Session Info"]
	    set txt "<PRE>\n"
	    foreach a [lsort [array names ADE_ATTR]] {
		append txt [format "%10s %s\n" $a $ADE_ATTR($a)]
	    }
	    append txt "</PRE>"
	    lappend pg body $txt
	    $album AddPage $pg
	}
        Raise
        set im [$cv Dump]
	if { $DUMPFILE != "" } {
	    set f [format "%s%02d.jpg" $DUMPFILE $DUMPFILENO]
	    $im write $f -format jpeg,quality=80
	    set img [$album AddTclImage $im]
	} else {
	    set img [$album AddTclImage $im]
	    set f $img
	}
	# -- timestamp
	set dt [clock format [clock seconds] -format "%Y/%m/%d %H:%M"]
	$album AddPage [list image [file tail $img] \
	    caption "$dt - step # $DUMPFILENO" title $dt]

        image delete $im
        incr DUMPFILENO
        InfoShow "screen dumped in $f ([$album cget -dir])"
    }
    # select a dump file
    public method mDumpSel {} {
        set ftypes {
            {{All Dirs} */           }
        }
	if [info exists ADE_ATTR(cwd)] { 
	    set dir $ADE_ATTR(cwd)
	} else {
	    set dir [pwd]
	}
        set f [file_save_dialog -filetypes $ftypes -initialdir $dir\
            -title "Select Album Directory" -initialfile ade-album]
        if { $f == "" } { return }
        set DUMPFILENO 1
	$album configure -dir $f
    }
    # --- show the chip overview --- 
    private method overview_show {chip attrs} {
        array set A $attrs

        $cpanel configure -bbox $A(bb)

	# -- define the filters
        $cpanel ShowFilters $A(filters)

        $cv ePanelBBox $A(bb)
    }
    # -- compute the list of layers
    private method layers {filters} {
        foreach {tag name descr} $filters {
            if { $tag == "layer" } {
                set res ""
                foreach {l _ _} $descr {
                    lappend res $l
                }
                return $res
            }
        }
        return ""
    }
    # -- user adds a bookmark
    public method mBookMarkAdd {x0 y0 x1 y1} {
	CMD addbookmark $x0 $y0 $x1 $y1
    }

    # -- user deletes a bookmark
    public method mBookMarkDelete {bid} {
	CMD deletebookmark $bid
    }

    # -- user jumps to a bookmark
    public method mBookMarkGoto {bid} {
	CMD gotobookmark $bid
    }

    # -- view the album
    public method mAlbumView {} {
	$album Render
	$album BrowserView
    }
    # -------------------------------------------------------------------
    # -- normalize bbox so the selection always fit the display canvas
    protected method bbox_norm {xx0 yy0 xx1 yy1} {
        upvar $xx0 x0 ; upvar $xx1 x1 ; upvar $yy0 y0 ; upvar $yy1 y1
        # centroid
        set mx [expr ($x0+$x1)/2.0]    ; set my [expr ($y0+$y1)/2.0]
        # half width/height
        set dx [expr abs($x1-$x0)/2.0] ; set dy [expr abs($y1-$y0)/2.0]
	if { $dx == 0 } { set dx 1 }
	if { $dy == 0 } { set dy 1 }
        # -- current scale factors (sx,sy would map dx,dy to wi/hi)
        set wi [winfo width $cv]       ; set hi [winfo height $cv]
        set sx [expr $wi/$dx]          ; set sy [expr $hi/$dy]
        # make x/y scaling uniform , using the largest scale
        if { $sx < $sy } {
            set sy $sx
        } else {
            set sx $sy
        }
        # now adjust dx,dy
        set dx [expr $wi / $sx] ; set dy [expr $hi / $sy]

        # update the coordinates
        set x0 [expr $mx - $dx] ; set x1 [expr $mx + $dx]
        set y0 [expr $my - $dy] ; set y1 [expr $my + $dy]
    }
    # ----------------------------------------------------------- cli
    private method console_make {w hist} {
        LConsole $w -hist $hist -width 40 -height 2 -out 0
        $w Bind <<cmd>> $this eConsoleCommand
        return $w
    }
    protected method cli_show {msg} {
        $console ReShow inf $msg
    }
    # ----------------------------------------------------------- Touch mode
    # ----------------------------------------------------------- Object Inspect
    protected method get_insp_choices {chip obj choices} {
        array set C $choices
if 0 {
        if ![info exists C(sel/add)] {
            lappend choices - "Clipboard"
            lappend choices "clip/add" "Add to"
        }
        if ![info exists C(sel/del)] {
            lappend choices "clip/del" "Remove from"
        }
}

        # -- let the user choose
        catch { $This.menu delete }
        set choice  [user_choice "*** $obj *** " $choices]
        if { $choice != "" } { 
            CMD inspect $obj $choice
        }
    }
    # a particular object needs to be examined
    # obj contains the object information as an array
    protected method obj_examine {obj button} {
        if { $obj == "" } {
            InfoShow ""
            $cv Select "" ""
            return
        }
        array set S $obj
        InfoShow "$S(id)"
        # -- is this is a menu request?
        if { $button == 3 } {
            return
        }

	BCMD select $obj 
    }
    protected method stime_afmt {time now} {
        time_afmt [expr $now - $time]
    }

    protected method zroute_context {} {
        global env
        global argv0 ; global argv
        set res ""
        append res "ZROUTE $argv0  \n"
        append res "  ARGS $argv \n"
        append res "   CWD [exec pwd] \n"
        append res "  HOST [info hostname] \n"
        append res "    FS [exec df . | tail -1] \n"
        append res "  PATH $env(PATH) \n"
        set res
    }
    # -- select a crawler 
    protected method crawler_choose {crlrs} {
	# -- is there only one choice?
	if { [llength $crlrs] == 2 } {
	    CMD crawler_set [lindex $crlrs 0]
	    return
	}
        set choices ""
        foreach {tag def} $crlrs {
            array set C $def
            lappend choices $tag $C(title)
            unset C
        }
        set ch [user_choice "*** Available Crawlers ***" $choices]
        if { $ch == "" } { return }
        CMD crawler_set $ch
    }
    protected method user_choice {title choices} {
        # -- let the user choose
	set m $This.menu
        catch { $m delete }
        LMenu $m -title "$title" -style list -width 0
        set choice  [$m Select $choices]
	# -- somehow this one may fail
        catch {$m delete}
        return $choice
    }
    # -- show a value (dataset or objects)
    # -- return the number of objects in the data set
    protected method zui_show {aa bbox} {
	upvar $aa A
	if [info exists A(dset)] {
	    show_dset $A(dset) $A(bbox)
	    $A(dset) Size
	} else {
	    # show the set of objects
	    ShowObjects $A(objects) $A(bbox)
	    llength $A(objects)
	}
    }
    # --------------------------------------------------------- CallBacks
    public method eCanvasMessage {msg} {
	$scope ShowStats $msg\n
    }
    # -- process a key pressed on canvas
    public method eCanvasKey {key} {
	switch $key {
	    delete { BCMD select {} }
	    escape { $this eCancel }
	    s      { $this mSnap }
	    default {
	    }
	}
    }
    # -- send an error report as a rich multimedia email
    public method mSendErrorReport {} {
        set f /tmp/zui.[pid].jpg
        Raise ; set im [$cv Dump]
        $im write $f ; image delete $im

        set em [LShortMessage .sm \
            -to $OPT(esupport) -subject "ADE/UI Error Report" \
            -server $OPT(eserver) \
             -body "describe problem here" ]
        $em Attach screen image "Screen" $f
        set params [zroute_context]
        $em Attach params text params $params
        $em Wait ; $em delete
        file delete $f
    }
    # user hit cancel
    public method eCancel {} {
        bell
        incr CANCEL
    }
    # -- the user has selected an object in ZScope
    public method eScopeSelect {id} {
        SelectionClear
        Select $id [CMD objdef $id]
    }
    # -- the scope command is sent to the console for simulation
    public method eScopeCommand {args} {
	eConsoleCommand $args
    }
    # ---------------------------------------------------------
    # -- SETTINGS ---------------------------------------------

    # -- apply toolbar settings -- called by the toolbar
    public method eSTBSettingsApply {} {
	eNewSettings [$STB Get]
    }

    # -- allow the user to change settings -- called from the menu
    public method mChangeSettings {} {
	set st [ZUISettingsDialog $This.zs]
	LPlaceCenter [$st Window]
	$st Set [array get CONF]
	if [$st Wait] {
	    if [$st Validate] {
		eNewSettings [$st Get]
	    }
	}
	$st delete
    }
    # -- apply new settings - update the CONF variable to hold it
    public method eNewSettings {new} {
	foreach {a v} $new {
	    if ![info exists CONF($a)] { set CONF($a) "" }
	    if { $CONF($a) == $v } { continue }
	    set CONF($a) $v

	    switch $a {
		stream {
		    CMD stream $v
		}
		balloon {
		    $bhelp configure -active $v
		    # -- update STB any way
		    $STB Set! $a $v
		}
	    }
	}
    }
    # ---------------------------------------------------------
    public method eLoaderStats {msg} {
	$scope ShowStats $msg\n
    }

    # -- a data set has been modified. Tell the overview window about it
    public method eDataSetUpdated {ds} {
	$cpanel ShowDataSet $ds
	$dslist ShowDataSet $ds
    }
    # -- dataset cache hit/miss
    public method eDataSetCache {op attrs} {
	$lights Set cache $op
    }

    # -- report Cursor Position
    public method eCursorPos {x y X Y} {
	set CURSOR(info) [format "%-10.0f %-10.0f" $X $Y]
	set CURSOR(X) $X ; set CURSOR(Y) $Y
	set CURSOR(x) $x ; set CURSOR(y) $y
	$cpanel ShowStatus $CURSOR(info)
	$bhelp Motion [$cv CanvasWindow]
    }

    # -- process balloon help events
    # -- but only if we are not dragging
    public method eBalloonHelp {x y} {
	# -- we use CURSOR(x),y because there seems to
	# -- be a disagreement between cursor and bhelp coords
        set bb [$cv CursorRegion $CURSOR(x) $CURSOR(y)]
	CMD peek $bb
	# -- do not display anything, since the CMD event  will do
	return ""
    }
    # --------------------------------------------------------- VISUALIZER
    # --- vizualize a ZCMD structured value
    #
    protected method visualize {res} {
        array set A "type unknown"
        array set A $res
        switch $A(type) {
	    connection {
		array set ADE_ATTR $A(attrs)
		connection_show ADE_ATTR
		# -- the album should be stored on the home directory of
		# -- the ade server
		$album configure -dir $ADE_ATTR(cwd)/ade-album
	    }
            overview {
                overview_show $A(chip) $A(attrs)
            }
	    view { 
		# -- set the view after normalizing it to the aspect ratio
		eval $this eSelectedRegion $A(bbox) [list $A(reason)]
	    }
            objects {
                # update the bbox
                if { $A(bbox) != "" } { 
                    eval $cpanel SelectionSet $A(bbox)
                }
		zui_show A $A(bbox)
            }
            group     {
                SelectionClear
                Select  $A(obj) [CMD objdef $A(obj)]
                Group   $A(obj) $A(group)
                InfoShow "$A(obj): [llength $A(group)] connections"
            }
            selection { 
		SelectionClear
		# -- did I manage to select it?
		set objdef [CMD objdef $A(obj)]
                if { ![Select $A(obj) $objdef] } {
		    InfoShow "--- no selection  ---"
		    return 0
		}
                $scope ShowSelection [list $objdef]

		# -- update the inspect window
                $winspect configure -text $A(obj)

		# -- show object information
                ObjInfo  $A(obj) $A(info)
            }
            hitlist {
                $scope ShowSelection $A(hitlist)
            }
            filters {
                $cpanel Set! $A(filters)
            }
            mselection { 
                # multiple selection
		SelectionClear
		InfoShow "--- no selection  ---"
		set sel ""
                foreach obj $A(set) {
                    Select $obj [CMD objdef $obj]
		    lappend sel [CMD objdef $obj]
                }
                InfoShow "[llength $A(set)] objects selected"
                $scope ShowSelection $sel
            }
            choose    {
                SelectionClear
                Select  $A(obj)  [CMD objdef $A(obj)]
                $winspect configure -text $A(obj)
                get_insp_choices $A(chip) $A(obj) $A(choices)
            }
            crawler_choices {
                crawler_choose $A(crawlers)
            }
            tiles {
                $cpanel LoadBV $A(tiles)
                InfoShow "[llength $A(tiles)] tiles"
            }
            touch {
                obj_examine $A(obj) $A(button)
            }
            sessionselect {
                set choices ""
                set now [clock seconds]
                foreach s $A(sessions) {
                    array set S $s
		    set h [lindex [split $S(host) .] 0]
                    lappend choices $S(port)@$S(host) \
                        "$h/$S(pid) ([stime_afmt $S(stim) $now] ago)"
                    unset S
                }
                if {[llength $choices] == 2 } {
                    CMD connect [lindex $choices 0]
                    return
                }
                set choice  [user_choice "Session Selection" $choices]
                if { $choice != "" } { 
                    CMD connect $choice
                }
            }


            inspection {
                switch -glob -- $A(info) {
                    */group {
                        # show a group of objects
                        SelectionClear
                        set nobj [zui_show A  $A(bbox)]
                        Select  $A(obj) [CMD objdef $A(obj)]
                        InfoShow "$A(obj): $nobj objects"
                    }
                    *conn - */links {
                        # show a connected group of objects
                        SelectionClear
                        set nobj [zui_show A  $A(bbox)]
                        Select  $A(obj) [CMD objdef $A(obj)]
                        Group   $A(obj) $A(inspection)
                        InfoShow "$A(obj): $nobj  connections"
                    }
                    clip/add {
                        mClipboardAdd $A(obj)
                        mClipboardShow
                    }
                    clip/rem {
                        mClipboardRemove $A(obj)
                        mClipboardShow
                    }
                    msg/msg {
                        $scope Show "$A(info): $res"
                        ObjInfo $A(obj) $A(obj)
                        SelectionClear
                        Select $A(id) [CMD objdef $A(id)]
                    }
                    self {
                        array set S [lindex $A(inspection) 0]
                        set S(id) $A(obj)
                        set s ""
                        foreach a "id type name layer bb" {
			    if ![info exists S($a)] { continue }
                            append s [format "%10s: %s\n" $a $S($a)]
                        }
                        $scope Show "$s"
                        ObjInfo $A(obj) [array get S]
                    }
                    default {
                        $scope Show "$A(info): $res"
                    }
                }
            }
	    bookmark {
		$cpanel BookMarkAdd $A(bid) $A(bbox)
	    }
            warning {
                bell
                $scope Show $A(info)
            }
            error {
                bell
                $scope Show $A(error)
            }
	    peek {
		# -- handle mouse over ballon
		array set I $A(info) ; set msg ""
		if [info exists I(msg)] {
		    set msg "\n$I(msg)"
		}
		$bhelp Message "$CURSOR(info)$msg"
	    }
	    plot {
		set tag  [lindex $A(data) 1]
		set data [lindex $A(data) 0]
		if { $plotter != "" } { 
		    $wplotter UnHide
		    $wplotter Plot $data $tag
		}
	    }
	    note {
		set tag  [lindex $A(data) 1]
		set data [lindex $A(data) 0]
		$scope ShowNote $data $tag
	    }
            default {
                $scope Show $res
            }
        }
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LLights
# Description: Simple LED like Light indicators
#      Author: Manolis M. Tsangaris
#
itcl::class LLights {
    inherit LFrame
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    public method Add {tag name cmap} {
	ll_add $tag $name $cmap
    }
    public method Set {tag value} {
	ll_set $tag $value
    }
    public method Get {tag} {
	set VALUE($tag)
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable WIN
    protected variable CMAP
    protected variable VALUE
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method ll_add {tag name cmap} {
	set WIN($tag) $This.light[array size WIN]
	set VALUE($tag) ""
	frame $WIN($tag) -width 10 -height 10 -border 1 -relief sunken -bg black
	pack $WIN($tag) -side left
	set CMAP($tag) $cmap
    }
    protected method ll_set {tag value} {
	set VALUE($tag) $value
	array set CM $CMAP($tag)
	if [info exists CM($value)] {
	    set color $CM($value)
	} else {
	    set color yellow
	}
	$WIN($tag) configure -bg $color
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}
