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

## rte.tcl
## Milos middleware for calling router
## Ported from zt_procs.tcl by Mattias Hembruch

##
## NOTES:
##   
##

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::rte 0.1

itcl::class RTE {

    inherit VOB

    ## Public Vars
    ##

    ## End Public Vars

    ## Protected Vars
    ##
    protected variable finert 1
    protected variable use_pins 0

    protected variable block_list
    ## End Protected Vars

    protected method constructor { parent } { 
        VOB::constructor $parent
        set NAME "RTE"
        set SELFNAME "RTE$this"
        ## ONLY for public vars
        set_state rte init
        set_state rte_routed 0
    }

    ## used to "close the hole"
    protected method info { } 

    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help RTE $cmd $param $long
        } else {
            mod_help RTE $cmd
        }
    }

#    public method collect_inactive_tile_stats  {}  {

#	[get_tm ZT] GetTileList -sorted -result= ::tl4rt
#	[get_tm ZT] GetTileList -sorted -all -result= ::alltls
#	set all_tlist [split $::alltls]
#	set inactive_tlist {}

#	foreach tlelem $all_tlist  {
#	    if { $tlelem == "" }  {
#		continue
#	    }
#	    if { [string match *$tlelem* $::tl4rt] == 0 }  {
#               puts "Appending element: $tlelem"
#		lappend inactive_tlist $tlelem
#	    }
#	}

