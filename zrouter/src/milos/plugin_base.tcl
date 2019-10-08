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

package require ade::milos::vob 0.1
package require ade::milos::dobj 0.1
package require ade::milos::papi 0.1

package provide ade::milos::plugin 0.1

itcl::class PLUGIN_base {

    inherit PLUGIN_DATA VOB

    protected variable FILE_DEBUG 0
    protected variable P_DEBUG 0
    ## protected vars
    ## variables with string values
    protected variable sobj [[SCENARIO #auto] this];
    protected variable corner [[CORNER #auto] this];
    protected variable thresh [[THRESH #auto] this];
    protected variable node_slack [ [NODE_SLACK #auto] this];
    protected variable si 0;
    protected variable tmg;
    protected variable plugin_tcl_file "";
    protected variable full_cnt 0;
    protected variable active_write_cnt 0;
    protected variable active_read_cnt 0;
    protected variable all_fname;
    protected variable active_fname;
    protected variable tcl_name "";

    protected variable api_obj [[PAPI_BASE #auto] this];

    protected variable startsocket 9000;
    protected variable endsocket 10000;

    public method constructor { } { 
        VOB::constructor
        set NAME "PLUGIN"
        set tmg [get_tm Tmg]
        #puts "PLUGIN BASE CONSTRUCTOR: $api_obj tmg= $tmg"

        set_state remote_scenario $sobj
        #puts "PLUGIN BASE CONSTRUCTOR: $api_obj"
    }
    public method reset_tmg { tmg_tm_inst } {
        debug PLUGIN:I "06192012 -- reset_tmg : tmg= $tmg"
	set tmg "::MSlave::$tmg_tm_inst"
        debug PLUGIN:I "06192012 -- reset_tmg : tmg= $tmg    END"
        puts "new tmg= $tmg"
    }
    public method start_plugin { start } {
        set startsocket $start
        set endsocket [expr $startsocket + 100]
        mystart
        myinit 
    }

    public method set_debug { {fdebug 0} {pdebug 0} } {
        set FILE_DEBUG $fdebug
        set P_DEBUG $pdebug
        #puts "FPDEBUG: $FILE_DEBUG $P_DEBUG ($debug)"
    }

    public method read_sdf { cnt } {
        #error "READ SDF not supported by current plugin type!"
    }

    public method save_session { dir } {
        plugin_cmd TE save_session $dir
    }

    protected method myinit {} {
        set plog [plugin_fname $sobj "plog_te" "log"]
        open_plog TE $plog

        if { [$sobj cget -preinit_file] != "" } {
            set rc [plugin_cmd TE "source [$sobj cget -preinit_file]"]
        }

        set translate [$sobj cget -translate]
        set restore [$sobj cget -restore_dir]

        if { ($restore ne "") && ($translate == 1) } {
            error "Cannot restore AND translate!"
        }

        set po [get_plugin_object TE]
        $po configure -translate $translate

        set ovf [$sobj cget -verilog_original_file]

        if { ($translate == 1) && ($ovf ne "" ) } {
            set nsdc [plugin_fname $sobj "new" "sdc"]
            set csdc [plugin_fname $sobj "cvt_new" "sdc"]
            set cmd  "### Initialization for scenario [$sobj cget -name]\n"
            append cmd "translate_netlist "
            append cmd "[$sobj cget -sdc_file]  "
            append cmd "$nsdc "
            append cmd "[$sobj cget -verilog_original_file] "
            append cmd "{"
            append cmd "[$corner cget -lib] "
            append cmd "} "
            append cmd "{"
            append cmd "[$sobj cget -verilog_module] "
            append cmd "} "
            append cmd "{"
            append cmd "[$sobj cget -db] "
            append cmd "}"

            debug PLUGIN:I "Plugin translation cmd: $cmd"
            plugin_cmd TE $cmd

            convert_sdc $nsdc $csdc

            $sobj configure -sdc_file $csdc
        }

        set cmd  "### Initialization for scenario [$sobj cget -name]\n"

        set dimitris_flow [$sobj cget -schedule_style]
	if { $dimitris_flow ne "111" } {
        	append cmd "init_netlist "
	} else {
        	append cmd "init_env "
	}
        append cmd "[$corner cget -min_derate]  "
        append cmd "[$corner cget -max_derate]  "
        append cmd "[$sobj cget -sdc_file]  "
        append cmd "[$sobj cget -verilog_file] "
        append cmd " { [$sobj cget -spef_file] } "
        append cmd " { [$sobj cget -max_spef_file] } "
        append cmd " { [$sobj cget -min_spef_file] } "
        if { ($translate == 1) && ($ovf ne "" ) } {
            append cmd "{ } { } { } "
        } else {
            append cmd "{"
            append cmd "[$corner cget -lib] "
            append cmd "} {"
            append cmd "[$corner cget -max_lib] "
            append cmd "} {"
            append cmd "[$corner cget -min_lib] "
            append cmd "} "
        }
        append cmd "$si "
        append cmd "[$sobj cget -verilog_module] "
        if { [$sobj cget -translate] == 1 } {
            append cmd "{ } "
        } else {
            append cmd "{"
            append cmd "[$sobj cget -db] "
            append cmd "} "
        }

        append cmd "{$restore} "

        ## only for first start
        set deb_pref [plugin_fname $sobj "debug" ""]
        plugin_cmd TE "start_init $deb_pref [$sobj cget -name]"

        debug PLUGIN:I "Plugin init cmd: $cmd"
        plugin_cmd TE $cmd

        if { [$sobj cget -postinit_file] != "" } {
            set rc [plugin_cmd TE "source [$sobj cget -postinit_file]"]
        }
	if { $dimitris_flow eq "111" } {
        	plugin_cmd TE "create_net _MILOS_DUMMYX"
        	set term_fname [plugin_fname $sobj "idterms" dat]
        	plugin_cmd TE "set_term_lists $term_fname"
	}
        set fname [plugin_fname $sobj "eco" log]
        plugin_cmd TE "open_eco $fname"
    }

    public method replay_plugin_cmds { } {
        foreach cmd [state plugin_cmd_list] {
            puts "Replay cmd $cmd"
            plugin_cmd TE $cmd
        }
    }
    ## for re-init, only reload verilog and spef, and re-set conditions..
    public method myreinit {} {
        plugin_cmd TE "milos_reset_design"
        if { [$sobj cget -preinit_file] != "" } {
            set rc [plugin_cmd TE "source [$sobj cget -preinit_file]"]
        }

        ## any translation would have already been done the first time..

        set cmd  "### Initialization for scenario [$sobj cget -name]\n"
        append cmd "init_netlist "
        append cmd "[$corner cget -min_derate]  "
        append cmd "[$corner cget -max_derate]  "
        append cmd "[$sobj cget -sdc_file]  "
        append cmd "[$sobj cget -verilog_file] "
        append cmd "[$sobj cget -spef_file] "

        ## libs are already there..
        append cmd "{ } { } { } "
        append cmd "$si "
        append cmd "[$sobj cget -verilog_module] "

        ## dbs would be there too
        append cmd "{ }"

        debug PLUGIN:I "Plugin re-init cmd: $cmd"
        plugin_cmd TE $cmd

        if { [$sobj cget -postinit_file] != "" } {
            set rc [plugin_cmd TE "source [$sobj cget -postinit_file]"]
        }
    }

    public method restart_plugin { } {
        myrestart
    }

    public method int_plugin_cmd { args } {
        plugin_cmd TE "[join $args]"
    }

    protected method mystart { } {
        start_server TE
    }

    protected method myrestart { } {
        restart_server TE [$sobj cget -start_name]
    }


    public method set_obj_value { obj option value } {
        #puts "SET OBJ: $obj, $option, $value"
        set myobj [set $obj]
        $myobj configure $option $value
        #set cmd "$myobj configure $option $value"
        #::eval $cmd
    }

    public method init_obj { obj args } {
        set myobj [set $obj]
        set cmd "$myobj configure [join $args]"
        ::eval $cmd
    }

    public method get_obj { obj } {
        return [ $[set $obj] get_conf]
    }

    public method set_sidx { val } {
        $sobj set_sidx $val
    }

    public method set_si { val } {
        set si $val
        plugin_cmd TE "set_si $val"
    }

    public method get_any_sta_result { {args ""} } {
            debug PLUGIN:S "get_any_sta_result $args"

        set ret_val [[get_plugin_object TE] cget -result]
        return ret_val
    }
    public method run_any_sta_cmd { args } {
            debug PLUGIN:S "run_any_sta_cmd $args"

        plugin_cmd TE "$args"
        set ret_val [[get_plugin_object TE] cget -result]
            debug PLUGIN:S "ret_val= $ret_val"
        return ret_val
    }
    public method update_node_slack { req_slack } {
            debug PLUGIN:S "t_design_slack $req_slack"

        plugin_cmd TE "get_design_slack $req_slack"
        foreach {wns tns viol nns wterm nsized nnew nremoved opt_method time_elaps} [[get_plugin_object TE] cget -result] {
            break;
        }
        set v  "-worst_${req_slack}_slack $wns -total_${req_slack}_slack $tns -num_${req_slack}_violations $viol -native_worst_${req_slack}_slack $nns -plugin_time $plugin_time(TE) -worst_${req_slack}_term [escape_1 $wterm] -sized_cells_${req_slack} $nsized -new_cells_${req_slack} $nnew -cells_removed_${req_slack} $nremoved -opt_method_${req_slack} $opt_method -time_elaps_${req_slack} $time_elaps"
        #set v  "-worst_${req_slack}_slack $wns -total_${req_slack}_slack $tns -num_${req_slack}_violations $viol -native_worst_${req_slack}_slack $nns -plugin_time $plugin_time(TE) -worst_${req_slack}_term [escape_1 $wterm]"
            debug PLUGIN:S "$v"
        return "-worst_${req_slack}_slack $wns -total_${req_slack}_slack $tns -num_${req_slack}_violations $viol -native_worst_${req_slack}_slack $nns -plugin_time $plugin_time(TE) -worst_${req_slack}_term [escape_1 $wterm] -sized_cells_${req_slack} $nsized -new_cells_${req_slack} $nnew -cells_removed_${req_slack} $nremoved -opt_method_${req_slack} $opt_method -time_elaps_${req_slack} $time_elaps"
        #return "-worst_${req_slack}_slack $wns -total_${req_slack}_slack $tns -num_${req_slack}_violations $viol -native_worst_${req_slack}_slack $nns -plugin_time $plugin_time(TE) -worst_${req_slack}_term [escape_1 $wterm]"
    }

    public method send_eco_string { str } {
        plugin_cmd TE $str
    }

    public method set_timing_engine { } {
        #set name [$sobj cget -block]
	#set db [get_tm DB]
        #$db  save_def -file  ${name}.def
        $tmg set_timing_engine -external
    }

    public method lists_to_plugin { afname efname } {
        plugin_cmd TE "read_allt $afname"
        plugin_cmd TE "read_endt $efname"
    }

    public method send_activeterms { } {
        if { $FILE_DEBUG == 1 } {
            set fname [plugin_fname $sobj "actt.$active_read_cnt" lst]
            puts "Writing active terms to: $fname"
            set fp [open $fname w]
            $tmg write_activeterms -file $fp
            close $fp
            plugin_cmd TE "read_actt $fname" 
            incr active_read_cnt
        } else {
            set sock [[get_plugin_object TE] cget -plugin_socket]
            send_plugin_data TE "set_actterms {"
            $tmg write_activeterms -file $sock
            send_plugin_data TE " }"
            send_plugin_data TE "."
            get_plugin_result TE
        }
        debug PLUGIN:S "ACT to PLUGIN: [ [get_plugin_object TE] cget -result]"
    }



    public method write_cterms_file { } {
        if { $FILE_DEBUG == 1 } {
            #incr active_read_cnt
            #set active_fname [plugin_fname $sobj "cterms.$active_write_cnt" dat]
            set active_fname [plugin_fname $sobj "cterms" dat]
            debug PLUGIN:S "write_cterms_file: Plugin cmd: write_cterms $active_fname"
            set rc [send_plugin_cmd TE "write_cterms $active_fname"]
        } else {
            debug PLUGIN:S "write_cterms_file: Plugin cmd: send_term_lists :cur_channel"
            set rc [send_plugin_cmd TE {send_term_lists $::cur_channel}]
            ## start streaming
        }
    }

    public method send_tmg_commands { filename args } {

	incr active_read_cnt
          
	#set active_fname [plugin_fname $sobj "tmg.$active_write_cnt" file]
	set active_fname $filename

	debug PLUGIN:S "WAS: Plugin cmd: run_tmg_cmd $active_fname $args"

	set rc [send_plugin_cmd TE "run_tmg_cmd $active_fname $args"]

	debug PLUGIN:S "WAS: $args Plugin cmd: run_tmg_cmd DONE: $rc"
    }
    public method accept_tmg_result { args } {
            set rc [get_plugin_result TE]
            debug PLUGIN:S "\t\taccept_tmg_result: $args -- rc= $rc"
    }
    public method write_slack_file { rmt_tmg_info } {

        #set name [$sobj cget -block]
	#set db [get_tm DB]
        #$db  save_def -file  ${name}.def

	#rmt_tmg_info: aterms for slacks, cterms for clock pin info
        if { $FILE_DEBUG == 1 } {
            incr active_read_cnt
            #020810D set active_fname [plugin_fname $sobj "aterms.$active_write_cnt" dat]
            set active_fname [plugin_fname $sobj "$rmt_tmg_info.$active_write_cnt" dat]
            debug PLUGIN:S "WAS: $rmt_tmg_info Plugin cmd: write_actt $active_fname"
	    if { $rmt_tmg_info eq "aterms" } {
            	set rc [send_plugin_cmd TE "write_actt $active_fname"]
	    } elseif { $rmt_tmg_info eq "cterms" } {
            	set rc [send_plugin_cmd TE "write_cterms $active_fname"]
	    }
        } else {
            debug PLUGIN:S "WAS: $rmt_tmg_info Plugin cmd: send_actt ::cur_channel"
	    if { $rmt_tmg_info eq "aterms" } {
            	set rc [send_plugin_cmd TE {send_actt $::cur_channel}]
	    } elseif { $rmt_tmg_info eq "cterms" } {
            	set rc [send_plugin_cmd TE {send_term_lists $::cur_channel}]
	    }
            debug PLUGIN:S "WAS: $rmt_tmg_info Plugin cmd: send_actt DONE: $rc"
            ## start streaming
        }
    }

    public method reset_timing_cache { } {
        ## Not needed for plugin?
        #$tmg reset_timing_cache
    }
    public method read_cterms_file { {scenario -1} {num_scenarios 1} } {
            debug PLUGIN:S "read_cterms_file: BEGIN"
        set myscen $scenario
        if { $scenario ==  -1 } {
            set myscen [$sobj get_sidx]
        }

        if { $FILE_DEBUG == 1 } {
            set rc [get_plugin_result TE]
            debug PLUGIN:S "read_cterm_file : active_fname= $active_fname rc= $rc"
            set fp [open $active_fname r]
            debug PLUGIN:S "read_cterm_file : before read_timing_cache"
            $tmg read_clock_structure -file $fp -num_scenarios $num_scenarios -scenario $myscen
            #$tmg merge_timing_cache -file $fp
            close $fp
            
            incr active_write_cnt
        } else {
            #debug PLUGIN:S "PLUGIN result from WAS: $rc"
            set po [get_plugin_object TE]
            set sock [ $po cget -plugin_socket]
            debug PLUGIN:S "read_cterm_file : PLUGIN socket: $sock"
            $tmg read_clock_structure -stream $sock -num_scenarios $num_scenarios -scenario $myscen
            show_plugin_time TE
            $po configure -plugin_cmd_pending 0

            #$tmg merge_timing_cache -file $sock
            debug PLUGIN:S "RSF RTC done, get result!"
            #set rc [get_plugin_result TE]
            debug PLUGIN:S "RSF RTC"
        }
    }
    public method send_allt_file { allt_file {translate 0} } {
	set fp [open $allt_file w]
	if { $translate>0 } {
		$tmg write_all_terms -file $fp -no_backslash
	} else {
		$tmg write_all_terms -file $fp -no_backslash
	}
	flush $fp
	close $fp

	plugin_cmd TE "read_allt $allt_file"

    }
    public method send_endt_file { endt_file } {
	set fp [open $endt_file w]
	$tmg write_end_terms -file $fp
	flush $fp
	close $fp

        plugin_cmd TE "read_endt $endt_file"
    }
    public method store_timing_cache { {num_scenarios 1} } {
	$tmg store_timing_cache -num_scenarios $num_scenarios
    }
    public method read_slack_file { rmt_tmg_info {scenario -1} {num_scenarios 1} } {
            debug PLUGIN:S "\t\t\rPLUGIN_BASE read_slack_file $rmt_tmg_info ============= BEGIN"
        set myscen $scenario
        if { $scenario ==  -1 } {
            set myscen [$sobj get_sidx]
        }

        if { $FILE_DEBUG == 1 } {
            set rc [get_plugin_result TE]
            debug PLUGIN:S "\t\t\t\tFILE_DEBUG: $rmt_tmg_info $active_fname rc= $rc"
            set fp [open $active_fname r]
            debug PLUGIN:S "\t\t\t\t$rmt_tmg_info before read_timing_cache"
	    if { $rmt_tmg_info eq "aterms" } {
            	$tmg read_timing_cache -file $fp -num_scenarios $num_scenarios -scenario $myscen
	    } elseif { $rmt_tmg_info eq "cterms" } {
            	#$tmg read_clock_structure -file $fp -num_scenarios $num_scenarios -scenario $myscen
            	$tmg merge_clock_structure -file $fp -num_scenarios $num_scenarios -scenario $myscen
	    }
            #$tmg merge_timing_cache -file $fp -scenario
            debug PLUGIN:S "\t\t\t\t$rmt_tmg_info after read_timing_cache"
            #$tmg merge_timing_cache -file $fp
            close $fp
            
            incr active_write_cnt
        } else {
            #debug PLUGIN:S "PLUGIN result from WAS: $rc"
            set po [get_plugin_object TE]
            set sock [ $po cget -plugin_socket]
            debug PLUGIN:S "\t\t\t\t$rmt_tmg_info socket: $sock"
	    if { $rmt_tmg_info eq "aterms" } {
            	$tmg read_timing_cache -file $sock -num_scenarios $num_scenarios -scenario $myscen
	    } elseif { $rmt_tmg_info eq "cterms" } {
            	#$tmg read_clock_structure -stream $sock -num_scenarios $num_scenarios -scenario $myscen
            	$tmg merge_clock_structure -stream $sock -num_scenarios $num_scenarios -scenario $myscen
	    }
            #$tmg merge_timing_cache -stream $sock -scenario
            debug PLUGIN:S "\t\t\t\t$rmt_tmg_info after read_timing_cache"
            show_plugin_time TE
            $po configure -plugin_cmd_pending 0

            #$tmg merge_timing_cache -file $sock
            debug PLUGIN:S "RSF RTC done, get result!"
            #set rc [get_plugin_result TE]
            debug PLUGIN:S "RSF RTC"
        }
        debug PLUGIN:S "\t\t\rPLUGIN_BASE read_slack_file $rmt_tmg_info ============= END"
    }


    public method get_wire_spread_paths { prefix } {
        set idx [$sobj get_sidx]
        set tmp [$sobj cget -tmp_dir]
        set ifile "${prefix}.in"
        set minfile [plugin_fname $sobj $prefix hold.rpt]
        set maxfile [plugin_fname $sobj $prefix setup.rpt]
        set rptfile "${prefix}.${idx}.rpt.lst"
        plugin_cmd TE "get_wire_spread_paths $ifile $rptfile $maxfile $minfile"
    }

    public method set_plugin_control { type } {
        plugin_cmd TE "set_plugin_control $type"
    }
    public method set_plugin_mode { mode } {
        plugin_cmd TE "set_plugin_mode $mode"
    }
    public method get_path_report { prefix {write_ep 0} {compress 1} {sl 4} {skip 1} } {
        set style [$sobj cget -schedule_style]
        set idx [$sobj get_sidx]
        set tmp [$sobj cget -tmp_dir]
        set ifile "${prefix}.in"
        set epfile "none"
        set clkfile "none"
        if { $write_ep == 1 } {
            set epfile ${prefix}.${idx}.ep.lst
            set clkfile ${prefix}.${idx}.clk.lst
        }
        set maxfile [plugin_fname $sobj $prefix setup.rpt]
        set rptfile "${prefix}.${idx}.rpt.lst"
        plugin_cmd TE "get_path_report $ifile $rptfile $maxfile $epfile $clkfile $style $compress $sl $skip"
    }

    public method get_clock_period { prefix } {
        set idx [$sobj get_sidx]
        set ofile "${prefix}.${idx}.periods.out"
        plugin_cmd TE "get_clock_period $ofile"
    }

    #public method set_requirements { min_slack max_slack min_slew max_slew } {
        #plugin_cmd TE set_requirements $min_slack $max_slack $min_slew $max_slew
    #}

    public method set_requirements { args } {
        plugin_cmd TE set_requirements $args
    }

    public method add_clock_delay { file } {
        plugin_cmd TE "add_clock_delay $file"
    }

    protected method write_plugin_file { fname } {
        set fp [open $fname w]
        set tcl_name $fname

        if { $P_DEBUG == 1 } {
            $api_obj write_timing_functions $fp
        }

        $api_obj write_server_utils $fp

        close $fp
    }

    protected method get_tmp_file { } {
        return [plugin_fname $sobj plugin log]
    }
    protected method get_plugin_file { } {
        return [plugin_fname $sobj plugin tcl]
    }


    protected method launch_plugin { startname scriptfile logfile} {
        set cmd "exec $startname $scriptfile >& $logfile &"
        debug PLUGIN:L "Plugin launch cmd: $cmd"
        ::eval $cmd
        debug PLUGIN:L "Waiting [$sobj cget -timeout]"
        after [expr [$sobj cget -timeout] * 1000]
    }

    protected method start_server { type } {
        set po [get_plugin_object $type]
        $po configure -server_socket [get_socket]
        set ::env(ADE_PLUGIN_SOCKET) [$po cget -server_socket]

        set plugin_tcl_file [get_plugin_file]
        write_plugin_file $plugin_tcl_file 

        restart_server TE [$sobj cget -start_name]

        if { $P_DEBUG == 0 } {
            set sock [$po cget -plugin_socket]
            $api_obj write_timing_functions $sock
        }

    }

    protected method restart_server { type start_name } {
        set po [get_plugin_object $type]
        ## make sure we start
        set rc -1
        set retry 0
        while { $rc == -1 }  {
            incr retry
            launch_plugin $start_name $plugin_tcl_file [get_tmp_file]
            notice "Trying to connect to plugin, try $retry"
            set rc [get_client_socket [$po cget -plugin_hostname] [$po cget -server_socket]]
            if { $rc != -1 } {
                notice "Successfully connected to plugin"
                $po configure -plugin_socket $rc
                $po configure -active 1
            } else {
                if { $retry == 5 } {
                    set x [read_file [get_tmp_file]]
                    notice "Error in starting Plugin: $x!"
                    error "ERROR: Cannot start Plugin: $x" 
                    exit
                } else {
                    notice "Retrying plugin start"
                }
            }
        }
    }

    protected method clean_files { } {
        if { $FILE_DEBUG == 0 } {
            file delete "[plugin_fname $sobj [$sobj cget -engine] "v"]"
            file delete "[plugin_fname $sobj [$sobj cget -engine] "spef"].gz"
            file delete $tcl_name
        }
    }

    public method exit_plugin { } {
        send_plugin_cmd TE "exit_remote"
        #clean_files
    }

    protected method convert_sdc { ifile ofile } {
        set replace [$sobj cget -replace_string]
        set ifp [open $ifile r]
        set ofp [open $ofile w]
        while { [gets $ifp line] >= 0 } {
            set nl [escape_line $BS_SHP $line {/}]
            set nl [escape_line $BS_BKP $nl {[}]
            puts $ofp $nl
        }
        close $ofp
        close $ifp

    }

    ###############################################################
    ## Plugin utility functions
    ###############################################################
    protected method get_client_socket {host ssock} {
        set rc [ catch { set client_sock [socket $host $ssock] } output ]
        set retry 0
        while { $rc != 0 } {
            incr retry 
            if { $retry == 5 } {
                notice "Timeout in getting connection to plugin!"
                return -1
            }
            after 2000
            set rc [ catch { set client_sock [socket $host $ssock] } output ]
        }

        fconfigure $client_sock -buffering line
        fconfigure $client_sock -buffersize 1000000
        notice "Opened socket: $client_sock"
        return $client_sock
    }

    protected method localsockfree {sock} {
        if {[catch {socket -server {} $sock} rc]} {
            return 0
        } else {
            # server could be started, so the port is not in use locally
            catch {close $rc}
            return 1
        }
    }

    ## Find a free local socket to use
    protected method get_socket { } {
        for { set i $startsocket } { $i < $endsocket } {incr i } {
            set rc [localsockfree $i]
            if { $rc == 1 } {
                return $i
            }
        }
        error "Could not find free socket in range $startsocket to $endsocket - problem with localhost?" "" -1
    }





    protected method remote_launch { file } {
        ## TODO - plugin task needs startup name
        get_tm MSH
        set msh::PROGRAM [state ext_timing_start_name]
        puts "msh::PROGRAM: $msh::PROGRAM"
        set cmd "[state ext_timing_start_args] $file"
        puts "Plugin Launch cmd: spawn $plugin_task [state ext_timing_start_args] $file -class $ttype"
        ## need to separate startname and start args
        spawn $plugin_task "[state ext_timing_start_args] $file" -class [$sobj cget -ext_timing]

    }

    protected method remote_restart_server { } {
        set waitsock [socket -server [itcl::code $this wait_for_plugin] 0]
        set waitsock_addr [ lindex [fconfigure $waitsock -sockname] 2]

        set host [exec hostname]
        set ::env(ADE_PLUGIN_SOCKET) "${host}:$waitsock_addr"
        puts "server sync socket: $::env(ADE_PLUGIN_SOCKET)"

        set ::plugin_socket ""
        launch_plugin [state server_tcl_file]
        vwait ::plugin_socket

        puts "Got my info: $::plugin_socket"
        parse_client_socket $::plugin_socket

        connect_to_plugin
    }



}

