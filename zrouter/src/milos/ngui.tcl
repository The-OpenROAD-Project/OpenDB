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

## ngui.tcl (GUI modules)
## System module for Milos
## Author: Mattias Hembruch

lappend auto_path /space2/athena/release/Standard-2008b/x86_64-linux/lib/blt2.4

package require Itcl
package require BWidget
package require BLT
package require mtlib 1.0

package provide ade::milos::ngui 0.1

package require ade::milos::dobj 0.1

################################################################################
# This is a GUI-binder class. It inherits LBinder so other classes can bind
# to events, and it will dispatch them. 
#
# This class will form the main communication link between milos and the GUI.
# Usage scenario:
# NGUI_BINDER receives events from the main thread. NGUI binds to those events
# during startup. This avoids function calls into NGUI for every data update
################################################################################

itcl::class NGUI_BINDER {
    inherit LBinder

    public method send { event } {

    }
}


################################################################################
#   This is the main GUI class - call it to set things up
#
#   Call order: set x [NGUI #auto . binder]
#               send Event <<CSS>> to set CSS config
#               send Event <<SHOW>> to show GUI
#
#   This class will build itself upon the first "show" 
#   so that sub-elements will know they've been "built"
#   when they are invoked..
#
#   This class inherits LBINDER so that its sub-components can "bind" to it 
#   to receive Events. The "Bind" method from LBinder is overridden to bind
#   to the equivalent Event from the NGUI_BINDER
#
################################################################################

itcl::class NGUI {

    inherit LBinder

    protected variable _binder "";
    public method constructor { mytop binder} {
        if { [info command table] ne "table" } {
            namespace eval :: {namespace import ::blt::*}
        }  
        set top $mytop
        set _binder $binder
        intBind <<BUILD>> $this build
        intBind <<SHOW>> $this show
        intBind <<RESIZE_UPDATE>> $this resizeUpdate
    }

    protected method intBind {event args} {
        set cmd "$_binder Bind $event $args"
        #puts "intBIND $event $args, $cmd"
        eval $cmd
    }

        
    public method Bind {event args} {
        ## if this is the first time this event is being bound, pass the Bind
        ## request on to the GUI BINDER
        if { ![info exists Bind($event) ] } {
            $_binder Bind $event $this Event $event
        }
        ## call the original Bind implementation
        set cmd "chain $event $args"
        eval $cmd
    }

    protected variable top
    protected variable tnb
    protected variable upTab
    protected variable upNBT
    protected variable statTab

    public method test { } {
        puts "TEST!!"
    }

    public method build { } {
        set tnb [tabnotebook ${top}tn]

        set rc [addTab UpdateTab "Update"]
        set upNBT [lindex $rc 0]
        set upTab [lindex $rc 1]

        #set statTab [StatTab #auto $tnb]
    }
    public method show { } {
        pack $tnb
    }

    protected method addTab { tclass text } {

        set nTab [$tnb insert end -text $text] 

        #puts "Test: $tnb"
        set cmd "$tclass #auto $tnb $this"
        #puts "cmd: $cmd"
        set nBT [eval $cmd]
        $nBT build
        return [list $nBT $nTab]
    }

    public method resizeUpdate { } {
        $tnb tab configure $upTab -window [$upNBT top] -windowwidth [$upNBT wwidth]
    }
}

############################################################################
#  NBTab - generic (base) Notebook Tab class
#        - derive all Tabs from this class
#
#  use ELEM array to store elements added to the page, rather than many
#  different variable names
#  
############################################################################

itcl::class NBTab {
    protected variable ELEM
    protected variable _parent

    constructor { top parent} {
        $this _init $top
        set _parent $parent
    }

    ## shortcut for getting events from the parent
    protected method Bind {event args} {
        ## need the "eval" to keep multi-position arguments.. join/concat 
        ## won't work here..
        set cmd "$_parent Bind $event $args"
        eval $cmd
    }

    protected method elem_add { name element } {
        set ELEM($name) $element
    }

    protected method elem_pack { args } {
        foreach arg $args {
            pack $ELEM($arg)
        }
    }

    protected method elem_get { name } {
        return $ELEM($name)
    }

    protected method _init { top } {
        #puts "CARGS: $top"
        array set ELEM {}
        elem_add top [frame $top.myf]
        elem_pack top
    }

    public method top { } {
        return [$this elem_get top]
    }

    public method wwidth { } {
        return [$this _wWidth]
    }

    public method build { } {
        $this _build
    }

    ## INTERNAL methods

    protected method _build { } {
        puts "Must override method _build in derived class!"
    }

    protected method _wWidth { } {
        set ww [winfo reqwidth [elem_get top]]
        puts "wWidth: $ww"
        return [[elem_get top] cget -width]

    }

}

############################################################################
# UpdateTab - class for displaying node stats during multimode operation
#           - shows various useful stats
#           - uses "CSS" format to determine layout/data of 
#             individual multimode nodes
#
############################################################################


