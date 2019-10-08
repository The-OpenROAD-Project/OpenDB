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
#       Class: ZCMD
# Description: ZUI command handling
#      Author: Manolis M. Tsangaris
#
itcl::class ZCMD {
    inherit LObject LBinder
    # Event <<result>> value ;# value to be visualized
    # -------------------------------------------------- Public Variables --
    public variable zc ""                    ;# zcontext
    public variable loader ""                ;# loader
    public variable rpc ""                   ;# remote connection
    public variable opts "" { array set OPT $opts }
    public variable zhist ""                 ;# session history
    public variable zhelp ""                 ;# helper

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
    # all those commands affect the context (object zc)
    # subsequently, this object will handle context updates

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
            set s [$zc Get server]
        }
        $zc Set server $s
        hpush "connect $s"
    }

    # -- collect all available sessions to connect to
    public method zsessions {} {
        result type sessionselect sessions [session_collect]
    }

    # -- force cmd to refresh the GUI
    public method refresh {} {
        $zc Update
    }

    # -- set filters
    public method filters {f} {
        $zc Set filters $f
        hpush "filtering"
    }

    # --- select a set of objects in the GUI
    public method select {objs} {
        $zc Set selection $objs
        hpush "select objects"
    }

    # -- show a group of objects
    public method group {obj} {
        set group [$loader GetGroup [$zc Get chip] $obj [$zc Get bbox]]
        result type group group $group obj $obj
    }

    # -- show the entire chip
    public method fullchip {} {
        $zc Set start /
        $zc Set mode all
        if { [$zc Get bbox] == "" } {
            $zc Set bbox [$zc Get obbox]
        }
        hpush "entire chip"
    }

    # -- perform an inspection query -- show choices if necessary
    public method inspect {obj {info ""}} {
        if { $info == "" } {
            set choices [$loader ObjInspectChoices [$zc Get chip] $obj]
            result type choose chip [$zc Get chip] obj $obj choices $choices
            return
        }
        $zc Set start $obj 
        $zc Set mode $info
        $zc Set bbox   ""
        hpush "inspect $obj $info"
    }

    # -- change the bounding box to view
    public method view_set {x0 y0 x1 y1 reason} {
        if { $x0 > $x1 } { swap x0 x1 } 
        if { $y0 > $y1 } { swap y0 y1 } 
        $zc Set bbox "$x0 $y0 $x1 $y1"
        hpush "$reason"
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
        $zc Set! obbox $bb
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
        foreach a [array names P] {
            $zc Set $a $P($a)
        }
    }

    # ------------------------------------------------------------- find 
    public method find_select {args} {
        set objs [obj_find select [$zc Get bbox] $args]
        if { $objs == "" } { 
            return
        }
        set res ""
        foreach obj $objs {
            array set P $obj
            lappend res $P(id) ; unset P
        }
        select $res
    }

    # find and select objects
    public method find_show {args} {
        $zc Set start $args
        $zc Set mode find
        $zc Set bbox ""
        hpush "find & show"
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
        $zc Set crawler $cr
        hpush "crawler set to $cr"
    }
    # -------------------------------------------------- Protected Variables 
    protected variable oldfilters "" ;# previously used filters
    protected variable ACHIP         ;# chip attributes
    protected variable OPT           ;# options
    protected variable gfactor  4    ;# pixel lines to sample
    protected variable GX  ""        ;#
    protected variable GY  ""        ;#
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        set OPT(maxobjects) 1 ;# i.e. all objects
        eval configure $args
        $zc Bind <<update>> $this eContextUpdate
        context_init
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -- connect to a server and check if it is ok to use it
    protected method conn_check {server} {
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
        if { $S(user) == "$user"  } {
            return 1
        }
        return 0
    }
    # -- connect to the first possible port you can
    protected method conn_select {host from to} {
        foreach {s sinfo} [$rpc Scan $host $from $to zroute::whoami] {
            array set S $sinfo
            if [info exists S(error)] { continue }
            if [conn_check $s] {
                return $s
            }
            unset S
        }
        return ""
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
        #puts history:push:$reason
        set state [$zc Get]
        lappend state reason $reason
        $zhist Push $state
    }

    # -------------------------------------------------------- center object
    protected method cmd_center {obj} {
        set info [$loader ObjInfo [$zc Get chip] $obj]
        if { $info == "" } {
            result type error error "$obj: does not exist"
            return
        }
        array set A $info
        # get the bbox of the object
        foreach {cx0 cy0 cx1 cy1} $A(bb) { break }

        # compute centroid
        set cxx [expr ($cx0+$cx1)/2.0] ; set cyy [expr ($cy0+$cy1)/2.0]

        # use the current bbox, or the chip bbox if none
        set bb [$zc Get bbox] 
        if { $bb == "" } { set bb $ACHIP(bb) } 
        # any bbox after all?
        if { $bb == "" } { return }

        # compute the bbox size
        foreach {x0 y0 x1 y1} $bb { break }
        set cx [expr abs($x1 - $x0)/2.0] ; set cy [expr abs($y1 - $y0)/2.0]

        # change bbox to center around the selected object
        bbox [expr $cxx-$cx] [expr $cyy-$cy] [expr $cxx+$cx] [expr $cyy+$cy]
    }
    # --------------------------------------------------------------- touch
    protected method cmd_touch {bb mode} {
        # make a note of the cursor bbox (no need to cause an update)
        $zc Set! cbbox $bb

        # find all objects touched by the mouse
        set objects [cmd_cursor_objects $bb]

        # -- tell the gui about the hit list
        result type hitlist hitlist $objects

        if { $objects == "" } { return }

        # -- select the highest layer object
        set layer -1
        array set S {}
        foreach obj $objects {
            array set P $obj
            if {$P(layer) > $layer} {
                array set S [array get P]
            }
            unset P
        }

        # return the right value, based on the type of operation
        switch $mode {
            select   {
                $zc Set selection $S(id)
                hpush "select $S(id)"
            }
            inspect {
                $zc Set selection $S(id)
                hpush "select to inspect $S(id)"
                inspect $S(id)
            }
        }
    }

    # -- set the real grid size, based on gfactor
    protected method egrid_set {} {
        if { ($GX == "") || ($gfactor == "") } { 
            $zc Set gx ""
            $zc Set gy ""
        } else {
            $zc Set gx [expr $GX * $gfactor]
            $zc Set gy [expr $GY * $gfactor]
        }
    }

    # --------------------------------------------------------------- FIND
    # -- find an object
    protected method obj_find {name bb arg} {
        if { ( $arg == "" ) } {
            error [find_usage $name]
        }
        foreach {a b c} $arg { break }
        switch [llength $arg] {
            1 { return [list [$loader ObjInfo [$zc Get chip] $a]] }
        }
        if { ($a == "") || ($b == "") || ($c == "") } {
            error [find_usage $name]
        }
        $loader Find $a $b $c $bb [$zc Get filters]
    }
    protected method find_usage {name} {
        set list {net name "net by name" net id "net by id"}
        set msg "Usage:\n"
        append msg "  gui-id (to $name any gui object)\n"
        foreach {a b c} $list {
            append msg "  $name $a $b string (to $name $c)\n"
        }
        set msg
    }
    # --------------------------------------------------------- CALLBACKS

    # The Gui context has the following modes, designated by mode
    #   all        : when the global chip is shown (start is /)
    #   find       : the result of select/show commands (start is the query)
    #   default    : the result of inspect commands (start is the inspected obj)

    # ------------------------------------------------------------- REDRAW
    # --- The context has changed. Update the UI accordingly
    public method eContextUpdate {last current force} {
        array set LAST $last
        array set VAL  $current
        puts UPDATE:[array names LAST]

        # -- has the server changed?
        if [info exists LAST(server)] {
            if { [nconnect $VAL(server)] } {
                chip_overview_draw
            }
            # this way, we will force redraw
            incr force

            # we will eliminate selection -- just in case
            $zc Set! selection ""
        }
        # -- has the crawler changed?
        if { [context_changed LAST VAL {crawler}] != "" } {
            chip_overview_draw
            # -- force redraw & clear selection
            incr force
            $zc Set! selection ""
        }

        # -- do we have to redraw?
        set alist "chip start mode bbox filters gx gy"
        set ch [context_changed LAST VAL $alist]
        if { $force || ($ch != "") } {
            switch $VAL(mode) {
                all     { chip_draw }
                find    { find_draw [$zc Get bbox] }
                default { inspect_draw }
            }
        }
        # force the filters anyway
        result type filters filters $VAL(filters)

        # force the selection anyway
        selection_draw $VAL(selection)
    }

    # ----------------------------------------------------------------------
    # Have any of the attributes changed?
    protected method context_changed {ll vv attrs} {
        upvar $ll LAST
        upvar $vv VAL
        foreach v $attrs {
            if [info exists LAST($v)] { 
                if { $LAST($v) != $VAL($v) } { return $v }
            }
        }
        return ""
    }

    # initialize the context
    protected method context_init {} {
        set dl ""
        lappend dl crawler default    ;# default crawler
        lappend dl chip all           ;# the name of the chip
        lappend dl mode all           ;# navigation mode
        lappend dl start /            ;# start of navigation
        lappend dl bbox ""            ;# bbox being used
        lappend dl filters ""         ;# filters used 

        lappend dl selection ""       ;# list of selected objects

        lappend dl cbbox ""           ;# last used cursor bbox
        lappend dl gfactor 1          ;# grid factor (for aggragation)
        lappend dl obbox ""           ;# the real bbox (based on drawn objs)
        lappend dl gx "" gy ""        ;# pixel grid size

        foreach {a v} $dl { $zc Set! $a $v }
    }
    # ------------------------------------------------------------- Rendering
    # try to connect to a remote server
    protected method nconnect {server} {
        $loader Connected 0

        if [catch {$rpc Connect $server} err] {
            $loader Connected 0
            result type warning info "connect: $server: $err"
            return 0
        }
        $loader Connected 1
        return 1
    }

    # -- draw the chip overview 
    protected method chip_overview_draw {} {
        array set ACHIP [$loader GetAttr [$zc Get chip]]
        set ACHIP(bb) [string trim $ACHIP(bb)]
        set ACHIP(server) [$zc Get server]
	set x0 ""
        foreach {x0 y0 x1 y1} $ACHIP(bb) { break }
	if { $x0 == "" } { return }
        set ACHIP(width)  [expr 1.0*abs($x1-$x0)]
        set ACHIP(height) [expr 1.0*abs($y1-$y0)]
        set ACHIP(area)   [expr 1.0*abs($x1-$x0) * abs($y1-$y0)]

        # just update the bbox
        ###$zc Set! bbox $ACHIP(bb)

        if { $ACHIP(bb) == "" } {
            result type error error "[$zc Get chip] bbox is empty"
        } else {
            result type chip chip [$zc Get chip] attrs [array get ACHIP]
        }

        # now show the tiles
        result type tiles tiles [$loader GetTiles [$zc Get chip]]
    }
    # compute and draw a subset of the chip
    protected method chip_draw {} {
        set bb [$zc Get bbox]
        if { $bb == "" } { return }
        set objects \
            [$loader Get [$zc Get chip] $bb [$zc Get filters] \
            $OPT(maxobjects) [$zc Get gx] [$zc Get gy]]
        result type objects objects $objects bbox [$zc Get bbox]
    }

    # compute and draw a set of objects related to start
    protected method inspect_draw {} {
        set inspection [$loader ObjInspect [$zc Get chip] \
            [$zc Get start] [$zc Get mode ]  \
            [$zc Get bbox]  [$zc Get filters] ]

        result type inspection info [$zc Get mode] obj [$zc Get start] \
            inspection $inspection bbox [$zc Get bbox]
    }

    # compute and draw a selection
    # if only one object is selected, ask the backend for its extended
    # information.
    protected method selection_draw {objs} {
        switch [llength $objs] {
            0 {
                result type selection info "" obj ""
            }

            1 {
                # choose the first object to return
                set sel [lindex $objs 0]
                set info [$loader ObjMessage \
                    [$zc Get chip]  [$zc Get cbbox] [$zc Get filters] $sel]
                result type selection info $info obj $sel
            }

            default {
                result type mselection set $objs
            }
        }
    }

    # find objects and draw them on canvas
    # * if bb is set objects will be clipped there
    protected method find_draw {{bb ""}} {
        set res [obj_find show $bb [$zc Get start]]
        if { $res == "" } {
            bell
        }
        result type objects bbox $bb objects $res
    }
    # ------------------------------------------------------------- cursor
    # depending on the context mode, find objects under the cursor (bb)
    private method cmd_cursor_objects {bb} {
        switch [$zc Get mode] {
            all     { 
                # global chip
                $loader GetCursor [$zc Get chip] $bb [$zc Get filters]
            }
            find     { 
                obj_find cursor $bb [$zc Get start]
            }
            default { 
                # inspect
                $loader ObjInspect [$zc Get chip] \
                    [$zc Get start] [$zc Get mode ]  \
                    $bb [$zc Get filters]
            }
        }
    }
    # -------------------------------------------------- Common Initialization
}
