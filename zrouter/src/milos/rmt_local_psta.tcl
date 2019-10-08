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
package require ade::milos::plugin 0.1

package provide ade::milos::rmt 0.1

## RMT LOCAL is a handle for the LOCAL RMT object - its main function is plugin
## handling - everything else is pretty much a pass-through

itcl::class RMT_LOCAL_psta {

    inherit RMT_LOCAL_milos

    public method set_plugin_status { } {
        set plugins_local { TE CELTIC }
        set plugins_remote { }
        set plugins_ignore { }
        set plugins_invalid { }

        init_plugin_object TE PLUGIN_psta
    }

    protected variable active_fname

    public method send_activeterms { str_string } {
        debug PLUGIN:A "UO: PSTA GAT!"
        plugin_api TE send_activeterms 
        debug PLUGIN:A "UO: PSTA POST GAT!"
    }


    public proc new { {name ""} {async 0} {rmt_app "milos"} } {
        debug PLUGIN:A "PSTA: $name $async $rmt_app!!"
        set r ""
        if { $name eq "" } {
            set r [RMT_LOCAL_psta #auto]
            debug PLUGIN:A "PSTA #auto: $r!"
        } else {
            set r [RMT_LOCAL_psta $name]
            debug PLUGIN:A "PSTA $name: $r!"
        }
        return $r
    }

    public method get_any_sta_result { {args ""} } {
        debug PLUGIN:A "get_any_sta_result $args"
        set v [plugin_api TE get_any_sta_result $args]
        debug PLUGIN:A "get_any_sta_result: $v"

	return $v
    }
    public method run_any_sta_cmd { args } {
        debug PLUGIN:A "run_any_sta_cmd $args"
        set v [plugin_api TE run_any_sta_cmd $args]
        debug PLUGIN:A "run_any_sta_cmd: $v"
    }
    public method set_si { val } {
        if { [$sobj cget -celtic_on] == 1 } {
            ## NO SI when Celtic is also being used
            #warning "SET_SI: Celtic used, no set!"
            return
        }
        #notice "SET_SI: Celtic not used, set!"
        return [plugin_api TE set_si $val]
    }


    public method update_eco { name } {
        get_eco_string
    }

    public method write_clock_pins { {scenario 0} {num_scenarios 1} } {
        debug PLUGIN:A "\t\t\tRMT_LOCAL_psta::write_clock_pins write_cterms_file"
        plugin_api TE write_cterms_file
        debug PLUGIN:A "\t\t\tRMT_LOCAL_psta::write_clock_pins END write_cterms_file"
    }
    public method annotate_timing_structure { {scenario 0} {num_scenarios 1} } {
        debug PLUGIN:A "[$sobj cget -name] -----------------------------RMT_LOCAL_psta::annotate_timing_structure BEGIN"
        debug PLUGIN:A "\t\t\t\twrite_cterms_file"
        plugin_api TE write_cterms_file
        debug PLUGIN:A "\t\t\t\twrite_cterms_file END"

        plugin_api TE read_cterms_file $scenario $num_scenarios
        debug PLUGIN:A "\t\t\t\tread_cterms_file END"
        debug PLUGIN:A "[$sobj cget -name] -----------------------------RMT_LOCAL_psta::annotate_timing_structure END"
    }
    public method reset_default_tmg_tm { name } {
        debug PLUGIN:A "\treset_default_tmg_tm= $name\n"
        plugin_api TE reset_tmg $name
        debug PLUGIN:A "\tEND\t\t\treset_default_tmg_tm= $name\n"
    }
    public method store_timing_cache { {num_scenarios 1} } {
        debug PLUGIN:A "\t[$sobj cget -name] RMT_local_psta store_timing_cache ________________ BEGIN"
        plugin_api TE store_timing_cache $num_scenarios
        debug PLUGIN:A "\t[$sobj cget -name] RMT_local_psta store_timing_cache ________________ END"
    }
    public method update_timing { rmt_tmg_info {scenario 0} {num_scenarios 1} } {
        debug PLUGIN:A "\t[$sobj cget -name] RMT_local_psta update_timing ________________ BEGIN"
        #020810D plugin_api TE write_slack_file
        plugin_api TE write_slack_file $rmt_tmg_info

        #plugin_api TE reset_timing_cache
        plugin_api TE read_slack_file $rmt_tmg_info $scenario $num_scenarios
        debug PLUGIN:A "\t[$sobj cget -name] RMT_local_psta update_timing ________________ END"
    }

    public method run_remote_tmg_cmd { filename cmd_name args } {
        debug PLUGIN:A "\t[$sobj cget -name] run_remote_tmg_cmd $cmd_name $args ___________ BEGIN"
        
        plugin_api TE send_tmg_commands $filename "$cmd_name $args"

        #plugin_api TE reset_timing_cache
        plugin_api TE accept_tmg_result $cmd_name
        debug PLUGIN:A "\t[$sobj cget -name] run_remote_tmg_cmd $cmd_name $args ______  END"
    }

    public method update_node_slack { req_slacks } {
        debug PLUGIN:A "\n\n------------------------> UPDATE_NODE_SLACK BEGIN\n"
        foreach type $req_slacks {
            set rc [plugin_api TE update_node_slack $type]
        	debug PLUGIN:A "\trc= $rc\n"
            set cmd "$node_slack_object configure $rc"
        	debug PLUGIN:A "\tcmd= $cmd\n"
            eval $cmd
        }
        debug PLUGIN:A "\n------------------------> UPDATE_NODE_SLACK BEGIN\n\n"
    }
    
    public method replay_plugin_cmds { } {
        plugin_api TE replay_plugin_cmds
    }

    protected variable reinit_cnt 2
    public method reinit_db { name } {

	debug PLUGIN:A "reinit_db ------------------------"
        ## always re-write spef file at this point
        set sname [plugin_fname $sobj "psta${reinit_cnt}" "spef"]
        set ec [$corner cget -ext_corner]
        call_milos "pdb write_spef -psta 1 -outfile $sname -ext_corner_name $ec"
        $sobj configure -spef_file "${sname}[get_spef_ext]"
        incr reinit_cnt

        ## always re-write verilog at this point
        write_verilog $sobj

        plugin_api TE set_obj_value sobj -spef_file [$sobj cget -spef_file]
        check_files [$sobj cget -spef_file] [$sobj cget -verilog_file]

	debug PLUGIN:A "FINISHED reinit_db ------------------------"

        plugin_api TE myreinit
        lists_to_plugin
    }


    public method write_corner_spef_file { corner_arg min_max {fp ""} } {

            set ec [$corner cget $corner_arg]
	    if { $ec eq "" } {
		return 0
	    }
            set sname [plugin_fname $sobj "psta" "spef"]
            set spef_file_name $sname
	    if { $min_max ne "min_max" } {
            	set spef_file_name "${sname}.${min_max}"
	    }
            $sobj configure -${min_max}_spef_file ${spef_file_name}[get_spef_ext]

	    if {$fp eq ""} {

	    	debug PLUGIN:A "writting spef_file_name $spef_file_name"
            	call_milos "pdb write_spef -psta 1 -outfile $spef_file_name -ext_corner_name $ec"
		} else {
        		puts $fp "xt write_spef -psta -file $spef_file_name -gz -ext_corner_name $ec -net_id 0"
		}
		return 1
        }
   public method check_spef_files { } {
        set min_spef [$sobj cget -min_spef_file]
        set max_spef [$sobj cget -max_spef_file]
	if { $min_spef != "" } {
        	check_files $min_spef
        	plugin_api TE set_obj_value sobj -min_spef_file $min_spef
	} 
	if {$max_spef != ""} {
        	check_files $max_spef
        	plugin_api TE set_obj_value sobj -max_spef_file $max_spef
	}
	if {[$sobj cget -spef_file] != ""} {
        	check_files [$sobj cget -spef_file]
        	plugin_api TE set_obj_value sobj -spef_file [$sobj cget -spef_file]
	}
    }
    public method write_rmt_script { name } {
        set tmp_dir [$sobj cget -tmp_dir]
	set blk [$sobj cget -name]
        set min_spef [$sobj cget -min_spef_file]
        set max_spef [$sobj cget -max_spef_file]
	set spef_file [$sobj cget -spef_file]

                set fname [file join $tmp_dir $blk.ext.debug.tcl]
                set fp [open $fname w]
                puts $fp "::db write -file $blk.db"
                puts $fp "Ext xt"
                puts $fp "xt set_block -block_name $blk"


        	puts $fp "xt write_spef -psta -file $min_spef -gz -ext_corner_name $ec -net_id 0"
        	puts $fp "xt write_spef -psta -file $max_spef -gz -ext_corner_name $ec -net_id 0"
                flush $fp
                close $fp

            $rmt send_command acmd "source $fname"
}
    public method get_db { name } {
	debug PLUGIN:A "get_db $name  DELETE  rmt_local_psta.tcl"

        ## Write files (done on main - not sure of having DB on remote)

        # 033110D file mkdir [$sobj cget -tmp_dir]
        set tmp_dir [$sobj cget -tmp_dir]
	set scenario_dir [$sobj cget -name]

        file mkdir $tmp_dir
	#file mkdir [file join $tmp_dir $scenario_dir]

        UPOBJ enable_eco_log psta

        if { [$sobj cget -spef_file]  == "" } {
    		write_corner_spef_file "-min_ext_corner" "min"
    		write_corner_spef_file "-max_ext_corner" "max"
    		write_corner_spef_file "-ext_corner" "min_max"

            #set ec [$corner cget -ext_corner]
            #set sname [plugin_fname $sobj "psta" "spef"]
            #$sobj configure -spef_file "${sname}[get_spef_ext]"

            #call_milos "pdb write_spef -psta 1 -outfile $sname -ext_corner_name $ec"
        }

	debug PLUGIN:A "FINISHED SPEFs ------------------------"
        if { [state rc_models_si] > 0 } {
            warning "PSTA plugin mode does not support SI - results are unpredictable!"
        }

	debug PLUGIN:A "Writting Verilog file ...."

        ## Only write verilog if needed..
        if { [$sobj cget -verilog_file] eq "" } {
            write_verilog $sobj
        }
	debug PLUGIN:A "FINISHED Writting Verilog file ...."

        plugin_api TE set_timing_engine 

        # 012610D check_files [$sobj cget -spef_file] [$sobj cget -verilog_file]
        check_files [$sobj cget -verilog_file]
        check_spef_files
        copy_data_to_plugin
    }
    public method write_rmt_tmg_files { suffix { corner_cmd  ""} } {

        ## Write files (done on main - not sure of having DB on remote)

        # 033110D file mkdir [$sobj cget -tmp_dir]
        set tmp_dir [$sobj cget -tmp_dir]
	set scenario_dir [$sobj cget -name]
	set check_lib_options ""
	if { [$sobj cget -ignore_main_tmg] > 0 } {
		set check_lib_options "-ignore_tmg"
	}
        file mkdir $tmp_dir
	#file mkdir [file join $tmp_dir $scenario_dir]

        UPOBJ enable_eco_log psta

	set block [$sobj cget -block]
	set blk [$sobj cget -name]
        #set fname [file join $tmp_dir $blk.$suffix]
        set fname $blk.$suffix
        set fp [open $fname w]
        #puts $fp "gdb"
        puts $fp "::db write -file $blk.hier.db"
        puts $fp "::db flatten -copy_parasitics"
        puts $fp "::db write -file $blk.flat.db"
	puts $fp "::db save_def -file $blk.flat.def"
	puts $fp "::db set_number_of_scenarios -count 1"

        puts $fp "Ext xt"
        #puts $fp "xt set_block -block_name $blk"

        if { [$sobj cget -spef_file]  == "" } {
    		write_corner_spef_file "-min_ext_corner" "min" $fp
    		write_corner_spef_file "-max_ext_corner" "max" $fp
    		write_corner_spef_file "-ext_corner" "min_max" $fp

            #set ec [$corner cget -ext_corner]
            #set sname [plugin_fname $sobj "psta" "spef"]
            #$sobj configure -spef_file "${sname}[get_spef_ext]"

            #call_milos "pdb write_spef -psta 1 -outfile $sname -ext_corner_name $ec"
        }
	debug PLUGIN:A "Writting Verilog file ...."
        ## Only write verilog if needed..
        if { [$sobj cget -verilog_file] eq "" } {
	debug PLUGIN:A "get_verilog_file---------------------------------------------"
            	set vname [get_verilog_file $sobj]
        	debug PLUGIN:A "vname= $vname"
        	puts $fp "Tmg tmg"
 		puts $fp "tmg write_verilog -file $vname -delimiter / -module= ::ver_mod"
        	#puts $fp "::db flatten"
		#puts $fp "::db save_def -file $blk.flat.def"
 		#puts $fp "tmg write_verilog -file flat.$vname -delimiter / -module= ::ver_mod"
        }
	debug PLUGIN:A "Finished Writting Verilog file ...."
	debug PLUGIN:A "CORNER: $corner: $corner_cmd"

	if { $corner_cmd ne "" } {
 		puts $fp "$corner_cmd"
 		puts $fp "tmg check_lib $check_lib_options"
 		puts $fp "tmg init_corner -corner MAIN"
 		puts $fp "tmg set_make_rsegs"
 		puts $fp "tmg set_timing_engine -external -scenario 1"
 		#puts $fp "sta { read_sdc [$sobj cget -sdc_file]}"
	}

	flush $fp
	close $fp

	#send_command acmd "source $fname"

	debug PLUGIN:A "FINISHED SPEFs ------------------------"
        if { [state rc_models_si] > 0 } {
            warning "PSTA plugin mode does not support SI - results are unpredictable!"
        }


        plugin_api TE set_timing_engine 
	return $fname
    }
    public method check_tmg_files { } {
        # 012610D check_files [$sobj cget -spef_file] [$sobj cget -verilog_file]

        check_files [$sobj cget -verilog_file]
        check_spef_files
        copy_data_to_plugin
    }

    public method start_db { } {
	
	#040610D 
    	check_tmg_files

        ## Set up plugin
        incr startsocket 100
        plugin_api TE start_plugin $startsocket
    }

    public method apply_node_definitions { {rmt ""} } {
    }


    public method exit_plugin { } {
        plugin_api TE exit_plugin
    }
    public method lists_to_plugin_bbd { allt_file endt_file {translate ""} } {
        debug PLUGIN:A "\t[$sobj cget -name] RMT lists_to_plugin_bbd $allt_file $endt_file  ________________ BEGIN"

        plugin_api TE send_allt_file $allt_file $translate
        plugin_api TE send_endt_file $endt_file

        debug PLUGIN:A "\t[$sobj cget -name] RMT lists_to_plugin_bbd $allt_file $endt_file  ________________ END"
    }

    protected variable list_cnt 0
    public method lists_to_plugin { {bbd_opt ""} } {
        set afname [plugin_fname $sobj "allt_${list_cnt}.0" lst]
        set efname [plugin_fname $sobj "endt_${list_cnt}.0" lst]

	if { $bbd_opt ne "" } {
    		lists_to_plugin_bbd $afname $efname [$sobj cget -translate]
		return
	}
        if { [file exists $afname] == 0 } {
            set fp [open $afname w]
            if { [$sobj cget -translate] == 1 } {
                $tmg write_all_terms -file $fp -no_backslash
            } else {
                $tmg write_all_terms -file $fp
            }
            flush $fp
            close $fp
        }

        set efname [plugin_fname $sobj "endt_${list_cnt}.0" lst]
        if { [file exists $efname] == 0 } {
            set fp [open $efname w]
            $tmg write_end_terms -file $fp
            flush $fp
            close $fp
        }

        plugin_api TE lists_to_plugin $afname $efname
        incr list_cnt
    }

    public method int_plugin_cmd { args } {
        plugin_api TE int_plugin_cmd "$args"
    }

    public method get_wire_spread_paths { prefix } {
        plugin_api TE get_wire_spread_paths $prefix
    }

    public method set_plugin_mode { mode } {
        plugin_api TE set_plugin_mode $mode
    }
    public method set_plugin_control { type } {
        plugin_api TE set_plugin_control $type
    }

    public method get_path_report { prefix {write_ep 0} {sl 4} {skip 1} } {
        puts "GPR: $prefix/$write_ep/[state compress_reports]/$sl/$skip"
        plugin_api TE get_path_report $prefix $write_ep [state compress_reports] $sl $skip
    }

    public method add_clock_delay { file } {
        plugin_api TE add_clock_delay $file
    }

    public method get_clock_period { prefix } {
        plugin_api TE get_clock_period $prefix
    }

    #012710D public method set_requirements { min_slack max_slack min_slew max_slew } {
        #plugin_api TE set_requirements $min_slack $max_slack $min_slew $max_slew
    #}

    public method set_requirements { args } {
        plugin_api TE set_requirements $args
    }

    public method save_session { dir } {
        error "Cannot save session for PSTA!"
    }
}

