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

# sys.tcl
## System module for Milos
## Author: Mattias Hembruch

package require Itcl

package require ade::milos::vob 0.1
package provide ade::milos::sys 0.1

itcl::class SYS {

    inherit VOB

    ## protected vars
    ## variables with string values

    protected method constructor { parent } { 
        VOB::constructor $parent
        set NAME "SYS"
        set SELFNAME "SYS$this"
        set_state milos_rlog_on 0
        ## ONLY for public vars
    }

    ## used to "close the hole"
    protected method info { }

    public method help { {cmd ""} {param ""} {long "" } } {
        if { $cmd != "" && $cmd != "-long" } {
            cmd_help SYS $cmd $param $long
        } else {
            mod_help SYS $cmd
        }
    }

    VOB::add_args SYS_ECHO {
        {on bool 1}
    }

    VOB::add_help SYS_ECHO {
        {on "Turn command echo on (1) or off (0)"}
    }
    public method echo { args } {
        parse_args
        return [parent forall "set_echo [get_arg on]"]
    }

    VOB::add_args SYS_READ_FILE {
        {file ifile 1}
    }

    VOB::add_help SYS_READ_FILE {
        {file "read this file and return its contents"}
    }
    public method read_file { args } {
        parse_args
        return [VOB::read_file [get_arg file]]
    }

    #public method new_inst { } {
        #return [::apps::milos::newinst]
    #}

    #public method select_inst {idx} {
        #return [::apps::milos::setinst $idx]
    #}

    public method eval { script } {
        return [parent eval $script]
    }

    public method getCVS { pattern } {
        return [parent getCVS $pattern]
    }

    VOB::add_args SYS_CONFIGTEMPLATE {
        {configFile ofile 0 ""}
    }
    VOB::add_help SYS_CONFIGTEMPLATE {
        {configFile "filename to use for creating a Config File Template"}
    }
    public method configTemplate { configFile } {
        set fp ""
        set fp [open $configFile "w"]
        if { $fp eq "" } {
            error "Could not open output file $configFile" "" -1
        }

        puts $fp "## Milos Design Input File"
        puts $fp "## All values assigned to LIBS and DESIGN arrays should be filenames"
        puts $fp "## as they are checked for existance. ANY array name can be used."
        puts $fp "## "
        puts $fp "## You can also put any Tcl code you want here (including milos code)"
        puts $fp "##"
        puts $fp "## Libs section - put all libraries used by the design here"
        puts $fp "set LIBS(userlibs1) \[list /u/user/libs/a.lib \\"
        puts $fp "      /u/usr/libs/b.lib]"
        puts $fp "set LIBS(userlibs2) \[list /u/user/libs/a.lib \\"
        puts $fp "      /u/usr/libs/b.lib]"
        puts $fp "set LIBS(userfile1) a.lef"
        puts $fp ""
        puts $fp "set DESIGN(userfile2) a.def"
        puts $fp "set DESIGN(sdc) a.sdc"
        puts $fp "set DESIGN(ext_model_file) extRules.a"
        puts $fp ""
        puts $fp "## Any entries in DIRS will be created as directories"
        puts $fp "## if they don't already exist!"
        puts $fp "set DIRS(dir1) /home/user/mydir1"
        puts $fp "set DIRS(dir2) /home/user/mydir2"
        puts $fp ""

        foreach mod {pdb tmg mme opt rpt rte } {
            puts $fp "\n\n"
            puts $fp "##Configuration for module $mod"
            config_defaults $mod $fp
            puts $fp "\n##END of Configuration for module $mod\n\n\n"
        }
        close $fp

    }

    VOB::add_args SYS_LOADCONFIG {
        {configFile ifile 1 ""}
    }
    VOB::add_help SYS_LOADCONFIG {
        {configFile "Milos Config File to load"}
    }
    public method loadConfig { configFile } {
        if { [file exists $configFile] == 0 } {
            error "Configuration file $configFile does not exist!" "" -1
        }

        call_milos "array unset LIBS"
        call_milos "array unset DESIGN"

        call_milos "source $configFile"

        set errcnt 0

        foreach {name files} [call_milos "array get LIBS"] {
            foreach arg $files {
                if { [file exists $arg] == 0 } {
                    notice "ERROR: File $arg of entry $name in LIBS does not exist!" "" -1
                    incr errcnt
                }
            }
        }

        foreach {name files} [call_milos "array get DESIGN"] {
            foreach arg $files {
                if { [file exists $arg] == 0 } {
                    notice "ERROR File $arg of entry $name in DESIGN does not exist!" "" -1
                    incr errcnt
                }
            }
        }

        foreach {name files} [call_milos "array get DIRS"] {
            foreach arg $files {
                if { [file isdirectory $arg] == 0 } {
                    notice "Directory $arg does not exist - creating it!"
                    file mkdir $arg
                }
            }
        }

        if { $errcnt > 0 } {
            error "There were errors loading the config file!"
        } else {
            notice "loadConfig successful!"
        }

        cmd_rlog
        return 0
    }

    VOB::add_args SYS_RLOG {
        {tag nstr 0 ""}
        {on bool 0}
    }
    VOB::add_help SYS_RLOG {
        {tag "Identification string to print in front of resource usage log."}
    }

    public method rlog { args } {
        parse_args

        set db [get_tm DB]
        if { [check_arg on] } {
            set nr [get_arg on]
            if { $nr == 1 && [state milos_rlog_on] == 0 } {
                ::eval $db rlog -tag START_RLOG
            }
            set_state milos_rlog_on [get_arg on]
        }
        ::eval $db rlog -tag [get_arg tag]
    }

    public method cputime { } {
        notice "CPU seconds used: [::eval sys::getCPUTime]"
    }

    public method runtime { } {
        return [parent showtime]
    }

    public method runtime_incr { } {
        return [parent runtime_incr_ext]
    }

    public method set_dup_file { {filename ""} {w_a "w"} } {
        ::zr.zlog set_dup_file $filename $w_a
    }

    public method show_gui { window } {
        [state ns]::igui show $window
        update
    }

    VOB::add_args SYS_SET_PSCRIPT {
        { script_name nstr 0 "" }
        { cmd nlist 0 "" }
        { dir_list nlist 1 }
        { append bool 0 0}
    }

    protected variable WAIT 0
    protected variable SCHED 1
    protected variable DONE 2
    

    protected variable pdlist
    protected variable pd_cnt 0
    protected variable launch_idx 0
    protected variable launch_cnt 0
    protected variable fin_cnt 0
    protected variable wait_cnt 0
    public method set_pscript { args } {
        parse_args


        ## build list of dir/script pairs

        if { [get_arg append] == 0 } {
            set pd_cnt 0
            array get pdlist {}
        }
        foreach de [get_arg dir_list] {
            foreach {dir scr arg} $de {
                if { $scr eq "" } {
                    set scr [get_arg script_name]
                    set cmd [get_arg cmd]
                }
                if { $scr ne "" } {
                    set pdlist($pd_cnt,scr) $scr
                    set pdlist($pd_cnt,cmd) ""
                } elseif { $cmd ne "" } {
                    set pdlist($pd_cnt,scr) ""
                    set pdlist($pd_cnt,cmd) $cmd
                } else {
                    error "Both -script_name and -cmd are empty!" 
                }
                ## common settings
                set pdlist($pd_cnt,dir) $dir
                set pdlist($pd_cnt,arg) $arg
                set pdlist($pd_cnt,status) $WAIT
            }
            incr pd_cnt
        }
    }

    public method show_pscript { } {
        for { set i 0 } { $i < $pd_cnt } {incr i } {
            if { $pdlist($i,scr) ne "" } {
                puts "J$i: Directory: $pdlist($i,dir), CMD: $pdlist($i,scr), ARGS: $pdlist($i,arg)"
            } else {
                puts "J$i: Directory: $pdlist($i,dir), CMD: $pdlist($i,cmd), ARGS: $pdlist($i,arg)"
            }
        }
    }

    VOB::add_args SYS_LAUNCH_PSCRIPT {
        {timeout num 0 100}
    }

    public method launch_pscript { args } {
        parse_args

        show_pscript

        pscript_dircheck

        set_state sys_sched_rmts ""
        set_state sys_rmts ""


        if { $pd_cnt eq 0 } {
            error "No remote tasks have been defined"
        }

        set wait_cnt $pd_cnt

        get_tm MSH
        set max_rmt $::msh::OPT(nproc)
        if { ($max_rmt == 0) || ($max_rmt > $pd_cnt) } {
            ## if unset, or woudl start too many jobs
            set max_rmt $pd_cnt
        }


        for { set i 0 } { $i < $max_rmt } {incr i } {
            new_rmt R$i 1 athext
            set rmt [state ns]::R$i
            $rmt configure -mycallback [::itcl::code $this sysAddRmt $rmt]
            $rmt start
        }

        notice "Waiting for remotes to start"
        set cnt 0
        while { [state sys_rmts] eq "" } {
            set ::sysmutex 0
            after 1000 { set ::sysmutex 1}
            vwait ::sysmutex
            incr cnt
            notice "Waiting.. $cnt"
            if { $cnt == [get_arg timeout] } {
                for { set i 0 } { $i < $pd_cnt } { incr i } {
                    set rmt [state ns]::R$i
                    ## turn off callbacks
                    $rmt configure -mycallback ""
                }
                error "No remotes started in [get_arg timeout] seconds"
            }
        }
        job_stats

        set cnt 0
        while { ([state sys_rmts] ne "") || ([state sys_sched_rmts] ne "") } {
            update
            after 100
            schedule_rmts 1
            incr cnt
            if { [expr $cnt % 50] == 0 } {
                job_stats
            }
        }
        notice "Remote jobs finished!"
        job_stats
    }

    protected method pscript_dircheck { } {
        for { set i 0 } { $i < $pd_cnt } {incr i } {
            set script [file join $pdlist($i,dir) $pdlist($i,scr)]
            if { [file exists $script] == 0 } {
                if { $pdlist($i,cmd) eq "" } {
                    error "File/Script $script does not exist and -cmd empty!"
                }
            }
        }
    }


    protected method sysAddRmt { rmt } {
        lappend_state sys_rmts $rmt
        #notice "ADD RMT $rmt == [state sys_rmts]"
    }

    protected method remove_rmt { rmt } {
        #notice "RR: $rmt"
        set rl [state sys_rmts]
        set idx [lsearch $rl $rmt]
        if { $idx >= 0 } {
            set rl [lreplace $rl $idx $idx]
            set_state sys_rmts $rl
        }
    }

    protected method remove_sched_rmt { rmt } {
        #notice "RS: $rmt"
        set rl [state sys_sched_rmts]
        set idx [lsearch $rl $rmt]
        if { $idx >= 0 } {
            set rl [lreplace $rl $idx $idx]
            set_state sys_sched_rmts $rl
            incr launch_cnt -1
        }
    }

    protected method schedule_rmts { {cnt 0} } {
        set mycnt 0
        foreach rmt [state sys_rmts] {
            if { [$rmt status] == 0 } {
                scheduleTask $rmt
                incr mycnt
            }
            if { ($cnt > 0) && ($cnt == $mycnt) } {
                break;
            }
        }
    }

    protected method find_jobs_with_status { status } {
        set jl ""
        for { set i 0 } { $i < $pd_cnt } {incr i } {
            #puts "FJ: $i, $pdlist($i,status)"
            if { $pdlist($i,status) == $status } {
                append jl "J$i "
            }
        }
        return $jl
    }

    protected method job_stats { } {
        if { $fin_cnt > 0 } {
            notice "Finished Jobs ($fin_cnt): [find_jobs_with_status $DONE]"
        }
        if { $launch_cnt > 0 } {
            notice "Scheduled Jobs ($launch_cnt): [find_jobs_with_status $SCHED]"
        }
        if { $wait_cnt > 0 } {
            notice "Waiting Jobs ($wait_cnt): [find_jobs_with_status $WAIT]"
        }
        notice ""
    }


    protected method scheduleTask { rmt } {
        set rl [split $rmt :]
        set remote [lindex $rl end]
        ## no data to retrieve from previous..
        set ojob [$rmt get_name]
        #puts "OLD Job: $ojob"
        if { $ojob ne "" } {
            ## mark previous job finished
            set ol [string range $ojob 1 end]
            set pdlist($ol,status) $DONE
            incr fin_cnt
            remove_sched_rmt $rmt
        }
        remove_rmt $rmt

        set nidx $launch_idx
        if { $nidx >= $pd_cnt } {
            #notice "No more jobs to schedule!"
            return
        }
        incr launch_idx
        incr launch_cnt
        incr wait_cnt -1

        lappend_state sys_sched_rmts $rmt
        $rmt set_name "J$nidx"
        $rmt mdebug

        set pdlist($nidx,status) $SCHED

        $rmt send_command "puts \"Start of Job J$nidx!\" "
        $rmt send_command {set ::startdir [pwd]}
        $rmt send_command "cd $pdlist($nidx,dir)"
        set alen [llength $pdlist($nidx,arg)]
        if { $alen > 0 } {
            $rmt send_command "set ::argc $alen"
            $rmt send_command {set ::argv [list "" ] }
            foreach arg $pdlist($nidx,arg) {
                $rmt send_command "lappend ::argv $arg"
            }
        }
        if { $pdlist($nidx,scr) ne "" } {
            $rmt send_command "set ::argv0 $pdlist($nidx,scr)"
            $rmt send_command "source $pdlist($nidx,scr)"
        } else {
            ## assume cmd is valid
            puts "RMT CMD LIST: $pdlist($nidx,cmd)"
            foreach cmd $pdlist($nidx,cmd) {
                set cmd "$rmt send_command $cmd"
                puts "RMT CMD: $cmd"
                ::eval $cmd
            }
        }

        $rmt send_command "cd [pwd]"
        $rmt send_command "puts \"End of Job J$nidx!\\n\\n\""

    }

}
