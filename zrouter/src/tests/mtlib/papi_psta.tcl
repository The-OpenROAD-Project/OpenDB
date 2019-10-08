 global dlevel
set dlevel 1
   
    proc open_eco { fname } {
        set ::eco_fp [open $fname w]
    }

    proc close_eco { } {
        if { $::eco_fp != "" } {
            flush $::eco_fp
            close $::eco_fp
            set ::eco_fp ""
        }
    }
    proc bgerror { message } {
        puts "BG ERROR: $message "
        return 0
    }

    proc open_debug { } {
        global dpref
        set ::tmp_fp [open "${dpref}pcmd.log" w]
    }

    proc close_debug { } {
        if { $::tmp_fp != "" } {
            flush $::tmp_fp
            close $::tmp_fp
            set ::tmp_fp ""
        }
    }

    proc debug { cmd } {
        if { $::tmp_fp != "" } {
            puts $::tmp_fp $cmd
            flush $::tmp_fp
        }
    }
    proc eco { cmd } {
        if { $::eco_fp != "" } {
            puts $::eco_fp $cmd
            flush $::eco_fp
        }
    }


    proc exit_remote { } {
        close_debug
        close_pdebug
        close_eco
        if { $::tmp_fp != "" } {
            flush $::tmp_fp
            close $::tmp_fp
        }
        close $::cur_channel
        exit
    }

    proc open_pdebug { } {
        global dpref
        set ::tmp_deb [open "${dpref}pdebug.log" w]
    }

    proc close_pdebug { } {
        if { $::tmp_deb != "" } {
            flush $::tmp_deb
            close $::tmp_deb
            set ::tmp_deb ""
        }
    }

    proc pdebug { str } {
        if { $::tmp_deb != "" } {
            puts $::tmp_deb $str
            flush $::tmp_deb
        }
    }

    proc set_si { val } {
        set ::milos_si $val
    }

    proc init_local { } {
        set tmp_fp ""
        set tmp_deb ""
        set cur_channel ""
        set eco_fp ""
        set milos_si 0
        set dpref ""
        set dlevel 0

    }

