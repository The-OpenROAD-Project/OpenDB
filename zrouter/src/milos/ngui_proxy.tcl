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

## ngui_proxy.tcl (GUI modules)
## System module for Milos
## Author: Mattias Hembruch

package require Itcl
package require mtlib 1.0

package provide ade::milos::ngui_proxy 0.1

itcl::class NGUI_PROXY {

    inherit LBinder VOB

    protected variable thread_id -1;

    public method this { } {
        return $this
    }

    public method start_gui { {thread 1} } {
        if { [get_nodes] == -1 } {
            error "Gui information has not been defined" "" -1
        }

        if { $thread == 1 } {
            package require Thread
            tsv::set milos auto_path $::auto_path
            tsv::set milos thread [thread::id]
            tsv::set milos up_proxy ::apps::milos::up_proxy
            set thread_id [thread::create {

                set ::auto_path [tsv::get milos auto_path]
                #puts "TH: $::auto_path"
                package require ade::milos::ngui;
                set ::gui_binder [NGUI_BINDER #auto]
                set ::gui [NGUI #auto . $::gui_binder]
                thread::wait
            }]
        } else {
            package require ade::milos::ngui
            set ::gui [NGUI #auto .]
            $::gui show
        }
        gui_event <<BUILD>>
        gui_event <<CSS>> [get_css]
        gui_event <<RESIZE_UPDATE>> 
        gui_event <<SHOW>>
        #UPOBJ Bind <<SLACK>> $this gui_event <<SLACK>>
        #get_css
    }

    destructor {
        #puts "NGP: desctructor"
        end_gui
    }
    public method end_gui { } {
        #puts "NGP: end gui"
        if { $thread_id != -1 } {
            thread::release $thread_id
            set thread_id -1
            after 2000
        }
    }

    public method gui_event { event args } {
        debug GUI:E "GUI_EVENT: $event $args"
        if { $thread_id eq -1 } {
            $::gui Event $event [list $args]
        } else {
            thread::send -async $thread_id "\$::gui_binder Event $event [concat $args]"
        }
    }
    public method gui_msg { method {data ""} } {
        if { $thread_id eq -1 } {
            $::gui $method $data
        } else {
            thread::send -async $thread_id "\$::gui $method $data"
        }
    }

    ## first is main
    protected variable main ""
    protected variable rl {}
    protected variable fl {}
    protected method get_nodes { } {
        set main [state local_scenario]
        if { $main eq "" } {
            error "Cannot get nodelist for gui!" "" -1
        }
        set rl [state rmt_scenarios]
        set fl [concat $main $rl]
        #puts "Get_nodes: $main, $rl, $fl"
    }

    protected method get_css { } {
        set cnt 1
        set row 1
        set node_css ""
        set node_layout ""
        set maxcnt 1
        foreach sobj $fl {
            incr cnt
            set name [$sobj cget -name]
            set corner [$sobj cget -corner]
            append node_css [add_css_node $name "$name ($corner)" [rnode_entries $sobj]]
            append node_layout "\n $name $row $cnt"
            if { $cnt == 5 } {
                incr row
                set cnt 1
                set maxcnt 5
            }
        }
        ## add main - special
        append node_css [add_css_node main "Milos Statistics" [main_entries]]
        if { ($maxcnt > 4) && ($cnt == 2) } {
            ## just one item on this row - squeeze in main
            set mcol [expr $maxcnt - ($cnt)]
        } else {
            incr row
            set mcol 1
        }
        if { $cnt > $maxcnt } {
            set maxcnt $cnt
        }

        append node_layout "\n main $row $mcol"

        set gui_css_string [build_gui_css $node_css $node_layout]
        debug GUI:A "gui_css_string: $gui_css_string"
        return $gui_css_string
    }

    protected method add_css_node { node title entries } {
        return "\n$node \"$title\" $entries"
    }

    protected method build_gui_css { nodes layout} {
        set css "nodes { $nodes\n}\n"
        append css "layout { $layout\n}\n"
        append css [css_colors]
        append css "\n"
        append css [css_state_graph]
        append css "\n"
        append css [css_plot]
        append css "\n"
        return $css
    }

    protected method css_colors { } {
        return "colors {
            idle         gray
            init         lightblue
            opt-setup    orange1
            sending-db   orange2
            merge        green1
            send-timing  green

            eco          brown2
            sending-eco  brown2

            extraction   green2
            slack.update darkolivegreen
            merging-remote-timing green

            optimize blue
        }"

    }

    protected method css_state_graph { } {
        return "state-graph {\n     status \n}"
    }

    protected method css_plot { } {
        return "plot {
            sets {
                main.opts  \"optimizations\"
                main.undos \"undo\"
            }
            title  \"Convergence Process\"
            xtitle \"time (sec)\"
            ytitle \"count\" 
        }"
    }

    protected method rnode_entries {sobj } {
        set engine [$sobj cget -engine]

        set css "{
            timing.engine \"Timing Engine\"
        "
        append css "plugin.time \"[timing_name $engine] Time\""
        append css "
            host          \"Host\"
            action        \"Status\"
        - \"Initial Slacks\"
        initial.worst.setup \"Worst Setup\"
        initial.worst.hold \"Worst Hold\"
        initial.total.setup \"Total Setup\"
        initial.total.hold \"Total Hold\"
        - \"Current Slacks\"
        current.worst.setup \"Worst Setup\"
        current.worst.hold \"Worst Hold\"
        current.total.setup \"Total Setup\"
        current.total.hold \"Total Hold\" 
        }"
    }

    protected method main_entries { } {
        return "{
            size  \"Sizings / Undos\"
            buff  \"Bufferings / Undos\"
            tim_merges      \"Timing Updates\"
            action  \"Command\"
            msg     \"Status\"
            }"
    }
}
