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

itcl::class RMT_LOCAL_milos {

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
            set r [RMT_LOCAL_milos #auto]
        } else {
            set r [RMT_LOCAL_milos $name]
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


    public method get_remote_host { } {
        return [exec hostname]
    }

    ## generate the milos top-level pass-through methods automatically
    ## MILOS commands
    foreach mname {ext mme pdb opt rpt rte sta sys tmg } {
        set cmd "public method $mname { args } { "
        #append cmd "puts \"RLM $mname: \$args\";\n"
        append cmd "call_milos \"$mname \$args\" }"
        eval $cmd
    }

    ## do nothing for the "local" object
    public method reset_unpublish { } {
    }
    public method add_unpublish { name } {
    }
    public method do_unpublish { } {
    }

    ## NO_OP commands - these have no meaning for the "local" rmt object
    public method send_activeterms { str_string } {
        ##NO_OP - always up-to-date on main
    }

    public method replay_plugin_cmds { } {
        ##NO_OP - always up-to-date on main
    }

    public method reinit_db { name } {
        ##NO_OP - always up-to-date on main
    }
    public method get_db { name } {
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

    public method save_session { args } {
        error "Cannot save session for embedded mode!"
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
        plugin_api TE lists_to_plugin
    }

    public method exit_plugin { } {
    }
    ## END OF NO_OP commands

    protected method get_spef_ext { } {
        if { [call_milos "pdb get_default write_spef gz"] eq 1 } {
            return ".gz"
        } else {
            return ""
        }
    }


    ## Simple pass-through commands (may or may not be needed)
    public method set_update_method { method } {
        $tmg set_update_method -method $method
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
        #puts "IPO: $type, $datatype"
        set idx [lsearch $plugins_local $type]
        if { $idx >= 0 } {
            set_plugin_object $type [ [$datatype #auto] this]
            return
        }

        set idx [lsearch $plugins_remote $type]
        if { $idx >= 0 } {
            init_remote_plugin_obj $type $datatype
            return
        }

        set idx [lsearch $plugins_ignore $type]
        if { $idx >= 0 } {
            return
        }

        set idx [lsearch $plugins_invalid $type]
        if { $idx >= 0 } {
            error "This node does not support the $type plugin flow!"
        }

        error "Unknown plugin type $type"
    }

    protected method init_remote_plugin_obj { type datatype } {
    }

    public method plugin_api { type cmd {args ""} } {
        set idx [lsearch $plugins_local $type]
        if { $idx >= 0 } {
            if { $args eq "" } {
                return [send_local_plugin_api $type $cmd]
            } else {
                return [send_local_plugin_api $type $cmd $args]
            }
        }

        set idx [lsearch $plugins_remote $type]
        if { $idx >= 0 } {
            if { $args eq "" } {
                return [send_remote_plugin_api $type $cmd]
            } else {
                return [send_remote_plugin_api $type $cmd [join $args]]
            }
        }

        set idx [lsearch $plugins_ignore $type]
        if { $idx >= 0 } {
            return
        }

        set idx [lsearch $plugins_invalid $type]
        if { $idx >= 0 } {
            error "This node does not support the $type plugin flow!"
        }
    }

    protected method send_local_plugin_api { type cmd {args ""} } {
        set po [get_plugin_object $type]
        set mycmd ""
        if { $args eq "" } {
            set mycmd "$po $cmd"
        } else {
            set mycmd "$po $cmd [join $args]"
        }

        debug PLUGIN:A "CMD to plugin $type: $cmd $args"
        return [::eval $mycmd]
    }

    protected method send_remote_plugin_api { type cmd {args ""}} {
    }

    public method update_timing { {scenario 0} {num_scenarios 1} } {
puts "rmt_local_milos.tcl merged_method $rmt_local_milos.tcl"
        # DIMITRIS 1/14/10 $tmg reset_timing_cache -scenario $scenario -num_scenarios $num_scenarios
        $tmg reset_timing_cache -scenario $scenario -num_scenarios $num_scenarios -reset_only
        ###NO_OP - local timing up-to-date by calling update_obj
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

    public method set_requirements { args } {
        ## do nothing
    }
    ## ensure slacks are up to date - should always be the case, except for
    ## multimode - may need timing update?
    public method update_node_slack { slacks } {
        get_local_slack $node_slack_object $slacks 1
    }

    public method get_node_slack { slacks } {
        return $node_slack_object
    }

    public method read_spef_after_eco { fname ext_corner_option min_max } {
        set ec [$corner cget $ext_corner_option]
	if { $ec eq "" } {
		return
	}
        set f2 "$fname.[get_ec_idx $ec].spef"
        debug PLUGIN:E "fname=$fname, f2=$f2, ec=$ec"
        plugin_api TE read_spef_after_eco $f2 $min_max
    }
    public method read_incr_spef { fname } {
        debug PLUGIN:E "rmt_local_milos.tcl: read_incr_spef: BEGIN"
	read_spef_after_eco $fname "-max_ext_corner" "max"
	read_spef_after_eco $fname "-min_ext_corner" "min"
	read_spef_after_eco $fname "-ext_corner" "min_max"
        debug PLUGIN:E "rmt_local_milos.tcl: read_incr_spef: END"
	
        #set ec [$corner cget -ext_corner]
        #set f2 "$fname.[get_ec_idx $ec].spef"
        #puts "fname=$fname, f2=$f2, ec=$ec, [$corner get_conf]"
        #plugin_api TE read_incr_spef $f2
    }

    public method read_i_incr_spef { fname } {

        set ec [$corner cget -ext_corner]
        set f2 "$fname.[get_ec_idx $ec].spef"
        #puts "PARA: [$corner get_conf], $fname, $f2"
        plugin_api TE read_i_incr_spef $f2
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


    protected method sa { obj} {
        ## for milos, do nothing
    }

    protected method get_eco_string { } {
        ## Local version
        debug PLUGIN:E "GET ECO String:"
        set eco_string [UPOBJ get_eco_string]
        debug PLUGIN:E "ECO String: $eco_string"
        if { [$sobj cget -translate] == 1 } {
            set eco_string [escape_line $BS_P $eco_string ""]
        }
        plugin_api TE send_eco_string $eco_string
    }

    protected method copy_data_to_plugin { } {
        ## Copy values across
        plugin_api TE init_obj sobj [$sobj get_conf]
        plugin_api TE init_obj corner [$corner get_conf]

        if { [state thresh_obj] != "" } {
            plugin_api TE init_obj thresh [ [state thresh_obj] get_conf]
        }   

        plugin_api TE set_sidx [$sobj get_sidx]
        #plugin_api TE set_si [state rc_models_si]
        ## End copy values
    }

    protected method get_plugin_obj { obj } {
        return [plugin_api TE get_obj [set $obj]]
    }

    protected method write_cterms_file { } {
        plugin_api TE write_cterms_file
    }
    protected method read_cterms_file { } {
        plugin_api TE read_cterms_file
    }
    protected method write_slack_file { } {
            debug PLUGIN:A "[$sobj -cget name] == write_slack_file BEGIN"
        plugin_api TE write_slack_file
            debug PLUGIN:A "[$sobj -cget name] == write_slack_file END"
    }
    protected method read_slack_file { } {
        plugin_api TE read_slack_file
    }

    protected method check_files { args } {
        foreach f $args {
            if { ![file exists $f] } {
                error "File $f was not created!\n"
            }
            if { [file size $f] <= 0 } {
                error "File $f has size 0!"
            }
            debug PLUGIN:A "File $f exists and has size [file size $f]"
        }
    }

    public method pdebug { {debug {0 0}} } {
        set FILE_DEBUG [lindex $debug 0]
        set P_DEBUG [lindex $debug 1]
        plugin_api TE set_debug $FILE_DEBUG $P_DEBUG
    }
    
    public method read_sdf { cnt } {
        plugin_api TE read_sdf $cnt
    }

    public method get_wire_spread_paths { prefix } {
        ##
    }
    public method set_plugin_mode { mode } {
        ##
    }
    public method set_plugin_control { type } {
        ##
    }
    public method get_path_report { args } {
        ## 
    }

    public method add_clock_delay { file } {
        ## 
    }

    public method get_clock_period { file } {
        ## 
    }


    #######################################################################
    ## CELTIC METHODS                                       
    #######################################################################
    public method create_celtic_tmp_dir { } {
        set tmp_dir [file join [$sobj cget -tmp_dir] celtic_[$sobj get_sidx]]
        file mkdir $tmp_dir
    }

    protected variable celtic_tmp_dir ""
    public method write_celtic_data_files { } {
        set startdir [pwd]
        set celtic_tmp_dir [file join [$sobj cget -tmp_dir] celtic_[$sobj get_sidx]]
        cd $celtic_tmp_dir
        set csname celtic.spef
        $sobj configure -celtic_spef_file "$csname[get_spef_ext]"
        set ec [$corner cget -ext_corner]
        call_milos "pdb write_spef -prime_time 1 -outfile $csname -ext_corner_name $ec"

        set vn [$sobj cget -verilog_module]
        set vfn "${vn}.v"

        call_milos "tmg write_verilog -delimiter / -file $vfn"; ## Celtic
        cd $startdir
    }

    public method launch_celtic { celtic_cnt } {

        set logname "celtic_out_${celtic_cnt}.log"
        #set cmd "{cd $celtic_tmp_dir; exec [$sobj cget -celtic_start_name] celtic.tcl > celtic.log; file copy celtic.log celtic_${celtic_cnt}.log; file copy celtic.sdf celtic_${celtic_cnt}.sdf}"
        
        set cmd "$celtic_tmp_dir {[$sobj cget -celtic_start_name]} celtic.tcl celtic.log celtic_${celtic_cnt}.log celtic_${celtic_cnt}.sdf"
        puts "Spawning CELTIC ([pwd]): $cmd!"

        set tn "c_[$sobj get_sidx]_${celtic_cnt}"
        msh::spawn $tn "::apps::rexec $cmd" -class celtic

        return $tn
    }

}