#### ACTUAL METHODS
    proc init_variables { pref } {
        ## generic initialization section
        global allt alln allt_list endt endt_list actt actt_list
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

        global conv_factor
        set conv_factor 1e-9

        global milos_si
        set milos_si 1

        global milos_max_derate milos_min_derate
        set milos_max_derate 1.0
        set milos_min_derate 1.0

	global dimitris_flow
	set dimitris_flow 0

	global remote_mapping_flag
	set remote_mapping_flag 0

        global swap_cnt
        global remove_cell_cnt
        global create_cell_cnt
        global connect_pin_cnt
        global disconnect_pin_cnt
        global remove_net_cnt
        global create_net_cnt

        set swap_cnt 0
        set remove_cell_cnt 0
        set create_cell_cnt 0
        set connect_pin_cnt 0
        set disconnect_pin_cnt 0
        set remove_net_cnt 0
        set create_net_cnt 0

        global start_time
        set start_time [clock seconds]
    }

    proc init_req_vars { } {

        global flag_max_slack flag_min_slack flag_max_slew flag_min_slew
        global milos_requirements
        global milos_plugin_control
        set flag_min_slack 0
        set flag_max_slack 0
        set flag_min_slew 0
        set flag_max_slew 0
        set milos_requirements "0 0 0 0"

        set milos_plugin_control "auto"

        global timing_save_pin_arrival_and_slack
        set timing_save_pin_arrival_and_slack true
    }

    proc set_plugin_control { type } {
        set milos_plugin_control $type
    }

    proc mytype { args } {
        puts "args : $args : psta"
        return "psta"
    }
    proc test_sta { filename {args ""}  } {
        puts "test_sta -- filename: $filename   args:$args"
        return "psta"
    }

    proc start_init { dpref name } {
puts "$dpref $name"
        init_variables $dpref

        if { $::dlevel & 1 } {
            open_debug
        }
        if { $::dlevel & 2 } {
            open_pdebug
        }
	global remote_name
	set remote_name $name
    }
    proc init_netlist_1 { min_derate max_derate sdc_file } {
	puts "$min_derate $max_derate $sdc_file"
    }

    proc init_netlist { min_derate max_derate sdc_file blockname spefname max_spef min_spef libs max min si top dbs restore_dir } {

	global opt_progress_fp 
        global dpref
	set opt_progress_fp [open $dpref.progress.xml w]

        global link_path
        global timing_save_pin_arrival_and_slack
        global timing_remove_clock_reconvergence_pessimism

	global min_max_libs max_libs min_libs


        foreach lib $libs {
            read_lib $lib
        }
	set min_max_libs {}
        foreach lib [get_libs *] {
		lappend min_max_libs $lib
	}
	puts "min_max_libs: $min_max_libs"
        foreach lib $max {
            read_lib -max $lib
        }
	set max_libs {}
        foreach lib [get_libs *] {
		if { [lsearch $min_max_libs $lib] < 0 } {
			lappend max_libs $lib
		}
	}
	puts "max_libs: $max_libs"
        foreach lib $min {
            read_lib -min $lib
        }
	set min_libs {}
        foreach lib [get_libs *] {
		if { [lsearch $min_max_libs $lib] < 0 } {
			if { [lsearch $max_libs $lib] < 0 } {
				lappend min_libs $lib
			}
		}
	}
	puts "min_libs: $min_libs"

        read_verilog $blockname
        link_design $top

        init_req_vars
        #set_requirements 1 1 1 1
        #set_plugin_mode "auto"
        #set_plugin_mode "ocv"
        #set_operating_conditions -analysis_type on_chip_variation

	global flag_min_slack
	global flag_max_slack
	global flag_min_slew
	global flag_max_slew

	set flag_min_slack 1
	set flag_max_slack 1
	set flag_min_slew 1
	set flag_max_slew 1

	global opt_method
	set opt_method "init"

	sta::set_path_min_max min_max

        set timing_save_pin_arrival_and_slack true

        read_sdc $sdc_file

	global milos_min_derate
	global milos_max_derate
	set milos_min_derate $min_derate
	set milos_max_derate $max_derate
        set_derating

        foreach s $spefname {
        	read_spef $s "min_max"
	}
        foreach s $max_spef {
        	read_spef $s "max"
	}
        foreach s $min_spef {
        	read_spef $s "min"
	}

        set timing_remove_clock_reconvergence_pessimism true
        create_net _MILOS_DUMMYX
        set_vertex
        #sta::set_debug search 2

	global dpref
	set initfp [open $dpref.init.tcl w]
	
	puts $initfp "foreach lib {$libs} {\n"
	puts $initfp "\tread_lib \$lib\n"
	puts $initfp "}\n"
	puts $initfp "foreach lib {$max} {\n"
	puts $initfp "\tread_lib -max \$lib\n"
	puts $initfp "}\n"
	puts $initfp "foreach lib {$min} {\n"
	puts $initfp "\tread_lib -min \$lib\n"
	puts $initfp "}\n"
        puts $initfp "read_verilog $blockname\n"
        puts $initfp "link_design $top\n"
	puts $initfp "sta::set_path_min_max min_max\n"
        puts $initfp "set timing_save_pin_arrival_and_slack true\n"
        puts $initfp "read_sdc $sdc_file\n"
        puts $initfp "foreach s {$spefname} {"
        puts $initfp "\tread_parasitics -analysis_point min_max \$s"
	puts $initfp "}"
        puts $initfp "foreach s {$max_spef} {"
        puts $initfp "\tread_parasitics -analysis_point max \$s"
	puts $initfp "}"
        puts $initfp "foreach s {$min_spef} {"
        puts $initfp "\tread_parasitics -analysis_point min \$s"
	puts $initfp "}"

        puts $initfp "set timing_remove_clock_reconvergence_pessimism true"
        puts $initfp "create_net _MILOS_DUMMYX"
	puts $initfp "set_timing_derate -early $min_derate"
	puts $initfp "set_timing_derate -late $max_derate"

	close $initfp

	#report_timing -delay_type min_max
    }
    proc init_env { min_derate max_derate sdc_file blockname spefname libs max min si top dbs restore_dir } {


        global timing_save_pin_arrival_and_slack
        global timing_remove_clock_reconvergence_pessimism

        init_req_vars
        # TODO set_requirements 1 1 1 1
        #set_plugin_mode "auto"
        set_plugin_mode "ocv"
        set_operating_conditions -analysis_type on_chip_variation

        set timing_save_pin_arrival_and_slack true

	global milos_min_derate
	global milos_max_derate
	set milos_min_derate $min_derate
	set milos_max_derate $max_derate
        set_derating

        set timing_remove_clock_reconvergence_pessimism true
        #create_net _MILOS_DUMMYX
        #set_vertex
#dpref
        #sta::set_debug search 2
    }

    proc set_engine_req { min max } {
        if { $min == 1 } {
            puts "Changing operating type to OCV!"
            set_derating
            set_operating_conditions -analysis_type on_chip_variation
        } else {
            puts "Changing operating type to single!"
            reset_timing_derate
            set_operating_conditions -analysis_type single
        }
    }
    proc set_derating { } {

        if { ($::milos_min_derate == 1.0) && ($::milos_max_derate == 1.0) } {
            reset_timing_derate
        } else {
            set_timing_derate -early $::milos_min_derate
            set_timing_derate -late $::milos_max_derate
        }
    }

    proc set_plugin_mode { mode } {
        switch $mode {
            setup { set_engine_req 0 1 }
            hold  { set_engine_req 1 0 } 
            ocv   { set_engine_req 1 1 }
        }
    }

    ## should work for both PT and PSTA
    proc set_requirements_old  { min_slack max_slack min_slew max_slew } {
        global flag_min_slack flag_max_slack flag_min_slew flag_max_slew
        global milos_requirements
        set nreq "$min_slack $max_slack $min_slew $max_slew"
	puts "FORCE to default: 1 1 1 1 < $nreq"
        set nreq "1 1 1 1"

        if { $nreq eq $milos_requirements } {
            ## no changes
            return
        } else {
            set min 0
            set max 0
            if { ($min_slack == 1) || ($min_slew == 1) } {
                set min 1
            }
            if { ($max_slack == 1) || ($max_slew == 1) } {
                set max 1
            }
            if { $::milos_plugin_control eq "auto" } {
                set_engine_req $min $max
            }

            ## save values
            set flag_min_slack $min_slack
            set flag_max_slack $max_slack

            set flag_min_slew $min_slew
            set flag_max_slew $max_slew

            set milos_requirements $nreq
        }
    }
    proc set_requirements { arg_list } {
        global milos_requirements

	set args [join $arg_list]
        set milos_requirements $args
	#puts "set_requirements: $args"
        global flag_min_slack flag_max_slack flag_min_slew flag_max_slew

	global target_min_slack target_max_slack target_max_slew target_min_slew
	global opt_method

	set target_min_slack [lindex $args 0]
	set target_max_slack [lindex $args 1]
	set target_min_slew [lindex $args 2]
	set target_max_slew [lindex $args 3]
	set opt_method [lindex $args 4]

	#puts "$target_min_slack $target_max_slack $target_min_slew $target_max_slew $opt_method"
    }

    proc read_text_file { ifile } {
        if { [file exists $ifile] } {
            set fp [open $ifile r]
            set data [read $fp]
            close $fp
            return $data
        } else {
            return "-1"
        }
    }
    proc run_tmg_cmd { filename cmd_name {args ""} } {
	puts "run_tmg_cmd: $filename -- $cmd_name << $args >>"

	puts "args:$args:[lindex $args 0]"

        set new_args [lindex [lindex $args 0] 0]
        #report_timing $new_args > $filename

	$cmd_name $new_args > $filename
	return "::run_tmg_cmd"
    }

    proc write_cterms { filename } {
    puts "write_cterms $filename"
        set fp [open $filename w]
        set rc [send_term_lists $fp]
        close $fp
        return $rc
    }
    proc write_actt { filename } {
        set fp [open $filename w]
        set rc [send_actt_data $fp]
        close $fp
        return $rc
    }

    proc read_actt { filename } {
        return [set_actterms [read_text_file $filename]]
    }

    proc read_endt { filename } {
        return [set_endterms [read_text_file $filename]]
    }

    proc read_allt_old { filename } {
        return [set_allterms [read_text_file $filename]]
    }

    proc read_allt { filename } {

        set stream [open $filename r]
        if { [eof $stream] } {
		return "empty file"
	}
        gets $stream line
        set tl [split $line " \n"]
	set n0 [lindex $tl 0]
	set n1 [lindex $tl 1]

	if { ($n0 == 0) && ($n1 == 0) } {
		global remote_mapping_flag
		set remote_mapping_flag 1
    		return [read_allterms_opt $stream]
	} else {
    		return [read_allterms $stream]
        }
    }
    proc read_allterms_opt { fp } {

	#puts "read_allterms_opt"
        global allt allt_list alln id2pin
        array set allt { }
        set allt_list {}

        while { ![eof $fp] } {
            	gets $fp line
        	set tl [split $line " \n"]
		set id   [lindex $tl 0]
		set n1 [lindex $tl 1]
#puts "$id $n1 "
		set name [get_mapped_name $n1]

		
#puts "$id $n1 $name"
                ## check for port/pin split
                if { $name eq "" } { continue }
                if { $id eq $name } { continue }

                lappend allt_list $id
                # set allt($id) [get_vl $name]
                #set allt($id) $id2vl($n1)
                set allt($id) $::id2pin($n1)
                set alln($id) $name
#puts "$id $n1 : $name -- allt= $::allt($id)"
        }
	close $fp
        return "ALL size [llength $::allt_list]"
    }
    proc read_allterms { fp } {

        global allt allt_list alln
        array set allt { }
        set allt_list {}

        while { ![eof $fp] } {
            	gets $fp line
        	set tl [split $line "\n"]
		set id   [lindex $tl 0]
		set name [lindex $tl 1]
#puts "$id $name"
                ## check for port/pin split
                if { $name eq "" } { continue }
                if { $id eq $name } { continue }

                lappend allt_list $id
#there is bug HERE!
                set allt($id) [get_vl $name]
                set alln($id) $name
puts "$allt($id) $id $name"
        }
	close $fp
        return "ALL size [llength $::allt_list]"
    }


    proc set_actterms { alist } {
        set ::actt_list $alist
        return "ACT size [llength $::actt_list]"
    }

    proc set_endterms { elist } {
        set ::endt_list $elist
        return "END size [llength $::endt_list]"
    }

    proc set_allterms { tlist } {
        global allt allt_list alln
        array set allt { }
        set allt_list {}
        set tl [split $tlist "\n"]
        foreach arg $tl {
            foreach { id name } $arg {
                ## check for port/pin split
                if { $name eq "" } { continue }
                if { $id eq $name } { continue }

                lappend allt_list $id
                set allt($id) [get_vl $name]
                set alln($id) $name
            }
        }
        return "ALL size [llength $::allt_list]"
    }

    proc get_any_pin { name } {
	set rc [ catch { \
		set pin [sta::get_port_pin_error "pin" $name] \
							} output ]
	if { $rc != 0 } {
		pdebug "get_any_pin $name : $output"
		puts "get_any_pin $name : $output"
		return ""
	}
	return $pin
    }
    proc get_vertex { name } {
	set pin [get_any_pin $name]
	if { $pin eq "" } {
		return ""
	}
	set vertex [lindex [$pin vertices] 0]
	return $vertex
    }
    proc get_vl { name } {
	set name [lindex $name 0]
        global vl
        if { [info exists vl($name)] == 0 } {
		set v [get_vertex $name]

            	set vl($name) $v
        }
        return $vl($name)
    }
