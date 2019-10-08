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

package provide ade::ui 1.0

#
#       Class: LZUI
# Description: Athena ZUI 
#      Author: Manolis M. Tsangaris
#
itcl::class LZUI {
    inherit LApplication LBinder
    # Event cmd args ;# for every ui command
    # -------------------------------------------------- Public Variables --
    public variable rpc   ""              ;# pass this to mshviewer
    public variable opts "" { array set OPT $opts }
    # -------------------------------------------------- Public Methods ----
    # vizualize a value produced by CMD
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

    public method ShowObjects {objects {bb ""}} {
        show_objects $objects $bb
    }

    public method SelectionClear {} {
        $cv SelectionClear
    }
    # select an object -- return true if it was selected
    public method Select {obj} {
        if ![$cv Select $obj] {
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

    # Show object information -- return the object id 
    public method ObjInfo {attrs} {
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
            InfoShow [format "%s : %s @ %s (%s)" \
                $S(hierarchy).$S(type) $S(id) $S(layer) $S(bb)]
        }
    }

    # -- does this object exist?
    public method Exists {obj} {
        $cv Exists $obj
    }

    public method Zoom {mode} {
        $cv Zoom $mode
    }

    public method AutoFit {} {
        $cv AutoFit
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- CallBacks
    # an object has been touched
    public method eTouch {button x y} {
        set bb [$cv CursorRegion $x $y]

        # -- get the selection mode
        array set S [$TB Get]

        switch -glob $button/$S(click1) {
            3/*      { CMD touch $bb inspect }
            1/select { CMD touch $bb select }
            1/group  { CMD touch $bb group }
            default  { bell }
        }
    }

    # -- a user has selected a filter
    public method eSelectFilter {_filters} {
        set filters $_filters
        $cv Visible $filters
        CMD filters $filters
    }
    # -- the user has selected a region on bird's eye view window
    public method eSelectRegion {x0 y0 x1 y1 reason} {
        bbox_norm x0 y0 x1 y1
        CMD view_set $x0 $y0 $x1 $y1 "$reason (overview)"
        CMD grid_set [$cv PixelGrid 1 1 $x0 $y0 $x1 $y1]
    }

    # -- the user has selected a region on the main canvas
    # -- cmd will actually update the bbox of the bview
    public method eSelectRegionCV {x0 y0 x1 y1 reason} {
        bbox_norm x0 y0 x1 y1
        CMD view_set $x0 $y0 $x1 $y1 "$reason (main)"
        CMD grid_set [$cv PixelGrid 1 1 $x0 $y0 $x1 $y1]
    }

    # -- the user is selecting a region on the main canvas
    # -- update birds'eye view
    public method eSelectingRegionCV {x0 y0 x1 y1} {
        $cpanel SelectionSet $x0 $y0 $x1 $y1
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
        foreach obj [array names CLIPBOARD] {
            $cv Select $obj
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
    public method vMakeToolbar {w} {
        toolbutton_add $w.quit -text "Quit"     -command "$this Quit"
        toolbutton_add $w.conn -text "Connect"  -command "$this CMD zsessions"
        toolbutton_add $w.refr -text "Refresh"  -command "$this CMD refresh"
        toolbutton_add $w.dump -text "Dump"     -command "$this mDump"

        toolbutton_add_right $w.next -text "Next"  -command "$this eNext"
        toolbutton_add_right $w.back -text "Back"  -command "$this eBack"


        set TB [LLinearToolBar $w.tb]
        $TB winconfig -border 2 -relief sunken
        $TB Add select  Select click1
        $TB Add group   Group  click1
        $TB Set select 1
        pack $w.tb -side right

        # - the mark toolbar
        set MTB [LLinearToolBarMS $w.mtb]
        $MTB Bind <<update>> $this eUpdateView
        pack $w.mtb -side right

        # add inspect button
        label $w.sel -text "" -width 10 -border 1 -relief sunken -bg gray70
        set winspect $w.sel
        bind $w.sel <1> "$this eInspect $w.sel"
        bind $w.sel <3> "$this eInspect $w.sel"
        pack $w.sel -side left -fill y -pady 2

        set w
    }
    # -------------------------------------------------- Protected Variables 
    protected variable cv
    protected variable cmd ""
    protected variable console ""     ;# zui command console
    protected variable winspect ""    ;# object inspection window
    protected variable hist ""
    protected variable cpanel
    protected variable filters ""     ; # currently selected filters
    protected variable SELECTED ""    ;# Holds the last selected object
    protected variable CLIPBOARD      ;# Holds all objects in CLIPBOARD
    protected variable TB             ;# toolbar
    protected variable MTB            ;# Mark toolbar
    protected variable OPT            ;# options
    protected variable DUMPFILE "screen" ;# prefix to dump screen
    protected variable DUMPFILENO 1      ;# sequence number
    protected variable CANCEL 0       ;# true, if user wants to cancel
    protected variable scope          ;# scope
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        wm geometry $This 800x640
        css_setup
        zui_setup $This
    }
    destructor {
        $cv delete
        $TB delete
        $console delete
        $hist delete
        $scope delete
    }
    # -------------------------------------------------- Private Methods ---
    protected method zui_setup {w} {
        set cpanel [CPanel $This.cpanel -rpc $rpc]
        pack $This.cpanel -side left -fill y

        $cpanel Bind <<select-region>> $this eSelectRegion
        $cpanel Bind <<select-filter>> $this eSelectFilter
        # -- task info event from msh viewer
        $cpanel Bind <<task-info>>     $this eTaskInfo

        set cv [CCanvas $w.cv -invy 1 -deltax 5 -deltay 5 -keepwbb 1]

        $cv Bind <<escape>>           $this eCancel
        $cv Bind <<touch>>            $this eTouch
        $cv Bind <<select-region>>    $this eSelectRegionCV
        $cv Bind <<selecting-region>> $this eSelectingRegionCV
        pack $w.cv -side top -expand 1 -fill both

        set hist [LConsoleHistory $this.hist]
        pack [console_make $w.wcon $hist] -side top -fill x


        $cv configure -colorize layer
        $cv configure -colors {
            default gray50
            0 mediumseagreen
            1 lawngreen
            2 magenta
            3 lightskyblue
            4 orange
            5 indianred
            6 orangered
            7 mediumslateblue
            8 magenta3
            15 yellow4
        }
        $cpanel configure -colors [$cv cget -colors]

        set scope [ZScope $This.scope]
        $scope configure -geometry -0+0
        $scope Bind <<select>> $this eScopeSelect
    }
    public method vMakeMenus {w} {
        set m [menu_add $w.f -text File]
            $m add command -label "Open"    -command "$this mOpen"
            $m add command -label "Connect" -command "$this CMD zsessions"
            $m add command -label "Select Dump File"  -command "$this mDumpSel"
            $m add command -label "Screen Dump"       -command "$this mDump"
            $m add command -label "Quit"    -command "$this Quit"
        set m [menu_add $w.e -text Clipboard]
            $m add command -label "Clear"    -command "$this mClipboardClear"
            $m add command -label "Show"     -command "$this mClipboardShow"
        set m [menu_add $w.v -text View]
            $m add command -label "Chip Mode"  -command "$this mEntireChip"
            $m add command -label "Crawlers ... "  -command "$this CMD crawlers"
            $m add separator
            $m add command -label "Zoom in"    -command "$this Zoom in"
            $m add command -label "Zoom out"   -command "$this Zoom out"
            $m add command -label "Auto Fit"   -command "$this AutoFit"
            $m add command -label "Show Console" \
                -command "$this Event <<show-console>>"
            $m add command -label "Refresh"    -command "$this CMD refresh"
            $m add command -label "clear "     -command "$this Clear"

        set m [menu_add $w.hist -text History]
            $m add command -label "Next"  -command "$this CMD history next"
            $m add command -label "Back"  -command "$this CMD history back"
            $m add command -label "Clear" -command "$this CMD history clear"

        set m [menu_add_right $w.h -text Help]
            $m add command -label "About"    -command "$this CMD help /about"
            $m add command -label "Graphical Commands" \
                -command "$this CMD help /gcmds"
            $m add command -label "User Commands" \
                -command "$this CMD help /ucmds"
            $m add command -label "ZDB Commands" \
                -command "$this CMD help /zdb"
            $m add separator
            $m add command -label "Send Error Report" \
                -command "$this mSendErrorReport"

    }
    # --------------------------------------------------------- 
    # -- set the window title
    protected method connection_show {aa} {
        upvar $aa A
        configure -title "zroute: $A(server)"
    }

    # -- Draw a number of objects
    protected method show_objects {objects bb} {
        $cv Clear
        set CANCEL 0
        $cv configure -bbox $bb
        set auto_mark $OPT(automark)
        timeit N "objects" {
            foreach arg $objects {
                array set S $arg
                # add marks if you have to
                if { $auto_mark } {
                    lappend S(mark) $S(type)
                }
                $cv AddObj $S(id) S
                unset S
                incr N
                if { ![expr $N % 3000] } {
                    InfoShow "$N: objects loaded"
                    update idletasks ; update
                    if { $CANCEL } { break }
                }
            }
        }
        $cv Show
        $cv Visible $filters

        # -- add tristate buttons for all marks present in the dataset
        array set M [$cv Marks]
        $MTB Clear
        foreach mark [lsort [array names M]] {
            $MTB Add $mark  $M($mark) "" 3
            $MTB Set! $mark 1
        }
        # set the bbox to the max canvas coords for the panning to work
        $cv SetSelectionBox 0 0 [winfo width $cv] [winfo height $cv]
        # update the backend on the real bbox
        CMD obbox_set [$cv GetBBox]
    }
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
    public method eCmd {cmd} {
        $console Show cmd $cmd
        eval Event <<lcmd>> $cmd
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
    public method mEntireChip {} {
        # $cpanel SelectAll
        CMD fullchip
    }
    public method mDump {{d ""}} {
        set f [format "%s%02d.jpg" $DUMPFILE $DUMPFILENO]
        Raise
        set im [$cv Dump]
        $im write $f -format jpeg,quality=80
        image delete $im
        incr DUMPFILENO
        InfoShow "screen dumped in $f"
    }
    # select a dump file
    public method mDumpSel {} {
        set ftypes {
            {{All Files} *           }
        }
        set f [file_save_dialog -filetypes $ftypes\
            -title "Please Select File (Prefix)"]
        if { $f == "" } { return }
        set DUMPFILENO 1
        set DUMPFILE  [file root $f]
    }
    # --------------- show the chip --- 
    private method chip_show {chip attrs} {
        array set A $attrs
        connection_show A
        $cv configure -layers [layers $A(filters)]
        $cv Clear
        update idletasks
        $cpanel configure -bbox $A(bb)
        $cv configure     -bbox $A(bb)
        $cpanel ShowFilters $A(filters)
    }
    # compute the list of layers
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
    # -------------------------------------------------------------------
    # normalize bbox so the selection always fit the display canvas
    protected method bbox_norm {xx0 yy0 xx1 yy1} {
        upvar $xx0 x0 ; upvar $xx1 x1 ; upvar $yy0 y0 ; upvar $yy1 y1
        # centroid
        set mx [expr ($x0+$x1)/2.0]    ; set my [expr ($y0+$y1)/2.0]
        # half width/height
        set dx [expr abs($x1-$x0)/2.0] ; set dy [expr abs($y1-$y0)/2.0]
        # current scale factors (sx,sy would map dx,dy to wi/hi)
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
    public method eCLIPaste {w} {
        # no need to do much, since we have insterted selected obj to the 
        # window clipboard and ^V is bound to paste
        #$w insert insert "{$SELECTED}"
    }
    private method console_make {w hist} {
        set console  [LConsole $w -hist $hist \
            -width 40 -height 2 -out 0]
        $console Bind <<cmd>> $this eCmd
        return $w
    }
    protected method cli_show {msg} {
        $console ReShow inf $msg
    }
    # ----------------------------------------------------------- Touch mode
    # ----------------------------------------------------------- Object Inspect
    protected method get_insp_choices {chip obj choices} {
        $cv Select $obj
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
            $cv Select ""
            return
        }
        array set S $obj
        InfoShow "$S(id)"
        # -- is this is a menu request?
        if { $button == 3 } {
            return
        }

        array set S [$TB Get]
        switch $S(click1) {
            select {
                BCMD select $obj 
            }
            group {
                BCMD select $obj 
            }
        }
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
    # select a crawler 
    protected method crawler_choose {crlrs} {
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
        catch { $This.menu delete }
        set m [LMenu $This.menu -title "$title"]
        set choice  [$m Select $choices]
        $m delete
        return $choice
    }
    # --------------------------------------------------------- CallBacks
    # send an error report
    public method mSendErrorReport {} {
        set f /tmp/zui.[pid].jpg
        Raise ; set im [$cv Dump]
        $im write $f ; image delete $im

        set em [LShortMessage .sm \
            -to $OPT(esupport) -subject "ZUI Error Report" \
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
        Select $id
    }
    # --------------------------------------------------------- VISUALIZER
    # --- vizualize a ZCMD structured value
    #
    protected method visualize {res} {
        array set A "type unknown"
        array set A $res
        switch $A(type) {
            chip {
                chip_show $A(chip) $A(attrs)
            }
            objects {
                # update the bbox
                if { $A(bbox) != "" } { 
                    eval $cpanel SelectionSet $A(bbox)
                } else {
                    # ???
                }
                # show the set of objects
                ShowObjects $A(objects) $A(bbox)
            }
            group     {
                SelectionClear
                Select  $A(obj)
                Group   $A(obj) $A(group)
                InfoShow "$A(obj): [llength $A(group)] connections"
            }
            selection { 
                SelectionClear
                if { ![Select $A(obj)]} { return 0 }
                $winspect configure -text $A(obj)
                ObjInfo  $A(info)
            }
            hitlist {
                $scope ShowObjects $A(hitlist)
            }
            filters {
                $cpanel Set! $A(filters)
            }
            mselection { 
                # multiple selection
                SelectionClear
                foreach obj $A(set) {
                    Select $obj
                }
                InfoShow "[llength $A(set)] objects selected"
            }
            choose    {
                SelectionClear
                Select  $A(obj)
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
                    lappend choices $S(port)@$S(host) \
                        "$S(host)/$S(pid) ([stime_afmt $S(stim) $now] ago)"
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
                        ShowObjects $A(inspection) $A(bbox)
                        Select  $A(obj)
                        InfoShow "$A(obj): [llength $A(inspection)] objects"
                    }
                    *conn - */links {
                        # show a connected group of objects
                        SelectionClear
                        ShowObjects $A(inspection) $A(bbox)
                        Select  $A(obj)
                        Group   $A(obj) $A(inspection)
                        InfoShow "$A(obj): [llength $A(inspection)] connections"
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
                        ObjInfo $A(obj)
                        SelectionClear
                        Select $A(id)
                    }
                    self {
                        array set S [lindex $A(inspection) 0]
                        set S(id) $A(obj)
                        set s ""
                        foreach a "id type name layer bb" {
                            append s [format "%10s: %s\n" $a $S($a)]
                        }
                        $scope Show "$s"
                        ObjInfo [array get S]
                    }
                    default {
                        $scope Show "$A(info): $res"
                    }
                }
            }
            warning {
                bell
                $scope Show $A(info)
            }
            error {
                bell
                $scope Show $A(error)
            }
            default {
                $scope Show $res
            }
        }
    }
    # -------------------------------------------------- Common Initialization
}
