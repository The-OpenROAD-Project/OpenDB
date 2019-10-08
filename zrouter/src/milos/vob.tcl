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

## vob.tcl: Virtual OBject 
## Author: Mattias Hembruch
## Purpose: define an abstract object for implementing functional modules
##          in Milos - all Milos modules inherit from this object

package require Itcl

package provide ade::milos::vob 0.1


itcl::class VOB {


    protected method cget { } 
    protected method isa { }
    protected method configure { }

    protected common ECO_LIST {milos pt psta quartz}
    protected common SPEF_LIST {milos prime_time psta quartz }
    protected common RESTOREABLE_ENGINES { pt } 

    protected common ENGINE_LIST {milos psta pt quartz}

    protected common BS_P {\\}
    protected common BS_SHP {\\/}
    protected common BS_BKP {\\\[}
    protected common BS_NLP {\\[^]}
    protected common BS_DLR {\$}

    ## default executable names for plugins
    protected common START_NAME 
    set START_NAME(milos) ""
    set START_NAME(psta) "ca psta <"
    set START_NAME(pt) "ca pt_shell -f"
    set START_NAME(quartz) "ca quartz_time -f"


    protected common GUINS "::ugui"

    ## Private variables
    protected variable PARENT "";   ## Parent of this instance

    protected variable NAME "VOB";    ## Instance name

    protected variable debug 0
    protected variable CHILDREN { };  ## Children of this instance


    protected variable ECHO 0; ## should we echo?
    protected common watch_states;
    set watch_states(0) ""

    protected common STATUS_VARS;

    array set STATUS_VARS {
        db_loaded 1
    }

    protected common VTYPE;
    protected common MYLICENSE "milos.debug";


    protected common starttime -1;
    protected variable ltime -1;
    protected variable ltime_ext -1;

    protected common plugin_objs;
    protected common plugin_time;

    array set VTYPE {
         str "ascii string" 
         nstr "non-empty ascii string" 
         slist "string list"
         nslist "non-empty string list"
         file "filename" 
         flist "file list"
         ifile "input filename" 
         ofile "output filename" 
         iflist "input file list" 
         oflist "output file list" 
         bool "boolean" 
         num "integer or floating point value" 
         pnum "POSITIVE integer or floating point value" 
    }

    protected common GUI_PARMS;
    protected common PARMS;
    protected common ENUM;
    #array set PARMS { 0 "test" };
    ## PARMS format (CMD_NAME in caps):
    ##  CMD_NAME { name type required default }
    ##
    ## name: parameter name
    ##
    ## types: 
    ##        bool - one or zero (specified implies 1)
    ##        num - a number
    ##        pnum - a positive number (>= 0)
    ##        str - string, can be empty (default)
    ##        nstr - non-empty string, cannot be empty
    ##        slist - string list
    ##        nslist - non-empty string list
    ##        file - filename
    ##        i/ofile - input/output filename - input file 
    ##        flist - a list of files
    ##        i/oflist - input/output flist - iflist 
    ##        ** all filenames/lists must be non-empty strings
    ##        enum_x - enumerated type, limited values - x indicates which enum
    ##                 definition to use
    ##
    ##        *** HIDDEN PARAMETERS ***
    ##        same type as above list, but type starts with 'h'
    ##        Hidden Parameters will be accepted, but will not be printed
    ##        by help
    ##        
    ##
    ##
    ## conditions:
    ##           0 - parm is not required (can be defaulted)
    ##           1 - parm must be specified
    ##           AND_parm_name - if specified, also requires parm_name
    ##           OR_parm_name - this or parm_name is required
    ##           NOT_parm_name - cannot be specified together
    ##
    ## default: default value - cannot have one if required, so ignored


    protected common P_ALIAS;
    ## P_ALIAS { cmd_name parm_name alias }
    ## alias: name of equivalent backend parameter (for build_command)
    ##
    
    protected common P_HELP;
    ## P_HELP { cmd_name parm_name help_string }
    ## help_string - used for help on a given command

    protected common P_FMT "%-30s %s"


    ## ui adaptor
    protected common ui "";
    private common UICOL { name status space r0 r1 r2 r3 r4 space r5 };

    ## Private implementations
    protected method constructor { {parent "" } {st -1} } {
        set SELFNAME "VOB$this"
        set PARENT $parent
        if { $parent != "" } {
            $parent add_child $this
            return
        }
        set ::eco_s_pt ""
        set ::MILOS_DOLLAR_SIGN "$"

        foreach eng $ENGINE_LIST {
            if { $eng eq "milos" } { continue }
            set_state ${eng}_min_licenses 0
            set_state ${eng}_max_licenses 0
        }


        if { $starttime == -1} { 
            if { ($st == -1) || ($st eq "") } {
                set starttime [clock seconds]
            } else {
                set starttime $st
            }
            #puts "Set starttime to: $starttime"
        }

        if { $ui == "" } {
            #set ui [UIAdapter_milos #auto -stimeout 0.2 -application "Milos Test"]
            #TODO
            #set ui [UIAdapter_milos #auto -stimeout 0.2]
        }
    }


    protected method pause { } {
        if { [state mme_gui] == 1 } {
            set s [LSemaphore $this.sema]
            $s Wait 200
            $s delete
        }
    }

    protected method this { } {
        return $this
    }
    ## Public Interface


    protected method call_milos { args } {
        interp eval $::apps::milos::SLAVE [join $args]
    }

    protected method call_node { node cmd } {
        set nn $node
        notice "CALL_NODE $node $cmd"
        if { $nn != "" } { 
            set ct [lindex [split $cmd " "] 0]
            set cl [expr [string length $ct] + 1 ]
            set cr [string range $cmd $cl end]
            #puts "node: $nn, ct: $ct, cr: $cr"
		$nn send_command acmd "$ct $cr"
	    return
            #return [$nn send_command $ct $cr]
        } else {
            ## local
            return [call_milos $cmd]
        }
    }

    protected method unalias_node { alias } {
        foreach {name val} [array get [state ns]::RSLAVES] {
            if { $val eq $alias } {
                return $name
            }
        }
        return ""
    }

    protected method append_multi_cmd { stage node args } {
        set cmd [string trim [join $args]]
        if { $cmd == "reset" } {
            switch $stage {
                1 { set_state precmdq {} }
                2 { set_state midcmdq {} }
                3 { set_state postcmdq {} }
            }
        } else {
            switch $stage {
                1 { set opre [state precmdq]
                    lappend opre $node [join $args]
                    set_state precmdq $opre
                }
                2 { set omid [state midcmdq]
                    lappend omid $node [join $args]
                    set_state midcmdq $omid
                }
                3 { set opost [state postcmdq]
                    lappend opost $node [join $args]
                    set_state postcmdq $opost
                }
                default {
                    error "Stage $stage not understood for append_multi_cmd!" "" -1
                }
            }
        }
    }

    protected method check_timing { } {
        if { [state multi] eq "ACTIVE" } {
            ## ALWAYS means full update
            set_state eco_flag 1
            call_milos "mme get_db_timing"
        }
    }

    protected method get_merged_slack { obj slacks } {
        return [get_local_slack $obj $slacks 0]
    }

            
    protected method get_local_slack { obj slacks {local 1} } {
        set tmg [get_tm Tmg]
        set ::myws ""
        set ::myviol ""
        set ::mytns ""
        foreach mm $slacks {
            if { $local == 1 } {
                $tmg get_worst_slack -local -delay_type $mm -slack= ::myws -viol= ::myviol -term ::myterm
                $tmg get_total_negative_slack -local -delay_type $mm -tns= ::mytns
            } else {
                $tmg get_worst_slack -delay_type $mm -slack= ::myws -viol= ::myviol -term ::myterm
                $tmg get_total_negative_slack -delay_type $mm -tns= ::mytns
            }
            #$obj configure -worst_${mm}_slack $::myws -num_${mm}_violations $::myviol -total_${mm}_slack $::mytns -worst_${mm}_term [escape_1 $::myterm]
            $obj configure -worst_${mm}_slack $::myws -num_${mm}_violations $::myviol -total_${mm}_slack $::mytns -worst_${mm}_term $::myterm
        }
    }




    protected method get_worst_timing { {iterm_id 0} {bterm_id 0} } {

        set tmg [get_tm Tmg]
        if { $iterm_id > 0 } {
            # changed 'get_worst_timing_data' to 'get_sta_timing' - payam
            set cmd "$tmg get_sta_timing -iterm_id $iterm_id -slack_rise_min= ::s0 -slack_rise_max= ::s1 -slack_fall_min= ::s2 -slack_fall_max= ::s3 -slew_rise_min= ::s4 -slew_rise_max= ::s5 -slew_fall_min= ::s6 -slew_fall_max= ::s7"
        } else {
            # changed 'get_worst_timing_data' to 'get_sta_timing' - payam
            set cmd "$tmg get_sta_timing -bterm_id $bterm_id -slack_rise_min= ::s0 -slack_rise_max= ::s1 -slack_fall_min= ::s2 -slack_fall_max= ::s3 -slew_rise_min= ::s4 -slew_rise_max= ::s5 -slew_fall_min= ::s6 -slew_fall_max= ::s7"
        }

        ::eval $cmd

        return [list $::s0 $::s1 $::s2 $::s3 $::s4 $::s5 $::s6 $::s7]
    }

    protected method sync_nodes { {delay 0.1} {msg "" }} {
        set flag 0
        set node ""
        foreach node [state rmt_list] {
            $node addwait
        }
        if { $node != "" } {
            $node gwait $msg
        }
    }
    

    ## Gui-related commands

    protected method send_node_status { field value } {
        foreach sobj [state scenario_list] {
            gui_send [$sobj cget -gname] $field $value
        }
    }

    protected method up_opt { type count undo msg } {
        puts "GUI, UP_OPT: $type $count, $undo, $msg"
        set mno [state main_num_${type}]
        incr mno $count
        set_state main_num_${type} $mno

        set mnu [state main_num_${type}_undo]
        incr mnu $undo
        set_state main_num_${type}_undo $mnu

        gui_send main $type "$mno / $mnu"
        gui_send main msg $msg

        return 0
    }

    protected method update_opt_data { } {
        ## catch mme setup
        if { [state multi] eq "ACTIVE" } { return }

        set_state ep_slacks_updated 1
        set cb [state gui_ep_callback]
        if { $cb != "" } { 
            ::eval $cb
        }
    }
    
    protected method init_gui { } {
        UPOBJ start_gui
        #set gui [NGUI_PROXY #auto]
        #$gui start_gui
    }

    # -- create an external gui and wait for it to connect
    # -- it will return immediately, if there is already a gui connected
    protected method gui_start {} {
        if { $::msh::OPT(dryrun) == 0 } {
            GUI::Start
            GUI::Wait
        }
    }
    # -- send a gui event
    protected method gui_send { node attr val } {
        if { [state mme_gui] == 1 } {
            if { $::msh::OPT(dryrun) == 0 } {
                if { [info command gui] != "gui" } { return }
                $ui NewState ${node}.${attr} $val
            }
        } elseif { [state mme_gui] == 2 } {
            debug GUI:E "VOB GUI EVENT: $node $attr $val"
            UPOBJ send_gui_event <<SET_FIELD>> $node $attr [list $val]
            #set node [string tolower $node]
            #set ${GUINS}::FIELDS(${node}.$attr) $val
            #update
        }
    }

    # -- send the new gui stylesheet
    protected method gui_css {css} {
        if { [state mme_gui] == 1 } {
            if { $::msh::OPT(dryrun) == 0 } {
                if { [info command gui] != "gui" } { return }
                $ui StyleSheet $css
            }
        }
    }

    # -- send the short status tag and the long state info
    protected method gui_status {node status info} {
        if { [state mme_gui] > 0 } {
            if { $::msh::OPT(dryrun) == 0 } {
                gui_send $node status $status
                gui_send $node action $info
            }
        }
    }

    protected proc format_time { difftime } {
        set hours [expr {$difftime / 3600} ]
        set ndiff [expr {$difftime - $hours*3600}]

        set mins [expr {$ndiff / 60} ]
        set secs [expr {$ndiff - $mins*60}]

        return [format "%02d:%02d:%02d" $hours $mins $secs]
    }

    protected method showtime { } {
        set str "Elapsed time: [runtime]"
        return $str
    }

    protected proc runtime { } {
        set ctime [clock seconds]
        set t1 [format_time [expr {$ctime - $starttime} ]]
        return $t1
    }

    protected method runtime_incr { } {
        set ctime [clock seconds]
        set t1 [format_time [expr {$ctime - $starttime} ]]
        if { $ltime != -1 } {
            set t2 [format_time [expr {$ctime - $ltime} ]]
            set ltime $ctime
            return "$t1 (incr $t2)"
        }
        set ltime $ctime
        return "$t1"
    }

    protected method runtime_incr_ext { } {
        set ctime [clock seconds]
        set t1 [format_time [expr {$ctime - $starttime} ]]
        if { $ltime_ext != -1 } {
            set t2 [format_time [expr {$ctime - $ltime_ext} ]]
            set ltime_ext $ctime
            return "$t1 (incr $t2)"
        }
        set ltime_ext $ctime
        return "$t1"
    }
    protected method runtime_incr_ext_2 { } {
        set ctime [clock seconds]
        set tt [expr {$ctime - $starttime} ]
        return "$tt"
    }

    ######################################################################
    ## Argument/help related commands
    ######################################################################

    public proc create_enum { name args } {
        #puts "Create ENUM $name: $args"
        set ENUM($name) $args
        set VTYPE(enum_$name) "Option list: $args"
    }

    public proc get_enum { name } {
        return $ENUM($name)
    }

    public proc add_args { cmd args } {
        set PARMS($cmd) $args
    }

    public proc add_alias { cmd args } {
        set P_ALIAS($cmd) $args
    }

    public proc add_help { cmd args } {
        set P_HELP($cmd) $args
    }

    protected method status { var } {
        foreach {arg val } [array get STATUS_VARS] {
            if { $arg eq $var } {
                return [state $arg]
            }
        }
        return ""
    }

    protected method settings { } {
        upvar 1 OPT MYOPT
        set names [lsort -increasing [array names MYOPT]]
        set stt ""
        foreach aname $names {
            append stt "$aname: [lindex [array get MYOPT $aname] 1]\n"
        }
            
        if { [string length $stt] > 0 } {
            set stt "Active parameters:\n$stt"
        } else {
            set stt "No active parameters."
        }
    }

    protected method config_defaults { mod fp } {

        set cidx [string toupper "${mod}_*"]

        set ml [expr [string length $mod] + 1]
        set cmd ""

        foreach {idx val} [array get PARMS $cidx] {
            set cmd [string tolower [string range $idx $ml end]]
            
            set argl $val
            set mcount 0
            foreach arg [join $argl] {
                foreach { name type required default } $arg { break;}

                ## skip hidden values
                if { [string index $type 0] == "h" } { 
                    continue 
                }
                if { $required != 0 } {
                    continue
                }
                ## get help...
                set hlp [parse_desc $mod $cmd $name]
                ## add comments to any newlines
                puts $fp ""
                set idx [string first "\n" $hlp]
                while { $idx >= 0 } {
                    incr idx -1
                    puts $fp "##[string range $hlp 0 $idx]"
                    incr idx 2
                    set hlp "[string range $hlp $idx end]"
                    set idx [string first "\n" $hlp]
                }
                puts $fp "## $hlp"
                if { $default eq "" } {
                    set default [default_value $type]
                }
                puts $fp "#$mod set_default $name $default $cmd"
            }
        }
    }

    public method show_defaults { {cmd "*"} {param ""} } {
        set cl [info class]

        ## strip leading ::
        set mod [string trim [join [split $cl :]]]
        
        set cidx "${mod}_[string toupper $cmd]"

        set ml [expr [string length $mod] + 1]
        set cmd ""
        set lcmd ""

        foreach {idx val} [array get PARMS $cidx] {
            set cmd [string range $idx $ml end]
            
            set argl $val
            set mcount 0
            foreach arg [join $argl] {
                foreach { name type required default } $arg { break;}

                if { $required != 0 } {
                    continue
                }
                if { !($cmd eq $lcmd) && $mcount == 0 } {
                    notice "\nCommand: [string tolower $cmd]"
                    set lcmd $cmd
                    set mcount 1
                }
                if { ($param ne "") && ($param ne $name) } {
                    continue
                }
                notice "    -$name: $default"
            }
        }
    }

    public method get_default { cmd param } {
        set cl [info class]

        if { ([string trim $cmd] eq "") || ([string trim $param] eq "") } {
            return
        }

        ## strip leading ::
        set mod [string trim [join [split $cl :]]]
        
        set cidx "${mod}_[string toupper $cmd]"

        set ml [expr [string length $mod] + 1]
        set cmd ""
        set lcmd ""

        foreach {idx val} [array get PARMS $cidx] {
            set cmd [string range $idx $ml end]
            
            set argl $val
            set mcount 0
            foreach arg [join $argl] {
                foreach { name type required default } $arg { break;}

                if { $required != 0 } {
                    continue
                }
                if { !($cmd eq $lcmd) && $mcount == 0 } {
                    set lcmd $cmd
                    set mcount 1
                }
                if { ($param ne "") && ($param ne $name) } {
                    continue
                }
                return $default
            }
        }
    }

    public method set_default { pname value {cmd "*"} } {
        set cl [info class]

        ## strip leading ::
        set mod [string trim [join [split $cl :]]]
        
        set cidx "${mod}_[string toupper $cmd]"

        set pcount 0
        set mcount 0
        foreach {idx val} [array get PARMS $cidx] {
            incr mcount
            set argl $val
            set nargl { }
            set li 0
            foreach arg [join $argl] {
                debug VOB:P "arg: $arg"
                foreach { name type required default } $arg { break;}

                if { !($name eq $pname) } {
                    lappend nargl $arg
                    continue
                }

                set ll [llength $arg]

                if { $required != 0 } {
                    error "Can only set defaults for optional values!" "" -1
                }

                if { [arg_check $value $type] != 0 } {
                    error "Value `$value' incompatible with type $type." "" -1
                }
                set nl [list $name $type $required [arg_format $value $type] ]

                lappend nargl $nl

                incr li
            }
            if { $li > 0 } {
                set nargl "{\n $nargl \n}"
                set PARMS($idx) $nargl
                incr pcount
            }
        }
        if { $pcount == 0 } {
            warning "No paramaters matching $pname were found for module $mod!"
            return 1
        }

        if { $mcount == 0 } {
            warning "No commands matching pattern $cmd were found for module $mod!"
            return 1
        }
    }

    protected method parse_req { required name } {

        set req [string toupper $required]
        if { $required == 1 } {
            set help "yes\n"
        } elseif { [string range $req 0 2] eq "OR_" } {
            set help "either -$name or -[string range $required 3 end] is required\n"
        } elseif { [string range $req 0 3] eq "AND_" } {
            set help "if specified, -[string range $required 4 end] is also required\n"
        } elseif { [string range $req 0 3] eq "NOT_" } {
            set help "cannot be specified together with -[string range $required 4 end]\n"
        } else {
            set help "no\n"
        }
        return $help
    }

    protected method parse_desc { mod cmd name } {
        set cidx "[string toupper $mod]_[string toupper $cmd]"

        if { ![info exists P_HELP($cidx)] } {
            return ""
        }

        set dscl $P_HELP($cidx)
        foreach arg [join $dscl] {
            foreach {pname desc} $arg { break; }
            if { $pname eq $name } {
                return $desc
            }
        }
        return ""
    }



    protected method mod_help { mod {fmt ""} } {
        set long 0
        set sum 0

        if { $fmt eq "" } {
            set sum 1
        }
        if { $fmt eq "-long" } {
            set long 1
        }

        set mod2 [public_commands $mod]
        if { $sum == 1 } {
            notice "\nCommand summary for $mod:"
            foreach cmd $mod2 {
                set str [format $P_FMT $cmd [parse_desc $mod $cmd _fcn]]
                notice $str
            }

        } else {
            foreach cmd $mod2 {
                cmd_help $mod $cmd $fmt
            }
        }
    }

    protected method cmd_help { mod cmd {param ""} {p2 "" } } {

        set cidx "${mod}_[string toupper $cmd]"
        set cmdl [public_commands $mod]

        set long 0
        set sum 0
        if { $p2 eq "-long" || $p2 == 1} {
            set long 1
        } elseif { $param eq "-long" } {
            set long 1
            set param ""
        } elseif { $param eq "-sum" } {
            set sum 1
        }

        set rc [lsearch -exact $cmdl $cmd]
        
        if { $rc == -1 } {
            set rc [lsearch $cmdl $cmd]
            if { $rc == -1 } {
                set display 0
                foreach c $cmdl {
                    set hlp [parse_desc $mod $c _fcn]
                    #notice "$cmd $c $hlp"
                    if { ([string match -nocase $cmd $c] == 1)
                        || ([string match -nocase $cmd $hlp] == 1) } {
                        set str [format $P_FMT $c $hlp]
                        notice $str
                        incr display
                    }
                }
                if { $display == 0 } {
                    error "[string tolower $mod] $cmd is not a valid command!" "" -1
                }
            } else {
                if { $long == 1} {
                    foreach c $cmdl {
                        if { [string match -nocase $cmd $c] == 1 } {
                            cmd_help $mod $c -long
                        }
                    }
                } elseif { $sum == 1 } {
                    foreach c $cmdl {
                        if { [string match -nocase $cmd $c] == 1 } {
                            cmd_help $mod $c 
                        }
                    }
                } else {
                    puts "Command summary for $mod:"
                    foreach c $cmdl {
                        set hlp [parse_desc $mod $c _fcn]
                        if { ([string match -nocase $cmd $c] == 1) } {
                            set str [format $P_FMT $c $hlp]
                            notice $str
                        }
                    }
                }
            }
            return

        }


        if { ![info exists PARMS($cidx)] } {
            #notice "cidx: $cidx"
            #parray PARMS
            notice "Command $cmd (module $mod) does not take any parameters!" 
            return 1
        }


        set argl $PARMS($cidx)

        set help "\nCommand $cmd (module $mod): [parse_desc $mod $cmd _fcn]"
        set ret [parse_desc $mod $cmd _rtn]
        if { $ret ne "" } {
            append help "\nReturns: $ret"
        }

                                                 

        set displayed 0

        ## loop through each arg..
        set name ""
        set type ""
        set required ""
        set flag 0
        set default ""
        foreach arg [join $argl] {
            #puts "arg: $arg"
            foreach { name type required default } $arg { break;}

            #skip hidden parameters
            if { [string index $type 0] == "h" } { continue }
            if { [string index $type 0] == "_" } { continue }

            if { $param eq "" || [string match -nocase $param $name] } {
                if { $long == 1 && $param eq "" } {
                    append help "\n"
                }
                append help "\n  Parameter: -$name\n"
                append help "  Required?: [parse_req $required $name]"
                if { $long == 1 } {
                    set desc "[parse_desc $mod $cmd $name]"
                    if { $desc != "" } {
                        append help "Description: $desc\n"
                    }
                    append help "       Type: $VTYPE($type)\n"
                    append help "    Default: $default"
                }
                incr displayed
            }
        }

        if { $displayed == 0 } {
            if { $param eq "" } {
                notice "Command $cmd does not take any parameters!" 
            } else {
                warning "Parameter $param not found for command $cmd of module $mod."
            }
            return $displayed
        }

        if { ($param eq "") || ($displayed > 0) } {
            notice $help
        } else {
            warning "Parameter $param not found for command $cmd of module $mod."
        }
        return $displayed
    }

    protected method gui_help { mod cmd } {

        set cidx "[string toupper $mod]_[string toupper $cmd]"

        if { [info exists GUI_PARMS($cidx,name)] } {
            return
        }

        #puts "GHelp: $cidx"

        set plist {}
        if { ![info exists PARMS($cidx)] } {
            #notice "Command $cmd does not take any parameters!" 
            set GUI_PARMS($cidx,name) ""
            return
        }


        set argl $PARMS($cidx)

        ## loop through each arg..
        set name ""
        set type ""
        set required ""
        set flag 0
        set default ""
        set idx 0
        foreach arg [join $argl] {
            #puts "arg: $arg ($idx)"
            foreach { name type required default } $arg { break;}

            #skip hidden parameters
            if { [string index $type 0] == "h" } { continue }

            set desc "[parse_desc $mod $cmd $name]"
            foreach arg { name type required default desc } {
                set GUI_PARMS($cidx,${arg}${idx}) [set $arg]
            }
            incr idx
        }
    }

    private method arg_check { val type } {
        set val [string trim $val]

        ## account for hidden commands
        if { [string index $type 0] == "h" } {
            set type [string range $type 1 end]
        }

        switch -glob $type {
            bool { 
                switch $val { 
                    1 { return 0 } 
                    0 { return 0 } 
                    default { return -1} 
                } 
            }
            num { 
                if { [string is double $val] || [string is integer $val] } {
                    return 0
                } else {
                    return -1
                }
            }
            pnum { 
                if { [string is double $val] || [string is integer $val] } {
                    if { $val >= 0.0 } {
                        return 0
                    } else {
                        return -1
                    }
                } else {
                    return -1
                }
            }
            nstr {
                if { [string length $val] > 0 } {
                    return 0
                } else {
                    return -1
                }
            }
            nslist {
                if { [llength [list $val]] > 0 } {
                    return 0
                } else {
                    return -1
                }
            }
            slist {
                if { [llength [list $val]] >= 0 } {
                    return 0
                } else {
                    return -1
                }
            }
            ifile {
                if { [file exists $val] == 0 } {
                   error "Input file $val does not exist" "" -1 
                }
                return 0
            }
            iflist {
                foreach if $val {
                    if { [file exists $if] == 0 } {
                        error "Input file $if of list $val does not exist" "" -1
                    }
                }
                return 0
            }
            enum_* {
                set ename [string range $type 5 end]
                set elist [get_enum $ename]
                debug VOB:P "ELIST: $elist"
                foreach av [join $elist] {
                    debug VOB:P "Testing $av vs $val"
                    if { $av eq $val } {
                        return 0
                    }
                }
                return -1
            }

            default {
                ## covers all file types
                if { [string length $val] > 0 } {
                    return 0
                } else {
                    return -1
                }
            }
        }
    }

    private method arg_format { val type } {
        set val2 [string trim $val]
        ## account for hidden commands
        if { [string index $type 0] == "h" } {
            set type [string range $type 1 end]
        }
        switch $type {
            bool { 
                switch $val2 { 
                    1 { return 1 } 
                    0 { return 0 } 
                    default { return 0} 
                } 
            }
            num { 
                if { [string is double $val2] || [string is integer $val2] } {
                    return $val2
                } else {
                    error "Unable to interpret \"$val2\" as a numeric value." "" -1
                }
            }
            pnum { 
                if { [string is double $val2] || [string is integer $val2] } {
                    if { $val2 >= 0.0 } {
                        return $val2
                    } else {
                        error "Unable to interpret \"$val2\" as a positive numeric value." "" -1
                    }

                } else {
                    error "Unable to interpret \"$val2\" as a numeric value." "" -1
                }
            }
            nstr {
                if { [string length $val] > 0 } {
                    return $val
                } else {
                    error "Empty string passed when non-empty string required." "" -1
                }
            }
            nslist {
                set l [concat $val]
                if { [llength $l] > 0 } {
                    return $l
                } else {
                    error "Empty list passed when non-empty list required." "" -1
                }
            }
            slist {
                set l [concat $val]
                if { [llength $l] >= 0 } {
                    return $l
                } else {
                    error "Could not convert value \"$val\" into a list." "" -1
                }
            }
            default {
                ## no restrictions on the default
                return $val
            }
        }
    }

    private method default_value { type } {
        ## account for hidden commands
        if { [string index $type 0] == "h" } {
            set type [string range $type 1 end]
        }
        switch $type {
            bool { 
                return 0
            }
            *num { 
                return 0
            }
            nstr {
                return "stringvalue"
            }
            nslist {
                return "stringlist"
            }
            default {
                ## covers all file types
                return "filename"
            }
        }
    }

    protected method check_arg {aname args} {
        upvar 1 OPT MYOPT
        foreach arg "$aname $args" {
            if { $arg eq "" } { continue }
            if { [info exists MYOPT($arg)] == 0 } {
                return 0
            }
        }
        return 1
    }

    protected method set_arg { aname val } {
        upvar 1 OPT MYOPT
        set MYOPT($aname) $val
    }


    protected method keep_arg { args } {
        upvar 1 OPT MYOPT
        foreach arg [array names MYOPT] {
            if { [lsearch $args $arg] < 0 } {
                array unset MYOPT $arg
            }
        }
    }
    protected method rm_arg { aname args } {
        upvar 1 OPT MYOPT
        foreach arg "$aname $args" {
            array unset MYOPT $arg
        }
    }
        

    protected method get_arg_names { } {
        upvar 1 OPT MYOPT
        return [array names MYOPT ]
    }

    protected method get_arg { aname } {
        upvar 1 OPT MYOPT
        return $MYOPT($aname)
    }

    protected method cmd_rlog { {mycmd ""} } {
        if { [state milos_rlog_on] == 0 } {
            return
        }

        if { $mycmd eq "" } {
            set cl [info class]

            ## strip leading ::
            set mod [string trim [join [split $cl :]]]
            
            set cmd [lindex [info level -1] 0]

            set cmd "End: [string tolower $mod] $cmd"
        } else {
            set cmd  $mycmd
        }

        set db [get_tm DB]

        $db rlog -tag  "$cmd"
    }

    protected method cmdline { } {
        set cl [info class]

        ## strip leading ::
        set mod [string trim [join [split $cl :]]]
        
        set cmd [lindex [info level -1] 0]
        set cidx "${mod}_[string toupper $cmd]"

        debug VOB:P "Command: $cidx"

        set args [string trim [info level -1]]
        debug VOB:P "args: $args"
        return "[string tolower $mod] [join $args]"
    }

    protected method start_timer { } {
        set mytimer [::sys::startTimer]
        debug HTIMER:A "VOB: start_timer $mytimer [info level -1] [info level -2]"
        return $mytimer
    }
    protected method end_timer { timer {msg "milos internal operation"} } {
        ::sys::showTimer $msg ZTIMER A $timer
        debug HTIMER:A "VOB: end_timer $timer [info level -1] [info level -2]"
        reset_timer $timer
    }

    protected method get_timer { timer } {
        if { $timer < 0 } { return -0.1 }
        return [::sys::getTimer $timer]
    }
    protected method reset_timer { timer } {
        debug HTIMER:A "VOB: reset_timer $timer [info level -1] [info level -2]"
        ::sys::resetTimer $timer
    }

    protected method date { } {
        return [clock format [clock seconds]]
    }

    protected method parse_args { } {

        set cl [info class]

        ## strip leading ::
        set mod [string trim [join [split $cl :]]]
        
        set cmd [lindex [info level -1] 0]
        set cidx "${mod}_[string toupper $cmd]"

        debug VOB:P "Command: $cidx"

        set args [string trim [lrange [info level -1] 1 end]]
        debug VOB:P "args: $args"

        if { ![info exists PARMS($cidx)] } {
            #parray PARMS
            error "Command $cmd does not take any parameters!" "" -1
        }

        if { [string index $args 0] == "?" } {
            cmd_help $mod $cmd [string trim [string range $args 1 end]]
            return -code return
        }

        set argl $PARMS($cidx)

        ## contains returned values
        array set OPT {}

        ## contains required parameters not yet seen
        array set REQS {}

        ## contains FULL requirements - REQS gets modified
        array set REQF {}

        ## contains types of each arg
        array set TYPE {}

        ## contains parameters actually specified by user
        array set ARGS {}

        ## loop through each arg.. - populate OPT and REQ from 
        ## original definitions

        set name ""
        set type ""
        set required ""
        set default ""
        foreach arg [join $argl] {
            debug VOB:P "arg: $arg"
            foreach { name type required default } $arg { break;}

            set ll [llength $arg]
            debug VOB:P "arg name: $name"
            debug VOB:P "arg type: $type"
            debug VOB:P "required? $required"
            debug VOB:P "default: $default"

            if { $ll == 4 } {
                ## there IS a default
                set OPT($name) $default
            }
            if { $ll >= 3  && $required != "0"} {
                set REQS($name) $required
                set REQF($name) $required
            }
            set TYPE($name) $type
        }

        debug VOB:P "options:"
        debug VOB:P [array get OPT]

        debug VOB:P "reqs:"
        debug VOB:P [array get REQS]

        debug VOB:P "types:"
        debug VOB:P [array get TYPE]

        ## Now, parse args

        set aname ""
        set atype ""
        foreach arg $args {

            if { [string index $arg 0] eq "-" } {
                ## arg name
                if { $aname != "" } {
                    if { $atype == "bool" || $atype == "hbool" } {
                        ## default a previous bool to true
                        set OPT($aname) 1
                    } else {
                        ## check if it's the right value
                        debug VOB:P "checking $arg for type $atype for parm $aname"
                        if { [arg_check $arg $atype] == 0 } {
                            debug VOB:P "setting value $arg for parm $aname"
                            set OPT($aname) [arg_format $arg $atype]
                            set aname ""
                            continue
                        } else {
                            error "Invalid value `$arg' for parameter -$aname (type $VTYPE($atype))." "" -1
                        }
                    }
                } 
                ## strip leading -
                set aname [string range $arg 1 end]

                ## check for shortforms
                set alist [array names TYPE "${aname}*"]
                set ll [llength $alist]
                if { $ll == 1 } {
                    set aname $alist
                } elseif { $ll > 1 } {
                    error "-${aname}: ambigous parameter. Possible matches: $alist"
                } else {
                    error "Unknown parameter -$aname to command $cmd." "" -1
                }

                set atype $TYPE($aname)
                set ARGS($aname) 1
            } else {
                if { $aname == "" } {
                    cmd_help $mod $cmd 
                    return -code return
                } else {
                    ## valid arg
                    if { [arg_check $arg $atype] == 0 } {
                        debug VOB:P "2 setting value $arg for parm $aname"
                        set OPT($aname) [arg_format $arg $atype]
                        set aname ""
                    } else {
                        error "Invalid value `$arg' for parameter -$aname (type $VTYPE($atype))." "" -1
                    }
                }
            }
        }
        if { $aname != "" } {
            if { $atype == "bool" || $atype == "hbool" } {
                set OPT($aname) 1
            } else {
                error "No value specified for parameter -$aname (type $VTYPE($atype))." "" -1
            }
        }

        ## final check for "and" reqs that were actually seen

        foreach aname [array names REQS] {
            ## check each time, because else can modify two entries in REQS
            debug VOB:P "REQS arg: $aname"
            if { [info exists REQS($aname)] } {
                debug VOB:P "REQS arg2: $aname ($REQS($aname))"
                if { $REQS($aname) == 1 } {
                    if { ![info exists ARGS($aname)] } {
                        error "Required parameter -$aname for command $cmd was not specified." "" -1
                    } else {
                        debug VOB:P "removing simple $aname"
                        array unset REQS $aname
                    }
                } elseif { [string range $REQS($aname) 0 3] eq "AND_" } {
                    set other [string range $REQS($aname) 4 end]
                    if { [info exists ARGS($aname)] } {
                        ## parameter was specified
                        if { [info exists ARGS($other)] } {
                            ## both were specified - remove original
                            ## second will be processed later..
                            debug VOB:P "removing AND: $aname"
                            array unset REQS $aname
                        }
                    } else {
                        ## parameter was NOT specified
                            debug VOB:P "removing NAND: $aname"
                            array unset REQS $aname
                    }
                } elseif { [string range $REQS($aname) 0 2] eq "OR_" } {
                    ## remove single-ORs
                    set other [string range $REQS($aname) 3 end]
                    if { [info exists ARGS($aname)] 
                        || [info exists ARGS($other)] } {
                        ## can remove this requirement
                        debug VOB:P "removing OR: $aname"
                        array unset REQS $aname
                    }
                } elseif { [string range $REQS($aname) 0 3] eq "NOT_" } {
                    debug VOB:P "NOT: $aname!"
                    if { [info exists ARGS($aname)] } {
                        set other [string range $REQS($aname) 4 end]
                        if { [info exists ARGS($other)] } {
                            error "Parameter -$aname cannot be specified together with parameter -$other." "" -1
                        } else {
                            debug VOB:P "removing NOT: $aname - $other not found"
                            array unset REQS $aname
                        }
                    } else {
                        debug VOB:P "removing NOT: $aname - not specified"
                        array unset REQS $aname
                    }

                }
            }
        }

        ##
        foreach aname [array names REQS] {
            if { [string range $REQS($aname) 0 3] eq "AND_" } {
                set other [string range $REQS($aname) 4 end]
                error "Parameter $aname requires parameter $other to be specified also." "" -1
            } elseif { [string range $REQS($aname) 0 2] eq "OR_" } {
                set other [string range $REQS($aname) 3 end]
                error "Either parameter $aname or parameter $other must be specified." "" -1
            } else {
                warning "Uh oh - unhandled requirement: $aname, $REQS($aname)"
            }
        }

        debug VOB:P "final options:"
        debug VOBP:P [array get OPT]

        debug VOB:P "final parms:"
        debug VOB:P [array get ARGS]

        debug VOB:P "final reqs:"
        debug VOB:P [array get REQS]


        set cmd "array set OPT {[array get OPT]}"
        uplevel 1 $cmd

    }

    protected method get_all_args { } {
        upvar 1 OPT MYOPT
        foreach arg [array names MYOPT] {
            #puts "set $arg \"$MYOPT($arg)\""
            uplevel 1 "set $arg \"$MYOPT($arg)\""
        }
    }

    protected method build_cmd { obj ocmd } {

        upvar 1 OPT OPT
        #puts "bc: opts"
        #parray OPT

        set cl [info class]

        ## strip leading ::
        set mod [string trim [join [split $cl :]]]
        
        set cmd [lindex [info level -1] 0]
        set cidx "${mod}_[string toupper $cmd]"

        set argl $PARMS($cidx)
        set cmd "$obj $ocmd "

        foreach arg [array names OPT] {
            #puts "bc arg: $arg"
            append cmd [build_arg $arg [lindex [array get OPT $arg] 1] $cidx]
        }

        #puts "cmd as built: $cmd"
        return $cmd
    }


    protected method get_alias { aname cidx } {
        set nname $aname

        if { [info exists P_ALIAS($cidx)] } {
            set argl $P_ALIAS($cidx)
            foreach arg [join $argl] {
                foreach {name alias} $arg { break; }
                if { $name eq $aname } {
                    set nname $alias
                    break
                }
            }
        }
        return $nname
    }

    protected method build_arg { aname aval cidx } {

        set alist {}
        set argl $PARMS($cidx)

        foreach arg [join $argl] {
            if { [lindex $arg 0] eq $aname } {
                set alist $arg
                break
            }
        }

        if { [llength $alist] == 0 } {
            error "Unable to find arg $aname for build_arg in $argl" "" -1
        }
        foreach { name type required default } $alist { break;}

        set aname [get_alias $aname $cidx]

        ## account for hidden commands
        if { [string index $type 0] == "h" } {
            set type [string range $type 1 end]
        }
        switch -glob $type {
            bool {
                if { $aval == 1 } {
                    return "-$aname "
                } else {
                    return ""
                }
            }
            *list {
                #puts "List build: $aname!"
                return "-$aname {$aval} "
            }
            default {
                #puts "Default build: $aname!"
                return "-$aname $aval "
            }

        }
    }

    protected method rebuild_cmd { obj ocmd } {

        upvar 1 OPT OPT
        #puts "bc: opts"
        #parray OPT

        set cl [info class]

        ## strip leading ::
        set mod [string trim [join [split $cl :]]]
        
        set cmd [lindex [info level -1] 0]
        set cidx "${mod}_[string toupper $cmd]"

        set argl $PARMS($cidx)
        set cmd "$obj $ocmd "

        foreach arg [array names OPT] {
            #puts "rc arg: $arg"
            append cmd [rebuild_arg $arg [lindex [array get OPT $arg] 1] $cidx]
        }

        #puts "cmd as rebuilt: $cmd"
        return $cmd
    }
    protected method rebuild_arg { aname aval cidx } {

        set alist {}
        set argl $PARMS($cidx)

        foreach arg [join $argl] {
            if { [lindex $arg 0] eq $aname } {
                set alist $arg
                break
            }
        }

        if { [llength $alist] == 0 } {
            error "Unable to find arg $aname for build_arg in $argl" "" -1
        }
        foreach { name type required default } $alist { break;}

        set aname [get_alias $aname $cidx]

        ## account for hidden commands
        if { [string index $type 0] == "h" } {
            set type [string range $type 1 end]
        }
        switch -glob $type {
            bool {
                return "-$aname $aval"
            }
            *list {
                #puts "List build: $aname!"
                return "-$aname {$aval} "
            }
            default {
                #puts "Default build: $aname!"
                return "-$aname {$aval} "
            }

        }
    }

    protected method class_configure { object } {
        upvar 1 OPT MYOPT
        debug VOB:A "CC: $object"
        foreach { name val } [array get MYOPT] {
            debug VOB:A "CC: $name, $val"
            $object configure -$name $val
        }
        debug VOB:A "FINAL CC: [$object configure]"
    }

    protected method find_corner { name } {
        return [find_obj corner_list corner $name]
    }
    protected method find_obj { state attrib val } {
        foreach var [state_list $state] {
            if { [$var cget -$attrib] eq $val } {
                #puts "Find_Obj: $var, $attrib - $val"
                return $var
            }
        }
        error "Object with attribute $attrib having value $val not found in list [state_list $state]" "" -1
    }

    ######################################################################
    ## General Utility commands
    ######################################################################

    protected method get_ec_idx { name } {
        set ec [get_extraction_corners]
        set idx 0
        foreach c $ec {
            if { $c eq $name } {
                return $idx
            }
            incr idx
        }
        return -1
    }
    protected method check_extraction_corner { name } {
        set idx [get_ec_idx $name]
        if { $idx != -1 } {
            return $name
        } else {
            return ""
        }
    }

    protected method get_extraction_corners { } {
        set db [get_tm DB]
        $db get_extraction_corners -names ::extnames
        return [join [join $::extnames]]
    }

    protected method parasitics_check { } {
        ## disable to avoid problems with remotes..
        return
        get_ext_status
        if { [state db_parasitics] != 1 } {
            error "DB must have parasitics to perform this action" "" -1
        }
        if { [state tmg_rcmodels] != 1 } {
            error "DB RC models are out of date - please run tmg update_timing" "" -1
        }
    }
    protected method get_ext_status { } {
        set db [get_tm DB]
        $db ext_info -extracted= ::myext -foreign= ::myforeign
        set_state db_parasitics $::myext
        set_state db_foreign $::myforeign
    }

    protected method get_db_change_status { } {
        set db [get_tm DB]
        $db get -top -object= ::block

        set altered [$db count_attribute_for_signal_nets -block $::block -altered= ::altered -disconnected= ::disc]

        set_state db_altered_cnt $::altered
        set_state db_disconnected_cnt $::disc
    }


    protected method set_echo { val } {
        set ECHO $val
    }

    protected method forall { script } {
        foreach arg $CHILDREN {
            set cmd "$arg $script"
            ::eval $cmd
        }
    }

    protected proc public_commands { mod } {
        set clr "::[string toupper $mod]"

        set rc { }
        set cmdlist [namespace eval $clr { ::itcl::builtin::info function } ]
        foreach arg $cmdlist {
            set clist [split $arg ":"]
            foreach {dummy dummy pref dummy name} $clist { break; }
            if { "::$pref" eq $clr } {
                set cmdtype [namespace eval $clr " ::itcl::builtin::info function $name" ]
                if { [lindex $cmdtype 0] == "public" } {
                    lappend rc $name
                }
            }
        }

        return [lsort $rc]
    }


    protected method echo { str } {
        if { $ECHO == 1 } {
            puts $str
        }
    }

    protected method add_child { child } {
        lappend CHILDREN $child
    }

    protected method commandline { } {
        return "$::argv0 $::argv"
    }

    ## close the hole..
    protected method _itcl_config { args } @itcl-builtin-configure
    

    protected method showmem { } {
        notice [parent getmem]
    }

    protected method parent { args } {
        if { $PARENT != "" } {
            #puts "Test: $PARENT $args"
            return [::eval [list $PARENT] [lrange $args 0 end] ]
        } else {
            return ""
        }
    }

    protected method getCVS { pattern } {
        debug LIC:A "GETCVS: checkout $MYLICENSE"
        set lic [::eval ::zr.lic Checkout $MYLICENSE rr]
        debug LIC:A "GETCVS: after checkout $MYLICENSE: $lic"

        if { $lic == 0 } {
            set cvl [split [::sys::getCVSData] "\n"]
            foreach f $cvl {
                if { [string match *$pattern* $f] } {
                    puts "$f"
                }
            }
        } else {
            if { [::zr.zlog get_debug_log_config -module LIC] == 1 } {
                puts "Failed license checkout: [parray rr]"
            }
            error "This option is not licensed to run on this machine." "" -1
        }
    }

    protected method eval { script } {

        for { set i 0 } { $i < 6 } { incr i } {
            debug LIC:A "VOB::EVAL: checkout $MYLICENSE"
            #set lic [::eval ::zr.lic Checkout $MYLICENSE rr]
            set lic 0
            debug LIC:A "VOB::EVAL: after checkout $MYLICENSE: $lic"
            if { $lic == 0 } { break }
            warning "Could not get debug license - retrying after 20 seconds!"
            after 20000
        }

        if { $lic == 0 } {
            set res [ catch { ::eval $script } output ]
            if { $res != 0 } {
                notice "Error $res while attempting to evaluate $script.\nError text: $output"  
            }
            return $output
        } else {
            if { [::zr.zlog get_debug_log_config -module LIC] == 1 } {
                puts "Failed license checkout: [parray rr]"
            }
            error "This option is not licensed to run on this machine." "" -1
        }

    }

    ## returns contents of a file in a variable
    protected proc read_file { ifile } {
       if { [file exists $ifile] } {
            set fileId [open $ifile r]
            set data [read $fileId]
            close $fileId
            return $data
        } else {
            return ""
        }
    }

    protected proc write_file {file data} {
        set fp [open $file w]
        puts $fp $data
        close $fp
    }

    ######################################################################
    ## STATE/TM management
    ######################################################################

    protected variable eco_inst_cnt 0
    public method new_eco_inst { } {
        set ename "[state ns]::eco${eco_inst_cnt}"
        incr eco_inst_cnt
        ECO $ename
        return $ename
    }

    protected method state { name } {
        #set np [string range $this 0 [expr {[string last ":" $this] - 2}] ]
        set np1 $::apps::cur_app
        set np [${np1}::cur_ns]
        #puts "state: $this, $np"
        foreach arg [array names ${np}::STATE] {
            if { $arg == $name } {
                set var "${np}::STATE($name)"
                return [set $var]
            } 
        }
        return ""
    }
    protected method state_num { name default } {
        set np [string range $this 0 [expr {[string last ":" $this] - 2}] ]
        foreach arg [array names ${np}::STATE] {
            if { $arg == $name } {
                set var "${np}::STATE($name)"
                set val [set $var]
                if { $val == "" } {
                    return $default
                } else {
                    return $val
                }
            } 
        }
        return $default
    }

    protected method state_list { name } {
        set np [string range $this 0 [expr {[string last ":" $this] - 2}] ]
        foreach arg [array names ${np}::STATE] {
            if { $arg == $name } {
                set var "${np}::STATE($name)"
                set val [set $var]
                if { $val == "" } {
                    return "{}"
                } else {
                    return $val
                }
            } 
        }
        return "{}"
    }

    protected method unwatch_state { name } {
        array unset watch_states $name
    }

    protected method watch_state { name } {
        array set watch_states [list $name 1]
        puts "Watching for changes of state $name, current value [state $name]"
        #puts "ws test: $watch_states($name)"
    }

    protected method check_watch { name } {
        
        set t [::info exists watch_states($name)]
        #puts "STATE: check for $name: $t"
        return $t
    }

    protected method set_state { name val } {
        set np [string range $this 0 [expr {[string last ":" $this] - 2}]  ]
        if { [check_watch $name] == 1 } {
            puts "STATE CHANGE: value $name changing from [state $name] to $val"
        }
        set ${np}::STATE($name) $val
    }

    protected method append_state { name val } {
        set ov [state $name]
        append ov $val
        if { [check_watch $name] == 1 } {
            puts "STATE CHANGE: value $name changing from [state $name] to $val"
        }
        set_state $name $ov
    }

    protected method lappend_state { name val } {
        set ov [state $name]
        lappend ov $val
        if { [check_watch $name] == 1 } {
            puts "STATE CHANGE: value $name changing from [state $name] to $val"
        }
        set_state $name $ov
    }

    protected method lremove_state { name val } {
        set idx [lsearch [state $name] $val]
        while { $idx >= 0 } {
            set_state $name [lreplace [state $name] $idx $idx]
            set idx [lsearch [state $name] $val]
        }
    }


    protected method del_tm { name } {
        set np [state ns]
        parray ${np}::TM_INST
        set np [state ns]
        array unset ${np}::TM_INST $name
        parray ${np}::TM_INST
    }

    protected method get_tm { name } {

        ## I am root, keeper of the tech modules
        set np [state ns]

        foreach arg [array names ${np}::TM_INST] {
            if { $arg == $name } {
               #puts "Found: ${np}::TM_INST($name)"
                return "[itcl::code [set ${np}::TM_INST($name)]]"
            }
        }
        ## need to create an instance - use #auto
        if { $name != "MSH" } {

            #puts "np: $np"
            if { ([state db_loaded] == 1) || ($name eq "Tmg") } {
                set ${np}::TM_INST($name) [$name #auto]
                #puts "TMI after:"
                #parray ${np}::TM_INST
                return "[itcl::code [set ${np}::TM_INST($name)]]"
            } else {
                #parray ${np}::TM_INST
                error "Please load a design first!" "" -1
                #return -code error  -errorinfo "Please load a design first!" -errorcode -1
                return ""
            }
        } else {
            msh::init
            set ${np}::TM_INST(MSH) ""
            return ""
        }
    }

    ######################################################################
    ## Remote nodes
    ######################################################################

    protected method new_rmt { name {async 0} {rmt_app "milos"} {timing "milos"} } {
        set ns [state ns]

        set name [string trim $name]
        if { $name eq "" } {
            notice "Error: Remote Task name cannot be blank!"
            return 1 
        }
        #set ns [cur_ns]
        set n2 "${ns}::$name"
        if { $name eq "tasks" } {
            ## return task list
            set al { }
            foreach {obj alias} [array get ${ns}::RSLAVES] {
                lappend al $alias
            }
            return $al
        }

        if { [info command $n2] != "" } {
            notice "Error: Remote Node $name already exists!"
            return 1
        }

        ## make sure it's active
        get_tm MSH

        set a ""
        set output ""
        if { $rmt_app ne "local" } {
            puts "new_rmt: NEW RMT_${timing} $n2 $async $rmt_app"
            set rc [ catch {set a [::RMT_${timing}::new $n2 $async $rmt_app]} output ]
            #set rc 0
            #set a [::RMT_${timing}::new $n2 $async $rmt_app]
            if { $rc != 0 } { 
                error "Remote type $rmt_app does not exist $rc, $output!" "" -1
            }
        } else {
            #puts "New RMT of type ::RMT_LOCAL_${timing}, $n2 $async $rmt_app"
            set rc [ catch {set a [::RMT_LOCAL_${timing}::new $n2 $async $rmt_app]} output ]
            if { $rc != 0 } { 
                puts "Plugin type RMT_LOCAL_$timing does not exist: $rc, $a, $output!" 
                error "Plugin type RMT_LOCAL_$timing does not exist: $rc, $a, $output!"  "" -1
            }
        }
        #puts "New RMT: $a"
        set ${ns}::RSLAVES($a) $name
        set ${ns}::RSTATUS($a) 0
        set ${ns}::WRMT($a) { }
        set ${ns}::WME($a) { }
        puts "Exit New RMT: $a"
        return $a

    }


    protected method timing_name { val } {
        switch $val {
            milos { return "Internal" }
            pt { return "PrimeTime (tm)" }
            psta { return "PSTA" }
            quartz { return "Quartz (tm)" }
            default { return "" }
        }
    }

    protected method UPOBJ { cmd args } {
        if { [info exists ::apps::milos::up_obj] == 0 } {
            return ""
        }
        if { [llength $args] > 0 } {
            set cmd "$::apps::milos::up_obj $cmd $args"
            ::eval $cmd
        } else {
            $::apps::milos::up_obj $cmd
        }
    }

    ######################################################################
    ## Plugin commands
    ######################################################################
    protected common verilog_names;
    protected common verilog_count 1;

    protected method clear_verilog_names { } {
        #puts "RESET verilog names before: "
        #parray verilog_names
        array unset verilog_names
        array set verilog_names {}
        #puts "RESET verilog names after: "
        #parray verilog_names
        #set verilog_count 1
    }

    ## only write verilog if necessary
    protected method write_verilog { sobj {delimiter "/"} {no_backslash 0} } {
        set aidx "${delimiter}${no_backslash}"

        #puts "Write verilog: [parray verilog_names]"
        ## see if current combination was already written earlier!
        if { [info exists verilog_names($aidx)] == 0 } {
            set vname [plugin_fname $sobj "milos${verilog_count}" "v"]
            incr verilog_count
            #puts "Writing new verilog file $vname!"
            set vmod [call_milos "tmg write_verilog -delimiter $delimiter -no_backslash $no_backslash -file $vname"]
            set verilog_names($aidx) $vname
            set verilog_names(mod_$aidx) $vmod
            puts "Writing new verilog file $vmod!"
        } else {
            #puts "Re-using existing verilog file: $verilog_names($aidx)"
        }
        $sobj configure -verilog_file $verilog_names($aidx) 
        $sobj configure -verilog_module $verilog_names(mod_$aidx)
    }
    protected method get_verilog_file { sobj {delimiter "/"} {no_backslash 0} } {
	set block [$sobj cget -block]
        set aidx "${block}${delimiter}${no_backslash}"

        set vname ""
        puts "Write verilog File: aidx= $aidx --- block= $block"
        ## see if current combination was already written earlier!
        if { [info exists verilog_names($aidx)] == 0 } {
            set vname [plugin_fname $sobj "milos${verilog_count}" "v"]
            incr verilog_count
            #puts "Writing new verilog file $vname!"
            #set vmod [call_milos "tmg write_verilog -delimiter $delimiter -no_backslash $no_backslash -file $vname"]
            set verilog_names($aidx) $vname
            #TODO set verilog_names(mod_$aidx) $vmod
            set verilog_names(mod_$aidx) $block
        } else {
            #puts "Re-using existing verilog file: $verilog_names($aidx)"
        }
            puts "get_verilog_file: verilog_names($aidx) verilog_names(mod_$aidx)"

        $sobj configure -verilog_file $verilog_names($aidx) 
        #TODO $sobj configure -verilog_module $verilog_names(mod_$aidx)
        $sobj configure -verilog_module $verilog_names(mod_$aidx)
	return $vname
    }
        


    protected method check_multi_error { } {
        foreach node [state rmt_list] {
            set a [$node getErrorStatus]
            debug PLUGIN_SYNC:A "Node $node, ES: $a"
            if { $a != 0 } {
                notice "Node [$node getAlias] had error(s). Error message:"
                notice "[$node getErrorMessage]"
                error "Unable to continue." "" -1
            }
        }
    }

    protected common startsocket 9000;
    protected common endsocket 10000;

    protected method escape_line { target line replace} {

        set sl [string trim $line]

        if { [string index $sl 0] eq "#" } {
            return $line
        }
        if { $sl eq "" } {
            return ""
        }

        regsub -all $target $line $replace nl
        return $nl
    }

    protected method escape_1 { str } {
        ## convert all regular backslashes into two
        ## or you do \ substitution on the new \[
        regsub -all "\[\[]" $str "\\\[" strn
        return $strn
    }

    protected method escape { str } {
        ## convert all regular backslashes into two
        regsub -all "\\\\" $str "\\\\\\\\" strn

        ## convert [ into \[ - do this AFTER the above,
        ## or you do \ substitution on the new \[
        regsub -all "\[\[]" $strn "\\\[" strn
        return $strn
    }

    protected method get_client_socket {host ssock} {
        set rc [ catch { set client_sock [socket $host $ssock] } output ]
        set retry 0
        while { $rc != 0 } {
            incr retry
            if { $retry == 5 } {
                notice "Error in getting client connection to plugin!"
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
    protected method get_socket { startsocket endsocket} {
        for { set i $startsocket } { $i < $endsocket } {incr i } {
            set rc [localsockfree $i]
            if { $rc == 1 } {
                return $i
            }
        }
        error "Could not find free socket in range $startsocket to $endsocket - problem with localhost?" "" -1
    }


    protected method get_plugin_object { type } {
        #puts "GPO: $plugin_objs($type)"
        if { [info exists plugin_objs($type)] == 0 } {
            error "Plugin object $type has not been initialized!" 
        } else {
            debug PLUGIN:S "GPO: $plugin_objs($type)"
            return $plugin_objs($type)
        }
    }
    protected method set_plugin_object { type obj } {
        set plugin_objs($type) $obj
        set plugin_time($type) 0
    }

    protected method add_plugin_time { type seconds } {
        ## ignore errors.
        catch {set plugin_time($type) [expr {$plugin_time($type) + $seconds} ] }
    }

    protected method get_plugin_time { type } {
        return $plugin_time($type)
    }

    ## check sobj for conflicts in setup configuration
    protected method check_sobj { obj } {
        switch [$obj cget -engine] {
            psta {
                if { [$obj cget -db] ne "" } {
                    warning "PSTA does not support the -db option - using -lib files!"
                }
            }
        }
        switch [$obj cget -ltype] {
            static - transient { }
            default { error "License type $[$obj cget -ltype] is invalid. Valid values: static, transient" }
        }
    }

    protected method write_allterms { file } {
        if { [state allterms_fname] ne "" } {
            return
        }
        set fp [open $file w]
        [get_tm Tmg] get_name_allterms -ochan $fp
        close $fp
        set_state allterms_fname $file
    }

    protected method plugin_fname { sobj prefix suffix } {
        return [file join [$sobj cget -tmp_dir] ${prefix}.[pid].[$sobj get_sidx].${suffix}]
    }

    protected method plugin_cmd { type args } {
            debug PLUGIN:S "plugin_cmd type $type ($args)!"
        set po [get_plugin_object $type]
        if { [$po cget -active] == 0 } {
            debug PLUGIN:S "PLUGIN OBJECT not active for type $type ($args)!"
            return ""
        }
        debug PLUGIN:S "Plugin cmd ([runtime_incr]): [string range [join $args] 0 100]\n"
        set t [start_timer]
        send_plugin_cmd $type [join $args]
        set rc [get_plugin_result $type]
        end_timer $t "Plugin cmd"
        debug PLUGIN:S "Plugin response ([runtime_incr]): $rc, [$po cget -result] // [$po cget -error]\n"
        return $rc
    }

    protected method open_plog { type fname } {
        set po [get_plugin_object $type]
        $po configure -plog [open $fname w]
    }

    protected method plog { type str } {
        #notice "PLOG: $type, $str"
        set po [get_plugin_object $type]
        set plog [$po cget -plog]
        if { $plog ne "" } {
            ## here is where we would put a comment in front of "." used by
            ## the streaming version. However, the streaming log is incomplete,
            ## so a comment would only lend false security that we could 
            ## "source" the result. Leaving the "." in will generate an error
            ## and signal that we CANNOT source the resulting file past this
            ## point
            puts $plog "$str"
            flush $plog
        }
    }

    ## send command to plugin include terminating .
    protected method send_plugin_cmd { type args } {
        set po [get_plugin_object $type]
        if { $po eq "" } {
            return
        }
        #puts "PO: $po"
        set str [string range [join $args] 0 100]
        #puts "verbose PT command: $str"
        set sock [$po cget -plugin_socket]
        if { $sock == -1 } {
            return
        }

        set l [join $args]
        if { [$po cget -translate] eq 9 } {
            set el [escape_line $BS_P $l ""]
            puts $sock $el
            plog $type "#START at [date]"
            plog $type $el
        } else {
            puts $sock $l
            plog $type "#START at [date]"
            plog $type $l
        }
        puts $sock "."
        if { [$po cget -plugin_cmd_pending] == 0 } {
            $po configure  -plugin_cmd_pending 1
            $po configure -timer [start_timer]
            debug HTIMER:A "INITIAL SPC [$po cget -timer]!"
        }
    }

    ## send command to plugin WITHOUT terminating .
    protected method send_plugin_data { type args } {
        set po [get_plugin_object $type]
        set sock [$po cget -plugin_socket]
        if { $sock == -1 } {
            return
        }

        set l [join $args]
        if { [$po cget -translate] eq 9 } {
            set el [escape_line $BS_P $l ""]
            puts $sock $el
            plog $type "#START at [date]"
            plog $type $el
        } else {
            puts $sock $l
            plog $type "#START at [date]"
            plog $type $l
        }
        if { [$po cget -plugin_cmd_pending] == 0 } {
            $po configure -timer [start_timer]
            $po configure  -plugin_cmd_pending 1
            debug HTIMER:A "INITIAL SPD [$po cget -timer]!"
        }
    }

    protected method get_plugin_result { type } {
        set po [get_plugin_object $type]
        if { $po eq "" } {
            return
        }
        if { [$po cget -plugin_cmd_pending] == 0 } {
            return
        }
        set result ""
        set rc 0
        set sock [$po cget -plugin_socket]
        if { $sock == -1 } {
            return
        }
        ## reset error
        $po configure -error ""
        while { true } {
            #puts "Reading line from plugin" 
            gets $sock line
            #puts "Read line from plugin $line"
            if { $line == "." } {
                break;
            } elseif { $line == ".E." } {
                set rc 1
                $po configure -error $result
                #error "Plugin error: $result"
                break;
            } else {
                append result $line
            }
        }
        $po configure -result $result

        show_plugin_time $type

        $po configure  -plugin_cmd_pending 0
        return $rc
    }

    protected method show_plugin_time { type } {
        set po [get_plugin_object $type]

        set timer [$po cget -timer]
        if { $timer < 0 } { 
            debug HTIMER:A "Invalid timer index: $timer [info level -1] [info level -2]"
            return
        }

        debug HTIMER:A "VOB: get_timer $timer [info level -1] [info level -2]"
        set elapsed [get_timer $timer]
        reset_timer $timer

        debug HTIMER:A "VOB: RESET TIMER $timer: [info level -1] [info level -2]"
        if { $timer eq 0 } {
            set inv_timer -999
        } else {
            set inv_timer [expr {0 - $timer}]
        }

        $po configure -timer $inv_timer
        add_plugin_time $type $elapsed

        set el2 [format "%.3f" $elapsed]
        set total [format "%.3f" [get_plugin_time $type]]

        plog $type "\n#RESULT([$po cget -error]): [$po cget -result]"
        plog $type "#Elapsed Time ($el2/$total) at [date]\n"
    }


}