#Dimitris 12/01/09 make get_vl_old

    proc get_vl_old { name } {
        global vl
        if { [info exists vl($name)] == 0 } {
            set vl($name) ""

            set_vertex
            set vl($name) $vertex
        }
        return $vl($name)
    }

    proc replay_string { v } {
		return $v
	}


    proc clock_term_lists { fp } {

	global name2id
	global end_term_list

        foreach_in_collection clk [all_clocks] {
            set clk_name [get_object_name $clk]
            set period [get_attribute -class clock $clk period]
            set x [get_attribute -class clock $clk sources]

            set y ""
	    set rc [ catch { \
            	set y [get_attribute -class net $x object_class] \
				} output ]
	    if { $rc != 0 } { continue }

            if { $y eq "" } {
                #continue
                set y "net"
            }
	    #incr idcount
	    #set idtable($clk_name) $idcount

            puts $fp "_Clk: $clk_name Period: $period SourceType: $y SourceName: $x "                              
            foreach_in_collection pin [all_registers -clock_pins -clock $clk] {
		#set name [lindex [get_object_name $pin] 0]
		set name [$pin name]
            	if { ![info exists name2id($name)] } {
    			map_terms $fp $pin
			puts "CLOCK: $name $name2id($name)"
            	}
		set id $name2id($name)
		#puts "C $id"
		puts $fp "C $id "
	    }
            foreach_in_collection pin [all_registers -data_pins -clock $clk] {
		set name [lindex [get_object_name $pin] 0]
            	if { [info exists name2id($name)] == 0 } {
			puts "endpoint: $name"
    			map_terms $fp $pin
            	}
		set id $name2id($name)
		lappend end_term_list $id
		#puts "D $id"
		puts $fp "D $id "
	    }
            foreach_in_collection pin [all_registers -output_pins -clock $clk] {
		set name [lindex [get_object_name $pin] 0]
            	if { [info exists name2id($name)] == 0 } {
			puts "endpoint: $name"
    			map_terms $fp $pin
            	}
		set id $name2id($name)
		# 031610 lappend end_term_list $id
		#puts "O $id"
		puts $fp "O $id "
	    }
        }
    }
    proc map_pin_and_name { pin pin_name } {

	global v_cnt
        global id2pin
        global id2name
        global name2id
        global allt_id_list

	global end_term_list

	incr v_cnt
	set id2pin($v_cnt) $pin
	#set id2vl($v_cnt) [lindex [$pin vertices] 0]
	set id2name($v_cnt) $pin_name
	set name2id($pin_name) $v_cnt

	return $v_cnt
    }

    proc map_terms { fp pin } {

	set pin_name [lindex [get_object_name $pin] 0]

        set cnt [map_pin_and_name $pin $pin_name]

	puts $fp "P $cnt $pin_name "
	# puts "P $cnt $pin $pin_name "
    }
    proc get_mapped_name { n1 } {
	global id2name
	if { ![info exists ::id2name($n1)] } {
		return ""
	}
	return $::id2name($n1)
    }
    proc map_ports { fp port } {

	global v_cnt
        global id2vl
        global id2pin
        global id2name
        global name2id
        global allt_id_list

	global end_term_list

	set pin_name [$port name]
       	set pin [sta::get_port_pin_error "pin" $pin_name]
       	set vertex [lindex [$pin vertices] 0]

	incr v_cnt
	#set id2vl($v_cnt) [lindex [$pin vertices] 0]
	set id2pin($v_cnt) $pin
	set id2name($v_cnt) $pin_name
	set name2id($pin_name) $v_cnt

	puts $fp "P -$v_cnt $pin_name "
	#puts "P -$v_cnt $pin $pin_name "
    }

    proc send_term_lists { fp } {

	global remote_name

	puts "# $remote_name"
	puts "# $fp"

        global mame2id
	global v_cnt
	set v_cnt 0
	global dimitris_flow
	set dimitris_flow 1

	puts $fp "#RMT $remote_name"

	foreach pin [get_pins *] {
    		map_terms $fp $pin
	}
	foreach_in_collection port [get_ports *] {
    		map_ports $fp $port
	}
        clock_term_lists $fp

        return "::send_term_lists"
    }
    proc set_vertex { } {

	global v_cnt
	set v_cnt 0
        global vl
        foreach cell [get_cell *] {
            set it [$cell pin_iterator]
            while { [$it has_next] == 1 } {
                set pin [$it next]
#puts "pin = $pin"
                set name [$pin name]
#puts "name = $name"
                set vertex [$pin vertices]
#puts "vertex = $vertex"
                #set vertex [[sta::get_port_pin_error "pin" $name ] vertices]
#puts "vertex = $vertex"
                if { [lindex $vertex 0] == "NULL" } {
                    set vl($name) ""
                } else {
set new_name [replay_string $name]
                    set vl($name) [lindex $vertex 0]
			incr v_cnt
#puts "$v_cnt $name $new_name $vl($name)"
                }
            }
        }
        set start [$cell parent]
        set it [$start pin_iterator]
        while { [$it has_next] == 1 } {
            set pin [$it next]
            set name [$pin name]
            set vertex [$pin vertices]
            if { [lindex $vertex 0] == "NULL" } {
                set vl($name) ""
            } else {
                set vl($name) [lindex $vertex 0]
		incr v_cnt
#puts "$v_cnt $name $vl($name)"
            }
        }

#    set fp [open vl.dat w]
    #    puts $fp [array get vl]
    #    close $fp
    }
    proc get_design_slack_dimitris { minmax } {
        global end_term_list

	set MM 1
	if { $minmax eq "min" } {
		set MM 0
	}
	set unformatted 0
        set tns 0.0
        set tns 0.0
        set wns 1e30
        set viol 0
        set worst_id "none"
        fmoreach term_id $::end_term_list {
            if { ! [info exists ::id2vl($term_id)] } {
                continue
            }
            set v $::id2vl($term_id)
            if { $v eq "" } {
                continue
            }
	    set rs [get_vertex_slack $v "r" $MM $unformatted]
	    set fs [get_vertex_slack $v "f" $MM $unformatted]
            #set rs [$v slack r $minmax]
            #set fs [$v slack f $minmax]
            if { $fs < $rs } {
                set rs $fs
            }
            if { $rs < $wns } {
                set wns $rs
                set worst_id $term_id
            }
            if { $rs < 0.0 } {
                set tns [expr {$tns + $rs}]
                incr viol
            }
        }

        set wns [expr {$wns * 1e+9}]
        set tns [expr {$tns * 1e+9}]
        return "$wns $tns $viol $wns $worst_id"
    }
    proc pin_slack_by_name { term MM { formatted 0 } } {
            if { [info exists ::alln($term)] == 0 } {
                return "N/A"
            }
            set v $::alln($term)
            if { $v eq "" } {
                return "N/A"
            }
	    set p [get_any_pin $v]
	    if {$p eq ""} {
		return "N/A"
	    }
	    set rs [get_pin_slack_attr $p ${MM}_rise_slack $formatted]
	    set fs [get_pin_slack_attr $p ${MM}_fall_slack $formatted]

            if { $fs < $rs } {
                set rs $fs
            }
	    return $rs
    }
    proc pin_slack_by_vertex { term MM unformatted } {
            if { [info exists ::allt($term)] == 0 } {
                return "N/A"
            }
            set v $::allt($term)
            if { $v eq "" } {
                return "N/A"
            }
	    set rs [get_vertex_slack $v r $MM $unformatted]
	    set fs [get_vertex_slack $v f $MM $unformatted]
            if { $fs < $rs } {
                set rs $fs
            }
	    return $rs
    }
    proc get_pin_design_slack { pin minmax_rise minmax_fall } {
	set sl_r [get_pin_slack_attr $pin $minmax_rise ]
	if { [string first INF $sl_r] >= 0} {
		return ""
	}
	set sl_f [get_pin_slack_attr $pin $minmax_fall ]
	if { [string first INF $sl_f] >= 0} {
		return ""
	}
	set rs $sl_r
	if { $rs > $sl_f } {
		set rs $sl_f
	}
	return $rs
    }
    proc get_design_slack { minmax } {
    	puts "get_design_slack $minmax"
        #global dimitris_flow
	#if { $dimitris_flow>0 } {
		#return [get_design_slack_dimitris $minmax]
	#}
        global swap_cnt
	global create_cell_cnt
	global remove_cell_cnt

	set unformatted 1

	set minmax_rise "max_rise_slack"
	set minmax_fall "max_fall_slack"
	if { $minmax eq "min" } {
		set minmax_rise "min_rise_slack"
		set minmax_fall "min_fall_slack"
	}
        set tns 0.0
        set wns 1e30
        set viol 0
        set worst_id "none"
        foreach term $::endt_list {
    		set pin [get_pin $term]
		if { $pin eq "" } {
			continue
		}
    		set rs [get_pin_design_slack $pin $minmax_rise $minmax_fall]
		if { $rs eq "" } {
			continue
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
	if {$worst_id eq ""} {
		set worst_id "none"
		puts "worst_id: DESIGN_SLACK $minmax : WNS=$wns TNS=$tns"
	}
	#puts "DESIGN_SLACK $minmax : WNS=$wns TNS=$tns"

        set wns [expr {$wns * 1e+9}]
        set tns [expr {$tns * 1e+9}]
        set ret_val "$wns $tns $viol $wns $worst_id"

	set w [format "%7.4f" $wns]
	set t [format "%9.4f" $tns]

        global start_time
        set elaps [format "%6d" [expr {[clock seconds] - $start_time}]]
	if {$elaps eq ""} {
		set elaps 0
	}
	global opt_method
	if {$opt_method eq ""} {
		set opt_method "no_opt"
	}

        set RESULT "$elaps $minmax: WNS= $w TNS= $t VIOL= $viol -- SIZED= $swap_cnt NEW= $create_cell_cnt REMOVED= $remove_cell_cnt $opt_method"
	eco "#"
	eco "# $RESULT"
	eco "#"
        eco "# WORST ENDTERM: $w $worst_id"
	eco "#"
    	puts "get_design_slack $minmax: $ret_val"

        #return $ret_val
        set new_ret_val "$wns $tns $viol $wns $worst_id $swap_cnt $create_cell_cnt $remove_cell_cnt $opt_method $elaps"
	eco "$new_ret_val"
        progress_xml $wns $tns $viol $wns $worst_id $swap_cnt $create_cell_cnt $remove_cell_cnt $opt_method $elaps
        return $new_ret_val
    }
    proc progress_xml { wns tns viol wns worst_id swap_cnt create_cell_cnt remove_cell_cnt opt_method elaps } {

	global opt_progress_fp 

        puts $opt_progress_fp "<opt_step>"
        puts $opt_progress_fp "  <wns> $wns </wns> "
	puts $opt_progress_fp "  <tns> $tns </tns>"
	puts $opt_progress_fp "  <viol> $viol </viol>"
	puts $opt_progress_fp "  <wns> $wns </wns>"
	puts $opt_progress_fp "  <worst_id> $worst_id </worst_id>"
	puts $opt_progress_fp "  <swap_cnt> $swap_cnt </swap_cnt>"
	puts $opt_progress_fp "  <create_cell_cnt> $create_cell_cnt </create_cell_cnt>"
	puts $opt_progress_fp "  <remove_cell_cnt> $remove_cell_cnt </remove_cell_cnt>"
	puts $opt_progress_fp "  <opt_method> $opt_method </opt_method>"
	puts $opt_progress_fp "  <elaps> $elaps </elaps>"
        puts $opt_progress_fp "</opt_step>"

	flush $opt_progress_fp
    }

    proc format_value { val } {
	#set r [format "%7.4e" [expr {[sta::time_sta_ui $val] * 1e+9} ] ]
	#set ur [sta::time_sta_ui $val]

	set r [format "%7.4e" $val]

#puts "format_value : $val : $r"
	return $r
    }
    proc get_pin_slack_attr { pin sl_type { formatted 1 } } {
	set slack [get_attribute -class pin $pin $sl_type ]

	if { $slack eq "" } {
		puts "\t$sl_type: $slack EMPTY"
		return 0.0
	}
	if { [string first INF $slack] >= 0} {
		#puts "get_pin_slack_attr : [$pin name] $sl_type $formatted : $slack"
		return "1e29"
	}
	if { $formatted >0 } {
		return [format_value [expr {$slack * 1e-9}]]
	} else {
		return $slack
	}
    }
    proc get_pin_slacks { p } {

        if { $::flag_max_slack == 1 } {
		set sl_max_r [get_pin_slack_attr $p max_rise_slack ]
		set sl_max_f [get_pin_slack_attr $p max_fall_slack ]
	}
        if { $::flag_min_slack == 1 } {
		set sl_min_r [get_pin_slack_attr $p min_rise_slack ]
		set sl_min_f [get_pin_slack_attr $p min_fall_slack ]
        }
        return "$sl_min_r $sl_max_r $sl_min_f $sl_max_f"
    }
    proc get_slacks_attr { id v } {
        set rl ""
	set sl_min_r "1e29"
	set sl_min_f "1e29"
	set sl_max_r "1e29"
	set sl_max_f "1e29"
	if { $id == 0 } {
		return "$sl_min_r $sl_max_r $sl_min_f $sl_max_f"
	}
	set p [get_any_pin $v]
	if {$p eq ""} {
		return "$sl_min_r $sl_max_r $sl_min_f $sl_max_f"
	}
#puts "get_slacks_attr: $p"
        if { $::flag_max_slack == 1 } {
		set sl_max_r [get_pin_slack_attr $p max_rise_slack ]
		set sl_max_f [get_pin_slack_attr $p max_fall_slack ]
	}
        if { $::flag_min_slack == 1 } {
		set sl_min_r [get_pin_slack_attr $p min_rise_slack ]
		set sl_min_f [get_pin_slack_attr $p min_fall_slack ]
        }
        return "$sl_min_r $sl_max_r $sl_min_f $sl_max_f"
    }
    proc get_slacks { v } {
        set rl ""
	set sl_min_r "1e29"
	set sl_min_f "1e29"
	set sl_max_r "1e29"
	set sl_max_f "1e29"
        if { $::flag_max_slack == 1 } {
		set sl_max_r [get_vertex_slack $v r 1 ]
		set sl_max_f [get_vertex_slack $v f 1 ]
	}
        if { $::flag_min_slack == 1 } {
		set sl_min_r [get_vertex_slack $v r 0 ]
		set sl_min_f [get_vertex_slack $v f 0 ]
        }
        return "$sl_min_r $sl_max_r $sl_min_f $sl_max_f"
    }
    proc get_slack_2 { v } {
        set rl ""
	set sl_min_r "1e29"
	set sl_min_f "1e29"
	set sl_max_r "1e29"
	set sl_max_f "1e29"
        if { $::flag_max_slack == 1 } {
		set sl_max_r [format_value [$v slack r max] ]
		set sl_max_f [format_value [$v slack f max] ]
	}
        if { $::flag_min_slack == 1 } {
		set sl_min_r [format_value [$v slack r min] ]
		set sl_min_f [format_value [$v slack f min] ]
        }
        return "$sl_min_r $sl_max_r $sl_min_f $sl_max_f"
    }
    proc get_slack_bak { v } {
        set rl ""
        if { $::flag_max_slack == 1 } {
            if { $::flag_min_slack == 1 } {
                append rl "[$v slack r min] [$v slack r max] "
                append rl "[$v slack f min] [$v slack f max]"
            } else {
                append rl "1.011e29 [$v slack r max] "
                append rl "1.011e29 [$v slack f max]"
            }
        } else {
            if { $::flag_min_slack == 1 } {
                append rl "[$v slack r min] 1.011e29 "
                append rl "[$v slack f min] 1.011e29 "
            } else {
                append rl "1.011e29 1.011e29 1.011e29 1.011e29"
            }
        }
        return $rl
    }
    proc get_slack_old { v } {
        set rl ""
        if { $::flag_max_slack == 1 } {
            if { $::flag_min_slack == 1 } {
                append rl "[$v slack_minmax r min] [$v slack_minmax r max] "
                append rl "[$v slack_minmax f min] [$v slack_minmax f max]"
            } else {
                append rl "1.011e29 [$v slack_minmax r max] "
                append rl "1.011e29 [$v slack_minmax f max]"
            }
        } else {
            if { $::flag_min_slack == 1 } {
                append rl "[$v slack_minmax r min] 1.011e29 "
                append rl "[$v slack_minmax f min] 1.011e29 "
            } else {
                append rl "1.011e29 1.011e29 1.011e29 1.011e29"
            }
        }
        return $rl
    }
    proc get_vertex_slack { v rise_fall min_max {formated 1} } {
	# min_max -- 0=min 1=max

        if {[ catch { set slacks [$v slacks $rise_fall] } output ] != 0 } {
		puts "cannot get $rise_fall $min_max slack: $v: $output"
		pdebug "cannot get $rise_fall $min_max slack: $v: $output"
		return 1e+29
	}

        set slack ""
        if { [llength $slacks] > 1 } {
		set slack [lindex $slacks $min_max]
        } else {
                set slack $slacks
        }
	if { [string first "e+30" $slack] >= 0 } {
		puts "Invalid slack: $slack -- [[$v pin] name] $v $rise_fall $min_max"
		return 1e+27
	}
	if {$formated>0} {
        	return [format_value $slack]
	}
        return $slack
    }
    proc get_pin_slack { pin_name rise_fall min_max } {

	# min_max -- 0=min 1=max

	set pin [sta::get_port_pin_error "pin" $pin_name]
	set vertex [lindex [$pin vertices] 0]

	set slack [get_vertex_slack $vertex $rise_fall $min_max]

        #puts "$slack $rise_fall  $min_max $pin_name: $pin $vertex "

        return $slack
    }
    proc get_pin_slack_bak { pin_name rise_fall min_max } {
	# min_max -- min max
	set pin [sta::get_port_pin_error "pin" $pin_name]
	set vertex [lindex [$pin vertices] 0]
	set slack [$vertex slack $rise_fall $min_max]

        set slacks [$vertex slacks $rise_fall]

        set slack ""
        if { [llength $slacks] > 1 } {
                if { $min_max == "min" } {
                        set slack [lindex $slacks 0]
                } elseif { $min_max == "max" } {
                        set slack [lindex $slacks 1]
                }
        } else {
                set slack $slacks
        }
        #puts "$slack $rise_fall  $min_max $pin_name: $pin $vertex "

        return $slack
    }

    proc get_slack_new { v } {
                return "1.011e29 1.011e29 1.011e29 1.011e29"
        set rl ""
        if { $::flag_max_slack == 1 } {
		set rl "[get_attribute -class pin $v max_slack] "
            pdebug "# max $v get_slack rl= $rl get_attribute"
return $rl

            if { $::flag_min_slack == 1 } {
                append rl "[$v slack_minmax r min] [$v slack_minmax r max] "
                append rl "[$v slack_minmax f min] [$v slack_minmax f max]"
            } else {
                append rl "1.011e29 [$v slack_minmax r max] "
                append rl "1.011e29 [$v slack_minmax f max]"
            }
            pdebug "# max $v get_slack rl= $rl after"
        } else {
            pdebug "# min $v get_slack rl= $rl after"
            if { $::flag_min_slack == 1 } {
                append rl "[$v slack_minmax r min] 1.011e29 "
                append rl "[$v slack_minmax f min] 1.011e29 "
            } else {
                append rl "1.011e29 1.011e29 1.011e29 1.011e29"
            }
        }
        return $rl
    }

    proc get_slew_formatted { pin_type v keyword } {
	set r [get_attribute -class $pin_type $v $keyword]
	return [format "%7.4f" $r]
    }
    proc get_slew { id v } {
	set pin "pin"
	if { $id < 0 } {
		set pin "port"
	} 
	set slew_r_min 1e29
	set slew_r_max 1e29
	set slew_f_min 1e29
	set slew_f_max 1e29

        if { $::flag_max_slew == 1 } { 
		set slew_r_max [get_slew_formatted $pin $v actual_rise_transition_max]
		set slew_f_max [get_slew_formatted $pin $v actual_fall_transition_max]
	}
	if { $::flag_min_slew == 1 } {
		set slew_r_min [get_slew_formatted $pin $v actual_rise_transition_min]
		set slew_f_min [get_slew_formatted $pin $v actual_fall_transition_min]
        }
        return "$slew_r_min $slew_r_max $slew_f_min $slew_f_max"
    }
    proc get_slew_bak { id v } {
	set pin "pin"
	if { $id < 0 } {
		set pin "port"
	}
	set rl ""
        if { $::flag_max_slew == 1 } {
            if { $::flag_min_slew == 1 } {
                append rl "[get_attribute -class $pin $v actual_rise_transition_min] [get_attribute -class $pin $v actual_rise_transition_max ] "
                append rl "[get_attribute -class $pin $v actual_fall_transition_min] [get_attribute -class $pin $v actual_fall_transition_max ] "
            } else {
                append rl "1.011e29 [get_attribute -class $pin $v actual_rise_transition_max ] "
                append rl "1.011e29 [get_attribute -class $pin $v actual_fall_transition_max ] "
            }
        } else {
            if { $::flag_min_slew == 1 } {
                append rl "[get_attribute -class $pin $v actual_rise_transition_min] 1.011e29 "
                append rl "[get_attribute -class $pin $v actual_fall_transition_min] 1.011e29 "
            } else {
                append rl "1.011e29 1.011e29 1.011e29 1.011e29"
            }
        }
        return $rl
    }
    proc get_slew_old { v } {
        set rl ""
        if { $::flag_max_slew == 1 } {
            if { $::flag_min_slew == 1 } {
                append rl "[$v slew_minmax r min] [$v slew_minmax r max] "
                append rl "[$v slew_minmax f min] [$v slew_minmax f max]"
            } else {
                append rl "1.011e29 [$v slew_minmax r max] "
                append rl "1.011e29 [$v slew_minmax f max]"
            }
        } else {
            if { $::flag_min_slew == 1 } {
                append rl "[$v slew_minmax r min] 1.011e29 "
                append rl "[$v slew_minmax f min] 1.011e29 "
            } else {
                append rl "1.011e29 1.011e29 1.011e29 1.011e29"
            }
        }
        return $rl
    }



    proc send_actt_data_dimitris { fp } {
        set var "::actt_list"
        if { [llength $::actt_list] == 0 } {
		for { set i 1 } { $i <= $v_cnt } { incr i } {
			lappend ::allt_list $i
		}
            	set var "::allt_list"
        }
        puts $fp "[llength [set $var]]"
        pdebug "SAD ($fp): [llength [set $var]]"
        foreach term [set $var] {
            if { [info exists ::allt($term)] == 0 } {
                puts $fp "# $term INVALID"
                puts -nonewline $fp "INFINITY INFINITY INFINITY INFINITY "
                puts $fp "INFINITY INFINITY INFINITY INFINITY"

                pdebug "# $term INVALID"
                pdebug "INFINITY"

                continue
            }
            puts $fp "# $term "
            #pdebug "# $term"
            set v $::id2vl($term)
            set vs $::id2name($term)
            #pdebug "# $v"
            if { $v eq "" } {
                puts -nonewline $fp "INFINITY INFINITY INFINITY INFINITY "
                puts $fp "INFINITY INFINITY INFINITY INFINITY"

                pdebug "INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY"
            } else {
#puts "$term $v slack: [get_pin_slack $vs r max]"
            #pdebug "# $v before get_slack"
                puts $fp "[get_slacks $v] [get_slew $term $vs]"
            }
        }
        puts $fp "."
        pdebug "."
        return $var
    }
    proc get_pin { term } {
	    # allt -- holds pins, alln -- holds names
	if { [info exists ::allt($term)] } {
		return $::allt($term)
	}
	if { [info exists ::alln($term)] == 0 } {
		return ""
	}
	set p [get_any_pin $::alln($term)]
	return $p
    }
    proc send_actt_data { fp } {
        #puts "send_actt_data $fp ------------------------------------- BEGIN"
	#if {$dimitris_flow>0} {
    		#return [send_actt_data_dimitris $fp]
	#}
        set var "::actt_list"
        if { [llength $::actt_list] == 0 } {
            set var "::allt_list"
        }
        puts $fp "[llength [set $var]]"
        pdebug "SAD ($fp): [llength [set $var]]"

	set INF_RES "INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY INFINITY"
        foreach term [set $var] {
	    set pin [get_pin $term]

            if { $pin eq "" } {
                puts $fp "# $term INVALID"
                puts $fp "$INF_RES"
                pdebug "# $term INVALID"
                pdebug "$INF_RES"
                continue
            }
            puts $fp "# $term "
            pdebug "# $term"

	    set slews "1e+19 1e+19 1e+19 1e+19"
	    if { [ catch { set slews [get_slew $term $pin] } output ]!=0 } {
		pdebug "$output"
		puts "cannot get slew: $term $pin: $output"
	    }
	    set slacks "1e+29 1e+29 1e+29 1e+29"
	    if {[ catch { set slacks [get_pin_slacks $pin] } output ]!=0} {
		pdebug "$output"
		puts "cannot get slacks: $term $pin: $output"
	    }

               #puts $fp "[get_slacks $v] $slews"
            puts $fp "$slacks $slews"
	    #puts "$slacks $slews"
        }
        puts $fp "."
        pdebug "."
        #puts "send_actt_data $fp ------------------------------------- END"
        return $var
    }

    proc getCellLib_min_max { master } {
	global min_max_libs max_libs min_libs
	global opt_method

	set min_max "max"
	if { [string first "hold" $opt_method] >= 0 } {
		set min_max "min"
	}
	if { $min_max eq "max" } {
        	foreach lib $max_libs {
            		set lib [$lib name]
            		if { [get_lib_cell $lib/$master] ne "" } {
				#puts "FOUND MAX lib $lib for $lib/$master"
                		return $lib
            		}
		}
        }
	if { $min_max eq "min" } {
        	foreach lib $min_libs {
            		set lib [$lib name]
            		if { [get_lib_cell $lib/$master] ne "" } {
				#puts "FOUND MIN lib $lib for $lib/$master"
                		return $lib
            		}
		}
        }
        foreach lib $max_libs {
        	set lib [$lib name]
            	if { [get_lib_cell $lib/$master] ne "" } {
			#puts "FOUND MIN_MAX lib $lib for $lib/$master"
               		return $lib
            	}
	}
        error "Cannot find cell type $master"
    }
    proc getCellLib { master } {
        foreach lib [get_libs *] {
            set lib [$lib name]
            if { [get_lib_cell $lib/$master] ne "" } {
                return $lib
            }
        }
        error "Cannot find cell type $master"
    }
    proc find_inst { master } {
        foreach lib [get_libs *] {
            set lib [$lib name]
            if { [get_lib_cell $lib/$master] ne "" } {
                return $lib
            }
        }
        error "Cannot find cell type $master"
    }

    ######################################################################
    #### Plugin API
    ######################################################################



    proc milos_get_id_name { id } {
        return [array get ::allt $id]
    }
    ## PIN commands
    proc milos_createPin { id name } {
    	pdebug "milos_createPin $id $name"

        lappend ::allt_list $id
        connect_net _MILOS_DUMMYX $name

	set pin [get_any_pin $name]
    	set vcnt [map_pin_and_name $pin $name]

        set ::allt($id) $pin
        set ::alln($id) $name

	#puts "milos_createPin $vcnt $id $name"
        disconnect_net _MILOS_DUMMYX $name
    }
    proc milos_createPin_old { id name } {
    	pdebug "milos_createPin $id $name"
        lappend ::allt_list $id
        connect_net _MILOS_DUMMYX $name
        set ::allt($id) [get_vertex $name]
        set ::alln($id) $name
        disconnect_net _MILOS_DUMMYX $name
    }

    proc milos_removePin { id } {
        array unset ::allt $id
        set idx [lsearch $::allt_list $id]
        if { $idx >= 0 } {
            set ::allt_list [lreplace $::allt_list $idx $idx]
        }
    }

    ## CELL commands

    proc milos_swapCell { name master } {
    	pdebug "milos_swapCell $name $master"
        #set lib [getCellLib $master]
        set lib [getCellLib_min_max $master]

	if { [ catch { swap_cell $name $lib/$master } output ] != 0 } {
		pdebug "$output"
		puts "Cannot do <milos_swapCell $name $master> : $output"
		return
	}
        #swap_cell $name $lib/$master
        eco "swap_cell $name $lib/$master"
	global swap_cnt
	incr swap_cnt
    }

    proc milos_removeCell { name } {
    	pdebug "milos_removeCell $name"
	if { [ catch { remove_cell $name } output ] != 0 } {
		pdebug "$output"
		puts "$output"
		return
	}
        #remove_cell $name
        eco "remove_cell $name"
	global remove_cell_cnt
	incr remove_cell_cnt
    }

    proc milos_createCell { name master } {
    	pdebug "milos_createCell $name $master"
        #set lib [getCellLib $master]
        set lib [getCellLib_min_max $master]
        create_cell $name $lib/$master
        eco "create_cell $name $lib/$master"
	global create_cell_cnt
	incr create_cell_cnt
    }


    ## NET commands
    
    proc milos_connectNet { id net } {
    	#puts "milos_connectNet $id $net"
    	pdebug "milos_connectNet $id $net"
	if { [info exists ::allt($id)] == 0 } {
		puts "Cannot find allt($id): milos_connectNet $id $net"
		return
	}
	if { $::allt($id) == "" } {
		puts "empty allt($id): milos_connectNet $id $net"
		pdebug "empty allt($id): milos_connectNet $id $net"
		return
	}
	if { [ catch { set name [[get_pin $id] name] } output ] != 0 } {
		 puts "pin: [get_pin $id]"
		 puts "pin_name: [ [get_pin $id] name]"
		pdebug "milos_connectNet: $id $net: $output"
		puts "<allt=$::allt($id) -- milos_connectNet: $id $net: $output>"
		return
	}
    	#puts "milos_connectNet $id $net $name"
    	pdebug "milos_connectNet $id $net $name"
	if { [ catch { [connect_net $net $name] } output ] != 0 } {
		pdebug "$output"
		puts "<connect_net $net $name> : $output"
		return
	}
        eco "connect_net $net $name"
	global connect_pin_cnt
	incr connect_pin_cnt
    }

    proc milos_disconnectNet { id net } {
    	#puts "milos_disconnectNet $id $net"
    	pdebug "milos_disconnectNet $id $net"
        #set pin [$::allt($id) pin]
        #disconnect_net $net $pin

	if { ! [info exists ::allt($id)] } {
		puts "Cannot find allt($name): milos_disconnectNet $id $net"
		return
	}
	
        #set name [[$::allt($id) pin] name]
        set name [$::allt($id)  name]
        disconnect_net $net $name
        eco "disconnect_net $net $name"
	global disconnect_pin_cnt
	incr disconnect_pin_cnt
    }

    proc milos_removeNet { name } {
    	pdebug "milos_removeNet $name"
        remove_net $name
        eco "remove_net $name"
	global remove_net_cnt
	incr remove_net_cnt
    }

    proc milos_createNet { name } {
    	pdebug "milos_createNet $name"
        create_net $name
        eco "create_net $name"
	global create_net_cnt
	incr create_net_cnt
    }

    proc read_spef { file min_max} {
        if { $::milos_si > 0 } {
            read_parasitics -analysis_point $min_max $file
            eco "read_parasitics -analysis_point $min_max $file"
            #read_parasitics -coupling_reduction_factor 1.5 $file
            #eco "read_parasitics -coupling_reduction_factor 1.5 $file"
        } else {
            read_parasitics -analysis_point $min_max $file
            eco "read_parasitics -analysis_point $min_max $file"
        }
	return "$min_max.get_design_slack: [get_design_slack $min_max]"
    }

    proc read_ni_spef { file } {
        if { $::milos_si > 0 } {
            read_parasitics -keep_capacitive_coupling $file
            eco "read_parasitics -keep_capacitive_coupling $file"
        } else {
            read_parasitics $file
            eco "read_parasitics $file"
        }
    }
    proc read_i_spef { file } {
        if { $::milos_si > 0 } {
            read_parasitics -increment -keep_capacitive_coupling $file
            eco "read_parasitics -increment -keep_capacitive_coupling $file"
        } else {
            read_parasitics -increment $file
            eco "read_parasitics -increment $file"
        }
    }
    proc  add_clock_delay { if } {
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

    proc  get_clock_period { of } {
	global idtable
	global idcount

	set idtable("0") 0

	if { ! [info exists idcount] } {
		set idcount 0
	}
        set fp [open $of w]
        foreach_in_collection clk [all_clocks] {
            set clk_name [get_object_name $clk]
            set period [get_attribute -class clock $clk period]
            set x [get_attribute -class clock $clk sources]

            set y ""
	    set rc [ catch { \
            	set y [get_attribute -class net $x object_class] \
				} output ]
	    if { $rc != 0 } { continue }

            if { $y eq "" } {
                #continue
                set y "net"
            }
	    #incr idcount
	    #set idtable($clk_name) $idcount

            puts $fp "Clk: $clk_name Period: $period SourceType: $y SourceName: $x"                              

            foreach_in_collection pin [all_registers -clock_pins -clock $clk] {
            	set name [lindex [get_object_name $pin] 0]
	    	incr idcount
	    	set idtable($name) $idcount
            	puts $fp "C $idcount  $name"
	    }
            foreach_in_collection pin [all_registers -data_pins -clock $clk] {
            	set name [lindex [get_object_name $pin] 0]
	    	incr idcount
	    	set idtable($name) $idcount
            	puts $fp "D $idcount  $name"
	    }
        }
        close $fp
    }


    ## PATH REPORTING PROCS

    proc init_rpt_vars { } {
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

    proc write_report_data { varname } {
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

    proc debug_report { msg } {
        ## dump some debug info
        if { $::rpt_debug_fp ne "" } {
            puts $::rpt_debug_fp $msg
        }
    }

    proc finalize_reports { } {
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

    proc next_rpt_data_file { } {
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

    proc init_path_reports { name compress maxfile } {

        set ::rpt_basename $maxfile
        set ::rpt_cnt 0
        set ::rpt_compress $compress

        set ::rpt_fp [open "$name" w]

        set ::rpt_debug_fp [open "${name}.debug" w]

        next_rpt_data_file
    }

proc parse_to_keyword { stream keyword } {

	set ll ""
        while { ![eof $stream] } {

        	gets $stream ll
		set word0 [lindex $ll 0]
#puts "parse_to_keyword $keyword $word0"
		if { $word0 eq $keyword } {
			return $ll
		}
	}
	return ""
}
proc parse_passed_clock { stream } {

	set found 0
	set ll ""
        while { ![eof $stream] } {

        	gets $stream ll
		set word0 [lindex $ll 0]

		if { $found>0 } {
			if { $word0 eq "clock" } {
				continue
			} else {
				break
			}
		}
		if { $word0 ne "clock" } {
			continue
		}
		set found 1
	}
	if { $found == 0 } {
		set ll ""
	}
	return $ll
}
proc parse_passed_keyword { stream keyword } {

        while { ![eof $stream] } {

        	gets $stream ll
		set word0 [lindex $ll 0]

		if { $word0 eq $keyword } { continue }
		return $ll
	}
	return ""
}
proc print_name_id { out_fp name keyword } {
	global idtable
	global idcount
	if { $name eq "0" } {
		return "0"
	}
	if { ! [info exists idtable($name)] } {
		incr idcount
		set idtable($name) $idcount
		puts "$keyword $idcount $name"
		puts $out_fp "$keyword$idcount $name"
		return $idcount
	} else {
		return $idtable($name)
	}
}
proc get_name_id { name } {
	global idtable
	global idcount
	set rr ""
	if { ! [info exists idtable($name)] } {
		incr idcount
		set idtable($name) $idcount
		set rr "$idcount $idcount"
	} else {
		set rr "$idtable($name) 0"
	}
#puts "get_name_id= $name == $rr"
	return $rr
}
proc make_path_line { slack arv grom_clk_g from_clk from_i to_clk_g to_clk to_i path_t trans } {

	set lines ""

	set minmax(min) 0
	set minmax(max) 1

	set RF(r) 1
	set RF(f) 0

	set from_g "0 0"
	if { $grom_clk_g ne "0" } {
		set from_g [get_name_id $grom_clk_g]
	}
	set to_g "0 0"
	if { $to_clk_g ne "0" } {
		set to_g [get_name_id $to_clk_g]
	}
	set f1 "0 0"
	if { $from_clk ne "input" } {
		set f1 [get_name_id $from_clk]
	}
	set f2 [get_name_id $from_i]

	set t1 [get_name_id $to_clk]
	set t2 [get_name_id $to_i]

	if { [lindex $from_g 1] > 0 } {
		append lines "N: CLK [lindex $from_g 0] $grom_clk_g" "\n"
	}
	if { [lindex $to_g 1] > 0 } {
		append lines "N: CLK [lindex $to_g 0] $to_clk_g" "\n"
	}
	if { [lindex $f1 1] > 0 } {
		append lines "N: [lindex $f1 0] $from_clk" "\n"
	}
	if { [lindex $f2 1]>0 } {
		if { $from_clk ne "input" } {
			append lines "N: [lindex $f2 0] $from_i" "\n"
		} else {
			append lines "N: -[lindex $f2 0] $from_i" "\n"
		}
	}
	if { [lindex $t1 1]>0 } {
		append lines "N: [lindex $t1 0] $to_clk" "\n"
	}
	if { [lindex $t2 1]>0 } {
		append lines "N: [lindex $t2 0] $to_i" "\n"
	}
	append lines "P: $slack $arv $RF($trans) $minmax($path_t) \
		[lindex $from_g 0] [lindex $f1 0] [lindex $f2 0] \
		[lindex $to_g 0] [lindex $t1 0] [lindex $t2 0]" "\n"

	return $lines
}
proc make_from_to { grom_clk_g from_clk from_i io_keyword FROM } {

	upvar $FROM ret_from
	set lines ""

	set from_g "0 0"
	if { $grom_clk_g ne "0" } {
		set from_g [get_name_id $grom_clk_g]
	}
	set f1 "0 0"
	if { $from_clk ne $io_keyword } {
		set f1 [get_name_id $from_clk]
	}
	set f2 [get_name_id $from_i]

	if { [lindex $from_g 1] > 0 } {
		append lines "N: CLK [lindex $from_g 0] $grom_clk_g" "\n"
	}
	if { [lindex $f1 1] > 0 } {
		append lines "N: [lindex $f1 0] $from_clk" "\n"
	}
	if { [lindex $f2 1]>0 } {
		if { $from_clk ne $io_keyword } {
			append lines "N: [lindex $f2 0] $from_i" "\n"
		} else {
			append lines "N: -[lindex $f2 0] $from_i" "\n"
		}
	}
	set ret_from "[lindex $from_g 0] [lindex $f1 0] [lindex $f2 0]"

	return $lines
}
proc parse_to_clock_pin { stream inout_keyword out_fp } {

	set group_clock "0"
	set clock_pin "0"
	set latency 0

	set line [parse_to_keyword $stream "clock"]
	if { [lindex $line 1] ne "(input" } {
		set group_clock [lindex $line 1]
	}

	gets $stream line
	if { ([lindex $line 0] eq "clock") && ([lindex $line 2] eq "delay") } {
		set latency [lindex $line 4]
	} else {
		puts "clock network delay expected: \t $line"
	}
	gets $stream line
	if { [lindex $line 0] ne $inout_keyword } {
		if { $inout_keyword eq "input" } {
			set clock_pin [lindex $line 0]
		} else {
			set line [parse_passed_keyword $stream "clock"]
			if { [lindex $line 0] ne "output" } {
				set clock_pin [lindex $line 0]
			}
		}
	}
	set gc [print_name_id $out_fp $group_clock "N: CLK "]
	set cp [print_name_id $out_fp $clock_pin "N: "]

	return "$gc $latency $cp"
}

proc read_tmp_report_brief { filename out_fp prev_idcount } {

	global idcount
	global idtable
        set lines ""

	set MM(min) 0
	set MM(max) 1

	set RF(r) 1
	set RF(f) 0

	set pCnt 0
	set port_found 0
	set from_clock_term ""
	set fdp ""
	set from_group_clock ""
	set to_group_clock ""
	set path_type ""
	set fi 0
	set look_for_arrival 0
        set stream [open $filename]
        while { ![eof $stream] } {

            	gets $stream line
		set word0 [lindex $line 0]

		if { $look_for_arrival > 0 } {
			if { $word0 ne "data" } {
				set prev_line $line
				continue
			}
			if { [lindex $line 1] ne "arrival" } {
				continue
			}
			set port_found 0
			set look_for_arrival 0

			set arrv [lindex $line 3]
			#puts "FROM : $from_inst(0) $from_inst(1)"
			#puts "TO   : $prev_line"

			set to_iterm [lindex $prev_line 0]
			set len [expr [llength $prev_line] -1]
			set trans [lindex $prev_line $len]

			if { [lindex $prev_line 1] eq "(out)" } {
				set dp [print_name_id $out_fp $to_iterm "N: -"]
			} else {
				set dp [print_name_id $out_fp $to_iterm "N: "]
			}
#puts "len= $len  trans= $trans : $prev_line"

			#set line [parse_to_keyword $stream "clock"]

			# check for output
			#set to_group_clock [lindex $line 1]
			#if { $to_group_clock eq "" } {
				#set to_group_clock "0"
			#}
			#set line [parse_passed_keyword $stream "clock"]
			#if {$line eq ""} {
				#puts "Cannot find clock pin for to_term $to_iterm"
				#continue
			#}
			#set to_clock [lindex $line 0]
#puts "to_clock= $to_clock"
			set to_clock_term [parse_to_clock_pin $stream "output" $out_fp]
			set line [parse_to_keyword $stream "slack"]
			if { $line eq "" } {
				puts "Cannot find slack for $from_inst(1)"
				continue
			}
			set slack [lindex $line 2]
puts "$to_clock_term $to_iterm : $slack\n"

			puts $out_fp "P: $MM($path_type) $slack $arrv \
					$RF($trans) $from_clock_term $fdp \
					$to_clock_term $dp"
incr pCnt
continue
			if { $idcount>=0 } {
				#append lines [make_path_line $slack $arrv \
					#$from_group_clock \
					#$from_inst(0) $from_inst(1) \
					#$to_group_clock \
					#$to_clock $to_iterm \
					#$path_type $trans]

				set from_line [make_from_to $from_group_clock \
					$from_inst(0) $from_inst(1) "input" "ret_from" ]
				set to_line [make_from_to $to_group_clock \
					$to_clock $to_iterm "output" "ret_to" ]

				append lines "$from_line $to_line" \
					"P: $slack $arrv $RF($trans) \
					$MM($path_type) $ret_from $ret_to \n"

			}
			set group_clock ""
			set from_group_clock ""
			set to_group_clock ""
			set path_type ""
			#append lines "P: $slack $from_inst(0) $from_inst(1) $to_clock $to_iterm\n"
			continue
		}
		set start ""
		set end ""
		if { $word0 eq "Startpoint:" } {
			set start [lindex $line 1]
			set port_found 0
			if { [lindex $line 2] eq "(input" } {
				set port_found 1
			}

            		gets $stream line
			set word0 [lindex $line 0]

			if { $word0 eq "Endpoint:" } {
				set end [lindex $line 1]
			}
			set line [parse_to_keyword $stream "Path"]
			set line [parse_to_keyword $stream "Path"]
			if { [lindex $line 1] eq "Type:" } {
				set path_type [lindex $line 2]
			}

			set from_clock_term [parse_to_clock_pin $stream "input" $out_fp]
			gets $stream line
			set data_pin [lindex $line 0]
			if { [lindex $line 1] eq "(in)" } {
				set fdp [print_name_id $out_fp $data_pin "N: -"]
			} else {
				set fdp [print_name_id $out_fp $data_pin "N: "]
			}
puts "$from_clock_term $data_pin"
			#set line [parse_to_keyword $stream "clock"]
			#set from_group_clock "0"
			#if { [lindex $line 1] eq "(input" } {
				#set fi 0
				#set from_inst($fi) "input"
				#incr fi
			#} else {
				#set from_group_clock [lindex $line 1]
			#}
			#set line [parse_passed_keyword $stream "clock"]
			#if { [lindex $line 0] eq "input" } {
				#set fi 0
				#set from_inst($fi) "input"
				#incr fi
            			#gets $stream line
				#set from_inst($fi) [lindex $line 0]
			#} else {
				#set fi 0
				#set from_inst($fi) [lindex $line 0]
				#incr fi
            			#gets $stream line
				#set from_inst($fi) [lindex $line 0]
			#}
			set look_for_arrival 1
		}
	}
	close $stream
	# exec rm $filename
	return $pCnt
    }

    proc read_tmp_report { filename } {
        set stream [open $filename]
        set lines ""
        while { ![eof $stream] } {
            gets $stream line
            append lines $line "\n"         }
            close $stream
            # exec rm $filename
            return $lines
    }
    proc get_report_adhoc { filename args } {

		puts "args:$args:[lindex $args 0]"
   
            report_timing [lindex $args 0]
    }
    proc get_report_summary {cutoff delay style retvarname plist path_type tmp_rep_filename} {
        upvar $retvarname rpt

        if { $style == 2 } {
            report_timing -path_type $path_type -nosplit -slack_lesser_than $cutoff -delay_type $delay -significant_digits 6 -to $plist -nworst 250 -max_paths 2000000 > $tmp_rep_filename
        } else {
            report_timing -path_type $path_type -nosplit -slack_lesser_than $cutoff -delay_type $delay -significant_digits 6 -to $plist > $tmp_rep_filename
        }
	
        set rpt [read_tmp_report_brief $tmp_rep_filename -1 ]
    }
    proc get_report {cutoff delay style retvarname plist tmp_rep_filename} {
        upvar $retvarname rpt

        if { $style == 2 } {
            report_timing -path_type full_clock_expanded -nosplit -slack_lesser_than $cutoff -delay_type $delay -significant_digits 8 -to $plist -nworst 250 -max_paths 2000000 > $tmp_rep_filename
        } else {
            report_timing -path_type full_clock_expanded -start_end_pair -nosplit -slack_lesser_than $cutoff -delay_type $delay -significant_digits 8 -to $plist > $tmp_rep_filename
        }
        set rpt [read_tmp_report $tmp_rep_filename]
        return $rpt;
    }


    proc get_report_pt { cutoff delay style retvarname plist } {
        upvar $retvarname rpt
        if { $style == 2 } {
            redirect -variable rpt {report_timing -path_type full_clock -nosplit -slack_lesser_than $cutoff -delay_type $delay -significant_digits 8 -to $plist -nworst 250 -max_paths 2000000 }
        } else {
            redirect -variable rpt { report_timing -path_type full_clock -start_end_pair -nosplit -slack_lesser_than $cutoff -delay_type $delay -significant_digits 8 -to $plist}
        }
    }
    proc get_max_slack { v } {
    puts "max_slack $v -------------------"

        if { $::flag_max_slack == 1 } {
            #set r "[convert [get_attribute $v max_rise_slack]] "
            #set f "[convert [get_attribute $v max_fall_slack]] "
            set r [$v slack r max]
            set f [$v slack f max]

    puts "max_slack $r $f"
            if { $r < $f } {
                return $r
            } else {
                return $f
            }
        } else {
    puts "max_slack 1.011e29"
            return 1.011e29
        }
    }

    proc get_min_slack { v } {
    puts "minax_slack $v"

        if { $::flag_min_slack == 1 } {
            #set r "[convert [get_attribute $v max_rise_slack]] "
            #set f "[convert [get_attribute $v max_fall_slack]] "
            set r "[$v slack r min]"
            set f "[$v slack f min]"

            if { $r < $f } {
                return $r
            } else {
                return $f
            }
        } else {
            return 1.011e29
        }
    }

    proc rpt_get_endpoints { max_cutoff min_cutoff epfile } {
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
#puts "iregister $name"
            #### filter out clock gating pins
            #if { [get_attribute [get_pin $reg] is_clock_gating_pin] == true } {
                #puts "GPR: pin [get_object_name $reg] is a clock gating pin - eliminating it"
                #incr gc
                #continue
            #}
            ####
            if { $name ne "" } {
                #puts "Pin [get_object_name $reg] "
                #puts "Pin [get_object_name $reg] accepted [get_slack $reg]"
                lappend rl $name
#puts $rl
                if { $epfile ne "none" } {
                    puts $fp $name
                }
            } 
        }
#puts "epfile= $epfile"

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

    proc get_path_report { if rptfile maxfile epfile clkfile style {compress 1}  {start_line 4} {skip 1} } {
	init_rpt_vars
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
        puts "GPR LENGTH: $lt"
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

    proc internal_path_report_orig { rptfile max_cutoff min_cutoff maxfile gc style {compress 1} {start_line 4} { skip 1} old_new pl { path_type summary } } {

	global idcount
	if { ! [info exists idcount] } {
		set idcount 0
	}
        set t1 [lindex [split $maxfile "/"] 1]
        if { $t1=="" } {
                set t1 [lindex [split $maxfile "/"] 0]
        }
        append t1 ".tmp"
        set tmp_max_file "/tmp/"
        append tmp_max_file $t1

puts "tmp_max_file $tmp_max_file"

        init_path_reports $rptfile $compress $maxfile

	puts "start_line= $start_line gc= $gc skip= $skip"
        for { set i $start_line } { $i < $gc } { incr i $skip } {
            set line [string trim [lindex $pl $i]]
#puts "$line"
            #set line [string trim [lindex $pl 4]]
            if { $line eq "" } {
                continue
            }
            set plist {}
            if { $old_new == 0 } {
                foreach pin $line {
                    #lappend plist $::allt($pin)
                    lappend plist $::alln($pin)
                }
            } else {
                set plist [join $line]
            }

            #puts "GET RPT, line $i, [llength $plist] $plist"
            puts "GET RPT, line $i, [llength $plist]"
            debug_report "Line index: $i, [llength $plist]"
            foreach delay { min max } {
                set slack "::milos_${delay}_slack"
                if { [set $slack] == 1 } {
                    set cutoff "${delay}_cutoff"
		    if { $path_type ne "summary" } {
                    	get_report [set $cutoff] $delay $style "rpt_v" $plist $tmp_max_file
		    } else {
                    	get_report_summary [set $cutoff] $delay $style "rpt_v" $plist full $tmp_max_file
			if { $::dlevel == 0 } {
				exec rm $tmp_max_file
			} else {
				exec mv $tmp_max_file $tmp_max_file.$i
			}
		    }
                    write_report_data rpt_v
                }
            }
        }

        finalize_reports
    }

    proc internal_path_report { rptfile max_cutoff min_cutoff maxfile gc style {compress 1} {start_line 4} { skip 1} old_new pl { path_type summary } } {

	global idcount
	if { ! [info exists idcount] } {
		set idcount 0
	}
        set t1 [lindex [split $maxfile "/"] 1]
        if { $t1=="" } {
                set t1 [lindex [split $maxfile "/"] 0]
        }
        append t1 ".tmp"
        set tmp_max_file "/tmp/"
        append tmp_max_file $t1

puts "tmp_max_file $tmp_max_file"

        init_path_reports $rptfile $compress $maxfile

	set totalPathCount 0
	foreach delay { min max } {
		# DKF 01/22/11 set slack "::milos_${delay}_slack"
		# DKF 01/22/11 if { [set $slack] != 1 } { continue }

		set cutoff "${delay}_cutoff"

		report_timing   -path_type full -nosplit > $tmp_max_file.$delay

		#report_timing   -path_type full -nosplit \
				#-slack_lesser_than [set $cutoff] \
				#-nworst 5 -max_paths 2000000 \
				#-delay_type $delay \
				#-significant_digits 4 > $tmp_max_file.$delay

        	set pathCnt [ read_tmp_report_brief $tmp_max_file.$delay \
							$::rpt_data_fp -1 ]

		puts "Finished parsing $pathCnt $delay paths from file $tmp_max_file.$delay"
		set totalPathCount [expr $totalPathCount + $pathCnt]
        }
	puts "Finished parsing $totalPathCount min/max paths"

        finalize_reports
    }
proc get_hostname { } {
        set rc [info hostname]
        puts "hostname=$rc"
        flush stdout
}

proc echo_args { args } {
        puts "args=$args"
        flush stdout
}

puts "Finished Loading papi"
