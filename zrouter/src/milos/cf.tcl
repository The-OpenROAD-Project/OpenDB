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

## cf.tcl (Cheat Functions)
        set socketid 9000
## System module for Milos
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::cf 0.1

itcl::class CF {

    inherit VOB

    ## protected vars
    ## variables with string values

    protected method constructor { parent } { 
        VOB::constructor $parent
        set NAME "CF"
        set SELFNAME "CF$this"
        set_plugin_object TE ""
    }

    private variable scen 0;
    private variable si_cnt 0;

    protected variable node_slack_object [[NODE_SLACK #auto] this];

    public method init_plugin_obj { type obj_type } {
        #set_plugin_object $type [ [PLUGIN_${obj_type} #auto] this]
        puts "Init Plugin obj: $type"
        set_plugin_object $type [ [$obj_type #auto] this]
        puts "Plugin obj result: [get_plugin_object $type]"
    }

    public method plugin_cmd { type cmd {args ""} } {
        #puts "PC: $cmd"
        #puts "PA: $args"

        set po [get_plugin_object $type]
        if { $po eq "" } {
            return
        }
        if { $args eq "" } {
            $po $cmd 
        } else { 
            set pcmd "$po $cmd $args"
            ::eval $pcmd
            #$po $cmd [join "$args"]
        }
    }
    public method get_slacks { args } {
        get_local_slack $node_slack_object "$args"
        return [$node_slack_object get_conf]
    }

    public method get_cpath { inst } {
        #notice "get CP values: $inst!"
        set ::cpl {}
        [get_tm Tmg] show_critical_path -through_instance $inst -noshow -cplist= ::cpl
        #puts "cpl: $::cpl"
        return $::cpl
    }

    public method cf_set_state { state val } {
        set_state $state $val
    }

    public method cf_get_state { type } {
        return [state $type]
    }


    public method set_scen { sc } {
        set scen $sc
    }
    
    public method set_si { si } {
        set si_cnt $si
    }

    public method get_tm { tm } {
        #puts "CF: get_tm $tm"
        return [parent get_tm $tm]
    }

    public method del_tm { tm } {
        return [parent del_tm $tm]
    }

    public method cmd_rlog { {tag ""} } {
        if { [state milos_rlog_on] == 0 } {
            return
        }
        set db [get_tm DB]
        $db rlog -tag $tag
    }

    public method send_plugin_cmd { type cmd } {
        return [parent send_plugin_cmd $type $cmd]
    }

    public method get_plugin_result { type  } {
        return [parent get_plugin_result $type]
    }

    public method up_opt { type count undo msg } {
        debug PLUGIN:U "UP_OPT: $type $count $undo, $msg"
        if { $undo == 1  && [state need_timing_for_undo] == 1} {
            #020810D UPOBJ update_timing 1
            UPOBJ update_timing "aterms" "atfl" 1
        }
        return [parent up_opt $type $count $undo $msg]
    }

    public method add_clock_delay { file } {
        UPOBJ add_clock_delay $file
        set_state node_slack_status "invalid"
    }

    public method get_wire_spread_paths { prefix } {
        debug PLUGIN:U "GWSP: $prefix"
        UPOBJ get_wire_spread_paths $prefix
    }

    public method get_path_report { prefix } {
        debug PLUGIN:U "NEW GPS: $prefix"
        UPOBJ get_clock_period $prefix
        set of [state use_report_file]
        if { $of ne "" } {
            set data [read_file $of]
            set fp [open "${prefix}.rpt.lst" w]
            puts $fp $data
            close $fp
        } else {
            UPOBJ get_path_report $prefix
        }
    }

    public method update_int_timing { } {
        debug PLUGIN:U "NEW UPDATE TIMING CF!!"
        set ns [UPOBJ update_int_timing]
        debug TMG_DB:S "Num of Scenarios from TCL: $ns"
        return $ns
    }

    public method update_timing { } {
        debug PLUGIN:U "NEW UPDATE TIMING CF!!"
        #020810D UPOBJ update_timing
        UPOBJ update_timing "aterms" "atfl"
    }

    public method sync_nodes { {delay 0.1} {msg "" }} {
        return [parent sync_nodes $delay $msg]
    }

    public method public_commands { mod } {
        return [::VOB::public_commands $mod]
    }


    #######################
    ## ROUTING Utilities ##
    #######################

    public method collect_inactive_tile_stats  {}  {

	[get_tm ZT] GetTileList -sorted -result= ::tl4rt
	[get_tm ZT] GetTileList -sorted -all -result= ::alltls
	set all_tlist [split $::alltls]
	set inactive_tlist {}

	foreach tlelem $all_tlist  {
	    if { $tlelem == "" }  {
		continue
	    }
	    if { [string match *$tlelem* $::tl4rt] == 0 }  {
                # puts "Appending element: $tlelem"
		lappend inactive_tlist $tlelem
	    }
	}

	notice "Statistics of Inactive Tiles:"
	notice "Tile Name  Instances  Nets  Vias  Wiring Length"
	foreach tlelem $inactive_tlist  {
	    [get_tm ZT] GetTileAttributes -name $tlelem -insts= ::tlinsts -nets= ::tlnets -viacnt= ::tlvias -netlength= ::tlnetlen 
	    notice "$tlelem  $::tlinsts  $::tlnets  $::tlvias  $::tlnetlen"
	}
    }

}