#	foreach tlelem $inactive_tlist  {
#	    [get_tm ZT] GetTileAttributes -name $tlelem -insts= ::tlinsts -nets= ::tlnets -viacnt= ::tlvias -netlength= ::tlnetlen 
#	    puts "For tile $tlelem : $::tlinsts $::tlnets $::tlvias $::tlnetlen"
#	}
#    }



    VOB::add_args RTE_REPAIR {
        { debug_skip_routing bool 0 0}
        { dump_to_ext_files bool 0 0}
        { output_verilog bool 0 0}
    }

    VOB::add_help RTE_REPAIR {
        { debug_skip_routing "If set, skips the routing step" }
        { dump_to_ext_files "If set, output files in Lef/Def format" }
        { output_verilog "If set, output files in Verilog format" }
    }

    public method repair { args } {
        if { [state db_loaded] != 1 } {
            warning "Please load a design before calling this command."
            return 1
        }
        parse_args
        puts "Start Repair" 
        echo [cmdline]
        set db [get_tm DB]
        
        $db order_wires -force -quiet
        $db rlog -tag "Find Wires to repair"
        $db find_disconnects
        $db get -top -object= ::block
        $db make_area_search -block $::block -insts -nets
        $db rlog -tag "Grouping insts and nets for mapping"
    
        set gr [get_tm GR]
        set cmd "$gr initialize"
        set rtc [::eval $cmd]
        db rlog -tag "Initialize structures for P2P" 
        if { $rtc != 0 } {
            error "Error $rtc from P2P (initialize)" "" -1
        }
        set cmd "$gr p2p"
        set rtc [::eval $cmd]
        db rlog -tag "Start rouring for P2P" 
        if { $rtc != 0 } {
            error "Error $rtc from P2P (routing)" "" -1
        }
        # The following is used for P2P testing
        $db order_wires -force -quiet
        $db rlog -tag "Verify wires"
        $db find_disconnects

        return $rtc 
    }

    VOB::add_args RTE_VERIFY {
        { debug_skip_routing bool 0 0}
        { dump_to_ext_files bool 0 0}
        { output_verilog bool 0 0}
        { router nstr 0 "nanoroute" }
    }

    VOB::add_help RTE_VERIFY {
        { debug_skip_routing "If set, skips the routing step" }
        { dump_to_ext_files "If set, output files in Lef/Def format" }
        { output_verilog "If set, output files in Verilog format" }
        { router "Which external router to run" }
    }
    public method verify { args } {
        if { [state db_loaded] != 1 } {
            warning "Please load a design before calling this command."
            return 1
        }
        parse_args
        puts "Start Verify" 
        echo [cmdline]
        set db [get_tm DB]
	    set router [get_arg router]

	    if { $router ne "nanoroute" }  {
		     warning "The $router does not support Verify command."
             return 1
	    }
        set msh [get_tm MSH]
        Publish chip.tech
        Publish chip.libs
      
        set ::block "forDrc"
        set pubblks($::block) [NextVersion $::block]
        Publish $pubblks($::block).block

        set subblks($::block) [NextVersion $::block.res]
        Subscribe -dbargs "-wires" -async $subblks($::block).block

        set class $router
        set straighten_ids "None" 
        $db get_block_stats -net_count= ::netCnt -inst_count= instCnt -iterm_count= itermCnt -bterm_count= btermCnt

        set tasks ""
        spawn v$::block  "
            ::apps::rx $router $pubblks($::block) $subblks($::block) 1 $::netCnt $straighten_ids 0 0 1" -class $class
        lappend tasks v$::block
        monitor $tasks {
            done { 
                notice "Task status: $task(task) : $task(return)"
            } 
        }
        foreach tmptsk $tasks {
            puts "Deleting task: $tmptsk"
            $tmptsk delete 
        }

        return 0 
    }

    VOB::add_args RTE_TILE_AND_ROUTE {
        { tiling_degree num 0 4 }
        { debug_skip_routing bool 0 0}
        { dump_to_ext_files bool 0 0}
        { output_verilog bool 0 0}
        { debug_db ofile 0 ""}
	    { straighten_nets slist 0 "" }
        { omit_local_wires bool 0 0 }
        { omit_fill_wires bool 0 1 }
        { omit_block_wires bool 0 1 }
        { top_routing_layer num 0 0 }
        { router_insert_file ifile 0 } 
	    { max_tilemem hnum 0 -1 }
	    { tiling_only hbool 0 0 }
	    { routing_only hbool 0 0 }
        { no_tiling bool 0 0 }
        { router nstr 0 "nanoroute" }
    }

    VOB::add_help RTE_TILE_AND_ROUTE {
        { tiling_degree "Controls number of tiles created -- a higher number results in more tiles" }
        { debug_skip_routing "If set, skips the routing step" }
        { dump_to_ext_files "If set, output files in Lef/Def format" }
        { output_verilog "If set, output files in Verilog format" }
        { debug_db "Filename to save debugging db to" }
        { straighten_nets "Invoke straightening function of router on the nets named in this list" }
        { omit_local_wires "If set, does not route local wires" }
        { omit_fill_wires "If set, exclude special net FILLWIRE wires from remote routing job" }
        { omit_block_wires "If set, exclude special net BLOCKWIRE wires from remote routing job" }
        { top_routing_layer "Shorthand to specify topmost routing layer to router" }
        { router_insert_file "A file containing routing commands.  Will be inserted in the router command file for all tiles at the line just prior to global/detailed routing" } 
        { no_tiling bool "If set, does not tile the design" }
        { router "Which external router to run" }
    }
    public method tile_and_route { args } {
        if { [state db_loaded] != 1 } {
            warning "Please load a design before calling this command."
            return 1
        }

        parse_args

        puts "Start Tile and Route"

        echo [cmdline]
        set mytimer [start_timer]
        #echo [settings]

	# Initialize variables that are active in both segments

	set zt [get_tm ZT]
	set db [get_tm DB]
	set force_all ""
	set debug_db [get_arg debug_db]
	set block_list ""
	set router [get_arg router]

	if { [get_arg routing_only] == 0 }  {
	    ## do legalization
	    call_milos "opt make_legal"

	    if { ($router ne "wroute") && ($router ne "nanoroute") }  {
            end_timer $mytimer "Routing"
		error "Currently, $router not supported" "" -1
	    }

	    if { [get_arg max_tilemem] > 4 }  {
		set tile_downcnt [get_arg max_tilemem]
		set tile_upcnt 0
		while { $tile_downcnt > 4 }  {
		    set tile_upcnt [expr $tile_upcnt + 1]
		    set tile_downcnt [expr $tile_downcnt / 2.0]
		}
		set tile_upcnt [expr $tile_upcnt * 2]
		if { $tile_downcnt > 3 }  {
		    set tile_upcnt [expr $tile_upcnt + 1]
		}
		set tildeg [expr 21 - $tile_upcnt]
		notice "Set tiling degree to $tildeg, since user specified a maximum tile memory size of [get_arg max_tilemem] Mb"
	    } else {
		set tildeg [get_arg tiling_degree]
#####################################################################
# DKF  3/5/2007
# small blocks should have no tiling or tililg degree of 2
#####################################################################
			if { $tildeg > 1 } {
				db get_block_stats -net_count= netCnt -inst_count= instCnt -iterm_count= itermCnt -bterm_count= btermCnt
				# puts "instCount= $instCnt"
				# puts "netCount= $netCnt"
				# puts "itermCount= $itermCnt"
				# puts "btermCount= $btermCnt"

				if { $netCnt < 20000 } {
					set tildeg 2
					notice "Reset tiling degree to $tildeg, since net count <$netCnt> is less than 20000"
				}
	    	}
#####################################################################
		}
	    
	    set tilenum [expr pow(2,$tildeg)]

	    set block_list [tiling $tilenum [get_arg no_tiling] [get_arg straighten_nets]]
	    ## has a default, so ok
	    if { $finert != 1 } {
		set blocks4pa [lindex $block_list 1]
		route_blocks $router $blocks4pa { }
		if { $debug_db != "" } {
		    $db write -file "${debug_db}_assigned.db"
		}
	    }

	    set omit ""
	    if { [get_arg omit_local_wires] == 1} {
		set force_all "-all"
		set omit "-omit_local_wires"
	    }

	    set pins ""
	    if { $use_pins == 1 } {
		set pins "-use_pins"
	    }

	    set ptwires "-partition_wires"

	    set ofw "" ; set obw ""
	    if { [get_arg omit_fill_wires] > 0 }  {
		set ofw "-omit_fill_wires"
	    }
	    if { [get_arg omit_block_wires] > 0 }  {
		set obw "-omit_block_wires"
	    }

	    set cmd "$zt ProcessPinRoutes $pins $omit $ptwires $ofw $obw"
	    set rc [::eval $cmd]
	    if { $rc != 0 } {
            end_timer $mytimer "Routing"
		error "Error $rc from ProcessPinRoutes." "" -1
	    }
	} else {
	    notice "Skipping tile creation, since -routing_only selected"
	}

	if { [get_arg tiling_only] == 0 }  {
	    cmd_rlog "At routing"
	    set ::mytiles ""
	    set ::straighten_ids ""
	    set cmd "$zt GetTileList -sorted $force_all -result= ::mytiles -straighten_ids= ::straighten_ids"
	    set rc [::eval $cmd]
	    if { $rc != 0 } {
            end_timer $mytimer "Routing"
		error "Error $rc from GetTileList." "" -1
	    }

	    if { [check_arg router_insert_file] }  {
               if { [catch {open $refname "r"} refchan] == 0 } {
		   set lineno 0
		   while { [gets $refchan nxline] >= 0 }  {
		       set insert_file_arr($lineno) $nxline 
		       set lineno [expr $lineno + 1]
		   }
		   close $refchan
	       } else {
		   warning "Could not open router command insertion file [get_arg router_insert_file] : $refchan"
		   set insert_file_arr 0
	       }
	    } else {
	       set insert_file_arr 0
        }

	    if { [get_arg straighten_nets] ne "" }  {
		if { [llength $::straighten_ids] == 0 }  {
		    warning "Option -straighten_nets was specified, but the list of nets is empty"
		}
		if { $router ne "wroute" }  {
		    warning "Option -straighten_nets not supported with router $router"
		    set $::straighten_ids {}
		}
	    }

	    puts "Tiles that need routing: $::mytiles"

	    set tiles_not4route { }
        set dump_to_ext 0
        if { [get_arg dump_to_ext_files] == 1 } {
            set dump_to_ext 1
        }

        if { [get_arg output_verilog] == 1 } {
            set dump_to_ext [expr $dump_to_ext + 2]
        }

	    if { [get_arg debug_skip_routing] == 1 } {
		    set tiles_not4route $::mytiles
	    }

	    if { $debug_db != "" } {
		    $db write -file "${debug_db}_processed.db"
	    }

	    set tiles_routed [route_blocks $router $::mytiles $tiles_not4route $dump_to_ext $::straighten_ids [get_arg top_routing_layer] $insert_file_arr]

        if { $tiles_routed != "DUMPDEF" } {
   	        if { ([llength $tiles_routed] >= 1) 
		     && ([lindex $tiles_routed 0] == "ERROR") }  {
                    end_timer $mytimer "Routing"
                    error "Routing of tiles has failed: [set tiles_routed]" "" -1
	        }

	        set rc [$zt AssembleTiles -include_fixed]

	        if { $rc != 0 } {
                end_timer $mytimer "Routing"
                error "Error $rc from AssembleTiles" "" -1
	        }

	        if { $debug_db != "" } {
	        	$db write -file "${debug_db}_assembled.db"
	        }
    	    $db order_wires 
    	    $db find_disconnects -verbose

    	    if { $debug_db != "" } {
        		$db write -file "${debug_db}_done.db"
    	    }
        } else {
            set tiles_routed { }
        }

	    $zt TilerReleaseMem
	    cmd_rlog
	    set_state rt_routed 1
        end_timer $mytimer "Routing"
	    return $tiles_routed
	} else {
	    notice "Skipping routing stage, since -tiling_only selected"
	}

    end_timer $mytimer "Routing"
	return $block_list
    }

    private method tiling { tilenum no_tiling straighten_nets } {

        set db [get_tm DB]
        $db order_wires -quiet
        $db find_disconnects

        set zt [get_tm ZT]
        #set zpl [get_tm ZPLACE]

        set ftarg ""
        if { $finert == 0 }  {
           set ftarg "-glbrt"
        }

        set kparg "-keep_partial_wires"

	set straighten_len [llength $straighten_nets]
	if { $straighten_len > 0 }  {
	    puts "Total of $straighten_len nets to be straightened during routing"
	}

	if { [llength $straighten_nets] == 0 }  {
	    set cmd "$zt TilerInit $ftarg $kparg"
	} else {
	    set cmd "$zt TilerInit $ftarg $kparg -straighten_nets { $straighten_nets }"
	}
        set rc [::eval $cmd]
        if { $rc != 0 } {
            error "Error $rc from TilerInit" "" -1
        }
        
        if { $no_tiling == 1 }  {
               set tilenum 1
        }

        set rc [$zt TileDesign -tileno $tilenum -result= ::mytiling]
        if { $rc != 0 } {
            error "Error $rc from TileDesign" "" -1
        }

        puts "All Tiles: $::mytiling"

        #if { ($finert < 0) || ($finert > 1) } {
        #       zpl move_away
        #   }

        if { $finert != 1 } {
            $zt GenPinRoutes -result= ::mypinroutes
            puts "GPR results: $::mypinroutes"
            return [list $::mytiling $::mypinroutes]
        }

        return [list $::mytiling]
    }

    private method dump_not4route_blocks { route_blocks dump_to_ext } {

        puts "Starting Non-routing dump blocks to external files"

        foreach b [join $block_list] {
            if { [lsearch $route_blocks $b] >= 0 } { continue }
            set prefixdir [file join scratch norouting $b]
            set prefix [file join $prefixdir $b]
            # write input files
            make_scratch_dir $prefixdir
            if { $dump_to_ext == 1 || $dump_to_ext == 3} {
               write_block_lef_def $prefix $b
            }
            if { $dump_to_ext == 2 || $dump_to_ext == 3} {
               write_block_verilog $prefix $b
            }
        }
    }
