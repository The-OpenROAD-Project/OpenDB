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

## ext.tcl
## Extraction module for Milos
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::ext 0.1

itcl::class EXT {

    inherit VOB

    ## Protected Vars
    ##

    ## End Protected Vars

    protected method constructor { parent } { 
        VOB::constructor $parent 
        set NAME "Extraction"
        set SELFNAME "MODULE$this"
        set_state endpoints_updated 0
    }

    ## used to "close the hole"
    protected method info { }

    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help EXT $cmd $param $long
        } else {
            mod_help EXT $cmd
        }
    }

    protected method get_ext_type { {force 0} } {
    
        get_db_change_status

        if { [state db_disconnected_cnt] > 0 } {
            ## need to do routing first
            #return "routing"
        }

        if { $force == 1 } {
            ## force specified - full extraction required
            debug EXT_GET:A "P1"
            return "full"
        } else {

            get_ext_status

            ## need to do some sort of extraction
            if { [state db_parasitics] == 0 } {
                ## no parasitics, do full extraction
                debug EXT_GET:A "P2"
                return "full"
            } 
            ## myext must be 1 - have some kind of parasitic info

            ## see if any netlist info has changed
            if { [state db_altered_cnt] > 0 } {
                if { [state db_foreign] == 1 } {
                    ## parasitics are foreign - cannot do eco extraction
                    debug EXT_GET:A "P3"
                    return "full"
                } else {
                    ## local parasitics, some there - do ECO
                    debug EXT_GET:A "P4"
                    #return "eco"
                    #for now, do full
                    return "full"
                }
            }

            ## if db_altered_cnt == 0, do not need to do extraction
        }

        ## no extraction required
        debug EXT_GET:A "P5"
        return "none"
    }
                    

    public method set_block { block } {
        puts "SET BLOCK $block"
        [get_tm Ext] set_block -block $::BN
    }

    VOB::add_args EXT_EXTRACT {
        { min_model bool NOT_use_max_model 0 }
        { max_model bool NOT_use_typ_model 0 }
        { typ_model bool NOT_use_min_model 0 }
        { re_run bool 0 0 }
        { force bool 0 0 }
        { over_cell bool 0 0}
        { rlog hbool 0 }
        { tile hbool 0 }

        { ext_model_file ifile 0 }
        { cmp_file ifile 0}
        { corner_cnt num 0 1}
        { max_res num 0 75.0}
        { coupling_threshold num 0 0.1}
        { min_idx num 0 }
        { max_idx num 0 }
        { typ_idx num 0 }
        { res_factor num 0 1.0}
        { cc_factor num 0 1.0}
        { gndc_factor num 0 1.0}
        { cc_model num 0}
        { max_res num 0 }
        { no_merge_via_res bool 0 }
        { power_grid bool 0 }
        { exclude_cells nstr "" }
        { signal_table hnum 0 }

    }

    VOB::add_alias EXT_EXTRACT {
        { min_idx min }
        { max_idx max }
        { typ_idx typ }
    }

    VOB::add_help EXT_EXTRACT {
        { min_model "Use min capacitance model as defined in load_model" }
        { max_model "Use max capacitance model as defined in load_model" }
        { typ_model "Use typ capacitance model as defined in load_model" }
        { re_run "Re-run extraction" }
        { force "Force full extraction, regardless of database state." }
        { over_cell "Add cell iterm and obstruction shapes as extraction context nets" }

        { cmp_file "File to compare against" }
        { ext_model_file "File containing extraction models" }
        { corner_cnt "First process corners from cap model table extracted at the same time" }
        { max_res "Combine resistors in series up to <max_res> value in OHMS, 0.0 to disable" }
        { coupling_threshold "coupling caps smaller than value (in fF) will be grounded." }
        { min_idx "Index of Min Capacitance extraction Model in the db cap. table" }
        { max_idx "Index of Max Capacitance extraction Model in the db cap. table" }
        { typ_idx "Index of Typ Capacitance extraction Model in the db cap. table" }
        { res_factor "Multiply resistance by res_factor" }
        { cc_factor "Multiply capacitance by cc_factor" }
        { gndc_factor "Multiply ground capacitance by gndc_factor" }
        { cc_model "number of maximum tracks for coupling nets on same layer" }
        { max_res "Combine resistors in series up to max_res value (in ohms) - disabled by default" }
        { no_merge_via_res "Do NOT add via resistance to wires" }
        { power_grid "Extract power nets only" }
        { exclude_cells "Exclude cells from the power grid netlisting" }
    }

    public method extract { args } {

        if { [state db_loaded] != 1 } {
            notice "Please load a design first!"
            return 1
        }

        parse_args
        set force [get_arg force]
        rm_arg force

        if { ([state ext_model_file] == "") && ([check_arg ext_model_file] == 0) } {
            if { [check_arg power_grid] == 0 } {
            error "Extract requires the -ext_model_file parameter to be specified at least once." "" -1
            }
        }

        if { ([check_arg ext_model_file] == 1) && ([get_arg ext_model_file] ne "") } {
            set_state ext_model_file [get_arg ext_model_file]
        }

        if { [llength [get_extraction_corners]] == 1 } {
            set ctype "single corner"
        } else {
            set ctype "multi-corner"
        }
        
        set etype [get_ext_type $force]

        if { $etype == "routing" } {
            warning "Routing required to bring netlist up to date - cannot extract parasitics at this point."
            return 1
        }
        set etext [string toupper $etype]

        if { $etype == "none" } {
            notice "No parasitic extraction required."
            return 0
        }

        if { $etype == "full" } {
            ## Capture Adjust_rc factors
            set adjust_factors { }
            foreach arg {res_factor cc_factor gndc_factor} {
                lappend adjust_factors [get_arg $arg]
                ## remove from OPT so "build_cmd" can work
                rm_arg $arg
            }
        }


        ## remove args not used for build_cmd
        rm_arg res_factor cc_factor gndc_factor

        set ext [get_tm Ext]
        set_arg ext_model_file [state ext_model_file]


        ## at this point, must update remotes, if needed before extraction

        set up [state update_obj]
        ## if UP is empty, no multimode
        if { $up ne "" } {
            $up update [state rmt_list]
            $up disable_eco
        }

        notice "Performing $etext $ctype extraction."
        set ext_cmd [build_cmd $ext "extract"]

	puts "$ext_cmd"

        if { $etype == "eco" } {
            append ext_cmd " -eco"
        }

        set rc [::eval $ext_cmd]



        if { $rc != 0 } {
            error "Error $rc during $etext $ctype extraction" "" -1
        }
        cmd_rlog "End of $etext $ctype extraction"

        if { $etype == "full" } {
            ## Call adjust_rc
            notice "Adjusting Extraction factors to: $adjust_factors"
            $ext adjust_rc -res_factor [lindex $adjust_factors 0] -cc_factor [lindex $adjust_factors 1] -gndc_factor [lindex $adjust_factors 2]
        }

        update_opt_data
        cmd_rlog "End of Extract."

        if { $up ne "" } {
            $up update_parasitics [state rmt_list]
            $up enable_eco
        }
    }

    VOB::add_args EXT_DEFINE_DERIVED_CORNER {
        { ext_corner nstr 1 }
        { name nstr 1}
        { res_factor num 0 1.0}
        { cc_factor num 0 1.0}
        { gndc_factor num 0 1.0}
    }
    VOB::add_help EXT_DEFINE_DERIVED_CORNER {
        { ext_corner "Index of previously defined extraction corner" }
        { name "Name of corner to create" }
        { res_factor "Multiply resistance of original corner by res_factor" }
        { cc_factor "Multiply capacitance of original corner by cc_factor" }
        { gndc_factor "Multiply ground capacitance of original corner by gndc_factor" }
    }

    public method define_derived_corner { args } {

        if { [state db_has_extraction_corners] == 1 } {
            error "Database already has extraction/derived corners [get_extraction_corners] defined. No further definitions possible." "" -1
        }

        parse_args
        echo [cmdline]
        get_all_args

        ## will fail if corner does not exist

        if { [check_extraction_corner $ext_corner] eq "" } {
            error "Extraction corner $ext_corner has not been defined!" "" -1
        }
        if { [check_extraction_corner $name] ne "" } {
            error "Extraction corner $name has already been defined!" "" -1
        }


        set x [get_tm Ext]
        set rc [$x define_derived_corner -process_corner_name $ext_corner -name $name -res_factor $res_factor -cc_factor $cc_factor -gndc_factor $gndc_factor]
        if { $rc != 0 } {
            error "Error $rc from define_derived_corner!" "" -1
        }
        #set dobj [[DER_CORNER #auto] this]
        #class_configure $dobj
        #$dobj set_cmd [cmdline]
    }

    VOB::add_args EXT_INDEPENDENT_SPEF_CORNER {
    }
    VOB::add_help EXT_INDEPENDENT_SPEF_CORNER {
    }

    public method independent_spef_corner { } {
        set x [get_tm Ext]
        set rc [$x independent_spef_corner]
        if { $rc != 0 } {
            error "Error $rc from independent_spef_corner" "" -1
        }
    }

    VOB::add_args EXT_DEFINE_EXT_CORNER {
        { model_idx num 1 }
        { name nstr 1}
    }
    VOB::add_help EXT_DEFINE_EXT_CORNER {
        { model_idx "Index of corresponding model in extraction rules file" }
        { name "Name of corner to create" }
    }

    public method define_ext_corner { args } {

        if { [state db_has_extraction_corners] == 1 } {
            error "Database already has extraction/derived corners [get_extraction_corners] defined. No further definitions possible." "" -1
        }

        parse_args
        echo [cmdline]
        get_all_args

        if { [check_extraction_corner $name] ne "" } {
            error "Extraction corner $name has already been defined!" "" -1
        }
        set x [get_tm Ext]
        set rc [$x define_process_corner -ext_model_index $model_idx -name $name]
        if { $rc != 0 } {
            error "Error $rc from define_ext_corner" "" -1
        }

        #set eobj [[EXT_CORNER #auto] this]
        #class_configure $eobj
        #$eobj set_cmd [cmdline]
    }

    private variable special_ext "";
    private variable process_read 0
    VOB::add_args EXT_READ_PROCESS {
        {file iflist 1}
    }
    public method read_process { args } {
        if { $process_read == 1 } { return }
        parse_args
        get_all_args
        if { $special_ext eq "" } {
            set special_ext [Ext #auto]
        }
        foreach f $file {
            set rc [$special_ext read_process -file $f]
            if { $rc != 0 } {
                error "Error $rc during read_process" "" -1
            }
        }
        set process_read 1
    }

    VOB::add_args EXT_RULES_GEN {
        {file ofile 1}
        {name nstr 1}
        {pattern nstr 1}
    }
    public method rules_gen { args } {
        if { $special_ext eq "" } {
            error "Must call read_process first!" "" -1
        }
        parse_args
        get_all_args
        set rc [$special_ext rules_gen -file $file -name $name -pattern $pattern]
        if { $rc != 0 } {
            error "Error $rc during rules_gen" "" -1
        }
    }
    VOB::add_args EXT_METAL_RULES_GEN {
        {name nstr 1}
        {pattern nstr 1}
        {metal nstr 1}
    }
    public method metal_rules_gen { args } {
        if { $special_ext eq "" } {
            error "Must call read_process first!" "" -1
        }
        parse_args
        get_all_args
        set rc [$special_ext metal_rules_gen -name $name -pattern $pattern -metal $metal]
        if { $rc != 0 } {
            error "Error $rc during metal_rules_gen" "" -1
        }
    }

    VOB::add_args EXT_PGEN_RULES {
        {process_file iflist 1}
        {process_name nstr 1}
        {patterns nlist 1}
        {start_dir nstr 0 "."}
    }
    public method pgen_rules { args } {
        if { $special_ext eq "" } {
            error "Must call read_process first!" "" -1
        }
        parse_args
        get_all_args
        $special_ext get_ext_metal_count -metal_count= ::mc
        set append 0
        foreach pattern $patterns {
            for { set i 1 } { $i < $::mc } { incr i } {
                set cmd "sys set_pscript -append [expr $append>0] -cmd {[list "ext read_process -file {{$process_file}}" "ext metal_rules_gen -name $process_name -pattern $pattern -metal $i"]} -dir_list {$start_dir} "
                #puts "CM: $cmd"
                call_milos $cmd
                incr append
            }
        }

        call_milos "sys show_pscript"
        #call_milos "sys launch_pscript"
    }

    VOB::add_args EXT_PGEN_LOAD_DESIGN {
        {def ifile 1}
        {lefs flist OR_leffiles}
        {leffiles flist OR_lefs}
        {max_length num 1}
        {netid_file ifile 1}
    }
    public method pgen_load_design { args } {
        parse_args
        get_all_args

        rm_arg netid_file max_length

        set cmd [rebuild_cmd pdb load_design]
        append cmd " -no_order_wires 1"
        call_milos $cmd

        set nids [read_file $netid_file]
        if { $nids eq "" } {
            error "No net ids found in file $netid_file!" "" -1
        }
        set db [get_tm DB]
        foreach id $nids {
            $db order_wires -net $id -force -max_length $max_length"
        }
    }

    private variable indep_ext ""
    VOB::add_args EXT_RUN_SOLVER { 
        {net nstr 1}
        {shape nstr 1}
    }
    public method run_solver { args } {
        parse_args 
        get_all_args
        if { $indep_ext == "" } {
            set indep_ext [Ext #auto]
        }
        $indep_ext run_solver -net $net -shape $shape
    }

    VOB::add_args EXT_PGEN_CRITICAL {
        {start_dir nstr 0 "tmp"}
        {netid_file ifile 1}
        {process_file ifile 1}
    }
    public method pgen_critical { args } {
        if { $special_ext eq "" } {
            error "Must call read_process first!" "" -1
        }
        parse_args
        get_all_args

        $special_ext load_model -lef_rc
        $special_ext extract -max_res 0.0 -no_merge_via_res
        $special_ext read_process -file $process_file
        $special_ext write_spef -file ref.3d.spef -init
        
        set nids [read_file [get_arg netid_file]]
        if { $nids eq "" } {
            error "No net ids found in file [get_arg netid_file]!" "" -1
        }

        file mkdir tmp
        set append 1
        foreach id $nids {
            $special_ext bench_net -net $id -write_to_solver -dir tmp

            set shapelist [read_file "$start_dir/$id/shapeId"]
            foreach shape $shapelist {
                set cmd "sys set_pscript -append [expr $append>1] -dir_list {$start_dir} -cmd {[list "ext run_solver -net $id -shape $shape"]}" 
                #puts "CM: $cmd"
                call_milos $cmd
                incr append
            }
        }
        call_milos "sys show_pscript"
        #call_milos "sys launch_pscript"

    }

    VOB::add_args EXT_READ_QCAP {
        {file ifile 1}
        {cap_file ifile 0}
        {design nstr 0 "QCAP_BLOCK"}
    }

    VOB::add_help EXT_READ_QCAP {
        {file "file containing geometries" }
        {cap_file "file containing GND and CC caps" }
        {design "design name for defout"}
    }

    public method read_qcap { args } {
        parse_args
        set ext [get_tm Ext]
        set cmd [build_cmd $ext "read_qcap"]
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc during read_qcap!" "" -1
        }
    }

}

