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

## clk.tcl
## Clock Optimization module for Milos
## Author: Dimitris Fotakis
## VERSION: 2.1

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::clk 0.1

itcl::class CLK {

    inherit VOB

    private variable design_name       "cnfg";
    private variable exportedScenarios "SLOW FAST";

    ## Protected Vars
    ##

    protected variable lcorners {};
    ## End Protected Vars

    protected method constructor { parent } { 
        VOB::constructor $parent 
        set NAME "ClockOpt"
        set SELFNAME "MODULE$this"
        set_state tmg init
    }

    ## used to "close the hole"
    protected method info { }

    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help CLK $cmd $param $long
        } else {
            mod_help CLK $cmd
        }
    }

    VOB::add_args CLK_EXPORT_TIMING_VIEW {
	{ scenarios list 0 }
        { spef_file ifile 0 ""}
        { max_paths num 0 10000 }
        { min_slack num 0 }
        { max_slack num 0 }
        { init_script ifile 0 }
        { exec_name nstr 0 "ca pt_shell -f" }
    }
    VOB::add_help CLK_EXPORT_TIMING_VIEW {
	{ scenarios "List of scenarios for which timing view is to be exported" }
        { spef_file "SPEF file name for scenarios without spef_file" }
        { max_paths "Max number of paths for Timing Analysis reporting" }
        { min_slack "Min slack number for Timing Analysis reporting" }
        { max_slack "Max slack number for Timing Analysis reporting" }
        { init_script "3rd-party STA initialization script" }
        { exec_name "Name of executable to run" }
    }
    public method export_timing_view { args } {
        parse_args

	# manage the temporary directory
	set tempDir ./tmp
	if { [file exists $tempDir] && ![file isdirectory $tempDir] } {
		error "Error in export_timing_view: please delete file $tempDir" "" -1
	}

	if { ![file exists $tempDir] } {
		if { [catch { file mkdir $tempDir } errMsg] } {
			error "Error in export_timing_view: unable to open temporary folder $tempDir: $errMsg" "" -1
		}
	}

	# export verilog
	[get_tm Tmg] write_verilog -file $tempDir/clkwksdesign.v -delimiter / -module= ::designName
	set design_name $::designName


	set rtOptions "-max_paths [get_arg max_paths]"
	if { [check_arg min_slack] } {
		set rtOptions "$rtOptions -slack_greater_than [get_arg min_slack]"
	}
	if { [check_arg max_slack] } {
		set rtOptions "$rtOptions -slack_lesser_than [get_arg max_slack]"
	}

	set initScript [expr { [check_arg init_script] ? [get_arg init_script] : "" }]

	set scenarios ""
	if { [check_arg scenarios] } {
		foreach scenarioName [get_arg scenarios] {
			if { [catch { find_obj scenario_list name $scenarioName } scenario] } {
				error "Error in export_timing_view: scenario '$scenarioName' undefined" "" -1
			}

			lappend scenarios $scenario
		}
	} else {
		set scenarios [state scenario_list]
	}

	foreach scenario $scenarios {
		set scenarioName [$scenario cget -name]

        set spef_file [$scenario cget -spef_file]
        if { $spef_file eq "" } {
            set spef_file [get_arg spef_file]
        }

        ## insert external delay calculator here
		set corner [find_corner [$scenario cget -corner]]
        if { [$corner cget -delay_active] == 1 } {
            external_delay $scenario $design_name $spef_file $tempDir
        }

            
		set script$scenarioName [export_script $scenario $design_name $initScript $spef_file $rtOptions]
		set cmd "exec [get_arg exec_name] [set script$scenarioName]"
		::eval $cmd
		lappend exportedScenarios $scenarioName
	}

    ## reset sdf_done flags
	foreach scenario $scenarios {
		set corner [find_obj corner_list corner [$scenario cget -corner]]
        $corner configure -sdf_done 0
    }

    }

    protected variable delay_setup 0
    protected method external_delay { scenario module spef tmp } {

        if { $delay_setup == 0 } {
            set cwd [pwd]
            cd $tmp
            exec ln -sf clkwksdesign.v ${module}.v
            cd $cwd
            set delay_setup 1
        }

        ## get the corner corresponding to this scenario
		set corner [find_corner [$scenario cget -corner]]
        set ext_name [$corner cget -delay_exec_name]

        ## we've already created the sdf file for this corner
        if { [$corner cget -sdf_done] eq 1 } { return }


        if { [$corner cget -sdf_file] eq "" } {
            set cfname "${tmp}/[$corner cget -corner]_out.sdf"
            $corner configure -sdf_file $cfname
        }
        set outfile [$corner cget -sdf_file]

        ## substitute parameters
        set p [$corner cget -delay_parameter_string]

        ## standard substitutions
        regsub -all "%verilog_file%" $p "${tmp}/${module}.v" p
        regsub -all "%verilog_module%" $p "${module}" p
        regsub -all "%spef_file%" $p "${spef}" p
        regsub -all "%sdc_file%" $p "[$scenario cget -sdc_file]" p
        regsub -all "%libs%" $p "[$corner cget -lib]" p
        regsub -all "%outfile%" $p "${outfile}" p
        regsub -all "%si%" $p "[state rc_models_si]" p

        ## celtic substitutions
        set p [sobj_sub $scenario $p]

        notice "Substituted parameters: $p"
        if { [file exists $outfile] } { file delete $outfile }

        ## Now call the external delay engine
        set cmd "exec $ext_name $p >& edc.out "
        ::eval $cmd

        if { [file exists $outfile] == 0 } {
            error "Error creating outfile $outfile. Called external delay command: $cmd"
        }
    }

    protected method sobj_sub { sobj parms } {
        set conf [$sobj configure]
        set map {}
        foreach val $conf {
            foreach {var old new} $val {
                if { [string range $var 1 7] eq "celtic_" } {
                    lappend map "%[string range $var 1 end]%" "$new"
                }
            }
        }
        set p [string map $map $parms]
        return $p
    }

    
    
    private method export_script { scenario designName initScript spefFile rtOptions } {
	set tempDir ./tmp
	set scenarioName [$scenario cget -name]

	# prepare 3rd-party STA script
	set outFile $tempDir/$designName.pt.[$scenario cget -name].tcl
	set ptFP [open $outFile w]
	puts $ptFP "proc pt_write_pin_slack \{ fileName \} \{"
	puts $ptFP "  if \{ \[catch \{ open \$fileName.slack w\} fileH\] \} \{"
	puts $ptFP "    puts \"!!! ERROR !!! Unable to open output file \'\$fileName.slack\': \$fileH\""
	puts $ptFP "    return code error"
	puts $ptFP "  \}"
	puts $ptFP ""
	puts $ptFP "  global timing_save_pin_arrival_and_slack"
	puts $ptFP "  set timing_save_pin_arrival_and_slack true"
	puts $ptFP ""
	puts $ptFP "  foreach_in_collection pin \[get_pins\] \{"
	puts $ptFP "    puts -nonewline \$fileH \"\[get_attribute \$pin max_rise_slack\]	\""
	puts $ptFP "    puts -nonewline \$fileH \"\[get_attribute \$pin max_fall_slack\]	\""
	puts $ptFP "    puts -nonewline \$fileH \"\[get_attribute \$pin min_rise_slack\]	\""
	puts $ptFP "    puts -nonewline \$fileH \"\[get_attribute \$pin min_fall_slack\]	\""
	puts $ptFP "    puts            \$fileH \"\[get_object_name \$pin\]\""
	puts $ptFP "  \}"
	puts $ptFP ""
	puts $ptFP "  foreach_in_collection port \[get_ports\] \{"
	puts $ptFP "    puts -nonewline \$fileH \"\[get_attribute \$port min_rise_slack\]	\""
	puts $ptFP "    puts -nonewline \$fileH \"\[get_attribute \$port max_rise_slack\]	\""
	puts $ptFP "    puts -nonewline \$fileH \"\[get_attribute \$port min_fall_slack\]	\""
	puts $ptFP "    puts -nonewline \$fileH \"\[get_attribute \$port max_fall_slack\]	\""
	puts $ptFP "    puts            \$fileH \"\[get_object_name \$port\]\""
	puts $ptFP "  \}"
	puts $ptFP ""
	puts $ptFP "  close \$fileH"
	puts $ptFP ""
	puts $ptFP "  report_timing $rtOptions -delay max > \$fileName.max.rpt"
	puts $ptFP "  report_timing $rtOptions -delay min > \$fileName.min.rpt"
	puts $ptFP "\}"
	puts $ptFP ""

	if { $initScript != "" } {
		# user-provided script
		puts $ptFP "### PT user-provided initializarion script"
		puts $ptFP "source $init_script"
	} else {
		set corner [find_obj corner_list corner [$scenario cget -corner]]
		puts $ptFP "###############"
		puts $ptFP "##### Scenario: $scenarioName"
		puts $ptFP "###############"

		puts $ptFP ""
		puts $ptFP "### Define libraries ###"
		set isOCV false
		set lib [$corner cget -lib]
		if { $lib == "" } {
			set minLib [$corner cget -min_lib]
			foreach lib $minLib {
				puts $ptFP "read_lib $lib"
			}
			puts $ptFP ""

			puts $ptFP "set minLib \"\""
			puts $ptFP "foreach lib \[get_libs\] \{"
			puts $ptFP "  lappend minLib \$lib"
			puts $ptFP "\}"
			puts $ptFP ""

			set maxLib [$corner cget -max_lib]
			foreach lib $maxLib {
				puts $ptFP "read_lib $lib"
			}
			puts $ptFP ""

			puts $ptFP "set maxLib \"\""
			puts $ptFP "foreach lib \[get_libs\] \{"
			puts $ptFP "  if \{ \[lsearch \$minLib $lib\] == -1 \} \{"
			puts $ptFP "    lappend maxLib \$lib"
			puts $ptFP "  \}"
			puts $ptFP "\}"

			set isOCV true
		} else {
			foreach libName $lib {
				puts $ptFP "read_lib $libName"
			}
		}

		puts $ptFP ""
		puts $ptFP "set libGroups \"\""
		puts $ptFP "foreach_in_collection lib \[get_libs\] \{"
		puts $ptFP "	lappend libGroups \[get_object_name \$lib\]"
		puts $ptFP "\}"
		puts $ptFP ""
		puts $ptFP "set link_path \"\[concat * \$libGroups\]\""

		puts $ptFP ""
		puts $ptFP "### Load design ###"
		puts $ptFP "read_verilog $tempDir/clkwksdesign.v > $tempDir/verilog.log"
		puts $ptFP "link_design $designName > $tempDir/link_design.$scenarioName.log"

		if { [$corner cget -sdf_file] != "" } {
			puts $ptFP "read_sdf [$corner cget -sdf_file]"
			set isOCV true
		}

		puts $ptFP ""
		puts $ptFP "### Set variable ###"
		puts $ptFP "set report_default_significant_digits [sta set report_default_significant_digits]"
		puts $ptFP "set timing_disable_internal_inout_cell_paths [sta set timing_disable_internal_inout_cell_paths]"
		puts $ptFP "set timing_disable_internal_inout_net_arcs [sta set timing_disable_internal_inout_net_arcs]"
		puts $ptFP "set timing_clock_gating_propagate_enable [sta set timing_clock_gating_propagate_enable]"
		puts $ptFP "set timing_remove_clock_reconvergence_pessimism [sta set timing_remove_clock_reconvergence_pessimism]"
		puts $ptFP "set timing_clock_reconvergence_pessimism [sta set timing_clock_reconvergence_pessimism]"

		puts $ptFP ""
		puts $ptFP "### Operating Conditions ###"
		if { [$corner cget -max_derate] != "" } {
			puts $ptFP "set_timing_derate -late [$corner cget -max_derate]"
			set isOCV true
		}

		if { [$corner cget -min_derate] != "" } {
			puts $ptFP "set_timing_derate -early [$corner cget -min_derate]"
			set isOCV true
		}


		set opCond ""
		if { [$corner cget -op_cond] != "" } {
			set opCond [$corner cget -op_cond]
		} elseif { [$corner cget -min_op_cond] != "" } {
			set opCond "-min [$corner cget -min_op_cond] -max [$corner cget -max_op_cond]"
		}

		if { [$corner cget -min_lib] != "" } {
			set opCond "$opCond -min_lib \$minLib -max_lib \$maxLib"
		}

		if { $isOCV } {
			set opCond "$opCond -analysis_type on_chip_variation"
		}

		if { $opCond != "" } {
			puts $ptFP "set_operating_conditions $opCond"
		}

		puts $ptFP ""
		puts $ptFP "### Constraints, Parasitics, and Delay File ###"
		puts $ptFP "read_sdc [$scenario cget -sdc_file] > $tempDir/read_sdc.$scenarioName.log"

		if { $spefFile == "" } {
			set spefFile $tempDir/$design_name.spef
			::milos pdb write_spef -prime_time 1 -outfile $spefFileMax
		}

		puts $ptFP "read_parasitics -complete_with ZERO $spefFile > $tempDir/parasitics.$scenarioName.log"
	}

	puts $ptFP ""
	puts $ptFP "### run pt_write_pin_slack"
	puts $ptFP "pt_write_pin_slack $tempDir/$designName.$scenarioName"
	puts $ptFP ""
	puts $ptFP "exit"
	close $ptFP

	return $outFile
    }

    VOB::add_args CLK_OPTIMIZE {
	{ scenario_setup nstr 1 }
	{ scenario_hold nstr 1 }
        { cw_script ifile 0 }
        { cw_exec nstr 0 "cw" }
	{ def_file ifile AND_lef_files }
	{ lef_files flist AND_def_file }
	{ fix_setup bool OR_fix_hold 1 }
	{ fix_hold bool OR_fix_setup 0 }
        { schedule_file ofile 0}
        { dont_use_uncertainty bool 0 }
        { setup_slack_target num 0 0.0}
        { setup_slack_limit num 0 0.0}
        { hold_slack_target num 0 0.0}
        { hold_slack_limit num 0 0.0}
        { allow_setup_degradation bool 0 }
        { allow_hold_degradation bool 0 }
    }
    VOB::add_help CLK_OPTIMIZE {
	{ scenario_setup "Scenario to use for setup analysis" }
	{ scenario_hold "Scenario to use for hold analysis" }
        { dont_use_uncertainty "Cancel uncertainty" }
        { cw_script "Input script file name to bypass all user options" }
        { cw_exec "Name of executable/script to run for Clockworks" }
	{ fix_setup "fixes setup violations" }
	{ fix_hold "fixes hold violations" }
	{ schedule_file ofile "Clock scheduler report file" }
        { setup_slack_target "setup slack target" }
        { setup_slack_limit "setup slack limit" }
        { hold_slack_target "hold slack target" }
        { hold_slack_limit "hold slack limit" }
        { allow_setup_degradation "Allow setup degradation flag" }
        { allow_hold_degradation "Allow hold degradation flag" }
    }
    public method optimize { args } {
	parse_args

	if { $design_name == "" } {
		error "Error in optimize: please export the timing view first using 'clk export_timing_view'" "" -1
	}

	if { [check_arg cw_script] } {
		set rc [ catch {exec [get_arg cw_exec] -e [::sys::getCWToken] -t [get_arg cw_script] > /dev/null} output ]
		if { $rc != 0 } {
			puts "CW output: $output"
		}

		return
	}

	if { ![check_arg scenario_setup] && ![check_arg scenario_hold] } {
		error "Error in export_timing_view: a setup or hold scenario must be provided using '-scenario_setup' or '-scenario_hold'" "" -1
	}

	set setupScenario ""
	set setupCorner   ""
	if { [check_arg scenario_setup] } {
		set scenarioName [get_arg scenario_setup]
		if { [catch { find_obj scenario_list name $scenarioName } setupScenario] } {
			error "Error in optimize: scenario '$scenarioName' undefined" "" -1
		}

		if { [lsearch $exportedScenarios $scenarioName] == -1 } {
			error "Error in optimize: '$scenarioName' timing view not exported. Please use 'clk export_timing_view'" "" -1
		}

		set setupCorner [find_obj corner_list corner [$setupScenario cget -corner]]
	}

	set holdScenario ""
	set holdCorner   ""
	if { [check_arg scenario_hold] } {
		set scenarioName [get_arg scenario_hold]
		if { [catch { find_obj scenario_list name $scenarioName } holdScenario] } {
			error "Error in optimize: scenario '$scenarioName' undefined" "" -1
		}

		if { [lsearch $exportedScenarios $scenarioName] == -1 } {
			error "Error in optimize: '$scenarioName' timing view not exported. Please use 'clk export_timing_view'" "" -1
		}

		set holdCorner [find_obj corner_list corner [$holdScenario cget -corner]]
	}

	set tempDir ./tmp
	set outFile "$tempDir/$design_name.cw.tcl"
	set cwFP [open $outFile w]

	set sdcFile ""
	if { $setupScenario != "" } {
		set lib [$setupCorner cget -lib]
		if { $lib == "" } {
			set lib "[$setupCorner cget -max_lib] [$setupCorner cget -min_lib]"
		}

		puts $cwFP "read_synlib -max -filename \{$lib\}"
		if { [$setupCorner cget -max_derate] != "" } {
			puts $cwFP "set_timing_derate -late -setup_recovery -derate [$setupCorner cget -max_derate]"
		}
		if { [$setupCorner cget -min_derate] != "" } {
			puts $cwFP "set_timing_derate -early -setup_recovery -derate [$setupCorner cget -min_derate]"
		}

		# WARNING: AT THIS TIME SETUP and HOLD SDC MUST BE THE SAME (ClockWks limitation)
		set sdcFile [$setupScenario cget -sdc_file]
	}
	if { $holdScenario != "" } {
		set lib [$holdCorner cget -lib]
		if { $lib == "" } {
			set lib "[$holdCorner cget -min_lib] [$corner cget -min_lib]"
		}

		puts $cwFP "read_synlib -min -filename \{$lib\}"
		if { [$holdCorner cget -max_derate] != "" } {
			puts $cwFP "set_timing_derate -late -hold_removal -derate [$holdCorner cget -max_derate]"
		}
		if { [$holdCorner cget -min_derate] != "" } {
			puts $cwFP "set_timing_derate -early -hold_removal -derate [$holdCorner cget -min_derate]"
		}

		# WARNING: AT THIS TIME SETUP and HOLD SDC MUST BE THE SAME (ClockWks limitation)
		set sdcFile [$holdScenario cget -sdc_file]
	}

	# export LEF/DEF
	set lefFiles ""
	set defFile  ""
	if { [check_arg lef_files] } {
		set lefFiles [get_arg lef_files]
		set defFile  [get_arg def_file]
	} else {
		set lefFiles [state lef_loaded]
		set defFile  [state def_loaded]
		if { $lefFiles == "" } {
			set lefFiles $tempDir/$design_name.lef
			set defFile  $tempDir/$design_name.def
			[get_tm DB] save_lef -file $lefFiles
			[get_tm DB] save_def -file $defFile
		}
	}

	puts $cwFP "read_lef -filename $lefFiles"
	puts $cwFP "read_def -file     $defFile"

	puts $cwFP "read_sdc $sdcFile"

	# define repeater list
	puts $cwFP ""
	set ::repeaters ""
	if { [check_arg fix_setup] } {
		[get_tm Tmg] get_repeaters -setup -buffer -repeaters= ::repeaters
		set ::repeaters [lindex $::repeaters 0]
		if { [llength $::repeaters] != 0 } {
			puts -nonewline $cwFP "set_clock_buffers -max \{"
			foreach repeater $::repeaters {
				puts -nonewline $cwFP " [lindex $repeater 0]"
			}
			puts $cwFP " \}"
		}

		[get_tm Tmg] get_repeaters -setup -inverter -repeaters= ::repeaters
		set ::repeaters [lindex $::repeaters 0]
		if { [llength $::repeaters] != 0 } {
			puts -nonewline $cwFP "set_clock_invs -max \{"
			foreach repeater $::repeaters {
				puts -nonewline $cwFP " [lindex $repeater 0]"
			}
			puts $cwFP " \}"
		}
	}

	if { [check_arg fix_hold] } {
		[get_tm Tmg] get_repeaters -hold -buffer -repeaters= ::repeaters
		set ::repeaters [lindex $::repeaters 0]
		if { [llength $::repeaters] != 0 } {
			puts -nonewline $cwFP "set_clock_buffers -delay \{"
			foreach repeater $::repeaters {
				puts -nonewline $cwFP " [lindex $repeater 0]"
			}
			puts $cwFP " \}"
		}

		[get_tm Tmg] get_repeaters -setup -inverter -repeaters= ::repeaters
		set ::repeaters [lindex $::repeaters 0]
		if { [llength $::repeaters] != 0 } {
			puts -nonewline $cwFP "set_clock_invs -delay \{"
			foreach repeater $::repeaters {
				puts -nonewline $cwFP " [lindex $repeater 0]"
			}
			puts $cwFP " \}"
		}
	}

	puts $cwFP ""
	puts $cwFP "rebuild_clocks"
	if [check_arg dont_use_uncertainty] {
	    puts $cwFP "remove_clock_uncertainty -all ; "
	}

	puts $cwFP ""
	set ptOption ""
	set corner   ""
	if { $setupScenario != "" } {
		set fileName $tempDir/$design_name.[$setupScenario cget -name]
		set ptOption "-setup $fileName.max.rpt -setup_slacks $fileName.slack"
		set corner MaxSetup
	}
	if { $holdScenario != "" } {
		set fileName $tempDir/$design_name.[$holdScenario cget -name]
		set ptOption "$ptOption -hold $fileName.min.rpt -hold_slacks $fileName.slack"
		set corner ${corner}MinHold
	}
	puts $cwFP "read_primetime_reports $ptOption -mode system -corner $corner"

	puts $cwFP ""
	puts $cwFP "report_setup_slacks -output $design_name.setup.rpt"
	puts $cwFP "report_hold_slacks -output $design_name.hold.rpt"

	puts $cwFP ""
	puts $cwFP "## Optimize"
	set setupScheduleFile ""
	set holdScheduleFile  ""
	if { [check_arg fix_setup] } {
		set setupScheduleFile [expr { [check_arg schedule_file] ? [get_arg schedule_file] : "$tempDir/$design_name.clkcs.rpt" }].setup
		set schedule "schedule_clocks -setup -setup_slack_target [get_arg setup_slack_target] -setup_slack_limit [get_arg setup_slack_limit] -outfile $setupScheduleFile"

		if { [check_arg allow_hold_degradation] } {
			set schedule "$schedule -allow_hold_degradation"
		}

		puts $cwFP $schedule

		if { [file exists $setupScheduleFile] } {
			file delete $setupScheduleFile
		}
        }
	if { [check_arg fix_hold] } {
		set holdScheduleFile [expr { [check_arg schedule_file] ? [get_arg schedule_file] : "$tempDir/$design_name.clkcs.rpt" }].hold
		set schedule "schedule_clocks -hold -hold_slack_target [get_arg hold_slack_target] -hold_slack_limit [get_arg hold_slack_limit] -outfile $holdScheduleFile -incremental"

		if { [check_arg allow_setup_degradation] } {
			set schedule "$schedule -allow_setup_degradation"
		}

		puts $cwFP $schedule

		if { [file exists $holdScheduleFile] } {
			file delete $holdScheduleFile
		}
	}

	puts $cwFP ""
	puts $cwFP "## Report optimized slacks.."
	puts $cwFP "report_setup_slacks -optimized  -output $design_name.opt.setup.rpt"
	puts $cwFP "report_hold_slacks  -optimized  -output $design_name.opt.hold.rpt"

	puts $cwFP "exit"
	close $cwFP

	set token [::sys::getCWToken]
	puts "CW Token: $token"
	catch { exec [get_arg cw_exec] -e $token -t $outFile } output
	if { ![catch { open cwlogs/clkwork.log r } clkLogH ] } {
		while { [gets $clkLogH line] >= 0 } {
			notice $line
		}

		close $clkLogH
	}

	if { ![file exists $setupScheduleFile] && ![file exists $holdScheduleFile] } {
		error "Error in optimize: unable to generate the schedule" "" -1
	}

    if { [check_arg fix_setup] } {
        if { $setupScheduleFile != "" && [file exists $setupScheduleFile] } {
            [get_tm Tmg] implement_clock_schedule -file $setupScheduleFile -max
            set_state node_slack_status "invalid"
        }
    }

    if { [check_arg fix_hold] } {
        if { $holdScheduleFile != "" && [file exists $holdScheduleFile] } {
            [get_tm Tmg] implement_clock_schedule -file $holdScheduleFile
            set_state node_slack_status "invalid"
        }
    }
    }

    public method gt { } {
        puts [::sys::getCWToken]
    }
}