# dump_block starts here

    private method dump_blocks { router blocks straighten_ids top_routing_layer insert_file_arr dump_to_ext} {

        puts "Starting dump blocks to external files"

#       foreach b $blocks {
#           set pubblks($b) [NextVersion $b]
#       }
#       foreach b $blocks {
#           set subblks($b) [NextVersion $b.res]
#       }
        if { ($router eq "wroute") && ([file readable "wr_xtra.cfg"]) } {
            puts "Routing with supplemental config file wr_xtra.cfg"
        }
        ZT zt
        foreach b $blocks {
            set ::numnets 0
            set ::numinsts 0
            set rc [[get_tm ZT] GetTileAttributes -name $b -nets= ::numnets -insts= ::numinsts]
            puts "num nets: $::numnets, num insts: $::numinsts"
            if { ($rc > 0) || ($::numnets <= 0) }  {
                error "Could not get attributes of tile $b"
            }
            if { $router eq "wroute" }  {
                set config_cmd "write_wroute_config"
            }
            if { $router eq "nanoroute" }  {
                set config_cmd "write_nroute_config"
            }
            # Set up file names
            set prefixdir [file join scratch $b]
            set prefix [file join $prefixdir $b]
            set straighten_file "straighten_list"
            set straighten_num [llength $straighten_ids]
            puts "At remote for straightening: $straighten_num, $straighten_ids"
            # Create tiler instance, but do not initialize!
            # ZT zt
            set rc 0
            # write input files
            make_scratch_dir $prefixdir
            if { $dump_to_ext == 1 || $dump_to_ext == 3} {
               write_block_lef_def $prefix $b
            }
            if { $dump_to_ext == 2 || $dump_to_ext == 3} {
               write_block_verilog $prefix $b
            }
            zt xlate_straighten_list -nets $straighten_ids -router_file [file join $prefixdir $straighten_file] -num_straightened= ::num_straightened
            set config_name [$config_cmd $prefixdir $b $::numnets $::num_straightened $straighten_file $top_routing_layer $insert_file_arr $b.routed.def 0]
        }
        return "DUMPDEF"
    }