itcl::class UpdateTab {
    inherit NBTab

    protected variable CSS 
    protected variable have_css 0
    protected variable mywidth 1
    protected variable nsl
    protected variable scount 
    protected variable sup_count 0

    protected variable MYNS ::UpdateTab
    protected common SL_FMT "%-.3g"

    protected variable debug_fp

    constructor { top parent} {
        NBTab::constructor $top $parent
    } {
        Bind <<CSS>> $this set_css
        Bind <<SET_FIELD>> $this setField
        Bind <<TIMING_UPDATE_START>> $this setField main tim_merges
        Bind <<TIMING_UPDATE_END>> $this setField main tim_merges
        Bind <<NODE_SLACK>> $this nslack
        Bind <<SLACK_UPDATE>> $this slack_update
        array set ${MYNS}::FIELDS {}

        set nsl [ [NODE_SLACK #auto] this]
        array set scount {}

        set debug_fp [open gui_nb.log w]
    }

    protected method debug { args } {
        #puts "$args"
        puts $debug_fp $args
        flush $debug_fp
    }


    protected method sf { var value } {
        set ${MYNS}::FIELDS($var) $value
    }
    protected method num { value } {
        return [format $SL_FMT $value]
    }

    public method slack_update { } {
        debug "RECEIVED: SLACK UPDATE: $sup_count!"
        incr sup_count
        sf main.slack_updates $sup_count
    }

    public method nslack { node args } {
        debug "NSLACK: $node, $args"
        $nsl reset
        set cmd "$nsl configure [join $args]"
        ::eval $cmd

        set cur "current"
        if { $scount($node) == 0 } {
            set cur "initial"
            incr scount($node)
        }

        sf ${node}.${cur}.worst.setup [num [$nsl cget -worst_max_slack]]
        sf ${node}.${cur}.total.setup [num [$nsl cget -total_max_slack]]
        sf ${node}.${cur}.worst.hold [num [$nsl cget -worst_min_slack]]
        sf ${node}.${cur}.total.hold [num [$nsl cget -total_min_slack]]
        set pt [$nsl cget -plugin_time]
        if { $pt < 0 } {
            sf ${node}.plugin.time "N/A"
        } else {
            sf ${node}.plugin.time "[num $pt] s"
        }
    }


    public method setEntry { text } {
        $ELEM(entry) delete 0 end
        $ELEM(entry) insert $text
    }

    public method setField { node field value } {
        set value [join $value]
        debug "RECEIVED: SetField $node $field $value"
        #puts "[array get ${MYNS}::FIELDS]"
        set nd [string tolower $node]
        set fd [string tolower $field]
        set ${MYNS}::FIELDS(${nd}.${fd}) $value
        update
    }

    public method set_css { css } {
        array set CSS $css
        set have_css 1
        #puts "GOT MY CSS: $css!"
        set mywidth [build_nodes [elem_get top]]
    }

    protected method _wWidth { } {
        debug "WW: $mywidth"
        return $mywidth
    }

    protected method _build { } {
        #if { $have_css == 0 } {
        #    error "Must set CSS before building Update Tab!"
        #}
        #set ww [build_nodes [elem_get top]]
        #set ELEM(entry) [entry [elem_get top].e -text "abc"]
        #elem_pack entry
    }

    protected method build_nodes { top } {
        set wwidth 150
        foreach {node name css} $CSS(nodes) {
            set node [string tolower $node]
            set scount($node) 0
            debug "CSS node: $node, name: $name, css: $css"

            if { $node ne "main" } {
                set tf [frame $top.$node]
            } else {
                set tf [frame $top.$node -relief groove -border 6 -pady 10]
            }
            #Label $tf.title -text $name -fg white -bg black
            Label $tf.title -text $name -fg black -bg gray60
            set cmd "table $tf $tf.title 0,0 -cspan 2 -fill x "

            set ewidth 20
            if { $node eq "main" } {
                set ewidth 60
            }
            set idx 1
            foreach {field label } $css {
                if { $field eq "-" } {
                    if { $label eq "-" } {
                        Label $tf.field$idx
                    } else {
                        Label $tf.field$idx -text $label -fg black -bg gray60
                    }
                    append cmd "$idx,0 $tf.field$idx -cspan 2 -fill x "
                } else {
                    set ${MYNS}::FIELDS(${node}.$field) ""
                    set ${MYNS}::field ""
                    Label $tf.label$idx -text $label
                    Entry $tf.field$idx -textvariable ${MYNS}::FIELDS(${node}.$field) -width $ewidth
                    append cmd "$idx,0 $tf.label$idx -anchor e $idx,1 $tf.field$idx -anchor w "
                }
                incr idx
            }
            debug "NC: $cmd"
            ::eval $cmd
            incr wwidth 180
        }

        set cmd "table $top "
        set maxcol 1
        set maxrow 0
        set lastcol 0
        foreach {node row col} $CSS(layout) {
            set node [string tolower $node]
            append cmd "$row,$col $top.$node -pady 20 "
            if { $col > $maxcol } {
                set maxcol $col
            }
            if { $node eq "main" } {
                ## on a line by ourselves
                if { $row > $maxrow } {
                    append cmd "-cspan $maxcol "
                } else {
                    append cmd "-cspan [expr $maxcol - ($lastcol)]"

                }
            }
            if { $row > $maxrow } {
                set maxrow $row
            }
            set lastcol $col
        }
        debug "TC: $cmd"
        ::eval $cmd

        return $wwidth
    }
}
