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
#       Class: ZLoader
# Description: Load a stream
#      Author: Manolis M. Tsangaris
#
# ZLoader is responsible for loading a design into the zui canvas
#
itcl::class ZLoader {
    inherit LBinder
    # Event <<info>>
    # Event <<progress>>
    # -------------------------------------------------- Public Variables --
    public variable cv ""
    public variable debug 0
    public variable nlines 1000
    public variable rpc    ""
    # -------------------------------------------------- Public Methods ----
    public method Load {chip} {
    }
    # -- get all objects
    public method Get {chip bbox filters samples gx gy} {
        chip_get $chip $bbox $filters $samples $gx $gy
    }
    public method GetCursor {chip bbox filters} {
        chip_get_cursor $chip $bbox $filters
    }
    public method GetAttr {chip} {
        CRAWL chip_attr_get $chip
    }
    public method GetTiles {chip} {
        get_tiles $chip
    }
    public method Connected {status} {
        set connected $status
    }
    public method ObjInspectChoices {chip obj} {
        CRAWL inspect -chip $chip $obj
    }
    # inspect an object
    public method ObjInspect {chip obj info bbox filters} {
        # flatten filters
        array set F "layer {} hierarchy {} type {}"
        set filters [filter_flatten $filters] ; array set F $filters

        CRAWL inspect -chip $chip -info $info \
                -bbox $bbox -layer $F(layer) -type $F(type) $obj
    }

    # return object information
    public method ObjInfo {chip obj} {
        lindex [CRAWL inspect -chip $chip -info self $obj] 0
    }
    # return a message describing an object
    public method ObjMessage {chip bbox filters obj} {
        # flatten filters
        array set F "layer {} hierarchy {} type {}"
        set filters [filter_flatten $filters] ; array set F $filters

        lindex [CRAWL inspect -chip $chip \
            -bbox $bbox -layer $F(layer) -type $F(type) \
            -info msg/msg $obj ] 0
    }

    public method Find {a b c bbox filters} {
        array set F "layer {} hierarchy {} type {}"
        set filters [filter_flatten $filters] ; array set F $filters

        CRAWL inspect \
            -bbox $bbox -layer $F(layer) -type $F(type) \
            -s $a/$b $c
        # CRAWL inspect -info inst/links $c
    }

    public method MSH {args} {
        eval RPC ::msh::$args
    }
    #
    # given a group return its members
    #
    public method GetGroup {chip obj {bbox ""}} {
        CRAWL inspect -chip $chip -info conn $obj
    }
    # return all available crawlers
    # a list of crawler {crawler attrs}
    public method Crawlers {} {
        set out ""
        lappend out default [list title Default tm "" cr ""]
        lappend out test    [list title Test    tm "" cr ""]
        foreach cr [RPC itcl::find objects -class Dcr] {
            set crn [namespace tail $cr]
            lappend out $cr [list title $crn tm "" cr ""]
        }
        set out
    }
    # -- set the default crawler to use subsequently
    public method CrawlerSet {cr} {
        switch @$cr {
            @default - @test { set CR "" }
            default {
                set CR $cr
            }   
        }
    }
    public method Whoami {} {
        RPC whoami
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable connected 0
    protected variable CR ""             ;# default crawler to use
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        LBridge debug "$rpc configure -debug"
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    # get a number of objects under bbox ...
    private method chip_get {chip bbox filters samples gx gy} {
        set filters [filter_flatten $filters]
        set bbox [fixbbox $bbox]
        array set F "layer {} hierarchy {} type {}"
        array set F $filters
        CRAWL chip_get -chip $chip -bbox $bbox \
            -layer $F(layer) -hierarchy $F(hierarchy) -type $F(type) \
            -gx $gx -gy $gy -maxobjects $samples
    }

    # get a part of the chip under the cursor
    private method chip_get_cursor {chip bbox filters} {
        set filters [filter_flatten $filters]
        set bbox [fixbbox $bbox]
        array set F "layer {} hierarchy {} type {}"
        array set F $filters
        CRAWL chip_get -chip $chip -bbox $bbox \
                -layer $F(layer) -hierarchy $F(hierarchy) -type $F(type) \
                -noclip -q
    }
    private method get_tiles {chip} {
        CRAWL tiles_get $chip
    }
    protected method RPC {args} {
        set res ""
        set now [clock seconds]
        if { $connected } {
            $rpc Run puts $args
            set res [eval $rpc Run $args]
        } else {
            notice rpc "[lindex $args 0]: not connected"
        }
        set dt [expr [clock seconds ] - $now]
        if { $dt } {
            notice rpc "request: [lindex $args 0] ...: $dt seconds"
        }
        set res
    }
    # -- run a command, possibly using a crawler
    # -- will go way when the default crawler is implemented
    protected method CRAWL {cmd args} {
        # -- if no crawler, use the old style
        if { $CR == "" } { return [eval RPC $cmd $args] }

        set av  $args
        switch $cmd {
            inspect {
                # -- in case of inspect, we need to change the last free
                # -- argument to -obj
                set av [concat [lrange $av 0 end-1] -obj [lindex $av end]]
            }
        }
        eval RPC $CR $cmd $av
    }
    proc fixbbox {bbox} {
        if { $bbox == "" } { return "" }
        foreach {x0 y0 x1 y1} $bbox {break} 
        if { $x0 > $x1 } { set x $x1 ; set x1 $x0 ; set x0 $x } 
        if { $y0 > $y1 } { set y $y1 ; set y1 $y0 ; set y0 $y } 
        list $x0 $y0 $x1 $y1
    }
    # -------------------------------------------------------- filter handling
    # -- flatten a filter expression 
    protected method filter_flatten {filter} {
        if { $filter == "" } { return "" } 
        array set F {}
        foreach {class vclass} $filter {
            array set A {}
            foreach {attr val} $vclass {
                if { $val == 1 } {
                    set A($attr) 1
                }
            }
            set F($class) [array names A]
            unset A
        }
        array get F
    }
    # -------------------------------------------------- Common Initialization
}
