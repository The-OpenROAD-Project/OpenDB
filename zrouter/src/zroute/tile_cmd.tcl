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

## tile_cmd.tcl
## Author: Mattias Hembruch

package require Itcl

package provide ade::turborc 1.0

itcl::class TurboRC {

    ## Tiling related variables
    public variable ntiles 0

    ## Input/Output related variables
    public variable leffiles ""
    public variable leflist ""
    public variable deffile ""
    public variable datadir tmp
    public variable tmpcmdfile "qx_trc.cmd"
    public variable outfile ""

    public variable asyncwrite 0
    public variable demandtiling 0

    ## General Vars
    ##
    public variable mode "qx"
    public variable writedb 0
    public variable readdb 0

    ## QX related variables
    public variable debug_mem 0
    public variable qxbin "qx"
    public variable qxcmdfile "qx.cmd"
    public variable qxargs ""
    public variable qxlog "qx.log"
    public variable qxout "qx"

    public variable remote_trc "";

    public variable debug 0 {
        #notice "debug: $debug"
        if { [string is integer $debug] } {
            if { $debug > 0 } {
                notice "TurboRC Debug On!"
                ::zr.zlog enable_debug_log -module TRC
                #::zr.zlog configure -debug 1
            } else {
                notice "TurboRC Debug Off!"
                ::zr.zlog clear_debug_log -module TRC
                #::zr.zlog configure -debug 0
            }
        } else {
            warning "Debug value $debug not integer - resetting"
            notice "TurboRC Debug Off!"
            set debug 0
            ::zr.zlog clear_debug_log -module TRC
            #::zr.zlog configure -debug 0
        }
    }

    protected variable old_idle 2; #hold the old value of idle

    public variable idle_level 2 {
        if { [string is integer $idle_level] } {
            if { $idle_level >= 0 && $idle_level <= 2} {
                ::zr configure -IDLE_LEVEL $idle_level
            } else {
                warning "invalid idle_level value: $idle_level - ignored"
                set idle_level $old_idle
            }
        } else {
            warning "invalid idle_level value: $idle_level - ignored"
            set idle_level $old_idle
        }
    }


    ## protected vars
    ## variables with string values
    protected variable STRINGVARS {leffiles leflist deffile datadir tmpcmdfile outfile mode qxbin qxcmdfile qxargs qxlog qxout remote_trc } 
    ## variables with numeric values (0 = off, non-0 = show value)
    protected variable NUMVARS  {readdb writedb debug debug_mem asyncwrite demandtiling idle_level} 

    protected variable qxfile ""
    protected variable filebase ""
    protected variable curdir [exec pwd]
    protected variable lefl


    protected variable blocks 0

    protected variable starttime ""
    protected variable commandline ""
    protected variable errorcount 0
    protected variable rc 0

    protected variable tasks

    protected variable qx_total_c_threshold 5.0
    protected variable qx_relative_c_threshold 0.01

    protected variable init_te 0
    protected variable init_ext 0
    protected variable init_gm 0

    ## in microns^2 (0.06 mm^2)
    protected variable idealmintilesize 60000
    protected variable finalmintilesize 400
    protected variable goalterms 40000
    protected variable maxtiles 100
    protected variable die_area 0

    protected variable retry

    protected variable assembled 0

    protected variable ae_pw ""

    protected common NO_LIC_ERROR "----------------------   FATAL   ----------------------\n"
    append NO_LIC_ERROR "Unable to run; no license is available.\n"

    protected common MAXRETRY 10
    protected common NAME "TurboRC"

    protected common VERSION 1.0

    #Just in case...
    protected common Id ""
    protected common RCSINFO "$Id: tile_cmd.tcl,v 1.36 2006/05/23 00:27:41 mattias Exp $"



    ## To hide the "isa" method for Itcl objects
    private method isa { } {
    }

    private method search_data { data } {
        set dl [split $data "\n"]
        foreach arg $dl {
            set arg [string toupper [string trim $arg]]
            set opt ""
            set val ""

            set cnt [scan $arg "SETVAR %s %f" opt val]
            if { $cnt == 2 } {
                if { $opt ==  "TOTAL_C_THRESHOLD" } {
                    if { ($val >= 0.0) && ($val < 20.0) } {
                        debug TRC:Q "qx_total_c_threshold: $val"
                        set qx_total_c_threshold $val
                    }
                } elseif { $opt == "RELATIVE_C_THRESHOLD" } {
                    if { ($val >= 0.0) && ($val < 0.2) } {
                        debug TRC:Q "qx_relative_c_threshold: $val"
                        set qx_relative_c_threshold $val
                    }
                }
            }
        }
    }


    private method tile_debug { script } {
        if { $debug > 0 } {
            uplevel 1 $script
        }
    }


    private method check_error { event lrc } {

        if { $errorcount != 0 } {
            warning "$errorcount error(s) encountered while $event - processing ended!"
            timer Report
            set rc $lrc
            return "return $rc";
        }
        return ""
    }


    private method write_qx_config { ofile data } {

        append data "setvar log_file                    $qxlog\n"
        append data "setvar output_file_name            $qxout\n"
        append data "setvar compressed_output           true\n"
        
        set fileId [open $ofile w 0664]
        puts $fileId $data
        close $fileId
    }

    private method init_ext { } {
        if { $init_ext == 0 } {
            set init_ext 1
            Ext ext
            #debug ZTM:A "ZTM:A Ext ::TurboRC::ext"
        }
    }
    private method init_te { } {
        if { $init_te == 0 } {
            set init_te 1
            TE te
            #debug ZTM:A "ZTM:A TE ::TurboRC::te"
            te get_die_area -area= area
            notice "Area: $area"
            if { $area > 0 } {
                set die_area $area
                return 0
            } else { 
                te delete
                return -1
            }
        } else {
            return 0
        }
    }

    private proc check_license { output } { 
        set idx [string first $NO_LIC_ERROR $output]
        if { $idx >= 0 } {
            return 1
        } else {
            return 0
        }
    }


    private proc dir_cleanup {dir clean} {
        if { !$clean } { return }
        file delete -force $dir
    }

    public proc read_file { ifile } {
        if { [file exists $ifile] } {
            set fileId [open $ifile r]
            set data [read $fileId]
            close $fileId
            return $data
        } else {
            return ""
        }
    }

    public proc check_file { ifile } {
        if { [file exists $ifile] && [file isfile $ifile] } {
            return 0
        } else {
            return 1
        }
    }

    public method eval { script } {
        if { $ae_pw == "" } {
            if { [info exists ::msh::OPT(AE_PW)] } {
                set ae_pw $::msh::OPT(AE_PW)
            }
        }
        while { $ae_pw != $apps::turborc::AE_PW } {
            notice "Please Enter AE password (Empty password aborts)."
            set ae_pw [gets stdin]
            if { $ae_pw == "" } {
                return
            } elseif {$ae_pw != $apps::turborc::AE_PW} {
                notice "Invalid password!"
            }
        }
        if { $ae_pw == $apps::turborc::AE_PW } {
            set scr "::eval $script"
            set res [::eval $scr]
            return $res
        }
    }

    public method copy_qx_file { {ifile ""} {ofile ""} } {

        if { $ifile == "" } {
            set ifile $qxcmdfile
        }

        if { $ofile == "" } {
            set ofile $tmpcmdfile
        }

        set data [read_file $ifile]
        search_data $data
        write_qx_config $ofile $data
    }

    private method _itcl_config { option args } @itcl-builtin-configure

    private method getdebmem { state } {
        if { $debug_mem > 0 } {
            notice "TurboRC Memory $state: [getmem]"
        }
    }

    public method settings { } {
        notice "$NAME Settings"
        notice "Msh Config: $::env(MSHCONFIG)"
        foreach var $STRINGVARS {
            set val ""
            set cmd "set val \$${var}"
            ::eval $cmd
            set cmd "notice \"${var}: $val\""
            ::eval $cmd
        }

        foreach var $NUMVARS {
            set cmd "set $var"
            set a [::eval $cmd]
            if { $a == 0 } {
                puts "${var}: off"
            } else {
                set cmd "notice \"${var}: $$var\""
                ::eval $cmd
            }
        }
    }


    public method constructor { args } {
        set starttime [clock format [clock scan now]]
        set commandline "$::argv0 $::argv"
    }

    public method banner { } {
        notice "TurboRC Version $VERSION"
        notice "Start Time: $starttime"
        notice "commandline: $commandline"
        #set cli [lindex [::zr.zlog configure -cli] 2]
        #$cli Bind <<eof>> $this onexit
    }

    #public method onexit { } {
        ## Just in case we need to capture some form of exit!
        ## Uncomment 2 lines in banner above..
        #notice "Exiting TurboRC!!"
    #}

    public method getmem { } {
        if { $init_gm == 0 } {
            db rlog -tag "TurboRC"
            set init_gm 1
        }
        ::zr.zlog configure -return_notice 1
        db rlog
        ::zr.zlog configure -return_notice 0
        return [::zr.zlog getLastNotice]
    }


    public method run_tiling { {option ""} args } {

        if { $option != "" } {
            ::eval "_itcl_config $option $args"
        }

        tile_debug { settings }

        set errorcount 0

        if { [string toupper $mode] == "QX" } {

            notice "Start of Run: [exec date]"
            timer Mark run
            tile_debug { timer Mark init }

            set lefl { }
            if { [string trim $leffiles] != "" } {
                set lefl [read_file $leffiles]
            }

            foreach arg $leflist {
                append lefl $arg
            }

            foreach arg $lefl {
                if { [check_file $arg] != 0 } {
                    warning "Error: Invalid Lef File specified for processing: $arg."
                    return -1;
                }
            }

            if { [string trim $lefl] == "" } {
                warning "Error: No Lef Files specified for processing."
                warning "       Use parameter leflist or leffiles."
                return -1;
            }
            if { [string trim $deffile] == "" } {
                warning "Error: no Def File specified for processing."
                warning "       Use parameter deffile."
                return -1;
            }
            if { ([check_file $deffile] != 0)  } {
                warning "Error: Def File specified for processing does not exist or is not a file."
                return -1;
            }

            debug TRC:M "MSH Init"
            msh::init

            if { $remote_trc != "" } {
                set msh::PROGRAM $remote_trc
            }

            notice "Running $NAME with $::msh::OPT(nproc) processors."
            debug TRC:Q "qxfile: $tmpcmdfile"

            copy_qx_file $qxcmdfile $tmpcmdfile

            debug TRC:Q "qxfile: $qxfile"

            tile_debug { timer Mark load }

            notice "Processing input files."
            load_input_files $deffile $lefl

            ::eval [check_error "loading input files" -3]

            tile_debug { timer Mark tiling }
            init_tiles
            notice "Using $ntiles tiles for extraction."
            create_tiles $ntiles

            ::eval [check_error "creating tiles" -4]

            tile_debug { timer Mark process_tiles }
            process_tiles
            ::eval [check_error "processing tiles" -5]

            tile_debug { timer Mark assemble_tiles }
            assemble_tiles
            ::eval [check_error "assembling tiles" -6]

            tile_debug { timer Mark write_spef }
            write_output_files
            ::eval [check_error "writing output files" -7]

            msh::Analysis $tasks R
            msh::PrintAnalysis R
            timer Report
            notice "End of Run: [exec date]"

            return $rc;

        } else {
            warning "Unknown mode of operation $mode."
            return -9;
        }
    }
        

    public method init_tiles { } {

        if { [init_te] == 0 } {
            ## Let the user override it, if they want to
            if { [info exists ::env(NTILES)] } {
                set nt $::env(NTILES)
                if { [string is integer $nt] } {
                    if { ($nt > 0 ) } {
                        set ntiles $nt
                    }
                }
            } elseif { $ntiles <= 0 } {
                if { $die_area <= 0 } {
                    warning "Invalid chip area - has a chip been loaded?"
                    incr errorcount
                    return -1
                }

                te get_term_count -count= term_count

                debug TRC:T "Term Count: $term_count"

                set ntiles [expr int($term_count / $goalterms) + 1]
                debug TRC:T "Ntiles by tile size: $ntiles"

                set idealmin [expr 3*$::msh::OPT(nproc)]
                if { $idealmin < 32 } {
                    set idealmin 32 
                }

                if { $ntiles < $idealmin } {
                    set ntiles $idealmin
                }

                set tilesize [expr double($die_area) / double($ntiles) ]

                debug  TRC:T "Tile Size (die: $die_area): $tilesize"
                if { $tilesize < $idealmintilesize }  {
                    set ntiles [expr int($die_area / $idealmintilesize)]
                    debug TRC:T "Too many tiles for optimum performance (individual tiles would be too small)."
                    debug TRC:T "Adjusting number of tiles to: $ntiles."
                }

            }
            ## Final catch-all handlers
            if { $ntiles < 1 } {
                set ntiles 1
            }

            if { $ntiles > $maxtiles } {
                set ntiles $maxtiles
            }

            set tilesize [expr double($die_area) / double($ntiles) ]

            if { $tilesize < $finalmintilesize } {
                set tilenum [expr die_area / $finalmintilesize]
                warning "Tile size too small for accurate performance."
                warning "Maximum tile limit for this design: $tilenum"
                set ntiles $tilenum
            }

            if { $ntiles < 1 } {
                set ntiles 1
            }

            if { $ntiles > $maxtiles } {
                set ntiles $maxtiles
            }
            getdebmem init_tiles
        } else {
            warning "No chip has been loaded - use load_input_files first!"
        }

    }

    public method create_tiles { {numtiles -1} } {
        
        if { [init_te] == 0 } { 
            if { $numtiles < 0 } {
                if { [string is integer $ntiles] } {
                    set numtiles $ntiles
                }
            }

            if { (![string is integer $numtiles] ) } {
                warning "Invalid number of tiles: $numtiles" 
                incr errorcount
                return -1
            }

            if { $numtiles <= 0 } {
                warning "Number of tiles must be > 0 "
                incr errorcount
                return -1
            }

            if { $demandtiling } {
                te set_demand_tile -level 1
            }

            te set_corner_count -count 1

            te make_regular -nproc $numtiles
            te snap_tiles 
            tile_debug { te print_tiles }
            te make_tile_blocks
            te get_tile_block_names -string= blocks

            getdebmem create_tiles
            debug TRC:T $blocks

        } else {
            warning "No chip has been loaded - use load_input_files first!"
        }
    }


    public method load_input_files { {defin ""} {lefin ""} } {

        if { $defin == "" } {
            set defin $deffile
        }

        if { $lefin == "" } {
            set $lefin $lefl
        }

        set filebase [file rootname $defin]
        if { $outfile == "" } {
            set outfile "${filebase}.spef"
        }

        if { ([file exists $filebase.db]) && ($readdb == 1) } {
            notice "Reading database: $filebase.db"
            db read -file $filebase.db
        } else {

            notice "Reading Lef Files"
            debug TRC:L "Lef files: $lefin"
            db load_lef -list_file $lefin

            notice "Reading Def file $defin"
            db load_def -file $defin
            db order_wires 

            if { $writedb == 1 } {
                notice "Writing database: $filebase.db"
                db write -file $filebase.db
            }
            getdebmem load_input_files
        }
        set errorcount 0
    }

    public method write_output_files { {of ""} } {

        if { $of == "" } {
            set of $outfile 
        }

        if { $asyncwrite == 0 } {
            init_ext
            notice "Writing output file ${of}.gz"
            ext write_spef -gz -preserve_caps -file "${of}"
            getdebmem write_output_files
        }
    }

    public method assemble_tiles { } {

        notice "Integrating Tiles"
        te assemble -wrap -relative_c_threshold $qx_relative_c_threshold

        set assembled 1
        getdebmem assemble_tiles
    }

    public method report_total_cap { file {ref ""}} {

        init_ext
        if { $assembled == 1 } {
            if { $ref == "" } {
                ext report_total_cap -file $file
            } else {
                ext report_total_cap -file $file -ref $ref
            }
            getdebmem report_total_cap
        }
    }

    public method report_total_cc { file {ref ""}} {

        init_ext
        if { $assembled == 1 } {
            if { $ref == "" } {
                ext report_total_cc -file $file
            } else {
                ext report_total_cc -file $file -ref $ref
            }
            getdebmem report_total_cc
        }
    }



    public method process_tiles { } {

        getdebmem s_process_tiles
        Publish chip.tech
        Publish chip.libs
        foreach b $blocks {
            Publish $b.block
        }
                                                                                    
        foreach b $blocks {
            Subscribe  -dbargs "-parasitics" -async $b.res.block
        }


        set qxfile [file join $curdir $tmpcmdfile]

        # invoke as: 
        # rnode $b $datadir $qxbin $qxargs $qxfile $qxout $qxlog

        set rcmd ""
        foreach arg [list $datadir [list $qxbin] [list $qxargs] $qxfile $qxout $qxlog] {
            debug TRC:R "arg: $arg"
            if { [string trim $arg] == "" } {
                append rcmd " :none:"
            } else {
                append rcmd " $arg"
            }
        }

        set debcmd [list $debug $debug_mem]
        append rcmd " [list $debcmd]"
        debug TRC:R "rcmd: $rcmd"
         
        set tasks ""
        foreach b $blocks {
            spawn t$b "
               ::apps::turborc::rx $b $rcmd
            "
            lappend tasks t$b
        }

        if { $asyncwrite != 0 } {
            init_ext
            after 1000;
            update;
            notice "Writing output file ${outfile}.gz"
            te write_spef_init -file $outfile
        }

        monitor $tasks {
            done {
                set itile [string trimleft $task(task) "tT"]

                notice "Task information:"

                notice "T$itile: DONE & waiting for result"
                getdebmem "T${itile}_remote"

                if { [string trim $task(error)] == "" } {

                    ::debug TRC:R "Waiting for tile data!"
                    # -- wait for the data set to arrive. 
                    Wait T$itile.res.block

                    # -- assemble the tiles
                    notice "T$itile: assembling"

                    te assemble -tile $itile
                    notice "T$itile: finished"
                    getdebmem "T${itile}_finished"
                } else {

                    warning [exec date]
                    warning "Error evaluating task T$itile."
                    warning "Error message: $task(error)"

                    warning "Error encountered while processing tiles - cannot continue!"
                    incr errorcount
                    timer Report
                    exit -5
                }
            }
        }
        getdebmem e_process_tiles
    }


    public proc rnode { block dir qxcmd qxargs qxfile qxout qxlog deblist } {
        ## NO GLOBAL VARIABLES - everything needs to be passed as parameters!!

        parray ::env
        set debug [lindex $deblist 0]
        set debug_mem [lindex $deblist 1]

        if { $debug > 0 } { timer Mark Start }
        if { $debug_mem > 0} { db rlog -tag "TRC_Remote" }

        msh::init

        if { $debug > 0 } { timer Mark Subscribe }
        if { $debug_mem > 0} { db rlog -tag "TRC_Remote" }
        Subscribe chip.tech
        Subscribe chip.libs
        Subscribe $block.block

                                                                                    
        set errorcount 0

        switch $dir {
            tmp {
                # use /tmp
                set dir /tmp/zroute.[pid]
                set clean 1
            }
            ram {
                # use ramdisk
                set dir /dev/shm/zroute.[pid]
                set clean 1
            }
            default {
                # use current directory
                ## Attempt to create directory

                set dir [file join $dir $block]
                set clean 0
            }
        }
        
        set ::errorCode NONE
        file mkdir $dir
        if { $::errorCode != "NONE" } {
            ## try using /tmp
            set d2 [file join "/tmp" "zroute.[pid]"]
            puts "Unable to create directory $dir - using $d2"
            set dir $d2

            set ::errorCode NONE
            file mkdir $dir
            if { $::errorCode != "NONE" } {
                puts "Unable to create $dir - using /tmp"
                set dir "/tmp"
                if { $::errorCode != "NONE" } {
                    puts "Cannot create working directory - giving up."
                    return -2
                }
            }
        }

        cd $dir

        if { $debug > 0 } { timer Mark Write_Def }
        if { $debug_mem > 0} { db rlog -tag "TRC_Remote" }

        db save_def -use_ids -file $block.def


        ## invoke as: exec $qxcmd $qxargs -cmd $qxfile $block.def
        ##
        set excmd "exec "
        set targ [string trim $qxcmd]
        if { $targ != ":none:" && $targ != "" } {
            append excmd " [concat $targ]"
        }

        if { [string trim $qxfile] != "" } {
            append excmd " -cmd $qxfile"
        }

        set targ [string trim $qxargs]
        if { $targ != ":none:" && $targ != "" } {
            append excmd " [concat $targ]"
        }

        append excmd " $block.def"

        if { $qxlog == ":none:" } {
            set qxlog "qx.log"
        }

        if { $qxout == ":none:"  } {
            set qxout "qx"
        }

        set retry 0
        set flag 1
        set output ""
        if { $debug > 0 } { timer Mark Exec_QX }
        if { $debug_mem > 0} { db rlog -tag "TRC_Remote" }
        while { $flag } {
            set rc [catch {::eval $excmd} output]
            if { $rc != 0 } {
                set qxl [read_file $qxlog]
                if { [check_license $qxl] == 0 } {
                    dir_cleanup $dir $clean
                    puts "$qxcmd: $output"
                    incr errorcount
                    set flag 0
                } else {
                    ## was a no-license error
                    if { $retry < $MAXRETRY } {
                        incr retry
                        puts "Retry #$retry - sleeping for 5 seconds!"
                        after 5000
                    } else {
                        set flag 0
                        incr errorcount
                    }
                }
            } else {
                ## Ok, exit
                puts "No error - continue: $rc!"
                set flag 0
            }
        }

        if { [file exists $qxlog ] } {
            puts [ exec tail -30 $qxlog ]
        } else {
            error "$output"
        }

        if { ($errorcount == 0 ) && ([file exists $qxout.spef.gz]) } {
            if { $debug > 0 } { timer Mark Read_Spef }
            if { $debug_mem > 0} { db rlog -tag "TRC_Remote" }
            Ext ext 
            ext read_spef -use_ids -file $qxout.spef.gz
            Publish  -dbargs "-parasitics" -wait $block.res.block
            if { $debug > 0 } { timer Mark Publish_Result }
            if { $debug_mem > 0} { db rlog -tag "TRC_Remote" }
            dir_cleanup $dir $clean
            if { $debug_mem > 0} { db rlog -tag "TRC_Remote" }
            if { $debug > 0 } { timer Report_puts }
            return 0
        } else {
            error "$qxcmd did not complete successfully ($errorcount errors)!"
            dir_cleanup $dir $clean
            return -1
        }
    }
}
