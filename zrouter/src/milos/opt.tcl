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

## opt.tcl
## Optimization module for Milos
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::opt 0.1

itcl::class OPT {

    inherit VOB

    ## Public Vars
    ##

    ## End Public Vars

    ## Protected Vars
    ##

    ## End Protected Vars

    ## Private Vars

    ## End Private Vars

    protected method constructor { parent } { 
        VOB::constructor $parent 
        set NAME "MODULE"
        set SELFNAME "MODULE$this"
        ## ONLY for public vars
        set_state opt init

        set_state gui_slack_min 1
        set_state gui_slack_max 1
        set_state miller_effect 0
        set_state plugin_needs_miller 1
    }

    ## used to "close the hole"
    protected method info { } 

    protected method opt_reset { } {
        UPOBJ set_celtic_force
        if { [state multi] ne "ACTIVE" } {
            return
        }
        [get_tm Tmg] reset_activeterms
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]
        gui_send main msg ""
        #UPOBJ update_timing
    }

    protected method opt_init { } {

	#puts "opt_init: call_milos - tmg update_timing -- Dimitris -- BEGIN"

        call_milos "tmg update_timing"
	#puts "opt_init: call_milos - tmg update_timing -- Dimitris -- END"
    }

    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help OPT $cmd $param $long
        } else {
            mod_help OPT $cmd
        }
    }

    public method enable_miller { } {
        set_state miller_effect 1
        [get_tm Tmg] set_si_mcf
    }

    public method disable_miller { } {
        set_state miller_effect 0
        [get_tm Tmg] unset_si_mcf
    }

    VOB::add_args OPT_SET_PREFIX {
        { prefix str 1 }
    }
    VOB::add_help OPT_SET_PREFIX {
	{ prefix "prefix for newly created cells and nets to be used by subsequent optimizations"}
    }
    public method set_prefix { args } {
        parse_args
        echo [cmdline]
        set rc [[get_tm Tmg] set_prefix -prefix [get_arg prefix]]
        if { $rc != 0 } {
                error "Error $rc setting prefix." "" -1
        }
    }
    VOB::add_args OPT_REMOVE_BUFFERS {
	{test_undo hbool 0 0 }
	{preserve_hold bool 0 0 }
	{remove_all bool 0 0}
	{change_limit num 0 1000000}
	{iterations num 0 1000}
    }
    VOB::add_help OPT_REMOVE_BUFFERS {
	{test_undo "Undo removal uncconditionally" }
	{preserve_hold "preserve hold" }
	{remove_all "remove irrespective of slcak being negative"}
	{change_limit "upper bound on number of buffers removed"}
	{iterations "number of iterations"}
    }
    public method remove_buffers { args } {
        parse_args
        echo [cmdline]

        UPOBJ set_requirements "no no no no remove_buffers"
        #012710D UPOBJ set_requirements "1 1 0 0"
        UPOBJ enable_buffering

        set zp [get_tm Tmg]
        set cmd [build_cmd $zp remove_buffers]
        set rc [::eval $cmd]

        UPOBJ disable_buffering
        if { $rc != 0 } {
                error "Error $rc doing remove_buffers." "" -1
        }
    }

   VOB::add_args OPT_WIRE_SPREAD {
        { kfactor num 0 0.5 }
        { coupling num 0 0 }
        { change_limit num 0 1000 }
        { adjusted_nets nstr 0 }  
        { setup_target num 0 0 } 
        { only_wns num 0 10000 } 
	{ no_undo bool 0 0 }
   } 
   VOB::add_help OPT_WIRE_SPREAD {
        { kfactor "factor by which coupling caps will be adjusted"}
        { coupling "threshold for coupling" }
        { change_limit "number of nets affected"}
        { adjusted_nets "file in which adjusted nets written out" }  
        { setup_target "setup target" } 
        { only_wns "do only wns" } 
	{ no_undo "no undos" } 
   }
   public method wire_spread { args } {
        parse_args
        echo [cmdline]

        UPOBJ set_requirements "no [get_arg setup_target] no no remove_buffers"
        #UPOBJ set_requirements "0 1 0 0"
        UPOBJ enable_buffering 1

        set zp [get_tm ZPLACE]
        set cmd [build_cmd $zp wire_spread]
        set rc [::eval $cmd]

        UPOBJ disable_buffering
        if { $rc != 0 } {
                error "Error $rc doing wire_spread." "" -1
        }
   }

   VOB::add_args OPT_OPT_LEAKAGE {

        { change_limit num 0 }
        { slack_target num 0 0.0 }
        { area_limit_pct num 0 0.0 }
        { verbose bool 0 0 }
        { size_all bool 0 0 }
        { find_space num 0 50 }
   }
   VOB::add_help OPT_OPT_LEAKAGE {

        { change_limit "Maximum number of cell changes" }
        { slack_target "Desired Slack value after optimization" }
        { verbose "Enable Verbose mode (old only)" }
        { size_all "size sequential cells also (default only)" }
        { find_space "only size instance if there is some space in the vicinity (default only)" }
   }
    public method opt_leakage { args } {
        parse_args

        echo [cmdline]

        set_state mme_main_action "Sizing for Leakage"
        gui_send main action  [state mme_main_action]
        #set_state mme_update_freq 40

        set_state gui_slack_min 1
        set_state gui_slack_max 1

        opt_init

        set tmg [get_tm Tmg]
        set opt [get_tm ZPLACE]
        set cmd [build_cmd $tmg opt_leakage]

        UPOBJ set_requirements "[get_arg slack_target]  no size_for_leakage"
        #UPOBJ set_requirements "1 1 0 1"

        set rc [::eval $cmd]
        if { $rc != 0 } {
            notice "Error $rc in OPT::size_for_leakage,1"
        }
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]

        opt_reset

        update_opt_data
        cmd_rlog
    }
   VOB::add_args OPT_SIZE_FOR_HOLD {
        { change_limit num 0 }
        { slack_target num 0 0.0 }
        { setup_slack_target num 0 0.0 }
        { allow_upsize bool 0 }
        { verbose bool 0 0 }
        { inst nstr 0 } 
        { opt nstr 0 "" }
        { size_all bool 0 0 }
        { respect_fanout bool 0 0 }
        { find_space num 0 50 }
        { runtime num 0 0 }
        { only_size_down bool 0 0 }
        { iterations num 0 10 }
        { only_wns bool 0 0 }
   }

   VOB::add_help OPT_SIZE_FOR_HOLD {
        { _fcn "Size drivers to improve hold slacks" }
        { change_limit "Maximum number of cell changes" }
        { slack_target "Desired Slack value after optimization" }
        { setup_slack_target "Desired Slack value after optimization" }
        { allow_upsize "Allow upsizing of devices (old only)" }
        { verbose "Enable Verbose mode (old only)" }
        { inst "Sizes only this inst (old only)" }
        { opt "Optimization algorithm. Nothing (default) or 'old'"}
        { size_all "size sequential cells also (default only)" }
        { respect_fanout "Respect max fanout limit (default only)"}
        { find_space "only size instance if there is some space in the vicinity (default only)" }
        { runtime "finish in given runtime"} 
        { only_size_down "only size down"} 
        { iterations "number of iterations" }
        { only_wns bool "do only wns"} 

    }

    public method size_for_hold { args } {
        parse_args

        echo [cmdline]

        set_state mme_main_action "Sizing for Hold"
        gui_send main action  [state mme_main_action]
        #set_state mme_update_freq 40

        set_state gui_slack_min 1
        set_state gui_slack_max 1

        opt_init

        set tmg [get_tm Tmg]

        set opt [get_arg opt]
        if { $opt ne "old" } {
            keep_arg change_limit slack_target size_all find_space respect_fanout runtime only_size_down iterations only_wns setup_slack_target
            set cmd [build_cmd $tmg new_sizing]
            append cmd "-hold "
            if { [state multi] eq "OFF" } {
              append cmd "-single_mode"
            } 
        } else {
            keep_arg change_limit slack_target allow_upsize verbose inst setup_slack_target
            set cmd [build_cmd $tmg opt_size_for_hold]
            if { [state multi] != "OFF" } {
                append cmd " -multi_mode"
            }
        }

        UPOBJ set_requirements "[get_arg slack_target] [get_arg setup_slack_target] no no size_for_hold"
        #UPOBJ set_requirements "1 1 0 1"

        set rc [::eval $cmd]
        if { $rc != 0 } {
            notice "Error $rc in OPT::size_for_hold,1"
        }
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]

        opt_reset

        update_opt_data
        cmd_rlog
    }

    VOB::add_args OPT_SIZE_FOR_SETUP {
        { area_limit_pct num 0 0.5 }
        { change_limit num 0 10 }
        { slack_target num 0 0.0 }
        { verbose bool 0 }
        { legal bool 0 0 }
        { size_all bool 0 0 }
        { size_only_seq bool 0 0 }
        { find_space num 0 50 }
        { setup_incr hbool 0 1 }
        { opt str 0 "new"}
        { active_size hnum 0 }
        { respect_fanout bool 0 0 }
        { runtime num 0 0 }
        { test_undo hbool 0 }
        { iterations num 0 10000 }
        { continue_on bool 0 0 }
        { only_wns bool 0 0 }
        { num_sized_per_iteration num 0 1000 }
        { ocv bool 0 0 }
    }

    VOB::add_help OPT_SIZE_FOR_SETUP {
        { _fcn "Size drivers to improve setup slacks" }
        { area_limit_pct "Maximum percentage change in standard cell area" }
        { change_limit "Maximum number of cell changes" }
        { slack_target "Desired Slack value after optimization" }
        { verbose "Run in verbose mode" }
        { legal "Legalize in place" }
        { size_all "size sequential cells also" }
        { size_only_seq "size sequential cells only" }
        { find_space "only size instance if there is some space in the vicinity" }
        { only_wns "boolean parameter for wns optimization"}
        { num_sized_per_iteration "number of instances sized per iteration"}

   
        { opt "Optimization algorithm. Valid values: 7,8,new"}
        { respect_fanout "Respect max fanout limit"}
        { runtime "finish in the runtime given"}
        { test_undo "undo all sizings"}
        { iterations "num of iterations"}
        { continue_on "continue on even if things get worse in the current iteration"}
        { ocv "Run using OCV mode (as opposed to single corner)" }
    }

    public method size_for_setup { args } {
        parse_args

        set_state mme_main_action "Sizing for Setup"
        #set_state mme_update_freq 40

        set_state gui_slack_min 0
        set_state gui_slack_max 1

        gui_send main action  [state mme_main_action]

        echo [cmdline]

        opt_init

	set max_target_slack [get_arg slack_target]
	set min_target_slack $max_target_slack
	set max_target_slew "no_limit"
	set min_target_slew "no_limit"

        UPOBJ set_requirements "$min_target_slack $max_target_slack $min_target_slew $max_target_slew size_for_setup"
        #012710D if { [get_arg ocv] == 1 } {
            #UPOBJ set_requirements "1 1 1 1"
        #} else {
            #UPOBJ set_requirements "0 1 0 1"
        #}

        set tmg [get_tm Tmg]

        set opt [get_arg opt]
        if { $opt eq "new" } {
            keep_arg change_limit slack_target size_all find_space respect_fanout runtime iterations test_undo continue_on only_wns num_sized_per_iteration 
            set cmd [build_cmd $tmg new_sizing]
        } else {
            rm_arg runtime iterations num_sized_per_iteration only_wns continue_on test_undo ocv
            set cmd [build_cmd $tmg opt_size]
        }
        #puts "size built cmd: $cmd"

        set rc [::eval $cmd]
        if { $rc != 0 } {
            notice "Error $rc in OPT::size,1"
        }

        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]
        opt_reset
        cmd_rlog
    }
    VOB::add_args OPT_MAKE_LEGAL {
        { destroy_wires bool 0 0 }
        { move_flops bool 0 0 }
        { debug_file ofile 0 }
        { delta num 0 }
        { do_timing bool 0}
        { move_legal  bool 0 0}
    }

    VOB::add_help OPT_MAKE_LEGAL {
        { destroy_wires "Destroy wires" }
        { move_flops "Move flops during placement" }
        { debug_file "Write placement details to this file" }
        { delta "Overlapping instances within delta microns are moved" }
        { do_timing "Update spef and do timing after legalization for external timers" }
        { move_legal  "Move also legal instances" }

    }

    VOB::add_alias OPT_MAKE_LEGAL {
        { debug_file message }
    }

    public method make_legal {args } {

        echo [cmdline]
        parse_args 
        set_state mme_main_action "Legalizing Placement"
        gui_send main action  [state mme_main_action]
        set opt [get_tm ZPLACE]

        set rc [$opt check_legality]
        if { $rc != 0 } {
            set cmd [build_cmd $opt legalize_placement]
            set rc [::eval $cmd]
            if { $rc != 0 } {
                notice "Error $rc in OPT::make_legal,1"
            }
            set rc [$opt check_legality]
            if { $rc != 0 } {
                notice "Error $rc in OPT::make_legal,2"
            }
        }
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]

        opt_reset
        update_opt_data

        cmd_rlog
    }

    VOB::add_args OPT_BUFFER_FOR_HOLD {
        {change_limit num 0 0}
        {hold_thresh hnum 0 0}
        {pin nstr 0}
        {undo_bad_changes bool 0 1}  
        {test_undo hbool 0}
        {iterations num 0 4}  
        {find_space num 0 50}
        {setup_target num 0 0.0}
        {hold_target num 0 0.0}
        {ratio num 0 1.0}
        {number_per_iteration num 0 1}
        {only_wns bool 0 0}  
    }
    VOB::add_help OPT_BUFFER_FOR_HOLD {
        {_fcn "Insert buffers to improve hold slacks" }
        {change_limit "hold fixing" }
        {hold_thresh "hold threshold" }
        {pin "input pin where hold buffers need to be added" } 
        {undo_bad_changes "undo bad changes"}  
        {test_undo "always undo"}
        {iterations "number of iterations"}
        {find_space "look around delta microns for space"}
        {ratio "multiplicative factor for number of sites around"}
        {setup_target "setup slack to be targetted"}
        {hold_target "hold slack to be targetted" }
        {number_per_iteration num "number of hold buffers inserted at a load per iteration"}
        {only_wns "optimizes only wns"}
    }

    VOB::add_alias OPT_BUFFER_FOR_HOLD {
        { change_limit num_fixed }
    }

    public method buffer_for_hold {args } {
        parse_args
        echo [cmdline]

        opt_init

        UPOBJ enable_buffering

        set_state mme_main_action "Buffering for Hold"
        set_state gui_slack_min 1
        set_state gui_slack_max 1
        gui_send main action  [state mme_main_action]
        #set_state mme_update_freq 40

        set opt [get_tm ZPLACE]
        set cmd [build_cmd $opt "fix_for_hold"]
if { [state multi] != "OFF" } {
            append cmd " -multi_mode"
        }

	set max_target_slack [get_arg setup_target]
	set min_target_slack [get_arg hold_target]
	set max_target_slew "no_limit"
	set min_target_slew "no_limit"

        UPOBJ set_requirements "$min_target_slack $max_target_slack $min_target_slew $max_target_slew buffer_for_hold"
        #UPOBJ set_requirements "1 1 0 1"
        set rc [::eval $cmd]
        if { $rc != 0 } {
            notice "Error $rc in OPT::hold,1"
        }
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]


        opt_reset

        UPOBJ disable_buffering

        set_state slacks_needed 1
        cmd_rlog
    }

    VOB::add_args OPT_BUFFER_FOR_SETUP {
        { nets slist 0 "" }
        { verbose bool 0 }
        { num_worst_nets num 0 }
        { change_limit num 0 }
        { slack_target num 0 0.0 }
        { slew_target num 0 0.0 }
        { reuse_wires bool 0 1 }
        { min_area bool 0 0 } 
        { ignore_max_cap bool 0 0 } 
        { levels 0 0 }
        { test_undo hbool 0 }
        { do_not_legalize bool 0 }
        { coupled_rc bool 0 0 }
        { ocv bool 0 0 }
        { dual_incr_spef bool 0 0 }
        { opt_tns bool 0 0 }
    }

    VOB::add_help OPT_BUFFER_FOR_SETUP {
        { _fcn "Insert buffers to improve setup slacks" }
        { nets "List of nets to buffer" }
        { verbose "Use verbose mode" }
        { num_worst_nets "Number of worst nets to buffer (same as -change_limit)" }
        { change_limit "Number of worst nets to buffer (same as -num_worst_nets)" }
        { slack_target "Slack target to attempt to meet" }
        { slew_target "Slew target to attempt to meet" }
        { reuse_wires "Keep original wires in place for router to try to re-use" }
        { min_area "Get best possible slack with min area of buffers" }
        { ignore_max_cap "Ignore max_cap criterion" }
        { levels "Maxium number of levels removed in a buffer tree" }
        { do_not_legalize bool "Do not legalize inserted buffers" }
        { coupled_rc "Keep coupling caps when buffering" }
        { ocv "Run using OCV mode (as opposed to single corner)" }
        { dual_incr_spef "read one clean-up spef and one incremental spef)" }
        { opt_tns "optimize tns" }
    }

    VOB::add_alias OPT_BUFFER_FOR_SETUP {
        {change_limit num_worst_nets}
    }

    public method buffer_for_setup { args } {
        parse_args

        echo [cmdline]
        #echo [settings]

        opt_init

        UPOBJ enable_buffering

        set_state mme_main_action "Buffering for Setup"
        #set_state mme_update_freq 20
        set_state gui_slack_min 0
        set_state gui_slack_max 1
        gui_send main action  [state mme_main_action]
        set opt [get_tm ZPLACE]

        set cmd [build_cmd $opt buffer_nets]
        if { [state multi] != "OFF" } {
        }
        #append cmd " -multi_mode"

	set max_target_slack [get_arg slack_target]
	set min_target_slack $max_target_slack
	set max_target_slew [get_arg slew_target]
	if { $max_target_slew == 0.0 } {
		set max_target_slew "no_limit"
	}
	set min_target_slew $max_target_slew
        UPOBJ set_requirements "$min_target_slack $max_target_slack $min_target_slew $max_target_slew buffer_for_setup"
        #if { [get_arg ocv] == 1 } {
            #UPOBJ set_requirements "1 1 1 1"
        #} else {
            #UPOBJ set_requirements "0 1 0 1"
        #}

        if { [get_arg dual_incr_spef] == 1 } {
            UPOBJ set_dual_incr_spef
        }

        if { ([state rc_models_si] > 0) && ([get_arg coupled_rc] == 0) } {
            append cmd " -coupled_rc "
        }
        set rc [::eval $cmd]
        if { $rc != 0 } {
            notice "Error $rc in OPT::buffer,1"
        }
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]

        UPOBJ disable_buffering

        opt_reset
        cmd_rlog
    }

    VOB::add_args OPT_FIX_SLEW {
        { max_delta_width num 0 20.0 }
        { size_only bool NOT_buffer_only 0 }
        { buffer_only bool NOT_size_only 0 }
        { net nstr 0 }
        { reuse_wires bool 0 1 }
        { verbose bool 0 0 }
        { find_space num 0 50 }
        { multiplier num 0 0.95}
        { ocv bool 0 0 }
    }

    VOB::add_help OPT_FIX_SLEW {
        { max_delta_width "Maximum increase in cell width" }
        { size_only "Only do sizing (no buffering)" }
        { buffer_only "Only do buffering (no sizing)" }
        { net "Specific net to fix" }
        { reuse_wires "Reuse wires during optimization" }
        { verbose "Turn on verbose mode" }
        { find_space "size only if space available in vicinity"}
        { multiplier "scale factor for slew_limit"} 
        { ocv "Run using OCV mode (as opposed to single corner)" }
    }

    public method fix_slew { args } {

        parse_args
        parasitics_check

        echo [cmdline]
        UPOBJ enable_buffering

        set_state mme_main_action "Fixing Slew"
        set_state gui_slack_min 1
        set_state gui_slack_max 1
        gui_send main action  [state mme_main_action]

        opt_init


        UPOBJ set_requirements "no no no no fix_slew"
        if { [get_arg ocv] == 1 } {
            UPOBJ set_requirements "0 0 1 1"
        } else {
            UPOBJ set_requirements "0 0 0 1"
        }
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg fix_slew]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in fix_slew" "" -1
        }
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]

        opt_reset
        UPOBJ disable_buffering

        update_opt_data
        cmd_rlog
    }

    VOB::add_args OPT_LEAKAGE_POWER {
        { slack_target num 0 0.0 }
        { inst_list nstr 0 }
        { verbose bool 0 0 }
        { use_same_footprint bool NOT_allow_upsizing 0}
        { allow_upsizing bool NOT_use_same_footprint 0}
        { size_untimed_insts bool 0 }
        { do_fast hbool 0 }
        { num_pass num 0 1}
    }

    VOB::add_help OPT_LEAKAGE_POWER {
        { _fcn "Improve leakage power" }
        { slack_target "Slack Target" }
        { inst_list "Recover power for the specified insts" }
        { verbose "Run in verbose mode" }
        { use_same_footprint "Use only replacement devices with the same footprint." }
        { allow_upsizing "Allow replacement devices with significantly different footprints."}
        { size_untimed_insts "Allow sizing of instances with no timing data." }
        { num_pass "Number of iterations to run for fast mode" }
    }

    VOB::add_alias OPT_LEAKAGE_POWER {
        { do_fast doFast }
    }

    public method leakage_power { args } {

        parse_args
        parasitics_check
        echo [cmdline]

        opt_init

        set_state mme_main_action "Leakage Power"
        set_state gui_slack_min 1
        set_state gui_slack_max 1
        gui_send main action  [state mme_main_action]

        if { ([check_arg do_fast]) && ([get_arg do_fast] == 1) } {
            if { [state slack_snapshot] == 0 } {
                notice "Leakage Power with fast option requires Slack Snapshot."
                notice "Current Snapshot invalid, generating Slack Snapshot."
                call_milos "tmg slack_snapshot"
            }
        }

        UPOBJ set_requirements "no [get_arg slack_target] no no leakag_power"
        #UPOBJ set_requirements "1 1 1 1"
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "recover_leakage_power"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in leakage_power." "" -1
        }
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]

        opt_reset
        update_opt_data
        cmd_rlog
    }

    VOB::add_args OPT_FIX_MAX_CAP {
        { max_delta_width num 0 20.0 }
        { net nstr 0 }
        { reuse_wires bool 0 1 }
        { verbose bool 0 0 }
        { size_only bool NOT_buffer_only 0 }
        { buffer_only bool NOT_size_only 0 }
	{ respect_fanout bool 0 0 } 
    }

    VOB::add_help OPT_FIX_MAX_CAP {
        { _fcn "Ensure maximum capacitance is within limits" }
        { max_delta_width "Maximum allowed increase in cell width" }
        { net "Nets for which to fix max_cap" }
        { reuse_wires "Reuse wires during optimization" }
        { verbose "Enable verbose mode" }
        { buffer_only "Only buffer" }
        { size_only "Only size" }
	{ respect_fanout "respect fanout constraints"}
    }

    public method fix_max_cap { args } {
        parse_args
        parasitics_check
        echo [cmdline]

        UPOBJ enable_buffering

        set_state mme_main_action "Fix MAX_CAP"
        set_state gui_slack_min 1
        set_state gui_slack_max 1
        gui_send main action  [state mme_main_action]

        opt_init

        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "fix_max_cap"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in fix_max_cap." "" -1
        }
        set_state mme_main_action ""
        gui_send main action  [state mme_main_action]

        opt_reset
        UPOBJ disable_buffering

        update_opt_data
        cmd_rlog
    }

    VOB::add_args OPT_SET_DONT_SIZE {
        { lib_cells bool 0 }
        { cells bool 0 }
        { list nstr 0 }
        { pattern nstr 0 }
    }

    VOB::add_help OPT_SET_DONT_SIZE {
        { lib_cells "Apply list/pattern to library cells" }
        { cells "Apply list/pattern to regular cells" }
        { list "List of objects to not size" }
        { pattern "Pattern of objects to not size" }
    }

    public method set_dont_size { args } {
        parse_args
        echo [cmdline]
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "set_dont_size"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in set_dont_size" "" -1
        }
        cmd_rlog
    }

    VOB::add_args OPT_SET_DONT_TOUCH {
        { nets bool 0 }
        { cells bool 0 }
        { list nstr 0 }
        { pattern nstr 0 }
        { clocks bool 0 }
        { verbose bool 0 }
    }

    VOB::add_help OPT_SET_DONT_TOUCH {
        { nets "Apply list/pattern to nets" }
        { cells "Apply list/pattern to regular cells" }
        { list "List of objects to not touch" }
        { pattern "Pattern of objects to not touch" }
        { clocks "Set dont_touch attribute on all clock networks" }
        { verbose "Report list of 'dont_touch'ed clock nets and cells" }
    }

    private method get_clock_objects { node type } {
        # manage the temporary directory
        set tempDir ./tmp
        if { [file exists $tempDir] && ![file isdirectory $tempDir] } {
            error "Error in export_timing_view: please delete file $tempDir" "" -1
        }

        if { ![file exists $tempDir] } {
            if { [catch { file mkdir $tempDir } errMsg] } {
                error "Error '$errMsg' in get_clock_objects" "" -1
            }
        }

        set nodeName [[$node get_sobj] cget -name]
        set clockFile $tempDir/$nodeName.clk.$type
        close [open $clockFile w]; # this !@#$#% F*** TCL doesn't update the file list until back to the prompt

        set clockObjs ""
        if { $type == "net" } {
            call_node $node "tmg get_clock_nets  -file $clockFile"
        } else {
            call_node $node "tmg get_clock_cells -file $clockFile"
        }

        set clockH [open $clockFile r]
        while { [gets $clockH obj] >= 0 } {
            set obj [string trim $obj]
            if { $obj == "" } { continue }
            lappend clockObjs $obj
        }

        close $clockH
        return $clockObjs
    }

    public method set_dont_touch { args } {
        parse_args
        echo [cmdline]
        set tmg [get_tm Tmg]

        if { ![check_arg clocks] } {
            set cmd [build_cmd $tmg "set_dont_touch"]
            set rc [::eval $cmd]
            if { $rc != 0 } {
                error "Error $rc in set_dont_touch" "" -1
            }
        } else {
            array set clockNets  {}
            array set clockCells {}

            # Main/Standlone
            set ::objs ""
            $tmg get_clock_nets -clock_nets= ::objs
            foreach obj [lindex $::objs 0] { set clockNets($obj) 1 }

            set ::objs ""
            $tmg get_clock_cells -clock_cells= ::objs
            foreach obj [lindex $::objs 0] { set clockCells($obj) 1 }

            # if MME get it from all nodes
            if { [state multi] != "OFF"  } {
                set nodes [UPOBJ get_rmt]
                foreach node $nodes {
                    set objs [get_clock_objects $node net]
                    foreach obj [lindex $objs 0] { set clockNets($obj) 1 }
                    set objs [get_clock_objects $node cell]
                    foreach obj [lindex $objs 0] { set clockCell($obj) 1 }
                }
            }

            if { [array size clockNets] != 0 } {
                set rc [$tmg set_dont_touch -nets -list [array names clockNets]]
                if { $rc != 0 } {
                    error "Error $rc in set_dont_touch" "" -1
                }

                if { [check_arg verbose] } {
                    foreach clockNet [array names clockNets] {
                        notice "set 'dont_touch' attribute on clock net '$clockNet'"
                    }
                }
            }

            if { [array size clockCells] != 0 } {
                set rc [$tmg set_dont_touch -cells -list [array names clockCells]]
                if { $rc != 0 } {
                    error "Error $rc in set_dont_touch" "" -1
                }

                if { [check_arg verbose] } {
                    foreach clockCell [array names clockCells] {
                        notice "set 'dont_touch' attribute on clock cell '$clockCell'"
                    }
                }
            }
        }

        cmd_rlog
    }

    VOB::add_args OPT_RESET_DONT_SIZE {
        { lib_cells bool 0 }
        { cells bool 0 }
        { list nstr 0 }
        { pattern nstr 0 }
    }

    VOB::add_help OPT_RESET_DONT_SIZE {
        { lib_cells "Apply list/pattern to library cells" }
        { cells "Apply list/pattern to regular cells" }
        { list "List of objects allowed to be sized" }
        { pattern "Pattern of objects allowed to be sized" }
    }

    public method reset_dont_size { args } {
        parse_args
        echo [cmdline]
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "reset_dont_size"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in reset_dont_size" "" -1
        }
        cmd_rlog
    }

    VOB::add_args OPT_SET_DONT_USE {
        { list nstr 0 }
        { pattern hnstr NOT_list }
    }

    VOB::add_help OPT_SET_DONT_USE {
        { list "List of objects to not size" }
        { pattern "Pattern of objects to not size" }
    }

    public method set_dont_use { args } {
        parse_args
        echo [cmdline]
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "set_dont_use"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in set_dont_use" "" -1
        }
        cmd_rlog
    }

    VOB::add_args OPT_RESET_DONT_USE {
        { list nstr 0 }
        { pattern hnstr NOT_list }
    }

    VOB::add_help OPT_RESET_DONT_USE {
        { list "List of objects allowed to be sized" }
        { pattern "Pattern of objects allowed to be sized" }
    }

    public method reset_dont_use { args } {
        parse_args
        echo [cmdline]
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "reset_dont_use"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in reset_dont_use" "" -1
        }
        cmd_rlog
    }

    VOB::add_args OPT_IGNORE_REGIONS {
        { list nstr 1 }
    }
    VOB::add_help OPT_IGNORE_REGIONS {
        { list "List of regions to ignore during optimization" }
    }

    public method ignore_regions { args } {
        parse_args
        echo [cmdline]
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "ignore_regions"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in ignore_regions" "" -1
        }
        cmd_rlog
    }

    VOB::add_args OPT_RESPECT_REGIONS {
        { list nstr 1 }
    }
    VOB::add_help OPT_RESPECT_REGIONS {
        { list "List of regions to respect during optimization" }
    }

    public method respect_regions { args } {
        parse_args
        echo [cmdline]
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "respect_regions"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in respect_regions" "" -1
        }
        cmd_rlog
    }

    VOB::add_args OPT_SIZE_FOR_LEAKAGE {
        { slack_target num 0 }
        { change_limit num 0 }
        { use_same_footprint bool 0 }
        { allow_upsizing bool 0 }
        { no_timing bool 0 }
	{ runtime num 0 0 } 
        { iterations num 0 1000000}
	{ test_undo hbool 0 }
        { savings_required num 0 5 }
        { min_area bool 0 0 } 
         
    }
    VOB::add_help OPT_SIZE_FOR_LEAKAGE {
        { slack_target "Slack limit to respect" }
        { change_limit num "Maximum number of instances to size" }
        { use_same_footprint  "Use only same footprint cells to size" }
        { allow_upsizing "Allow upsizing of cells" }
        { no_timing  "Ignore timing data" }
	{ runtime "finish in given runtime"}
        { iterations "number of iterations"}
        { test_undo "undo every sizing if on: only for testing"}
        { savings_required "percentage savings required per instance"}  
        { min_area bool "minimize area"} 
    }

    public method size_for_leakage { args } {
        parse_args
        echo [cmdline]

        opt_init

        #UPOBJ set_requirements "1 1 0 1"
        UPOBJ set_requirements "no [get_arg slack_target] no no size_for_leakage"
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "size_for_leakage"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in size_for_leakage" "" -1
        }
        cmd_rlog
        opt_reset
    }

    VOB::add_args OPT_FIX_GLITCH {
        {size_only bool 0}
        {buffer_only bool 0}
        {reuse_wires bool 0 1}
        {find_space bool 0 50}
        {max_delta_width num 0}
        {glitch_report ifile 1}
        {vdd num 0}
        {vss num 0}
        {preserve_setup bool 0 0}
        {preserve_hold bool 0 0}
    }

    VOB::add_alias OPT_FIX_GLITCH {
        {glitch_report celtic_file}
        {vdd VDD}
        {vss VSS}
    }

    VOB::add_help OPT_FIX_GLITCH {
        {size_only "Only do instance sizing" }
        {buffer_only "Only do buffer insertion" }
        {reuse_wires "Try to follow existing routes"}
        {find_space "Size instances only if sufficient space available in vicinity"}
        {max_delta_width "Maximum increase in cell width" }
        {glitch_report "Glitch report file to read" }
        {vdd "VDD value" }
        {vss "VSS value" }
        {preserve_setup "preserve setup slack while fixing glitch" }
        {preserve_hold "preserve hold slack while fixing glitch" }
    }


    public method fix_glitch { args } {
        parse_args
        echo [cmdline]
        opt_init

        UPOBJ enable_buffering

        UPOBJ set_requirements "no no no no fix_glitch"
        #UPOBJ set_requirements "1 1 0 1"
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "fix_glitch"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in fix_glitch"
        }
        cmd_rlog
        UPOBJ disable_buffering
        opt_reset
    }

    VOB::add_args OPT_SET_GLITCH_BUF_LIST {
        {list nstr 1}
    }

    public method set_glitch_buf_list { args } {
        parse_args
        if { [state tmg_libs_loaded] ne 1 } {
            error "This command can only be called after loading timing libraries!"
        }
        echo [cmdline]
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "set_glitch_buf_list"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in set_glitch_buf_list"
        }

        cmd_rlog
    }

    VOB::add_args OPT_SET_GLITCH_INV_LIST {
        {list nstr 1}
    }

    public method set_glitch_inv_list { args } {
        parse_args
        if { [state tmg_libs_loaded] ne 1 } {
            error "This command can only be called after loading timing libraries!"
        }
        echo [cmdline]
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "set_glitch_inv_list"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in set_glitch_inv_list"
        }

        cmd_rlog
    }

    VOB::add_args OPT_SET_DONT_TOUCH_REGION {
        {top num 1}
        {bottom num 1}
        {left num 1}
        {right num 1}
    }

    VOB::add_help OPT_SET_DONT_TOUCH_REGION {
        {top "Top of dont_touch region (in microns)" }
        {bottom "Bottom of dont_touch region (in microns)" }
        {left "Left edge of dont_touch region (in microns)" }
        {right "Right edge of dont_touch region (in microns)" }
    }
    public method set_dont_touch_region { args } {
        parse_args
        echo [cmdline]

        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg set_dont_touch_region]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in set_dont_touch_region"
        }

        cmd_rlog
    }

    VOB::create_enum tfs {"balance" "preserve_setup" "preserve_hold" "opt_setup" "opt_hold" }
    VOB::add_args OPT_FIND_SCHEDULE {
        { slack_limit num 0 1.0 }
        { hold_slack_limit num 0 1.0 }
        { file nstr 1 }
        { pfile nstr 1 }
        { compress bool 0 1 }
        { split bool 0 0 }
        { goal enum_tfs 0 "balance" }
        { setup_target num 0 0 }
        { hold_target num 0 0 }
        { implement bool 0 1 } 
        { io_slack num 0 0 } 
        { corner nstr 0 }
        { use_report_file ifile 0 "" }
        { skip_endpoint_setup bool 0 1}
         
    }
    VOB::add_help OPT_FIND_SCHEDULE {
        { slack_limit "from to's less than this slack"}
        { hold_slack_limit "from to's less than this hold slack"}
        { file "Name of the file to dump shcedule in"}
        { pfile "Name of the file to dump periods in" }
        { compress "Use gzip (less disk space, more runtime)" }
        { split "Split up the reporting task between clone corners" }
        { goal "The optimization goal" }
        { implement "actually implement schedule" } 
        { setup_target "optimize setup slacks only less than this target" }
        { hold_target "optimize hold slacks only less than this target" }
        { io_slack "io_slack" } 
        { corner "corner on which delays computed"}
        { use_report_file "re-use an existing report file" }
        { skip_endpoint_setup "Endpoint calculation is done by: 0 - milos, 1 - plugin" }
        
    }   
    public method find_schedule { args } {
        if { [state db_loaded] != 1 } {
            notice "Please load a design first!"
            return 1
        }   
        
        parse_args
        set goal [get_arg goal]
        rm_arg goal

        set_state use_report_file [get_arg use_report_file]
        rm_arg use_report_file
        
        set_state compress_reports [get_arg compress]
        rm_arg compress

        set_state split_reports [get_arg split]
        rm_arg split

        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg find_schedule]

        ## -detail flag is needed at ADE level. Code that used to support
        ## old non-detail method is gone. -detail is no longer a "choice"

        # Dimitris 12/04/09 append cmd " -detail "
        set max_slack [get_arg setup_target]
        set min_slack [get_arg hold_target]

        UPOBJ enable_buffering
        switch $goal {
            balance { 
                UPOBJ set_requirements "$min_slack $max_slack no no find_schedule"
                #UPOBJ set_requirements "1 1 0 0"
            }   
            preserve_setup {
                UPOBJ set_requirements "$min_slack $max_slack no no find_schedule-preserve_setup"
                #UPOBJ set_requirements "1 1 0 0"
                append cmd " -preserve_setup"
            }   
            preserve_hold {
                UPOBJ set_requirements "$min_slack $max_slack no no find_schedule-preserve_hold"
                #UPOBJ set_requirements "1 1 0 0"
                append cmd " -preserve_hold"
            }   
            opt_setup {
                UPOBJ set_requirements "$min_slack $max_slack no no find_schedule-only_setup"
                #UPOBJ set_requirements "0 1 0 0"
                append cmd " -only_setup"
            }
            opt_hold {
                UPOBJ set_requirements "$min_slack $max_slack no no find_schedule-only_hold"
                #UPOBJ set_requirements "1 0 0 0"
                append cmd " -only_hold"
            }
        }

        set_state node_slack_status "invalid"

        set rc [::eval $cmd]

        if { $rc != 0 } {
            UPOBJ disable_buffering
            error "Error $rc while executing find_schedule" "" -1
        }
        UPOBJ disable_buffering

    }


}
