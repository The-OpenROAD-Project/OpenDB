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

## plugin.tcl - plugin methods
## System module for Milos
## Author: Mattias Hembruch

package require Itcl

package provide ade::milos::papi 0.1


itcl::class PAPI_PT {

    inherit PAPI_PSTA;

    ##### ACTUAL METHODS
    public method constructor { } {
        set plugin_proc_list { init_variables init_netlist translate_netlist read_text_file set_vertex set_allterms set_endterms set_actterms send_actt_data get_vl get_vl2 get_slack min_slack max_slack get_slew convert read_actt write_actt read_allt read_endt read_spef read_i_spef read_ni_spef write_a get_design_slack getCellLib get_path_report internal_path_report rpt_get_endpoints get_wire_spread_paths add_clock_delay get_clock_period add_clock_delay write_report_data get_report init_path_reports finalize_reports debug_report next_rpt_data_file init_rpt_vars milos_reset_design start_init show_net_cap }

    }

    public proc init_variables { pref } {
        ## generic initialization section
        global allt allt_list endt endt_list actt actt_list
        array set allt { }
        array set alln { }
        array set endt { }
        array set actt { }

        set allt_list {}
        set endt_list {}
        set actt_list {}

        global conv_factor
        set conv_factor 1e-9

        global dpref
        set dpref $pref

        global milos_si
        set milos_si 0

        global milos_cell_resolution
        set milos_cell_resolution 0

        global _saved_cp 
        array set _saved_cp {}

        init_rpt_vars
        global milos_max_derate milos_min_derate
        set milos_max_derate 1.0
        set milos_min_derate 1.0

        global new_milos_nets
        set new_milos_nets {}
    }

    proc mytype { } {
        return "pt"
    }

    public proc start_init { dpref } {
        init_variables $dpref 

        if { $::dlevel & 1 } {
            open_debug
        }
        if { $::dlevel & 2 } {
            open_pdebug
        }
    }

    public proc init_netlist { min_derate max_derate sdc_file blockname spefname libs max min si top dbs restore_dir } {

        global link_path
        global timing_save_pin_arrival_and_slack
        global timing_remove_clock_reconvergence_pessimism
        if { $restore_dir eq "" } {
            if { [llength $dbs] > 0 } {
                foreach db $dbs {
                    read_db $db
                }
            } else {
                foreach lib $libs {
                    read_lib $lib
                }
            }
            set lp "* "
            foreach_in_collection lib [get_libs *] {
                lappend lp [get_object_name $lib] 
            }
            set link_path $lp
            read_verilog $blockname


            link_design 
        } else {
            puts "RESTORE SESSION $restore_dir"
            restore_session $restore_dir
        }
        set timing_save_pin_arrival_and_slack true

        set ::milos_min_derate $min_derate
        set ::milos_max_derate $max_derate
        set_derating 

        init_req_vars
        set_requirements 1 1 1 1
        set_plugin_mode "auto"
        #set_operating_conditions -analysis_type on_chip_variation

        
        set ::milos_si $si
        if { $::milos_si > 0 } {
            set ::si_enable_analysis true
            read_parasitics -keep_capacitive_coupling $spefname
        } else {
            read_parasitics $spefname
        }

        read_sdc $sdc_file
        set timing_remove_clock_reconvergence_pessimism true

    }

    public proc milos_reset_design { } {
        remove_design -all
    }

    public proc translate_netlist { sdc_file sdc_out blockname libs top dbs } {
        global link_path
        if { [llength $dbs] > 0 } {
            foreach db $dbs {
                read_db $db
            }
        } else {
            foreach lib $libs {
                read_lib $lib
            }
        }
        set lp "* "
        foreach_in_collection lib [get_libs *] {
            lappend lp [get_object_name $lib] 
        }
        set link_path $lp
        read_verilog $blockname

        read_sdc $sdc_file

        write_sdc $sdc_out

        milos_reset_design
    }

    public proc set_derating { } {

        if { ($::milos_min_derate == 1.0) && ($::milos_max_derate == 1.0) } {
            reset_timing_derate
        } else {
            set_timing_derate -early $::milos_min_derate
            set_timing_derate -late $::milos_max_derate
        }
    }
    public proc get_vl2 { n2 id } {
        set rc ""
        if { $id > 0 } {
            set rc [get_pins $n2]
        } else {
            set rc [get_port -exact $n2]
        }
        return $rc
    }

    public proc get_vl { name } {
        regsub -all "\\\\" $name "\\\\\\\\\\\\\\\\" n2
        set rc ""
        set rc [get_pins $n2]
        if { $rc == "" } {
            set rc [get_port -exact $n2]
        }
        return $rc
    }

    public proc convert { val } {
        if { ($val eq "INFINITY") || ($val eq "")} {
            return "INFINITY"
        } else {
            return [expr {$val * $::conv_factor}]
        }
    }

    public proc min_slack { v } {
        if { $::milos_min_slack == 1 } {
            set r "[convert [get_attribute $v min_rise_slack]] "
            set f "[convert [get_attribute $v min_fall_slack]] "
            if { $r < $f } {
                return $r
            } else {
                return $f
            }
        } else {
            return 1.011e29
        }
    }

    public proc max_slack { v } {
        if { $::milos_max_slack == 1 } {
            set r "[convert [get_attribute $v max_rise_slack]] "
            set f "[convert [get_attribute $v max_fall_slack]] "
            if { $r < $f } {
                return $r
            } else {
                return $f
            }
        } else {
            return 1.011e29
        }
    }


    public proc get_slack { v } {
        set r ""
        if { $::milos_max_slack == 1 } {
            if { $::milos_min_slack == 1 } {
                append r "[convert [get_attribute $v min_rise_slack]] "
                append r "[convert [get_attribute $v max_rise_slack]] "
                append r "[convert [get_attribute $v min_fall_slack]] "
                append r "[convert [get_attribute $v max_fall_slack]]"
            } else {
                append r "1.011e29 [convert [get_attribute $v max_rise_slack]] "
                append r "1.011e29 [convert [get_attribute $v max_fall_slack]]"
            }
        } else {
            if { $::milos_min_slack == 1 } {
                append r "[convert [get_attribute $v min_rise_slack]] 1.011e29 "
                append r "[convert [get_attribute $v min_fall_slack]] 1.011e29"
            } else {
                append r "1.011e29 1.011e29 1.011e29 1.011e29"
            }
        }
        return $r
    }

    public proc get_slew { v } {
        set r ""
        if { $::milos_max_slew == 1 } {
            if { $::milos_min_slew == 1 } {
                append r "[convert [get_attribute $v actual_rise_transition_min]] "
                append r "[convert [get_attribute $v actual_rise_transition_max]] "
                append r "[convert [get_attribute $v actual_fall_transition_min]] "
                append r "[convert [get_attribute $v actual_fall_transition_max]]"
            } else {
                append r "1.011e29 [convert [get_attribute $v actual_rise_transition_max]] "
                append r "1.011e29 [convert [get_attribute $v actual_fall_transition_max]]"
            }
        } else {
            if { $::milos_min_slew == 1 } {
                append r "[convert [get_attribute $v actual_rise_transition_min]] 1.011e29 "
                append r "[convert [get_attribute $v actual_fall_transition_min]] 1.011e29"
            } else {
                append r "1.011e29 1.011e29 1.011e29 1.011e29"
            }
        }
        return $r
    }

    public proc get_design_slack { minmax } {
        set tns 0.0
        set wns 1e30
        set viol 0
        set worst_id "none"
        foreach term $::endt_list {
            if { [info exists ::allt($term)] == 0 } {
                continue
            }
            set v $::allt($term)
            if { $v eq "" } {
                continue
            }
            set rs [get_attribute $v ${minmax}_rise_slack]
            set fs [get_attribute $v ${minmax}_fall_slack]
            if { $fs < $rs } {
                set rs $fs
            }   
            if { $rs < $wns } {
                set wns $rs
                set worst_id $::alln($term)
            }   
            if { $rs < 0.0 } {
                set tns [expr {$tns + $rs}]
                incr viol
            }
        }

        return "$wns $tns $viol $wns $worst_id"
    }


    public proc send_actt_data { fp } {
        pdebug "Start SAD"
        set var "::actt_list"
        if { [llength $::actt_list] == 0 } {
            set var "::allt_list"
        }
        puts $fp "[llength [set $var]]"
        pdebug "var: $var, [llength [set $var]]"
        foreach term [set $var] {
            if { [info exists ::allt($term)] == 0 } {
                puts $fp "# $term INVALID"
                puts $fp "INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY"
                pdebug "# $term INVALID"
                pdebug "INF INF INF INF INF INF INF INF"
                continue
            }
            set v $::allt($term)
            puts $fp "# $term "
            pdebug "# $term "

            if { $v eq "" } {
                puts $fp "INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY"
                pdebug "INF INF INF INF INF INF INF INF"
            } else {
                puts $fp "[get_slack $v] [get_slew $v]"
                pdebug "[get_slack $v] [get_slew $v]"
            }
        }
        puts $fp "."
        pdebug "."
        return $var
    }


    public proc set_allterms { tlist } {
        global allt allt_list alln
        array unset allt
        array set allt { }
        set allt_list {}
        set tl [split $tlist "\n"]
        foreach arg $tl {
            set nl [split $arg " "]
            foreach { id name } $nl {
                lappend allt_list $id
                regsub -all "\\\\" $name "\\\\\\\\\\\\\\\\" n2
                set allt($id) [get_vl2 $n2 $id]
                set alln($id) $n2
            }
        }
        #write_a
        return "ALL size [llength $::allt_list]"
    }


    public proc write_a { } {
        global allt
        set f1 [open all_out.lst w]
        foreach idx [array names allt] {
            puts $f1 "$idx $allt($idx)"
        }
        close $f1
    }

    public proc getCellLib { master } {

        set libx ""
        foreach_in_collection lib [get_libs *] {
            set libn [get_object_name $lib]
            if { [get_lib_cell $libn/$master] ne "" } {
                if { $libx eq "" } {
                    set libx $libn
                } else {
                    error "Found two libraries with master $master"
                }
            }
        }
        if { $libx ne "" } {
            return $libx
        } else {
            error "Cannot find cell type $master"
        }
    }

    public proc show_net_cap { net } {
         set n [get_net $net]
         set wcn [get_attribute $n wire_capacitance_min] 
         set wcx [get_attribute $n wire_capacitance_max] 
         set pcn [get_attribute $n pin_capacitance_min] 
         set pcx [get_attribute $n pin_capacitance_max] 
         set tcn [get_attribute $n pin_capacitance_min] 
         set tcx [get_attribute $n pin_capacitance_max] 
         puts "Caps for net $net (Wnx / Pnx / Tnx)"
         puts "$wcn  $wcx   /   $pcn  $pcx   /   $tcn  $tcx"
    }

    public proc read_spef { file } {
        if { $::milos_si > 0 } {
            read_parasitics -complete_with ZERO -keep_capacitive_coupling $file
            eco "read_parasitics -keep_capacitive_coupling $file"
        } else {
            read_parasitics -complete_with ZERO $file
            eco "read_parasitics $file"
        }

        ## now update all nets that do not have valid parasitics
        foreach net $::new_milos_nets {
            set n [get_net $net]
            set vp [get_attribute $n has_valid_parasitics] 
            set dp [get_attribute $n has_detailed_parasitics] 
            if { $vp == false } {
                #puts "Fixing net $net ($vp, $dp)"
                #show_net_cap $net
                set_load -min -max 0 $n
                #puts "Fixed net $net ($vp, $dp)"
                #show_net_cap $net
            }
        }
        set ::new_milos_nets {}
    }

    public proc read_ni_spef { file } {
        if { $::milos_si > 0 } {
            read_parasitics -keep_capacitive_coupling $file
            eco "read_parasitics -keep_capacitive_coupling $file"
        } else {
            read_parasitics $file
            eco "read_parasitics $file"
        }
    }
    public proc read_i_spef { file } {
        if { $::milos_si > 0 } {
            read_parasitics -increment -keep_capacitive_coupling $file
            eco "read_parasitics -increment -keep_capacitive_coupling $file"
        } else {
            read_parasitics -increment $file
            eco "read_parasitics -increment $file"
        }
    }
    public proc  add_clock_delay { if } {
        puts "ACD: $if!!!"
        set dat [read_text_file $if]
        set pl [split $dat "\n"]
        puts "ACD: Lines: [llength $pl]"
        foreach line $pl {
            if { [string index [string trim $line] 0] eq "#" } { continue }
            puts "LINE: $line"
            foreach { pin setup hold } $line {
                puts "New setup Delay for pin $pin: $setup"
                set p [get_vl $pin]
                set_clock_latency -max $setup $p
                puts "New hold Delay for pin $pin: $hold"
                set_clock_latency -min $hold $p
            }
        }
        puts "END ACD!!!"
    }

    public proc  get_clock_period { of } {
        set fp [open $of w]
        foreach_in_collection clk [all_clocks] {
            set x [get_attribute $clk sources]
            set y [get_attribute $x object_class]
            if { $y eq "" } {
                continue
            }
            puts $fp "Clk: [get_object_name $clk] Period: [get_attribute $clk period] SourceType: $y SourceName: [get_object_name $x]"                              
        }
        close $fp
    }


    ## PATH REPORTING PROCS

    public proc init_rpt_vars { } {
        global rpt_fp rpt_data_fp rpt_debug_fp rpt_basename rpt_cnt
        global rpt_datasize rpt_compress 
        global RPT_MAX_COMPRESS_SIZE RPT_MAX_UNCOMPRESS_SIZE RPT_GROUP_SIZE
        set rpt_fp ""
        set rpt_data_fp ""
        set rpt_debug_fp ""
        set rpt_basename ""
        set rpt_cnt 0
        set rpt_datasize 0
        set rpt_compress 0

        ## don't use "exact" values of 2 GB to keep a safety margin
        ## Compressed should not be an issue, except that it's harder to
        ## debug such HUGE files - we get 25:1 compression, but searching
        ## through a 40 GB file takes too long
        set RPT_MAX_COMPRESS_SIZE   4000000000
        ## 2 GB 
        set RPT_MAX_UNCOMPRESS_SIZE 2000000000

        ## how many endpoints to group in a given timing report
        set RPT_GROUP_SIZE 1000
    }

    public proc write_report_data { varname } {
        ## use upvar to avoid passing potentially HUGE string on stack
        upvar $varname data


        ## increment data written counter
        set size [string length $data]
        incr ::rpt_datasize $size

        ## find which limit we need to compare to
        set max $::RPT_MAX_COMPRESS_SIZE
        if { $::rpt_compress == 0 } {
            set max $::RPT_MAX_UNCOMPRESS_SIZE
        }

        debug_report "rpt datasize: $::rpt_datasize > $max ?"
        ## see if we would exceed the limit
        if { $::rpt_datasize > $max } {
            next_rpt_data_file
            set ::rpt_datasize $size
            debug_report "reset rpt datasize"
        }

        ## rpt_data_fp needs to be valid at this point..
        puts $::rpt_data_fp $data
    }

    public proc debug_report { msg } {
        ## dump some debug info
        if { $::rpt_debug_fp ne "" } {
            puts $::rpt_debug_fp $msg
        }
    }

    public proc finalize_reports { } {
        ## clean up
        if { $::rpt_debug_fp ne "" } {
            close $::rpt_debug_fp
            set $::rpt_debug_fp ""
        }
        if { $::rpt_data_fp ne "" } {
            close $::rpt_data_fp
            set $::rpt_data_fp ""
        }
        if { $::rpt_fp ne "" } {
            close $::rpt_fp
            set $::rpt_fp ""
        }
    }

    public proc next_rpt_data_file { } {
        ## get the next data file in the sequence
        if { $::rpt_data_fp ne "" } {
            ## close one that's currently open
            close $::rpt_data_fp
        }

        ## get new name in sequence
        set xf "${::rpt_basename}.${::rpt_cnt}"

        ## increment sequence counter
        incr ::rpt_cnt

        ## see if we're compressing output or not
        if { $::rpt_compress == 1 } {
            set xf "${xf}.gz"
            set ::rpt_data_fp [open "| gzip > $xf" w]
        } else {
            set ::rpt_data_fp [open $xf w]
        }
        debug_report "Next report file: $xf"
        puts $::rpt_fp $xf
    }

    public proc init_path_reports { name compress maxfile } {

        set ::rpt_basename $maxfile
        set ::rpt_cnt 0
        set ::rpt_compress $compress

        set ::rpt_fp [open "$name" w]

        set ::rpt_debug_fp [open "${name}.debug" w]

        next_rpt_data_file
    }

    public proc get_report { cutoff delay style retvarname plist } {
        upvar $retvarname rpt
        if { $style == 2 } {
            redirect -variable rpt {report_timing -path_type full_clock -nosplit -slack_lesser_than $cutoff -delay_type $delay -significant_digits 8 -to $plist -nworst 250 -max_paths 2000000 }
        } else {
            redirect -variable rpt { report_timing -path_type full_clock -start_end_pair -nosplit -slack_lesser_than $cutoff -delay_type $delay -significant_digits 8 -to $plist}
        }
    }

    public proc rpt_get_endpoints { max_cutoff min_cutoff epfile } {
        set pl {}
        set rl {}
        puts "GPR: Initial PIN cutoffs: $max_cutoff $min_cutoff"
        set max_cutoff [expr {$max_cutoff * $::conv_factor}]
        set min_cutoff [expr {$min_cutoff * $::conv_factor}]
        puts "GPR: Adjusted PIN cutoffs: $max_cutoff $min_cutoff"
        set gc 0
        set cnt 0
        if { $epfile ne "none" } {
            set fp [open $epfile w]
        }
        foreach_in_collection reg [all_registers -data_pins -output_pins] {
            incr cnt
            set name [get_object_name $reg]

            #### filter out clock gating pins
            #if { [get_attribute [get_pin $reg] is_clock_gating_pin] == true } {
                #puts "GPR: pin [get_object_name $reg] is a clock gating pin - eliminating it"
                #incr gc
                #continue
            #}
            ####
            if { ([max_slack $reg] < $max_cutoff) 
                || ([min_slack $reg] < $min_cutoff) } {
                puts "Pin [get_object_name $reg] accepted [get_slack $reg]"
                lappend rl $name
                if { $epfile ne "none" } {
                    puts $fp $name
                }
            } 
        }
        if { $epfile ne "none" } {
            close $fp
        }
        puts "GPR: $cnt total register-pins"
        puts "GPR: Eliminated $gc clock gating pins."
        puts "GPR: [llength $rl] pins left after applying cutoffs"


        ## sort, just in case there are differences in order 
        ## between PT instances (when using -split)
        set rl [lsort $rl]


        set eidx [expr $::RPT_GROUP_SIZE - 1]
        while { [llength $rl] > $::RPT_GROUP_SIZE } {
            lappend pl [list [lrange $rl 0 $eidx]]
            set rl [lreplace $rl 0 $eidx]
        }
        lappend pl [list $rl]
        return $pl
    }

    public proc get_path_report { if rptfile maxfile epfile clkfile style {compress 1}  {start_line 4} {skip 1} } {
        set dat [read_text_file $if]
        set pl [split $dat "\n"]

        ## slack cutoff
        set max_cutoff [lindex [string trim [lindex $pl 0]] 1]
        set min_cutoff [lindex [string trim [lindex $pl 1]] 1]

        ## number of flip-flops
        set ffcount [lindex [string trim [lindex $pl 2]] 1]

        ### TESTING
        #set pl {}
        #set compress 0
        ### END TESTING
        if { $clkfile ne "none" } {
            set fp [open $clkfile w]
            foreach_in_collection clk [all_clocks] {
                puts $fp "CLOCK: [get_object_name $clk]"
                foreach_in_collection pin [all_registers -clock_pins -clock $clk] {
                    puts $fp "PIN: [get_object_name $pin]"
                }
            }
            close $fp
        }

        set old_new 0
        set lt [llength $pl]
        #puts "GPR LENGTH: $lt"
        if { [llength $pl] == 3 } {
            puts "GPR NEW!"
            set pl [concat $pl [rpt_get_endpoints $max_cutoff $min_cutoff $epfile]]
            set old_new 1
            set start_line [expr $start_line - 2]
            set gc [expr [llength $pl] + $start_line]
        } else {
            puts "GPR OLD!"
            set gcount [lindex [string trim [lindex $pl 3]] 1]
            set gc [expr $gcount + $start_line]
        }



        puts "internal_path_report $rptfile $max_cutoff $min_cutoff $maxfile $gc $style $compress $start_line $skip $old_new [llength $pl]"

        internal_path_report $rptfile $max_cutoff $min_cutoff $maxfile $gc $style $compress $start_line $skip $old_new $pl
    }

    public proc internal_path_report { rptfile max_cutoff min_cutoff maxfile gc style {compress 1} {start_line 4} { skip 1} old_new pl } {


        init_path_reports $rptfile $compress $maxfile

        for { set i $start_line } { $i < $gc } { incr i $skip } {
            set line [string trim [lindex $pl $i]]
            if { $line eq "" } {
                continue
            }

            set plist {}
            if { $old_new == 0 } {
                foreach pin $line {
                    lappend plist $::allt($pin)
                }
            } else {
                set plist [join $line]
            }

            puts "GET RPT, line $i, [llength $plist]"
            debug_report "Line index: $i, $plist"
            foreach delay { min max } {
                set slack "::milos_${delay}_slack"
                if { [set $slack] == 1 } {
                    set cutoff "${delay}_cutoff"
                    get_report [set $cutoff] $delay $style "rpt_v" $plist
                    write_report_data rpt_v
                }
            }
        }

        finalize_reports
    }


    public proc get_wire_spread_paths { if rptfile maxfile minfile } {
        set dat [read_text_file $if]
        set pl [split $dat "\n"]

        ## slack cutoff
        set num_max [lindex [string trim [lindex $pl 0]] 1]
        set num_min [lindex [string trim [lindex $pl 1]] 1]

        report_timing -path_type full -nosplit -slack_lesser_than 0.0 -delay_type max -significant_digits 8 -max_paths $num_max > $maxfile
        report_timing -path_type full -nosplit -slack_lesser_than 0.0 -delay_type min -significant_digits 8 -max_paths $num_min > $minfile
        set fp [open $rptfile w]
        puts $fp $maxfile
        puts $fp $minfile
        close $fp
    }


    ######################################################################
    #### Plugin API
    ######################################################################


    public proc milos_createNet { name } {
        create_net $name
        eco "create_net $name"
       lappend ::new_milos_nets $name
    }

    ## PIN commands
    public proc milos_createPin { id name } {
        lappend ::allt_list $id
        set ::allt($id) [get_vl $name]
        set ::alln($id) $name

    }

    public proc milos_removePin { id } {
        array unset ::allt $id
        array unset ::alln $id
        set idx [lsearch $::allt_list $id]
        if { $idx >= 0 } {
            set ::allt_list [lreplace $::allt_list $idx $idx]
        }
    }

    public proc milos_connectNet { id net } {
        #puts "\nNet: $net" >> nets.log
        regsub -all "\\\\" $net "\\\\\\\\\\\\\\\\" n2
        #report_net -connections $net >> nets.log
        pdebug "connect_net $n2 [set ::alln($id)]"
        connect_net $n2 [set ::alln($id)]
        #report_net -connections $net >> nets.log
        eco "connect_net $net $::alln($id)"
    }

    public proc milos_disconnectNet { id net } {
        regsub -all "\\\\" $net "\\\\\\\\\\\\\\\\" n2
        #report_net -connections $net >> nets.log
        #disconnect_net $net $::alln($id)
        pdebug "disconnect_net $n2 [set ::alln($id)]"
        disconnect_net $n2 [set ::alln($id)]
        #report_net -connections $net >> nets.log
        eco "disconnect_net $net $::alln($id)"
    }



    ## CELL commands
    public proc milos_createCell { name master } {
        if { $::milos_cell_resolution == 0 } {
            create_cell $name $master
            eco "create_cell $name $master"
        } elseif { $::milos_cell_resolution == 1 } {
            set lib [getCellLib $master]
            create_cell $name $lib/$master
            eco "create_cell $name $lib/$master"
        }
    }


    public proc milos_swapCell { name master } {

        #regsub -all "\\\\" $name "\\\\\\\\\\\\\\\\" n2
        set n2 $name

        if { $::milos_cell_resolution == 0 } {
            size_cell $n2 $master
            eco "size_cell $n2 $master"
        } elseif {  $::milos_cell_resolution == 1 } {
            set lib [getCellLib $master]
            size_cell $n2 $lib/$master 
            eco "size_cell $n2 $lib/$master"
        }

    }

    public proc milos_removeNet { name } {
        regsub -all "\\\\" $name "\\\\\\\\\\\\\\\\" n2
        remove_net $n2
        eco "remove_net $name"
    }

    public proc milos_removeCell { name } {
        regsub -all "\\\\" $name "\\\\\\\\\\\\\\\\" n2
        remove_cell $n2
        eco "remove_cell $name"
    }

}
