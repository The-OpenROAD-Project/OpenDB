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

## tmg.tcl
## Timing/Extraction module for Milos
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::tmg 0.1


itcl::class TMG {

    inherit VOB

    ## Protected Vars
    ##

    ## End Protected Vars


    protected method constructor { parent } { 
        VOB::constructor $parent 
        set NAME "Timing"
        set SELFNAME "MODULE$this"
        set_state tmg init
        set_state slack_snapshot 0

        set_state corner_list {}
        set_state cur_corner ""

        set_state rc_models_si 0

        set_state data_available 0
        set_state slacks_updated 0
        set_state endpoints_updated 0

        set_state thresh_obj ""
        set ::scen_idx 0

        set_state plugin_cmd_list {}
    }

    ## used to "close the hole"
    protected method info { }

    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help TMG $cmd $param $long
        } else {
            mod_help TMG $cmd
        }
    }

    VOB::add_args TMG_SET_SUPPLY_GROUP_OPERATING_CONDITION {
        { corner nstr 1 }
        { op_cond nstr 0 }
        { min_op_cond nstr 0 }
        { max_op_cond nstr 0 }
        { group_name nstr 1 }
    }
    VOB::add_help TMG_SET_SUPPLY_GROUP_OPERATING_CONDITION {
        { corner "Name of corner to apply conditions to" }
        { op_cond "Name of operating condition for min and max" }
        { min_op_cond "Name of operating condition for min" }
        { max_op_cond "Name of operating condition for max" }
        { group_name "Group to apply conditions to" }
    }

    public method set_supply_group_operating_condition { args } {
        if { [state db_loaded] != 1 } {
            notice "Please load a design first!"
            return 1
        }

        parse_args
        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg set_supply_group_operating_condition]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc while executing set_supply_group_operating_condition" "" -1
        }
    }


    VOB::add_args TMG_SET_DEFAULT_THRESHOLD {
        { upper_rise_pct num }
        { upper_fall_pct num }
        { upper_pct num }
        { lower_rise_pct num }
        { lower_fall_pct num }
        { lower_pct num }
        { derate    num }
    }

    VOB::add_help TMG_SET_DEFAULT_THRESHOLD {
        { upper_rise_pct "upper measurement point, percent of vdd" }
        { upper_fall_pct "upper measurement point, percent of vdd" }
        { upper_pct "upper measurement point, percent of vdd, both rise and fall" }
        { lower_rise_pct "lower measurement point, percent of vdd" }
        { lower_fall_pct "lower measurement point, percent of vdd" }
        { lower_pct "lower measurement point, percent of vdd, both rise and fall" }
        { derate "(delta of measure points)/(reported slew)" }
    }

    public method set_default_threshold { args } {
        set tmg [get_tm Tmg]
        parse_args
        echo [cmdline]

        set tobj [[THRESH #auto] this]
        class_configure $tobj 
        $tobj set_cmd [cmdline]
        set_state thresh_obj $tobj

        set cmd [build_cmd $tmg set_default_thresholds]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc while executing set_default_threshold" "" -1
        }
        set cmd [rebuild_cmd tmg set_default_threshold]
        set_state "thresh_init_cmd" $cmd
    }


    VOB::add_args TMG_DEFINE_CORNER {
        { corner nstr 1 } 
        { min_derate num }
        { max_derate num }
        { ext_corner nstr NOT_max_ext_corner }
        { min_ext_corner nstr AND_max_ext_corner }
        { max_ext_corner nstr AND_min_ext_corner }
        { op_cond nstr NOT_min_op_cond }
        { min_op_cond nstr AND_max_op_cond }
        { max_op_cond nstr AND_min_op_cond }
        { lib iflist OR_min_lib }
        { min_lib iflist AND_max_lib }
        { max_lib iflist AND_min_lib }
        { sdf_file iflist 0 }
        { delay_active bool 0 0} 
        { delay_exec_name nstr 0 "celtic_shell" }
        { delay_parameter_string nstr 0 "%verilog% %spef% %sdc% {%lib%} %vdd% %vdd_value% %gnd% %gnd_value% %outfile%" }

    }

    VOB::add_help TMG_DEFINE_CORNER {
        { _fcn "Defines an operating corner for timing" }
        { corner "Name of corner to define" } 
        { min_derate "Derating factor for min delays" }
        { max_derate "Derating factor for max delays" }
        { ext_corner "Extraction corner name to use for min and max" } 
        { min_ext_corner "Extraction corner for min" }
        { max_ext_corner "Extraction corner for max" }
        { op_cond nstr "Operating condition for min and max"}
        { min_op_cond "Operating condition for min" }
        { max_op_cond "Operating condition for max" }
        { lib "List of timing libraries to use for min and max" }
        { min_lib "List of timing libraries to use for min" }
        { max_lib "List of timing libraries to use for max" }
        { sdf_file "SDF file" }
        { delay_active "Use an external delay calculator before calling PT for CW" }
        { delay_exec_name "Script used to call external delay calculator" }
        { delay_parameter_string "String to format parameters for external delay calculator" }


    }

 method define_corner { args } {

        set tmg [get_tm Tmg]

        parse_args

        echo [cmdline]

        if { [check_arg ext_corner] == 1 } {
            set ec [get_arg ext_corner]

            if { [check_extraction_corner $ec] == 0 } {
                error "Extraction/Derived corner $ec does not exist!" "" -1
            }

        }

        set rc [catch { set cobj [find_corner [get_arg corner]]} output ]
        if { $rc == 1 } {
            ## corner not already defined
            #puts "Corner [get_arg corner] not already defined!"
            set cobj [[CORNER #auto] this]
            class_configure $cobj 
            rm_arg delay_active delay_exec_name delay_parameter_string
            $cobj set_cmd [rebuild_cmd tmg define_corner]

            lappend_state corner_list $cobj
            set_state cur_corner $cobj
        } else {
            warning "Corner [get_arg corner] was defined previously. Keeping old definition: [$cobj get_cmd]"
        }

        warning "define_corner:  state==  [state multi]"
        if { [state multi] ne "DEFINE" } {
            if { [$cobj get_flag] eq "" } {
                $cobj set_flag "called"
                rm_arg delay_active delay_exec_name delay_parameter_string
                set cmd [build_cmd $tmg define_corner]
                debug TMG:A "cmd to define corner: $cmd"
                set rc [::eval $cmd]
                if { $rc != 0 } {
                    error "Error $rc while executing define_corner." "" -1
                }
                cmd_rlog
            }
        } 
    }

    private method nc { } {
        set nc [CORNER #auto]
        $nc configure
        return [$nc this]
    }

    ## No args - nothing to do
    VOB::add_help TMG_CHECK_LIB {
        { _fcn "Validates libraries defined by define_corner" }
    }
    public method check_lib { { tmg_options "" } } {
        if { [state tmg_libs_loaded] == 1 } {
            ## have already done this..
            warning "Check lib has already been called - no action!"
            return
        }
    	#notice "check_lib tmg_options= $tmg_options"
        set tmg [get_tm Tmg]
        set rc [$tmg check_lib $tmg_options ]
        if { $rc != 0 } {
            error "Error $rc calling tmg check_lib." "" -1
        }
        set_state tmg_libs_loaded 1
        cmd_rlog
        return 0
    }

    ##Parameters
    VOB::add_args TMG_READ_SDC {
        {sdc_file ifile 1}
    }

    VOB::add_help TMG_READ_SDC {
        { _fcn "Reads a PrimeTime SDC file" }
        {sdc_file "SDC file to read"}
    }

    public method read_sdc { args } {

        parse_args
        #if { [state multi] ne "DEFINE" } {
            echo [cmdline]
            #echo [settings]


            set ifile [get_arg sdc_file]
            debug ZTM:A "ZTM:A sta read_sdc $ifile"
            if { ![file exists $ifile] } {
                error "SDC file $ifile does not exist!" "" -1
            }
            set rc [sta read_sdc $ifile]
            if { $rc != 0 && $rc != "" } {
                error "Error $rc reading SDC file $ifile." "" -1
            } else {
                set_state tmg_sdc_read 1
            }
            cmd_rlog
            set_state sdc_file [get_arg sdc_file]
            #puts "state sdc_file: [state sdc_file]"
        #} else {
        #     append_multi_cmd [list tmg read_sdc -sdc_file [get_arg sdc_file]]
        #}
        return 0
    }

    ##Parameters
    VOB::add_args TMG_CHECK_SDC {
        {sdc_file ifile 1}
    }

    VOB::add_help TMG_CHECK_SDC {
        { _fcn "Check if MILOS.sdc needs to be written" }
        {sdc_file "check if need to generate MILOS.sdc"}
    }

    public method check_sdc { args } {

        set tmg [get_tm Tmg]
        parse_args
        #if { [state multi] ne "DEFINE" } {
            echo [cmdline]
            #echo [settings]


            set ifile [get_arg sdc_file]
            debug ZTM:A "ZTM:A tmg check_sdc $ifile"
            if { ![file exists $ifile] } {
                error "SDC file $ifile does not exist!" "" -1
            }
            set rc [$tmg check_sdc -file $ifile]
            if { $rc != 0 && $rc != "" } {
                error "Error $rc checking SDC file $ifile." "" -1
            } else {
                set_state tmg_check_read 1
            }
            cmd_rlog
            set_state sdc_file [get_arg sdc_file]
            #puts "state sdc_file: [state sdc_file]"
        #} else {
        #     append_multi_cmd [list tmg read_sdc -sdc_file [get_arg sdc_file]]
        #}
        return 0
    }


    VOB::add_args TMG_SELECT_CORNER {
        { corner nstr 1 }
    }

    VOB::add_help TMG_SELECT_CORNER {
        { _fcn "Activates a defined timing corner" }
        { corner "Name of corner to activate" }
    }

    public method select_corner { args } {
        set tmg [get_tm Tmg]

        parse_args

        echo [cmdline]

        set cobj [find_obj corner_list corner [get_arg corner]]
        set rc [$tmg init_corner -corner [get_arg corner]]
        if { $rc != 0 } {
            error "Error: $rc during select_corner" "" -1
        }
        set_state cur_corner $cobj
        set_state slack_snapshot 0

        if { ([state si_corner] ne "" ) && ([state si_corner] ne $cobj) } {
            ## force rebuild of SI models
            make_rcmodels_si 1
        }
    }

    VOB::add_help TMG_SHOW_CORNER {
        { _fcn "Shows current timing corner" }
    }
    public method show_corner { } {
        puts "Current corner: [state cur_corner]"
        puts "[[state cur_corner] configure]"
    }




    VOB::add_args TMG_SET_HOLD_BUFS {
        { buf_list nstr 1 } 
    }

    VOB::add_help TMG_SET_HOLD_BUFS {
        { _fcn "Defines buffers to use in hold buffering" }
        { buf_list "Names of buffers to use during hold optimization" }
    }

    public method set_hold_bufs { args} {
        set tmg [get_tm Tmg]

        parse_args

        echo [cmdline]
        set rc [$tmg set_hold_buf_list -list_bufs [get_arg buf_list]]
        if { $rc != 0 } {
            warning "Error $rc during set_hold_bufs."
        }
        cmd_rlog
    }
    VOB::add_args TMG_REPORT_USE_AND_DONT_USE_CELLS {
    }
    VOB::add_help TMG_REPORT_USE_AND_DONT_USE_CELLS {
        { _fcn "Reports on use/dont use cells" }
    } 
    public method report_use_and_dont_use_cells { args } {
      set tmg [get_tm Tmg]
      echo [cmdline] 
      set rc [$tmg report_use_and_dont_use_cells]
      cmd_rlog
    }

    VOB::add_args TMG_SET_SETUP_BUFS {
        { buf_list nstr 1 } 
    }

    VOB::add_help TMG_SET_SETUP_BUFS {
        { _fcn "Defines buffers to use in setup buffering" }
        { buf_list "Names of buffers to use during setup optimization" }
    }
    public method set_setup_bufs { args } {
        set tmg [get_tm Tmg]

        parse_args
        echo [cmdline]

        set rc [$tmg set_buf_list -list_bufs [get_arg buf_list]]
        if { $rc != 0 } {
            warning "Error $rc during set_setup_bufs."
        }
        cmd_rlog
    }


    VOB::add_args TMG_SET_SETUP_INVS {
        { inv_list nstr 1 } 
    }

    VOB::add_help TMG_SET_SETUP_INVS {
        { _fcn "Defines invs to use in setup optimization" }
        { inv_list "Names of inverters to use during setup optimization" }
    }
    public method set_setup_invs { args } {
        set tmg [get_tm Tmg]

        parse_args
        echo [cmdline]

        set rc [$tmg set_inv_list -list_invs [get_arg inv_list]]
        if { $rc != 0 } {
            warning "Error $rc during set_setup_invs."
        }
        cmd_rlog
    }

    VOB::add_args TMG_SET_HOLD_INVS {
        { inv_list nstr 1 } 
    }

    VOB::add_help TMG_SET_HOLD_INVS {
        { _fcn "Defines invs to use in hold optimization" }
        { buf_list "Names of inverters to use during hold optimization" }
    }
    public method set_hold_invs {args } {
        set tmg [get_tm Tmg]

        parse_args
        echo [cmdline]

        set rc [$tmg set_hold_inv_list -list_invs [get_arg inv_list]]
        if { $rc != 0 } {
            warning "Error $rc during set_hold_invs."
        }
        cmd_rlog
    }

    VOB::add_args TMG_SLACK_SNAPSHOT { 
        { file ofile 0 }
        { verbose bool 0 }
    }

    VOB::add_help TMG_SLACK_SNAPSHOT {
        { _fcn "Saves a timing snapshot" }
        { file "File to save snapshot information to" }
        { verbose "Enable verbose mode" }
    }

    public method slack_snapshot { args } {
        if { [state multi] eq "OFF" } {
            set tmg [get_tm Tmg]

            parse_args
            echo [cmdline]

            set cmd [build_cmd $tmg slack_snapshot]

            set rc [::eval $cmd]
            if { $rc != 0 } {
                error "Error $rc while executing slack_snapshot." "" -1
            }
            set_state slack_snapshot 1
            cmd_rlog
        } else {
            warning "Slack Snaphot is NOT valid during Multimode!" 
        }
    }
    

    VOB::add_args TMG_SET_SI {
        { si num 1 }
    }
    VOB::add_help TMG_SET_SI {
        { _fcn "Turns SI (Signal Integrity) mode on/off" }
        { si "If > 0, enables si. If == 0, disables si" }
    }
    public method set_si { args } {
        parse_args
        set_state rc_models_si [get_arg si]
        UPOBJ set_si [get_arg si]
    }

    VOB::add_args TMG_UPDATE_TIMING {
        { force bool 0 1}
    }
    VOB::add_help TMG_UPDATE_TIMING {
        { _fcn "Updates RC timing models" }
        { force "Force complete rebuild of rc models, instead of incremental" }
    }
    public method update_timing { args } {
        parse_args
        echo [cmdline]
        make_rcmodels_si [get_arg force]
    }


    private method make_rcmodels_si { {force 1} } {

        set mysi [state rc_models_si]
        set tmg [get_tm Tmg]
        if { $force == 0 } {
            set rc [$tmg make_rcmodels]
        } else {
            set rc [$tmg make_rcmodels -force]
        }
        if { $rc != 0 } {
            error "Error $rc making RC models" "" -1
        }


        ## Logic:
        ## If SI mode on, then run the SI loop if:
        ## a) on main, in embedded mode
        ## b) on main, in plugin mode, SI and miller_effect on 
        ##    AND plugin_needs_miller on
        ## c) on remote, embedded mode

        if { ($mysi > 0) || ([state miller_effect] == 1) } {
            set run 0
            set idx 1
            set sobj [state local_scenario]
            if { $sobj ne "" } {
                if { ([$sobj cget -engine] eq "milos")  } {
                    ## embedded
                    set run 1
                } elseif { [state miller_effect] == 1 } {
                    ## plugin
                    debug PLUGIN:I "SI mode, main, plugin"
                    set run 1
                    if { [state plugin_needs_miller] == 0 } {
                        set mysi 0
                    }
                    debug PLUGIN:I "SI mode, main, run loop: $mysi"
                } else {
                    debug PLUGIN:I "SI mode, main, DO NOT run loop [state miller_effect] [state plugin_needs_miller]"
                }
            } else {
                ## if 0, we are still on main, but before multimode
                if { $::scen_idx == 0 } {
                    return
                }
                ## see if we are on a remote with embedded mode
                set sobj [state remote_scenario]
                if { $sobj eq "" } {
                    debug PLUGIN:I "SI mode, remote embedded node, run loop!"
                    set run 1
                    set idx $::scen_idx
                } else {
                        debug PLUGIN:I "SI mode, remote plugin node, DO NOT run loop 2!"
                }
            }

            if { $run == 1 } {
                ## we are on main - run if si on..
                for { set i 0 } {$i < $mysi } { incr i } {
                    notice "Signal Integrity iteration #$i."
                    call_milos "sta report_timing > tmp.${idx}.si"
                    set rc [$tmg make_rcmodels -force -si]
                    if { $rc != 0 } {
                        error "Error $rc making SI RC models" "" -1
                    }
                }
                ## to rebuild STA timing information
                call_milos "sta report_timing > tmp.si"
            }
        }
        set_state si_corner [state cur_corner]
        cmd_rlog "End of make RC Models"
        set_state tmg_rcmodels 1
    }



    VOB::add_args TMG_SET_FILLER_LIST {
        { list_fillers nstr 1 } 
    }
    VOB::add_help TMG_SET_FILLER_LIST {
        { _fcn "Sets list of filler cells" }
        { list_fillers nstr 1 } 
    }

    public method set_filler_list { args } {
        parse_args
        echo [cmdline]
        set rc [[get_tm Tmg] set_filler_list -list_fillers [get_arg list_fillers]]
        if { $rc != 0 } {
            error "Error $rc setting filler list." "" -1
        }
    }

    VOB::add_help TMG_REMOVE_FILLER_CELLS {
        { _fcn "Removes filler cells from design" }
    }
    public method remove_filler_cells { } {
        echo [cmdline]
        set tmg [get_tm Tmg]
        set rc [$tmg remove_filler_cells]
        if { $rc != 0 } {
            error "Error $rc removing filler cells" "" -1
            return 1
        }
        cmd_rlog
        return 0
    }


    public method enable_calibration { } {
        set tmg [get_tm Tmg]
        set rc [$tmg enable_calibration]
        if { $rc != 0 } {
            error "Error $rc in enable_calibration" "" -1
        }
        return 0
    }

    public method disable_calibration { } {
        set tmg [get_tm Tmg]
        set rc [$tmg disable_calibration]
        if { $rc != 0 } {
            error "Error $rc in disable_calibration" "" -1
        }
        return 0
    }

    VOB::add_args TMG_REPORT_CALIBRATION {
        { file nstr 0 "calibrate.rpt" }
    }

    VOB::add_help TMG_REPORT_CALIBRATION {
        { file "File to write calibration report to" }
    }

    public method report_calibration { } {
        if { [state tmg_calibrated] != 1 } {
            error "Must perform calibration first!" "" -1
        }
        set tmg [get_tm Tmg]
        set rc [$tmg report_calibration -file [get_arg file]]
        if { $rc != 0 } {
            error "Error $rc in report_calibration" "" -1
        }
        return 0
    }

    VOB::add_args TMG_PERFORM_CALIBRATION {
        { tolerance num 0 5 }
        { read_cdb nstr 0 "" }
        { write_cdb nstr 0 "" }
    }

    VOB::add_help TMG_PERFORM_CALIBRATION {
        { tolerance "Maximum error allowed by calibration (0 to 100)" }
        { read_cdb "Input calibration database file (optional)" }
        { write_cdb "Output calibration database file (optional)" }
    }

    public method perform_calibration { } {

        parse_args
        parasitics_check

        set tmg [get_tm Tmg]
        set cmd [build_cmd $tmg "perform_calibration"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in perform_calibration" "" -1
        }
        set_state tmg_calibrated 1
        cmd_rlog
        return 0
    }

    VOB::add_args TMG_MATCH {
        { spef_file ifile 1 }
        { si num 0 }
        { db_corner_name nstr AND_spef_corner }
        { spef_corner num AND_db_corner_name }
        { m_map hbool 0 0 }
    }

    VOB::add_help TMG_MATCH {
        { spef_file "Input spef file to read" }
        { si "Number of SI iterations to run" }
        { db_corner_name "Name of process corner to match to." }
        { spef_corner num "Spef file index to use for matching." }
    }
    public method match { args } {
        get_ext_status
        if { [state db_parasitics] == 0 } {
            error "no db parasitics - tmg match requires parasitics." "" -1
        }

        parse_args
        if { [check_arg si] == 1 } {
            set_state rc_models_si [get_arg si]
            rm_arg si
        }

        set ext [get_tm Ext]
        set cmd [build_cmd $ext "match"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc during tmg match" "" -1
        }
        make_rcmodels_si 
        return 0
    }

    VOB::add_args TMG_EXT_CALIBRATION {
        {spef_file ifile 1}
        {upper_limit num 0 2.0}
        {lower_limit num 0 0.5}
        {si num 0 }
        {db_corner_name nstr AND_spef_corner }
        {spef_corner num AND_db_corner_name }
        {m_map hbool 0 0}
    }

    VOB::add_help TMG_EXT_CALIBRATION {
        {spef_file "Spef file to calibrate against"}
        {upper_limit "Upper limit of the calibration factor"}
        {lower_limit "Lower limit of the calibration factor"}
        {si "Number of SI iterations to run" }
        {db_corner_name "Name of process corner to calibrate to." }
        {spef_corner "Spef file index to use for calibration." }
        { m_map "Use m_map debugging flag or not" }
    }

    public method ext_calibration { args } {

        parse_args
        parasitics_check

        if { [check_arg si] == 1 } {
            set_state rc_models_si [get_arg si]
            rm_arg si
        }

        set ext [get_tm Ext]
        set cmd [build_cmd $ext "calibrate"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in ext_calibration" "" -1
        }
        make_rcmodels_si 
        return 0
    }

    VOB::add_args TMG_GET_CLOCK_NETS {
        { file ofile 0 }
    }

    VOB::add_help TMG_GET_CLOCK_NETS {
        { file "output file" }
    }

    public method get_clock_nets { args } {
        set tmg [get_tm Tmg]
        parse_args
        echo [cmdline]

        set ::clockNets ""
        set cmd [build_cmd $tmg "get_clock_nets -clock_nets= ::clockNets"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in get_clock_nets" "" -1
        }

        return $::clockNets
    }

    VOB::add_args TMG_GET_CLOCK_CELLS {
        { file ofile 0 }
    }

    VOB::add_help TMG_GET_CLOCK_CELLS {
        { file "output file" }
    }

    public method get_clock_cells { args } {
        set tmg [get_tm Tmg]
        parse_args
        echo [cmdline]

        set ::clockCells ""
        set cmd [build_cmd $tmg "get_clock_cells -clock_cells= ::clockCells"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in get_clock_cells" "" -1
        }

        return $::clockCells
    }

    VOB::add_args TMG_WRITE_VERILOG {
        { file ofile 1 }
        { delimiter nstr 0 "/" }
        { exclude_cells nstr 0}
        { no_backslash bool 0}
        { block nstr 0 }
    }

    VOB::add_help TMG_WRITE_VERILOG {
        { _fcn "Writes a verilog file" }
        { _rtn "Verilog module name (string)" }
        { file "Output file name" }
        { delimiter "Delimiter for separating hierarchy" }
        { exclude_cells "Cells not to write to verilog" }
        { no_backslash "Strip backslashes from names/nets" }
        { block "DB Block to write verilog for (rarely needed)" }
    }
    public method write_verilog { args } {
        parse_args
        echo [cmdline]

        set cmd [build_cmd [get_tm Tmg] "write_verilog"]
        append cmd "-module= ::ver_mod"
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc in write_verilog"
        }

        return $::ver_mod
    }



    VOB::add_args TMG_USE_EXTERNAL_TIMING {
        { engine nstr 1 }
        { start_name nstr 0 "" }
        { set_units num 0 "" }
        { sdc_file ifile 0 }
        { preinit_file ifile 0 }
        { postinit_file ifile 0 }
        { tmp_dir nstr 0 "tmp" }
        { db nstr 0 "" }
        { translate bool 0 0 }
        { timeout num 0 30}
        { ltype hnstr 0 "static"}
        { spef_file nstr 0 "" }
        { verilog_file nstr AND_verilog_file "" }
        { verilog_module nstr AND_verilog_module "" }
        { celtic_on bool AND_celtic_noiselib 0}
        { celtic_start_name nstr 0 "ca celtic" }
        { celtic_noiselib iflist AND_celtic_vdd_value}
        { celtic_vdd_value num AND_celtic_on }
        { celtic_gnd_value num 0 0.0 }
        { celtic_vdd_signals nstr 0 "VDD" }
        { celtic_gnd_signals nstr 0 "GND" }
        { celtic_process nstr 0 "130nm" }
        { celtic_settings_file ifile 0 }
        { celtic_report_file ifile 0 }
        { celtic_copy_lib hbool 0 0 }
        { debug hlist 0 {0 0 0 0} }
        { schedule_style hnum 0 1}
    }

    VOB::add_help TMG_USE_EXTERNAL_TIMING {
        { engine "name of timing engine to use" }
        { start_name "name of script to execute to start timing engine" }
        { set_units "conversion unit to use for remote engine, if not available" }
        { sdc_file "SDC constraints file to use for the remote engine" }
        { preinit_file "file to source before startup of timing engine" }
        { postinit_file "file to source after startup of timing engine" }
        { tmp_dir "temporary directory to use to store files - must be accessible to ALL machines" }
        { db "Override list of .lib files with tool specific equivalent files" }
        { translate "Remove backslashes from design" }
        { timeout "How long to wait to initialize plugin" }
        { spef_file "Pass this spef_file to plugin: do not write a new one" }
        { verilog_file "Verilog file to use for plugin " }
        { verilog_module "Name of verilog module" }
        { celtic_on bool AND_celtic_noiselib 0}
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
    public method use_external_timing { args } {
        parse_args

        get_ext_status
        parasitics_check

        if { [state rmt_local] != "" } {
            error "Cannot mix use_external_timing and multimode" "" -1
        }

        set debug [get_arg debug]
        rm_arg debug

        if { [get_arg start_name] eq "" } {
            set_arg start_name $START_NAME([get_arg engine])
            puts "Defaulting start name to: [get_arg start_name]"
        }

        set sobj [ [SCENARIO #auto] this]


        $sobj configure -name "MAIN"
        $sobj set_sidx 1
        class_configure $sobj
        $sobj set_cmd [cmdline]

        set cc [state cur_corner]
        $sobj configure -corner [$cc cget -corner]

        if { [check_arg sdc_file] == 0 } {
            $sobj configure -sdc_file [state sdc_file]
        }

        if { [$sobj cget -sdc_file] eq "" } {
            error "No SDC file has been read, and no SDC file has been specified" "" -1
        }

        check_sobj $sobj

        ## create an actual scenario on main - for use with active term updates
        set tmg [get_tm Tmg]
        $tmg create_scenario -name MAIN -corner [$cc cget -corner] -sdc [$sobj cget -sdc_file]
        $tmg set_scenario_priority -name MAIN -priority 1
        $tmg set_current_scenario -name MAIN
        set ::mmdb [get_tm DB]
        $::mmdb get -top block= ::mmblock
        $::mmdb set_number_of_scenarios -block $::mmblock -count 1
        ## end create actual scenario



        set_state local_scenario $sobj
        #puts "LOCAL: [state local_scenario]"
        set rtype [get_arg engine]

        set rt [new_rmt "local" 0 "local" $rtype] 
        set rmt_local [$rt this]
        #puts "RL: $rmt_local"
        $rmt_local set_sobj $sobj
        $rmt_local init_rmt
        $rmt_local set_plugin_status
        $sobj set_remote $rmt_local

        UPOBJ set_debug $debug
        UPOBJ set_rmt_local $rmt_local
        UPOBJ init_db 

        ## run celtic once, the first time through
        UPOBJ set_celtic_force

    }

    public method plugin_cmd { args } {
        $::apps::milos::mycf plugin_cmd TE int_plugin_cmd [join $args]
        lappend_state plugin_cmd_list [join $args]
    }

}
