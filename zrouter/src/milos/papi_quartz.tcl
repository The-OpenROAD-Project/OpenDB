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


itcl::class PAPI_QUARTZ {

    inherit PAPI_PSTA;

    public method constructor { } {
        set plugin_proc_list { start_init init_variables init_netlist read_text_file set_allterms set_endterms set_actterms send_actt_data get_vl  read_actt write_actt read_allt read_endt get_slack get_slew set_all_ports get_worst_slack }
        set plugin_api_list { milos_get_id_name milos_createPin milos_removePin milos_swapCell milos_removeCell milos_createCell milos_connectNet milos_disconnectNet milos_removeNet milos_createNet read_spef get_design_slack set_requirements set_engine_req init_req_vars set_plugin_mode}
    }

    ##### ACTUAL METHODS
    public proc init_variables { pref } {
        ## generic initialization section
        global allt allt_list alln endt endt_list actt actt_list
        array set allt { } 
        array set alln { } 
        array set endt { } 
        array set actt { } 

        set allt_list {}
        set endt_list {}
        set actt_list {}

        global vl
        array set vl {}

        global dpref
        set dpref $pref

        global ports all_inputs all_outputs
        set ports {}
        set all_inputs {}
        set all_outputs {}

        global conv_factor
        set conv_factor 1e9
    }

    proc mytype { } {
        return "quartz"
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


        global m magmaLibs

        set magmaLibs ""
        foreach lib $libs {
            lappend magmaLibs [import lib $lib]
        }

        set m [import netlist $blockname -verilog]

        config timing native_ocv on
        config timing full_propagation on

        foreach lib $magmaLibs {
          run bind logical $m $lib
        }

        set ::milos_si $si

        force wire model final $::m
        read_spef $spefname

        config timing derate cell $min_derate -early
        config timing derate wire $min_derate -early
        config timing derate cell $max_derate -late
        config timing derate wire $max_derate -late

        ## ?? best?
        init_req_vars
        set_requirements 0 1 0 1
        set_plugin_mode "auto"
        #config condition case both

        config timing clock delay ideal


        set sdc_cnt 0
        foreach sdcFile $sdc_file {
            global dpref
            set tr_name "${dpref}${sdc_cnt}_trans.sdc"
            import sdc $m $sdcFile -translation $tr_name
            incr sdc_cnt
        }
        catch {check timing $::m}
        catch {check design $::m -print unbound_cells}

        set cmd {proc report_timing { args } {
            set flag 0
            set fname ""
            foreach arg $args {
                if { $flag == 1 } {
                    set fname $arg
                    break
                } elseif { $arg eq ">" } {
                    set flag 1
                }
            }
            if { $flag == 1 } {
                report timing path $::m -all -slack 1.0n -file $fname
            } else {
                report timing path $::m -all -slack 1.0n 
            }
        }}
        ::eval $cmd


        set cmd {proc all_inputs { } {
            set_all_ports
            return $::all_inputs
        }}
        ::eval $cmd
        set cmd {proc all_outputs { } {
            set_all_ports
            return $::all_outputs
        }}
        ::eval $cmd
    }

    public proc set_engine_req { min max } {
        if { $max == 1 } {
            if { $min == 1 } {
                config condition case both
            } else {
                config condition case worst
            }
        } else {
            if { $min == 1 } {
                config condition case best
            } else {
                ## unknown - let's be safe
                config condition case both
            }
        }
    }

    public proc set_all_ports { } {
        if { $::ports == {} } {
            set ::ports [data find $::m * -type mpin]
            foreach port $::ports {
                if { [data get $port direction] eq "in" } {
                    lappend ::all_inputs $port
                } else {
                    lappend ::all_outputs $port
                }
            }
        }
    }
        
    public proc read_spef { ifile } {
        if { $::milos_si > 0 } {
            import spef -case both -store_coupling $::m $ifile
        } else {
            import spef -case both $::m $ifile
        }
    }

    public proc read_text_file { ifile } {
        if { [file exists $ifile] } {
            set fp [open $ifile r]
            set data [read $fp]
            close $fp
            return $data
        } else {
            return "-1"
        }
    }

    public proc write_actt { filename } {
        set fp [open $filename w]
        set rc [ catch {set rc [send_actt_data $fp]} output ]

        if { $rc != 0 } {
            puts $fp "#ERROR $rc $output"
        }
        close $fp
        return $rc
    }

    public proc read_actt { filename } {
        return [set_actterms [read_text_file $filename]]
    }

    public proc read_endt { filename } {
        return [set_endterms [read_text_file $filename]]
    }

    public proc read_allt { filename } {
        return [set_allterms [read_text_file $filename]]
    }



    public proc set_actterms { alist } {
        set ::actt_list $alist
        return "ACT size [llength $::actt_list]"
    }

    public proc set_endterms { elist } {
        set ::endt_list $elist
        return "END size [llength $::endt_list]"
    }

    public proc get_vl { name } {
        if { [data exists $::m/$name] } {
            return [query node pin $::m/$name]
        } else {
            return ""
        }
    }

    public proc get_slack { v } {
        set x "1.011e29"
        if { $::milos_min_slack == 1 } {
            set x [unfix [query model slack $v -early]]
        } 

        set y "1.011e29"
        if { $::milos_max_slack == 1 } {
            set y [unfix [query model slack $v -late]]
        }
        return [list $x $y $x $y]
    }

    public proc old_get_slack { v } {
        set result [list [unfix [query node slack -cppr $v -early -edge rise]]]
        lappend result [unfix [query node slack -cppr $v -late  -edge rise]]
        lappend result [unfix [query node slack -cppr $v -early -edge fall]]
        lappend result [unfix [query node slack -cppr $v -late  -edge fall]]
        return $result
    }

    public proc get_slew { v } {
        set r ""
        if { $::milos_max_slack == 1 } {
            if { $::milos_min_slack == 1 } {
                append r "[unfix [query node slew $v -early -edge rise]] "
                append r "[unfix [query node slew $v -late  -edge rise]] "
                append r "[unfix [query node slew $v -early -edge fall]] "
                append r "[unfix [query node slew $v -late  -edge fall]]"
            } else {
                append r "1.011e29 [unfix [query node slew $v -late -edge rise]] "
                append r "1.011e29 [unfix [query node slew $v -late -edge fall]]"
            }
        } else {
            if { $::milos_min_slack == 1 } {
                append r "[unfix [query node slew $v -early -edge rise]] 1.011e29 "
                append r "[unfix [query node slew $v -early -edge fall]] 1.011e29"
            } else {
                set r "1.011e29 1.011e29 1.011e29 1.011e29"
            }
        }
        return $r
    }

    public proc get_worst_slack { minmax v } {
        set result [unfix [query node slack -cppr $v -$minmax -edge rise]]
        set x [unfix [query node slack -cppr $v -$minmax -edge fall]]
        if { $x < $result } {
            set result $x
        }
        return $result
    }

    public proc new_get_design_slack { minmax } {
        set tns 0.0
        set wns 1e30
        set qwns 1e30
        set bad 1.011e29
        set viol 0
        set edge "late"
        if { $minmax eq "min" } {
            set edge "early"
            if { $::milos_min_slack == 0 } {
                return "$bad 0 0 $bad"
            }
        }

        ## WNS
        set x [query model slack $::m -$edge]
        if { $x ne "INF" } {
            set wns [expr {[unfix $x] * $::conv_factor}]
        } 

        ## TNS
        set x [query model slack $::m -$edge -tns]
        if { $x ne "INF" } {
            set tns [expr {[unfix $x] * $::conv_factor}]
        } 

        set x [query timing path slack $::m -${edge}]
        if { $x ne "INF" } {
            set qwns [unfix [query timing path slack $::m -${edge}]]
        }
        set qwns [expr {$qwns * $::conv_factor}]

        return "$wns $tns $viol $qwns"
    }

    public proc get_design_slack { minmax } {
        set tns 0.0
        set wns 1e30
        set qwns 1e30
        set bad 1.011e29
        set viol 0
        set edge "late"
        set worst_id "none"
        if { $minmax eq "min" } {
            set edge "early"
            if { $::milos_min_slack == 0 } {
                return "$bad 0 0 $bad"
            }
        }

        foreach term $::endt_list {
            if { [info exists ::allt($term)] == 0 } {
                continue
            }
            set v $::allt($term)
            if { $v eq "" } {
                continue
            }
            set slack [get_worst_slack $edge $v]
            if { $slack < $wns } {
                set wns $slack
                set worst_id $::alln($term)
            }
            if { $slack < 0.0 } {
                set tns [expr {$tns + $slack}]
                incr viol
            }
        }
        set tns [expr {$tns * $::conv_factor}]
        set wns [expr {$wns * $::conv_factor}]

        set x [query timing path slack $::m -${edge}]
        if { $x ne "INF" } {
            set qwns [unfix [query timing path slack $::m -${edge}]]
        }
        set qwns [expr {$qwns * $::conv_factor}]

        return "$wns $tns $viol $qwns $worst_id"
    }

    public proc send_actt_data { fp } {
        #pdebug "Start SAD"
        set var "::actt_list"
        if { [llength $::actt_list] == 0 } {
            set var "::allt_list"
        }
        puts $fp "[llength [set $var]]"
        #pdebug "var: $var, [llength [set $var]]"
        foreach term [set $var] {
            if { [info exists ::allt($term)] == 0 } {
                puts $fp "# $term INVALID"
                puts $fp "INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY"
                #pdebug "# $term INVALID"
                #pdebug "INF INF INF INF INF INF INF INF"
                continue
            }
            set v $::allt($term)
            puts $fp "# $term "
            #pdebug "# $term "

            if { $v eq "" } {
                puts $fp "INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY"
                pdebug "INF INF INF INF INF INF INF INF"
            } else {
                #set rc [ catch {pdebug "[get_slack $v] [get_slew $v]"} output]
                #if { $rc != 0 } {
                    #pdebug "#ERROR $rc $output"
                #}
                puts $fp "[get_slack $v] [get_slew $v]"
            }
        }
        puts $fp "."
        #pdebug "."
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
                set allt($id) [get_vl $name]
                set alln($id) $name
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



    ######################################################################
    #### Plugin API
    ######################################################################



    public proc milos_get_id_name { id } {
        return [array get ::allt $id]
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

    ## CELL commands

    public proc milos_swapCell { name master } {
        global m
        data attach $m/$name cell_model [find_model $master]
        eco "swap_cell $name $master"
    }

    public proc milos_removeCell { name } {
        global m
        data delete object $m/$name
        eco "data delete object $m/$name"
        eco "remove_cell $name"
    }

    public proc milos_createCell { name master } {
        global m
        data create cell  [find_model $master] $m $name
    }


    ## NET commands
    
    public proc milos_connectNet { id net } {
        global m
        data attach $m/net:$net net_pin $::allt($id)
        #eco "connect_net $net $name"
    }

    public proc milos_disconnectNet { id net } {
        global m
        data detach $m/net:$net net_pin $::allt($id)
        #eco "disconnect_net $net $pin"
    }

    public proc milos_removeNet { name } {
        global m
        data delete object $m/net:$name
        eco "remove_net $name"
    }

    public proc milos_createNet { name } {
        global m
        data create net $m $name
        eco "create_net $name"
    }
}