# and end here, the dump_block

    private method route_blocks { router blocks blocks_not4route dump_to_ext straighten_ids top_routing_layer insert_file_arr } {
        if { ($dump_to_ext > 0) && ($blocks != "") } {
            dump_not4route_blocks $blocks $dump_to_ext
            set dump_ext [dump_blocks $router $blocks $straighten_ids $top_routing_layer $insert_file_arr $dump_to_ext]
            return $dump_ext
       }
        puts "Starting route_blocks"

        set tiles_routed { }

        if { ($router eq "wroute") && ([file readable "wr_xtra.cfg"]) } {
            puts "Routing with supplemental config file wr_xtra.cfg"
        }
        set msh [get_tm MSH]

	#
	# Uncomment this line to engage purify -- forces use of alternate build 
	# for remotes.
	# set msh::PROGRAM /fs/builds/daily/turbo12152005/Release/ads/bin/ade
	#
        Publish chip.tech
        Publish chip.libs

        foreach b $blocks {
            set pubblks($b) [NextVersion $b]
            Publish $pubblks($b).block
        }

        foreach b $blocks {
            set subblks($b) [NextVersion $b.res]
            Subscribe -dbargs "-wires" -async $subblks($b).block
        }

        set tasks ""

        ## check for dryrun mode
        if { $::msh::OPT(dryrun) == 1 } {
            set blocks {}
        }
        # -- depending on the router used, request the right task class
        # -- from msh
        switch $router {
            "wroute"   - "nanoroute"  { set class $router }
            default      { error "$router: router not supported" }
        }
        foreach b $blocks {
            set need2route 1
            if { [lsearch $blocks_not4route $b] > -1 } {
                set need2route 0
                if { $dump_to_ext > 0 } {
                    set need2route 2
                }
            }

	        set ::numnets 0
	        set ::numinsts 0
    	    set rc 0
    	    if { $router eq "nanoroute" }  {
    		    set class "nanoroute"
        		set rc [[get_tm ZT] GetTileAttributes -name $b -nets= ::numnets -insts= ::numinsts]
                puts "num nets: $::numnets, num insts: $::numinsts"
        		if { ($rc > 0) || ($::numnets <= 0) }  {
        		    error "Could not get attributes of tile $b"
        		}
        		if { $::numnets >= 200000 }  {
        		    set class "nanoroute2"
        		}
        		if { $::numnets >= 300000 }  {
        		    set class "nanoroute3"
        		}
        		if { $::numnets >= 600000 }  {
        		    set class "nanoroute4"
        		}
    	    }
            spawn t$b  "
            ::apps::rx $router $pubblks($b) $subblks($b) $need2route $::numnets $straighten_ids $top_routing_layer $insert_file_arr 0
            " -class $class

            lappend tasks t$b
        }

        set badtasks 0
        monitor $tasks {
            done {
                lappend tiles_routed [string trimleft $task(task) "t"]
                notice "Task status: $task(task) : $task(return)"
                if { ([string compare $task(return) ""] == 0) || ($task(return) != 0) }  {
                    incr badtasks
                }
		set DRCdx [string last "\#Total number of DRC violations = " $task(output)]
		if { $DRCdx == -1 }  {
		    warning "Could not determine number of DRC violations: no matching line in log file"
		} else {
		    set drc_sbstr [string range $task(output) $DRCdx end]
		    if { [regexp "^#Total number of DRC violations = (\[0-9\]+)\[ \t\]*\n.*" $drc_sbstr drc_numstr num_drcs] == 1 }  {
			notice "Number of DRC violations: $num_drcs"
		    } else {
			warning "Could not determine number of DRC violations: regular expression match failed:: $drc_sbstr"
		    }
		}
	    }
        }

        foreach tmptsk $tasks {
            puts "Deleting task: $tmptsk"
            $tmptsk delete
        }

        if { $badtasks }  {
            puts "ERROR: $badtasks routing tasks failed"
            return [list "ERROR"]
        }

        puts $tiles_routed
        puts "Ending route_blocks"
        return $tiles_routed
    }


    ## Everything below is for remote/rnode execution
    ## No instances/methods - everything is global/procs
    ## NO get_tm, no instances, etc
            
    public proc rnode { router bin bout need2route numnets straighten_ids top_routing_layer insert_file_arr verify_drc } {
        msh::init

        Subscribe chip.tech
        Subscribe chip.libs
        Subscribe $bin.block

	    set rte_cmd "run_cdsroute"
        if { $router eq "wroute" }  {
            set config_cmd "write_wroute_config"
	    }
        if { $router eq "nanoroute" }  {
            set config_cmd "write_nroute_config"
	    }

	    # Defensive, in case routers not covered by cdsroute routine.

        if { [string compare $rte_cmd ""] == 0 }  {
            error "Internal error: Router $router not acknowledged" "" -1
        }

	    # Set up file names
    	set prefixdir [file join scratch $bin]
    	set prefix [file join $prefixdir $bin]
    	set straighten_file "straighten_list"
        if { $verify_drc == 0 } {
    	    set straighten_num [llength $straighten_ids]
    	    puts "At remote for straightening: $straighten_num, $straighten_ids"
    	    # Create tiler instance, but do not initialize!
	        ZT zt
        }
        set rc 0
        if { $need2route == 1 }  {
            # write input files
    	    make_scratch_dir $prefixdir
    	    write_lef_def $prefix
            if { $verify_drc == 0 } {
	            zt xlate_straighten_list -nets $straighten_ids -router_file [file join $prefixdir $straighten_file] -num_straightened= ::num_straightened
                set config_name [$config_cmd $prefixdir $bin $numnets $::num_straightened $straighten_file $top_routing_layer $insert_file_arr $bin.routed.def $verify_drc]
            } else {
                set config_name [$config_cmd $prefixdir $bin $numnets 0 $straighten_file $top_routing_layer $insert_file_arr $bin.routed.def $verify_drc]
            }
            # run router
            set rc [ $rte_cmd $router $prefixdir $config_name ]
        } else {
            puts "Skip routing invoked..."
            if { ($need2route == 2) || ($need2route == 0) }  {
                # write input files
    	        make_scratch_dir $prefixdir
    	        write_lef_def $prefix
                after 200
            }
            after 2000
        }
        if { $rc }  {
            puts "Error: external router failed"
	        return $rc
        }
        if { $verify_drc == 0 } {
            # load wires 
            db get -top -object= block
            db load_def_eco -replacewires -block $block -file $prefix.routed.def
            # db order_wires
            if [file exists [file join $prefixdir wr.logx]]  {
                puts [exec tail -12 [file join $prefixdir wr.logx]]
            }
        } 
        # Send the results in
	    set task(DRC) 17
        Publish  -dbargs "-wires" -wait $bout.block
        return 0
    }

    ### Note: PROC - only used by rnode, NOT by functional module
    protected proc make_scratch_dir { prefixdir } {
	puts "Making sub-directory $prefixdir"
        file mkdir $prefixdir
    }
    protected proc write_block_lef_def { prefix block } {
        db save_lef -file $prefix.lef 
        set ::dblock ""
        db find -block $block -object= ::dblock
        if { $::dblock eq "" } {
            error "Cannot find db block $block!"
        }
        db save_def -file $prefix.def -block $::dblock
    }   
    protected method write_block_verilog { prefix block } {
    # define "method" instead of "proc" for [get_tm] to work
        set ::dblock ""
        db find -block $block -object= ::dblock
        if { $::dblock eq "" } {
            error "Cannot find db block $block!"
        }
        puts "Starting Verilog-Output"
        [get_tm Tmg] write_verilog -file $prefix.v -delimiter / -block $::dblock
    }   

    ### Note: PROC - only used by rnode, NOT by functional module
    protected proc write_lef_def { prefix } {
        db save_lef -file $prefix.lef
        db save_def -file $prefix.def
    }   
        
    ### Note: PROC - only used by rnode, NOT by functional module
    protected proc write_wroute_config {run_dir bin num_nets num_straightened straighten_file top_routing_layer insert_file_arr {defout "routed.def"} verify_drc {dbout ""} } {

        if { $defout eq "routed.def" }  {
            set defout $bin.routed.def
        }

        set config_name $bin.config
        set full_name [file join $run_dir $config_name]
        set fileId [open $full_name w 0644 ]

        puts " Conf file is stored on $run_dir $config_name"

        # puts $fileId "licenseName Envisia_SE_ultra_place_route"
        puts $fileId "inputLefName $bin.lef"
        puts $fileId "inputDefName $bin.def"

        if { $verify_drc == 0 } {
            if { [string length $defout] > 0 }   {
                puts $fileId "outputDefName $defout"
            }
            if { [string length $dbout] > 0 }   {
                puts $fileId "outputDbName $dbout"
            }
            puts $fileId "routeGlobal"
            puts $fileId "routeFinal"
            puts $fileId "frouteAutoStop false"
	        if { $num_straightened > 0 }  {
	            puts $fileId "routeStraightenNet <$straighten_file"
	        }
            if { [file readable "wr_xtra.cfg"] }  {
                set cfgFileId [open "wr_xtra.cfg" r]
                set cfgStr [read $cfgFileId]
                puts $fileId $cfgStr
            }
            # puts $fileId "routeDisableDefaultStacking true"
            # puts $fileId "routePinAccessMode compat+upvia"
        } else {
            puts $fileId "verify -drc"
        } 
        close $fileId

        return $config_name
    }

    ### Note: PROC - only used by rnode, NOT by functional module
    #  Attempt to unify wroute and nanoroute calls
    protected proc run_cdsroute {router dir config_name} {

	if { ($router ne "wroute") && ($router ne "nanoroute") }  {
	    error "Router $router not supported by cdsroute!"
	}

	set use_wroute [expr [string compare $router "wroute"] == 0]
	set basename "nr.log"
	if { $use_wroute }  {
	    set basename "wr.logx"
	}
	set fname $basename
	set refname [file join $dir $basename]
	set filecnt 0

	if { [file exists $refname] }  {
	    while { [file exists [file join $dir $fname]] }  {
		set filecnt [expr $filecnt + 1]
		set fname "$basename$filecnt"
	    }
	    file rename $refname [file join $dir $fname]
	}
 
	set need2call 1
	# Tom sez make license retries infinite (??)
	set callcnt 90000 
	while { ($need2call == 1) && ($callcnt > 0) }  {
	    set iocnt 10
	    set rc ""
	    set cmd "cd $dir ; ca nanoroute -batch -accel_any_lic -log $basename < $config_name"
	    if { $use_wroute }  {
		set cmd "cd $dir ; ca wroute -q $config_name -l wr.logx"
	    }
	    set res [catch {exec sh -c $cmd } rc ]

	    while { ([file exists $refname] == 0) && ($iocnt > 0) }  {
		after 60000
		set iocnt [expr $iocnt - 1]
	    }
	    if { $iocnt == 0 }  {
		puts "Could not find file $refname"
		puts "Result code from router/wrapper: $res"
		puts "Messages from router/wrapper: $rc"
		puts "Retrying router job, since it did not start"
		set callcnt [expr $callcnt - 30000]
		continue
	    }
	    if { [catch {open $refname "r"} refchan] } {
		puts "Error opening file $refname: $refchan"
		return 1
	    }

	    if { $use_wroute == 0 }  {
		set need2call 0
	    }

	    while { [gets $refchan curline] >= 0 }  {
		if { $use_wroute }  {
		    if { [regexp "violations\[ \t\]*=" $curline] == 1 }  {
			set need2call 0
			break
		    }
		}  else {
		    if { ([regexp "ERROR" $curline] == 1) && ([regexp -nocase "license" $curline] == 1) }  {
			# Return code for LSF config (OBSOLETED 11/17/06 GDA)
			# set res 97
			set need2call 1
			break
		    }
		    if { [regexp "ERROR" $curline] == 1 }  {
			set res 1
			break
		    }
		}
	    }

	    if { $need2call == 1 }  {
		puts "Repeating call to router, since it did not finish"
		if { [file exists $refname] }  {
		    file rename -force $refname $refname.debug
		    file delete $refname
                }
		after 10000
		set callcnt [expr $callcnt - 1]
	    }
	}

	if { $callcnt == 0  }  {
	    puts "Router failed to finish, giving up"
	    return 1
	}

	puts "Router finished with return code $res and message $rc"
	return $res
    }
        
    ### Note: PROC - only used by rnode, NOT by functional module
    protected proc write_nroute_config {run_dir bin num_nets num_straightened straighten_file top_routing_layer insert_file_arr {defout "routed.def"} verify_drc {dbout ""} } {
        
        if { $defout eq "routed.def" }  {
            set defout $bin.routed.def
        }
        set config_name ${bin}_nr.tcl
        set full_name [file join $run_dir $config_name]
        set fileId [open $full_name w 0644 ]

    	if { $num_nets >= 200000 }  {
    	    puts $fileId "pdi set_option env_number_processor 2"
    	}
        puts $fileId "pdi import_lib -lef $bin.lef"
        puts $fileId "pdi import_design -def $bin.def"
        if { $verify_drc == 0 } {
            puts $fileId "pdi set_option route_with_eco true"
        	if { $top_routing_layer > 1 }  {
        	    puts $fileId "pdi set_option route_top_routing_layer $top_routing_layer"
            }
	        if { [array exists insert_file_arr] }  {
	            set lineno 0
	            while { [info exists insert_file_arr($lineno)] }  {
            		puts "$insert_file_arr($lineno)"
	            	set lineno [expr $lineno + 1]
	            }
	        }
            puts $fileId "pdi global_route"
            puts $fileId "pdi detail_route"
            if { [string length $defout] > 0 }   {
                puts $fileId "pdi export_design -def $defout"
            }
            if { [string length $dbout] > 0 }   {
                puts $fileId "pdi save_design $dbout"
            }
        } else {
                puts $fileId "pdi set_option env_number_fail_limit 5"
                puts $fileId "pdi set_option env_number_warning_limit 2000"
                ## set net "DECOUPLER_initiator_wdata_reg28"
                ## puts $fileId "pdi set_attribute -net \"$net\" -is_selected true"
                ## puts $fileId "pdi set_option route_selected_net_only true"
                puts $fileId "pdi verify -drc"
        }
        close $fileId

        return $config_name
    }
}
