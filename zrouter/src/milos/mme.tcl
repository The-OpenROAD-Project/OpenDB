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

## mme.tcl
## MultiMode module for Milos
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::mme 0.1

itcl::class MME {

    inherit VOB


    private variable next_remote_name "A";


    private variable sec 0;

    private variable scratch 0;

    private variable rmt_local "";
    private variable local_name ""

    private variable slack_type "";
    ## Protected Vars
    ##

    ## End Protected Vars

    protected method constructor { parent } { 
        VOB::constructor $parent 
        set NAME "Multimode"
        set SELFNAME "MME$this"
        set_state mme init

        ## OFF, DEFINE, INIT, ACTIVE
        set_state multi OFF

        ## 1 = DEMAND, 2 = STA incr, 3 = Zroute incr
        ## DEMAND = Tmg incremental
        ## STA = Tmg Full, Zroute incr = Active Terms = activeterms

        set_state mme_update_type ACTIVE_TERMS

        set_state slacks_needed 1

        ## list of ALL scenarios
        set_state scenario_list {};
        set_state rmt_scenarios {};
        set_state local_scenario "";
        set_state rmt_list {};
        set_state rmt_local ""

        set_state main_num_size 0;
        set_state main_num_size_undo 0;
        set_state main_num_buff 0;
        set_state main_num_buff_undo 0;

        set_state mme_update_freq 100
    }

    public method update_eco { } {
        puts "MME UPDATE ECO!!"
    }
    VOB::add_args MME_SHOW_SLACKS {
        {merged bool 0 0}
        {delay_type nstr 0 "current"}
        {verbose bool 0 0}
        {xml bool 0 0}
    }
    VOB::add_help MME_SHOW_SLACKS {
        {merged "Show \"merged\" slacks (for main)" }
        {delay_type "Valid values: min, max, both, current" }
        {verbose "Show worst slack terms or not" }
        {xml "XML format" }
    }
    public method show_slacks { args } {
        parse_args
        get_all_args
        return [UPOBJ show_worst $merged $delay_type $xml $verbose]
    }

    public method update_node_slacks { } {
        UPOBJ update_node_slacks
    }

    public method get_node_slack { node } {
        return [UPOBJ get_node_slack $node]
    }

    public method get_db_timing { } {
        UPOBJ update_timing
    }

    VOB::create_enum mspc {"auto" "manual"}
    VOB::add_args MME_SET_PLUGIN_CONTROL {
        {type enum_mspc 0 "auto" }
    }
    VOB::add_help MME_SET_PLUGIN_CONTROL {
        {type "Valid values: auto or manual" }
    }
    public method set_plugin_control { args } {
        parse_args
        UPOBJ set_plugin_control [get_arg type]
    }

    VOB::create_enum mup {"setup" "hold" "ocv"}
    VOB::add_args MME_SET_PLUGIN_MODE {
        {mode enum_mup 0 "ocv" }
    }
    VOB::add_help MME_SET_PLUGIN_MODE {
        {mode "Valid values: setup, hold or ocv" }
    }
    public method set_plugin_mode { args } {
        parse_args
        UPOBJ set_plugin_mode [get_arg mode]
    }


    public method update_timing { } {
        #puts "MME UPDATE TIMING!!"
        UPOBJ update_timing
    }

    #public method init_gui { } {
        #parent init_gui
    #}


    VOB::add_args MME_STA_CMD {
        {node nstr 1}
        {cmd nstr 1}
    }

    VOB::add_help MME_STA_CMD {
        {node "Name of node (scenario name) to send command to. ALL_NODES means every node" }
        {cmd "Command string to send. If you use \[], enclose in {}" }
    }

    public method sta_cmd { args } {

        if { [state multi] eq "OFF"  } {
            error "node_cmd is invalid when not in multi-mode." "" -1
        }

        parse_args
        set nn [get_arg node]
        set cmd [get_arg cmd]
        #UPOBJ execute_remotes $nn $cmd
	notice "--------------------------> mme apply : run_tmg_remote_script -- BEGIN"
	UPOBJ run_tmg_remote_script $nn $cmd
	#UPOBJ get_path_report $cmd
	notice "--------------------------> mme apply : run_tmg_remote_script -- END"
    }

    VOB::add_args MME_NODE_CMD {
        {node nstr 1}
        {cmd nstr 1}
    }

    VOB::add_help MME_NODE_CMD {
        {node "Name of node (scenario name) to send command to. ALL_NODES means every node" }
        {cmd "Command string to send. If you use \[], enclose in {}" }
    }

    public method node_cmd { args } {

        if { [state multi] eq "OFF"  } {
            error "node_cmd is invalid when not in multi-mode." "" -1
        }
	set tmg [get_tm Tmg]

        parse_args
        set nn [get_arg node]
        if { [state multi] eq "ACTIVE" } {
                    puts "\n\nHERE call_node [get_arg cmd]"

            ## Local
            set sobj [state local_scenario]
	    if { $sobj ne "" } {
            set  nname [$sobj cget -name]
            if { ($nname eq $nn) || ($nn eq "ALL_NODES") } {
                    puts "\n\nHERE call_node nname= $nname nn=$nn [get_arg cmd]"
			$tmg plugin_cmd [get_arg cmd]
                    #call_milos [get_arg cmd]
            }
	    }

            ## Remotes

	puts "rmt_scenarios= [state rmt_scenarios]"

            foreach sobj [state rmt_scenarios] {
                set rmt [$sobj get_remote]
                set nname [$sobj cget -name]
                    puts "REMOTE: sobj= $sobj rmt= $rmt nname= $nname [get_arg cmd]"
                if { ($nname eq $nn) || ($nn eq "ALL_NODES") } {
                    puts "call_node $rmt [get_arg cmd]"
                    call_node $rmt [get_arg cmd]
                }
            }
        } else {
            ## "DEFINE"
            puts "DEFINE CMD!"
            append_multi_cmd 1 $nn [get_arg cmd]
        }
    }

    ## used to "close the hole"
    protected method info { }

    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help MME $cmd $param $long
        } else {
            mod_help MME $cmd
        }
    }

    public method set_gui_freq { val } {
        set_state mme_update_freq $val
    }

    VOB::add_args MME_INIT_MME {
        {gui bool 0 0}
        {gui2 bool 0 0}
        {update_type num 0}
    }
    public method init_mme { args } {

        parse_args

        if { [state multi] eq "OFF" } {
            set_state multi "DEFINE";
            set_state postcmdq { }
            set_state midcmdq { }
            set_state precmdq { }
            notice "Initializing Multimode definitions."

            if { [get_arg gui] == 1 } {
                set_state mme_gui 1
                gui_start
            }

            if { [get_arg gui2] == 1 } {
                set_state mme_gui 2
            }

        } elseif { [state multi] eq "DEFINE" } {
            warning "Multimode definition already in progress."
        } else {
            warning "Multimode can only be defined once per session."
        }
    }

    VOB::add_help MME_SET_NUM_PT_LICENSES {
        {num_licenses "number of PrimeTime licenses to use"}
    }

    public method set_num_pt_licenses { args } {
        parse_args
        set num_pt_licenses [get_arg num_licenses]
    }

    VOB::add_args MME_SAVE_SESSION {
        {node nstr 1}
        {save_dir nstr 1}
    }
    public method save_session { args } {

        if { [state multi] ne "ACTIVE"  } {
            error "save_session is only valid when multi-mode is active!" "" -1
        }
        parse_args

        set nn [get_arg node]

        ## Remotes
        foreach sobj [state rmt_scenarios] {
            set rmt [$sobj get_remote]
            set nname [$sobj cget -name]
            if { ($nname eq $nn)  } {
                call_node $rmt save_session [get_arg save_dir]
            }
        }

        ## Local
        set sobj [state local_scenario]
        set  nname [$sobj cget -name]
        if { ($nname eq $nn) } {
            if { $nname ne "NULL" } {
                set rmt [$sobj get_remote]
                $rmt save_session [get_arg save_dir]
            }
        }
    }

    VOB::add_args MME_RESTART_REMOTES {
        {replay bool 0 1 } 
    }

    public method restart_remotes { args } {
        parse_args

        UPOBJ restart_remotes [get_arg replay]

    }

    private method init_remotes { {debug 0} main_null } {

        if { $main_null } {
            set old_sobj [state local_scenario]
            set sobj [[SCENARIO #auto] this]
            $old_sobj copy_to $sobj
            $sobj configure -engine null
            $sobj configure -name NULL
            set_state local_scenario $sobj
            set_state local_null 1
            set_state nulled_scenario $old_sobj
            lappend_state rmt_scenarios $old_sobj
        }
        set sidx 1

        set sobj [state local_scenario]

	if { $sobj ne "" } {

        set_state local_name [$sobj cget -name]
        $sobj configure -gname [string tolower [state local_name]]
        set rtype [$sobj cget -engine]
        set rmt_local [[new_rmt "local" 0 "local" $rtype] this]
        $sobj set_remote $rmt_local
        puts "SIDX: $sidx"
        $sobj set_sidx $sidx
        set ::scen_idx $sidx
        $rmt_local set_sobj $sobj
        $rmt_local init_rmt
        $rmt_local set_plugin_status

        set_state rmt_local $rmt_local

        UPOBJ set_rmt_local $rmt_local
	} else {
		notice "No remote_local scenario set"
        	set sidx 0
	}

        if { $debug > 0 } {
            UPOBJ set_debug {1 1 1 1}
        }
        puts "Test: rmt_scenarios: [state rmt_scenarios]"
        puts "Test: scenario_list: [state scenario_list]"
        foreach sobj [state rmt_scenarios] {
            incr sidx

            set name [$sobj cget -name]
            $sobj configure -gname [string tolower $name]
            set rmt ""
            set rtype [$sobj cget -engine]
            puts "SOBJ: $name, $rtype" 
            set rmt [[new_rmt $name 0 milos $rtype] this]
            #notice "Scenario node (1): $rmt (corner [$sobj cget -corner])"
            $rmt init_rmt
            $rmt set_plugin_status
            $sobj set_remote $rmt
            $rmt set_sobj $sobj
            puts "SIDX: $sidx"
            $sobj set_sidx $sidx
            $rmt send_command acmd "set ::scen_idx $sidx"

            UPOBJ add_rmt $rmt

            notice "MME: SET SOBJ, $rmt, $sobj!"
            # 033110D move higher $rmt set_sobj $sobj
            #notice "MMET: [$rmt get_sobj]"

            if { $debug > 0 } {
                $rmt mdebug
                $rmt send_command acmd "::zr.zlog enable_debug_log -module ZTC"
                $rmt send_command acmd "::zr.zlog enable_debug_log -module PLUGIN"
                $rmt send_command acmd "::zr.zlog enable_debug_log -module ZTIMER"
                #$rmt send_command acmd "::zr.zlog enable_debug_log -module TMG"
                #$rmt send_command acmd "::zr.zlog enable_debug_log -module TMG_SLACK -only A"
                #$rmt send_command acmd "::zr.zlog enable_debug_log -module TIMING_CACHE"
            }
            $rmt set_state multi REMOTE

            #puts "scen_$sidx: [state scen_$sidx]"
            lappend_state rmt_list $rmt

            ## TODO
            #gui_status $node init Initializing
        }
        set_state num_scenarios $sidx
        $::mmdb set_number_of_scenarios -block $::mmblock -count $sidx
        sync_nodes
        check_multi_error

    }


    VOB::add_args MME_APPLY_MME {
        {name hnstr 0 "not needed"}
        {debug hnum 0 0}
        {scratch hbool 0 0}
        {show_timing bool 0 0}
        {main_null hbool 0 0}
        {merge_method hnum 0 2}
        {transfer_db hbool 0 0}
        {transfer_blocks hbool 0 0}
        {bbd_opt hbool 0 0}
        {ignore_local_rmt hbool 0 0}
        {ignore_main_tmg hbool 0 0}
    }
    public method apply_mme { args } {
        if { [state multi] eq "OFF" } {
            error "Multimode was not initialized." "" -1
        }

        if { [state multi] eq "ACTIVE" } {
            error "Multimode can only be defined once per session." "" -1
        }
        parse_args
        get_all_args

	if { $ignore_local_rmt == 0 } {
        if { [state local_scenario] eq "" } {
            error "No local node defined for multimode (call create_scenario with -priority 1)." "" -1
        }
	} else {
            	notice "No priority 1 scenario chosen"
        	set sobj [lindex [state rmt_scenarios] 0]
        	set rmt [[lindex [state rmt_scenarios] 0] get_remote]
        	set name [$sobj cget -name]
            	notice "No priority 1 scenario chosen; first scenario is: $sobj $name "
		set corner [$sobj cget -corner]
		#puts "ignore_local_rmt:REDEFINING CORNER $corner"

            	set rc [catch { set cobj [find_obj corner_list corner $corner]} output]
            	if { $rc == 1 } {
                	error "No corner named $corner has been defined ($rc) ($output)!" "" -1
            	} 
		puts "ignore_local_rmt:cobj= $cobj : [$cobj get_cmd]"
		set current_state [state multi]
                set_state multi OFF
		call_milos "[$cobj get_cmd]"

		set ignore_tmg ""
		if { $ignore_main_tmg == 1 } {
			set ignore_tmg "-ignore_tmg"
            		notice "Will ignore local tmg"
		}
                #042710D call_milos "tmg check_lib"
                call_milos "tmg check_lib $ignore_tmg"
                call_milos "tmg select_corner -corner [$sobj cget -corner]"
                call_milos "tmg read_sdc -sdc_file [$sobj cget -sdc_file]"
		set_state multi $current_state
	}

	# 033110D move before local_scenario
        ## parse_args
        ## get_all_args

        if { $debug >= 1 } {
            #::zr.zlog enable_debug_log -module MILOS_MME_MM
            if { $debug >= 2 } {
                #::zr.zlog enable_debug_log -module MILOS_RMT
            }
        } 

        if { $scratch == 1 } {
            file mkdir scratch
            notice "Saving debug files to scratch/"
        }


        get_ext_status
        if { [state db_parasitics] == 0 } {
            error "Cannot start multimode without parasitics!" "" -1
        }

        if { [state mme_gui] > 0 } {
            UPOBJ init_gui
            pause
        }


        set ::mmdb [get_tm DB]
        notice "Activating remote nodes: "
        gui_send main action  "Initialize Multimode"

        notice "Activating Multimode at time [runtime_incr]"

	notice "MME_APPLY merge_method= $merge_method"
        ## activate multimode
        set_state multi INIT

	notice "enable_bbd_remotes bbd_opt= $bbd_opt"
	if { $bbd_opt > 0 } {
		UPOBJ enable_bbd_remotes
		notice "enable_bbd_remotes bbd_opt= $bbd_opt"
	}
        ## NO MORE EXTRACTION HERE..
        
        ## reset, and start required remotes
        init_remotes $debug $main_null
	notice "init_remotes DONE"

        UPOBJ set_merge_method $merge_method
        after 20

        foreach sobj [state scenario_list] {
            gui_send [$sobj cget -gname] timing.engine [timing_name [$sobj cget -engine]]
            gui_send [$sobj cget -gname] host [[$sobj get_remote] get_remote_host]
        }
        pause
        update

        ## END LOCAL ONLY ##################################

        ## initialize css
        ## gui catchup


        gui_status main sending-db "DB => remote nodes"
        send_node_status action "Main DB => remotes"
        pause

        UPOBJ enable_timing $show_timing

        notice "init_db $transfer_db $transfer_blocks"
        UPOBJ init_db $transfer_db $transfer_blocks

        ## They all have the db - can now define stuff..
        ## - local definitions need to have already been done..

	if { $ignore_local_rmt == 0 } {
        $rmt_local apply_node_definitions 
        $rmt_local set_update_method activeterms
	}

        notice "Defining corners and Scenarios on remotes"
        foreach sobj [state rmt_scenarios] {
            set rmt [$sobj get_remote]
            set nname [$sobj cget -name]
            debug MILOS_MME_MM:A "Scenario node (2): $rmt"
            debug MILOS_MME_MM:A "name: $nname"
            debug MILOS_MME_MM:A "corner: [$sobj cget -corner]"
            debug MILOS_MME_MM:A "sdc file: [$sobj cget -sdc_file]"
            debug MILOS_MME_MM:A "priority: [$sobj cget -priority]"

            $rmt apply_node_definitions $rmt
            $rmt set_update_method activeterms
        }


        update
        after 2000
        update

        set ut [state mme_update_type]


        notice "Start Timing Merge at time [runtime_incr]"
        #UPOBJ update_timing

        check_multi_error

        set tmg [get_tm Tmg]
        $tmg set_make_rsegs

	# Dimitris 1/14/10
	if { $merge_method==2 } {
		if { $ignore_local_rmt == 0 } {
		set ii 1
        	foreach sobj [state scenario_list] {
            		set p [$sobj cget -priority]
			if { $p==1 } {
				notice "Optimization Priority Scenario : [$sobj cget -name]"
				break
			}
			incr ii
		}
        	$tmg set_timing_engine -external -scenario $ii
		} else {
        		set sobj [lindex [state scenario_list] 0]
			notice "Optimization Priority Scenario : [$sobj cget -name] -- No rmt_local"
        		$tmg set_timing_engine -external -scenario 1
		}
	}

        set_state multi ACTIVE

        ## restart eco..
        UPOBJ enable_eco

	notice "--------------------------> mme apply : update_clock_structure -- BEGIN"
	UPOBJ update_clock_structure
	notice "--------------------------> mme apply : update_clock_structure -- END"





        ## make this optional
        #UPOBJ show_worst
        set_state gui_slack_min 1
        set_state gui_slack_max 1
        #gui_worst "initial" 0

        #UPOBJ update_opt_data

        ## run celtic once, first time through
        UPOBJ set_celtic_force

        gui_status main optimize "Multimode Ready"
        #up_opt size 0 0 ""
        send_node_status action "Waiting for main"
        pause
        after 2000

        notice "Multi-mode initialization finished at time [runtime_incr]"
    }
    VOB::add_args MME_SET_PRIORITY {
        {scenario nstr 0 ""}
    }
    VOB::add_help MME_SET_PRIORITY {
        {scenario "Scenario name that opt will focus" }
    }
    public method set_priority { args } {
        parse_args
	set pname [get_arg scenario]

	set ii 1
	set found 0
	set prev 0
        foreach sobj [state scenario_list] {
        	set p [$sobj cget -priority]
		if { $p==1 } {
			set prev $ii
		}
		if { [$sobj cget -name] eq $pname } {
			notice "Setting Optimization Priority to Scenario $ii"
			$sobj configure -priority 1
			set found $ii
			notice "Setting Optimization Priority [$sobj cget -priority] to Scenario $ii"
			break
		}
		incr ii
	}
	if { $found == 0 } {
		notice "Cannot find Scenario $pname"
		return
	}
	set ii 1
        foreach sobj [state scenario_list] {

		if { $prev==$ii } {
			$sobj configure -priority 2
			break
		}
		incr ii
	}
	set tmg [get_tm Tmg]
       	$tmg set_timing_engine -external -scenario $found
    }




    ### Finish up a multi-mode session

    public method finish_mme { } {

        ## do last update

        #UPOBJ update_timing

        set_state gui_slack_min 1
        set_state gui_slack_max 1
        #gui_worst current 0

        set_state multi OFF

        notice "Multi-mode analysis finished at time [runtime_incr]!"
    }

    VOB::add_args MME_SET_CELTIC_UPDATE_FREQUENCY { 
        { frequency num 1 }
    }
    VOB::add_help MME_SET_CELTIC_UPDATE_FREQUENCY { 
        { frequency "Update with Celtic (if enabled) every n timing updates" }
    }

    public method set_celtic_update_frequency { args } {
        parse_args
        UPOBJ set_celtic_update_freq [get_arg frequency]
    }

    private method next_remote { } {
        set nn $next_remote_name

        scan $next_remote_name "%c" nnc
        incr nnc
        set next_remote_name [format "%c" $nnc]

        return $nn
    }


    VOB::add_args MME_CREATE_SCENARIO {
        { name nstr 1 }
        { corner nstr 1 }
        { sdc_file ifile 1 }
        { ext_model_file ifile 0 }
        { priority num 0 2 }
        { engine nstr 0 "milos"}
        { start_name nstr 0 "" }
        { set_units num 0 "" }
        { preinit_file ifile 0 }
        { postinit_file ifile 0 }
        { tmp_dir nstr 0 "tmp" }
        { block nstr 0 "__top__top__" }
        { db nstr 0 "" }
        { translate bool 0 0 }
        { timeout num 0 30 }
        { spef_file nstr 0 "" }
        { restore_dir ifile 0 "" }
        { verilog_file nstr AND_verilog_file "" }
        { verilog_module nstr AND_verilog_module "" }
        { celtic_on bool AND_celticnoiselib 0}
        { celtic_start_name nstr 0 "ca celtic" }
        { celtic_noiselib iflist 0 "" }
        { celtic_vdd_value num 0 1.0 }
        { celtic_gnd_value num 0 0.0 }
        { celtic_vdd_signals nstr 0 "VDD" }
        { celtic_gnd_signals nstr 0 "GND" }
        { celtic_process nstr 0 "130nm" }
        { celtic_settings_file ifile 0 }
        { celtic_report_file ifile 0 }
        { celtic_copy_lib hbool 0 1 }
        { schedule_style hnum 0 1}
        { ignore_main_tmg hbool 0 0 }
    }

    VOB::add_help MME_CREATE_SCENARIO {
        { name "name of the scenario" }
        { block "Hierarchical Block Name" }
        { corner "name of the already-defined corner to apply to this scenario" }
        { sdc_file "name of the sdc file to use for this scenario" }
        { ext_model_file "name of extraction rules to use for this scenario (obsolete)" }

        { priority "the priority to assign to this scenario" }
        { engine "which timing engine to use for this scenario. Valid choices: quartz, pt, or psta" }
        { start_name "name of script to execute to start timing engine" }
        { set_units "conversion unit to use for remote engine" }
        { preinit_file "file to source during startup of timing engine before" }
        { postinit_file "file to source during startup of timing engine before" }
        { tmp_dir "temporary directory to use to store files - must be accessible to ALL machines" }
        { ignore_main_tmg "do not initialize embedded timer" }
        { db "Override list of .lib files with tool specific equivalent files" }
        { translate "Remove backslashes from design" }
        { timeout "How long to wait to initialize plugin" }
        { spef_file "Spef file to use for CLKWorks" }
        { verilog_file "Verilog file to use for plugin" }
        { verilog_module "Name of verilog module" }
        { celtic_on "Whether or not to use celtic" }
        { celtic_start_name "Which script to run to start celtic" }
        { celtic_noiselib "List of noise libraries for celtic" }
        { celtic_vdd_value "Value of vdd to use for celtic calculations" }
        { celtic_gnd_value "Value of gnd to use for celtic calculations" }
        { celtic_vdd_signals "VDD signals to consider" }
        { celtic_gnd_signals "GND signals to consider" }
        { celtic_settings_file "Celtic settings (initialization) file to source " }
        { celtic_report_file "Celtic file to source after noise analysis (for reports, etc)" }

    }
    public method create_scenario { args } {

        parse_args

        echo [cmdline]

        debug MME:A "mme create_scenario"
        debug MME:A "[settings]"

        if { ([get_arg engine] ne "milos") && ([get_arg start_name] eq "") } {
            set_arg start_name $START_NAME([get_arg engine])
            puts "Defaulting start name to: [get_arg start_name]"
        }

        if { [state multi] eq "DEFINE" } {
            if { ![check_arg sdc_file] } {
                error "parameter -sdc_file must be specified in multimode definition phase." "" -1
            }

            set output ""
            set priority [get_arg priority]
            set cobj ""
            set rc [catch { set cobj [find_obj corner_list corner [get_arg corner]]} output]
            if { $rc == 1 } {
                error "No corner named [get_arg corner] has been defined ($rc) ($output)!" "" -1
            }


            ## validate timing engine definition
            set engine [get_arg engine]
            if { [timing_name $engine] == "" } {
                error "Unrecognized timing engine name: $engine" "" -1
            }

            set name [get_arg name]
            set rc [ catch {[find_obj scenario_list name $name]} ]
            if { $rc == 0 } {
                error "Scenario $name has already been defined!" "" -1
            }
            
            set sobj [[SCENARIO #auto] this]
            class_configure $sobj
            $sobj set_cmd [rebuild_cmd mme create_scenario]

            set old_list [$cobj cget -sobj_list]
            lappend old_list $sobj
            $cobj configure -sobj_list $old_list

            if { $priority == 1 } {
                if { [state local_scenario] != "" } {
                    error "Only one scenario can have priority 1!" "" -1
                } else {
                    set_state local_scenario $sobj
                }

                ## see if corner definition needs to be done
                if { [$cobj get_flag] eq "" } {
                    set_state multi OFF
                    #puts "REDEFINING CORNER [get_arg corner]!"
                    call_milos "[$cobj get_cmd]"
                }
                call_milos "tmg check_lib"
                call_milos "tmg select_corner -corner [$cobj cget -corner]"
                call_milos "tmg read_sdc -sdc_file [$sobj cget -sdc_file]"
                set_state multi DEFINE
            } else {
                lappend_state rmt_scenarios $sobj
            }

            lappend_state scenario_list $sobj
            

        puts "Test: rmt_scenarios: [state rmt_scenarios]"
        puts "Test: scenario_list: [state scenario_list]"
        } else {

            if { [state multi] eq "" } {
                error "Create Scenario cannot be called except during multimode definition" "" -1
            }
            notice "Remote Create Scenario"
            set tmg [get_tm Tmg] 

            $tmg create_scenario -name [get_arg name] -corner [get_arg corner] -sdc [get_arg sdc_file] 
            $tmg set_current_scenario -name [get_arg name]
            $tmg set_scenario_priority -name [get_arg name] -priority [get_arg priority]
            $tmg get_current_scenario -name= ::sn
            notice "Current Scenario: $::sn"
            return 0
        }
    }

}


