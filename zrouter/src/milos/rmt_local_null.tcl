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

## rmt.tcl
### Remote module/proxy used by Milos
### Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package require ade::milos::dobj 0.1

package provide ade::milos::rmt  0.1

## RMT LOCAL is a handle for the LOCAL RMT object - its main function is plugin
## handling - everything else is pretty much a pass-through

itcl::class RMT_LOCAL_null {

    inherit VOB;

    protected variable node_slack_object;
    protected variable sobj;
    protected variable corner;
    protected variable eco_string "";

    protected variable db;
    protected variable tmg;
    protected variable block;

    protected variable ext;
    protected variable buff_spef_cnt 0;
    protected variable buff_spef_orig_name "";
    protected variable buff_spef_new_name "";

    protected variable FILE_DEBUG 0
    protected variable P_DEBUG 0


    public method init_rmt { } {
        ## nothing to do
        set db [get_tm DB]
        set ext [get_tm Ext]
        set tmg [get_tm Tmg]
        $db get -top -object= ::mmblock
        set block $::mmblock
        set node_slack_object [[NODE_SLACK #auto] this];
    }


    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        if { $name eq "" } {
            set r [RMT_LOCAL_null #auto]
        } else {
            set r [RMT_LOCAL_null $name]
        }
        return $r
    }
    public method addwait { } {
        ## no waiting - commands always return
    }
    public method gwait { {msg ""} } {
    }
    public method mwait { } {
    }
    public method status { } {
        return 0
    }

    public method rc { } {
        return ""
    }

    public method set_state { state val } {
        VOB::set_state $state $val
    }

    public method set_sobj { obj } {
        #puts "RMT: SET SOBJ, $this, $obj"
        set sobj $obj
        set corner [find_obj corner_list corner [$sobj cget -corner]]
        if { $corner == "" } {
            error "Cannot find corner for sobj!"
        }
    }

    public method get_sobj { } {
        return $sobj
    }

    public method get_corner { } {
        return $corner
    }

    ## generate the milos top-level pass-through methods automatically
    ## MILOS commands
    foreach mname {ext mme pdb opt rpt rte sta sys tmg } {
        set cmd "public method $mname { args } { }"
        eval $cmd
    }


    ## IMPORTANT commands that actually do something!
    public method update_timing { args } {
        $tmg reset_timing_cache -reset_only
        ###NO_OP - local timing up-to-date by calling update_obj
    }
    public method get_db { name } {
        $tmg set_timing_engine -external
        ##NO_OP - always up-to-date on main
    }

    public method apply_node_definitions { {rmt ""} } {
        #puts "Define corner for remote $rmt: $corner [$corner get_cmd]!"
        if { $rmt ne "" } {
            call_node $rmt "[state thresh_init_cmd]"
            call_node $rmt "[$corner get_cmd]"

            call_node $rmt "tmg check_lib"
            call_node $rmt "tmg select_corner -corner [$corner cget -corner]"

            call_node $rmt "tmg read_sdc -sdc_file [$sobj cget -sdc_file]"
        }

        call_node $rmt "mme create_scenario -name [$sobj cget -name] -priority   [$sobj cget -priority] -corner [$corner cget -corner] -sdc_file [$sobj cget -sdc_file]"

    }


    ## NO_OP commands - these have no meaning for the "local" rmt object
    public method send_activeterms { str_string } {
        ##NO_OP - always up-to-date on main
    }
    public method start_db { } {
        ##NO_OP - always up-to-date on main
    }
    public method publish_timing { } {
        ##NO_OP - always have timing
    }
    public method unpublish_timing { } {
        ##NO_OP - always have timing
    }

    ## do nothing for the "local" object
    public method reset_unpublish { } {
    }
    public method add_unpublish { name } {
    }
    public method do_unpublish { } {
    }

    public method get_parasitics { name } {
        ##NO_OP - always up-to-date on main
    }
    public method publish { object } {
        ##NO_OP - done locally (explicitly) for main
    }
    public method unpublish { object } {
        ##NO_OP - done locally (explicitly) for main
    }
    public method subscribe { object } {
        ##NO_OP - done locally (explicitly) for main
    }

    public method wait { object } {
        ##NO_OP - done locally (explicitly) for main
    }

    public method lists_to_plugin { } {
    }

    public method exit_plugin { } {
    }
    ## END OF NO_OP commands

    ## Simple pass-through commands (may or may not be needed)
    public method set_update_method { method } {
        $tmg set_update_method -method activeterms
    }

    ## End of Simple commands


    ## Plugin-related (possibly) commands
    public method init_plugin { } {
        ##NO_OP - no plugin for milos mode
    }

    protected variable plugins_local { }
    protected variable plugins_remote { }
    protected variable plugins_ignore { TE }
    protected variable plugins_invalid { CELTIC }

    public method set_plugin_status { } {
    }

    public method init_plugin_object { type datatype } {
    }

    public method plugin_api { type cmd {args ""} } {
    }

    protected method send_local_plugin_api { type cmd {args ""} } {
    }

    protected method send_remote_plugin_api { type cmd {args ""}} {
    }

    public method update_eco { name } {
        ##NO_OP - ECOs come from main
    }

    public method set_si { val } {
        set_state rc_models_si $val
    }

    public method set_license_type { type } {
        set license_type $type
    }
    ## ensure slacks are up to date - should always be the case, except for
    ## multimode - may need timing update?
    public method update_node_slack { slacks } {
        
    }

    public method get_node_slack { slacks } {
        return $node_slack_object
    }

    public method read_incr_spef { fname } {
    }


    protected method sa { obj} {
        ## for milos, do nothing
    }

    protected method get_eco_string { } {
    }

    protected method copy_data_to_plugin { } {
    }

    protected method get_plugin_obj { obj } {
    }

    protected method write_slack_file { } {
    }
    protected method read_slack_file { } {
    }

    protected method check_files { args } {
    }

    public method pdebug { {debug {0 0}} } {
        set FILE_DEBUG [lindex $debug 0]
        set P_DEBUG [lindex $debug 1]
    }
    
    public method read_sdf { cnt } {
    }
}

