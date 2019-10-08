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

package require ade::help 1.0

package provide ade::milos 1.0

namespace eval apps { 
    variable cur_app "::apps::milos"
}

namespace eval apps::milos {

variable num_inst 0
variable PREFIX "::apps::milos"

variable MYLICENSE "milos.debug"
variable SLAVE ""
variable CUR_INST 0

variable EXT_CMDS "cd encoding exec fconfigure file glob load open pwd source"

variable INTERP_CMDS { }
variable STA_CMDS { }
variable STA_EXCEPT { auto_execok auto_import auto_load auto_load_index exit \
    open socket unknown }

variable MODS { ext pdb opt tmg clk rte rpt mme sys }
variable MOD_DESC

variable tstamp

variable dup_cnt 0
variable mycf
variable up_obj
variable up_proxy
variable cmd_resolution_init 0
variable cmd_list
#
# Milos application
#

# -- Initialize the Milos application
# --    Context: This call is made on the master interpeter
# --     Inputs: interp (the slave interpeter to be initialized)
# --    Outputs: "" if ok, "message" on error
# -- Exceptions: raise errors

proc init {slave {timestamp -1} } {
    variable SLAVE 
    variable EXT_CMDS
    variable INTERP_CMDS
    variable tstamp
    variable MYLICENSE

    set tstamp $timestamp

    init_sta_cmds $slave
    # -- expose the read command
    package require ade::milos::vob 0.1
    package require ade::milos::dobj 0.1
    package require ade::milos::rmt 0.1
    package require ade::milos::mproxy 0.1
    package require ade::milos::cf 0.1
    package require ade::milos::update 0.1
    package require ade::milos::igui 0.1
    package require ade::milos::ngui_proxy 0.1

    set SLAVE $slave

    ## setup I/O stuff
    $SLAVE eval "set errorInfo \"\""
    $SLAVE eval "set errorCode \"\""

    $SLAVE hide puts
    interp alias $SLAVE puts "" apps::milos::puts
    interp alias $SLAVE help "" apps::mod_help

    #stdout and stderr not really available..
    #foreach chan {stdin stdout stderr} 

    ## stdin could be useful for prompting in user scripts
    foreach chan {stdin} {
        interp share "" $chan $SLAVE
    }


    ## other setup stuff
    interp alias $SLAVE exit "" ::apps::milos::exit
    interp alias $SLAVE RMT "" ::apps::milos::RMT
    interp alias $SLAVE quit "" ::apps::milos::exit

    ## Not ready - but save for later?
    interp alias $SLAVE unknown "" ::apps::milos::myunknown

    interp alias $SLAVE sta "" ::apps::milos::sta_proc

    foreach cmd $EXT_CMDS {
        $SLAVE expose $cmd
    }


    set cwd [file normalize .]
    set twd [file join $::top "../lib/apps"]
    set cmd " cd $twd; source history.tcl; cd $cwd"

    $SLAVE eval $cmd

    set cmd "
            proc ::parray {a {pattern *}} {
                upvar 1 \$a array
                if {!\[array exists array]} {
                    puts \"\\\"\$a\\\" isn't an array\"
                }
                set maxl 0
                foreach name \[lsort \[array names array \$pattern]] {
                if {\[string length \$name] > \$maxl} {
                    set maxl \[string length \$name]
                }
                }
                set maxl \[expr {\$maxl + \[string length \$a] + 2}]
                foreach name \[lsort \[array names array \$pattern]] {
                set nameString \[format %s(%s) \$a \$name]
                puts \[format \"%-*s = %s\" \$maxl \$nameString \$array(\$name)]
                }
            }"

    $SLAVE eval $cmd

    set cmd "namespace eval ::usr {
                proc usr { args } {
                     if { \[lindex \$args 1] == \"proc\" && \[string trim \[lindex \$args 2] ] == \"usr\" } {
                         puts \"Cannot override user proc named usr\"
                         return
                     }
                     return \[namespace eval ::usr \[lrange \$args 0 end] ]
                 }
             }"

    $SLAVE eval $cmd

    interp alias $SLAVE ::usr $SLAVE ::usr::usr


    newinst
    setinst 1

    return ""
}
    


proc mkerror { fp outmsg } {
    set cmd "set ::errorInfo "
    append cmd "{can not find channel2 named \"$fp\""
    append cmd "\n    while executing\n"
    append cmd "\"puts $fp $outmsg\"}"
    return $cmd
}


proc puts { str args } {
    variable SLAVE
    set cnt [llength $args]
    if { $cnt == 2 } {
        set fp [lindex $args 0]
        set outmsg [lindex $args 1]
        if { [$SLAVE eval file channels $fp] != $fp } {
            $SLAVE eval [mkerror $fp $outmsg]
            $SLAVE eval "set ::errorCode NONE"
            puts "can not find channel named \"$fp\""
            return
        }
        return [$SLAVE invokehidden puts -nonewline $fp $outmsg]
    } elseif {$cnt == 1} {
        if { $str != "-nonewline" } {
            set fp $str
            set outmsg [lindex $args 0]
            if { [$SLAVE eval file channels $fp] != $fp } {
                $SLAVE eval [mkerror $fp $outmsg]
                $SLAVE eval "set ::errorCode NONE"
                puts "can not find channel named \"$fp\""
                return
            }
            return [$SLAVE invokehidden puts $fp $outmsg]
        } else {
            return [::puts -nonewline [lindex $args 0]]
        }
    } else {
        return [::puts $str]
    }

}

proc myunknown { cmd {args ""} } {
    variable SLAVE
    variable cmd_resolution_init
    variable mycf 
    variable ::apps::milos::MODS
    variable cmd_list
    if { $cmd_resolution_init == 0 } {
        foreach mod $MODS {
            foreach mcmd [$mycf public_commands $mod] {
                lappend cmd_list($mcmd) $mod
            }
        }
        #parray cmd_list
        set cmd_resolution_init 1
    }
    #puts "MYUNKNOWN: cmd: $cmd, args: $args"
    set possible_cmd [array names cmd_list ${cmd}*]
    #puts "PC: $possible_cmd"
    set plen [llength $possible_cmd]
    if { $plen == 0 } {
        ## errorcode -2 is a special "invalid command" code
        return -code 1 -errorinfo "invalid command name \"$cmd\"" -errorcode -2
    } elseif { $plen > 1 } {
        set str "ambiguous command name \"$cmd\". Possibilities are:  "
        foreach cl $possible_cmd {
            foreach mod $cmd_list($cl) {
                append str "'$mod $cl' "
            }
        }
        return -code 1 -errorinfo $str -errorcode -2
    } else {
        ## plen == 1
        if { [llength $cmd_list($possible_cmd)] != 1 } {
            set str "ambiguous command name \"$cmd\". Possibilities are:  "
            foreach mod $cmd_list($possible_cmd) {
                append str "'$mod $possible_cmd' "
            }
            return -code 1 -errorinfo $str -errorcode -2
        } else {
            set ncmd ""
            if { $args eq "" } {
                set ncmd "$cmd_list($possible_cmd) $possible_cmd"
            } else {
                set ncmd "$cmd_list($possible_cmd) $possible_cmd $args"
            }
            puts "Resolving command $cmd to: $ncmd"

            set n [catch {$SLAVE eval [list uplevel #0 $ncmd]} rc]
            if { $n } {
                return -code 1 -errorinfo $rc -errorcode -2
            }
            return $rc
        }
    }
}
proc update_ext_sta_timing { } {
    variable mycf
    puts "ERROR - call to UEST!!"
    error "UEST!!" "" -1
}

proc update_timing { } {
    variable up_obj
    notice "UT!!"
    return [$up_obj update_timing]
}

proc old_update_timing { } {
    variable mycf
    notice "Old UT!!"
    return [$mycf update_timing]
}

proc exit { args } {
    variable up_obj
    puts "EXIT - MAIN EXIT!"
    $up_obj sync_nodes 1 "Exit requested - waiting for Remote Nodes to finish."
    $up_obj exit_plugins
    $up_obj end_gui
    if { [string length [string trim $args]] == 0 } {
        ::quit 0
        ::exit
    } else {
        ::quit $args
        ::exit $args
    }
}

proc setinst { cnt } {
    variable num_inst 
    variable SLAVE
    variable CUR_INST
    variable MODS
    variable mycf
    variable up_obj
    variable up_proxy

    if { $num_inst < $cnt } {
        puts "Error: Instance $cnt does not exist!"
        return -1
    }

    if { $cnt == $CUR_INST } {
        ## already there
        return $cnt
    }


    ## remove all old aliases - uncomment when RMT is exposed
    #set ns [cur_ns]
    #foreach {obj alias} [array get ${ns}::RSLAVES] {
        #puts "old alias: $obj - $alias"
        #interp alias $SLAVE $alias "" {}
    #}

    set CUR_INST $cnt
    set ns [cur_ns]

    #puts "Selecting Instance $cnt!"

    foreach mod $MODS {
        #puts " interp alias $SLAVE $mod ${ns}::$mod"
        interp alias $SLAVE $mod "" "${ns}::$mod"
    }

    set mycf ${ns}::cf
    set up_obj ${ns}::up_obj
    set up_proxy ${ns}::up_proxy

    ## Uncomment when RMT is exposed
    #foreach {obj alias} [array get ${ns}::RSLAVES] {
        #puts "New alias: $obj -> $alias"
        #set n2 [cur_ns]::$obj
        #interp alias $SLAVE $alias "" $obj
    #}
    return $cnt
}

## Returns "current" namespace
proc cur_ns { } {
    variable PREFIX
    variable CUR_INST

    return ${PREFIX}::ml${CUR_INST}
}

proc get_tm { tm } {
    variable mycf
    return [$mycf get_tm $tm]
}

proc up_opt { type count undo msg } {
    variable mycf
    return [$mycf up_opt $type $count $undo $msg]
}

proc get_worst_timing { iterm bterm } {
    variable mycf
    return [$mycf get_worst_timing $iterm $bterm]
}

proc get_iterm_timing_data { args } {
    variable mycf
    return [$mycf get_iterm_timing_data [join $args]]
}

proc save_timing { scx {debug ""}} {
    variable mycf
    return [$mycf save_timing $scx]
}

proc state { name } {
    set ns [cur_ns]
    foreach arg [array names ${ns}::STATE] {
        if { $arg == $name } {
            set var "${ns}::STATE($name)"
            return [set $var]
        }
    }
    return ""
}

proc set_state { name val } {
    set ns [cur_ns]
    set ${ns}::STATE($name) $val
}


proc rcmodels { {debug ""} } {
    variable mycf
    return [$mycf rcmodels]
}


proc newinst { } {
    variable num_inst 
    variable SLAVE
    variable CUR_INST
    variable tstamp
    variable MODS

    incr num_inst

    ## slightly more awkward, but leaves all namespace details to cur_ns
    incr CUR_INST
    set ns [cur_ns]
    incr CUR_INST -1
    ## end

    set ::ts $tstamp
    set ::MODS $MODS
    namespace eval $ns {
        variable STATE
        set STATE(ns) [namespace current]
        set STATE(db_loaded) 0
        variable TM_INST
        array set TM_INST { }
        #variable USER
        #set USER(usr) ""

        ## List of slaves/aliases
        variable RSLAVES
        array set RSLAVES { }

        ## Status of remote tasks
        variable RSTATUS
        array set RSTATUS { }

        ## Who is task X waiting on
        variable WRMT
        array set WRMT { }

        ## Who is waiting on task X
        variable WME
        array set WME { }

        variable RMTWL
        set RMTWL { }

        if { $apps::milos::num_inst == 1 } {
            set TM_INST(DB) "db"
        } 

        VOB ml "" $::ts
        CF cf [namespace current]::ml
        UPDATE up_obj [namespace current]::ml
        UPDATE_PROXY up_proxy 
        IGUI igui [namespace current]::ml
        foreach mod $::MODS {
            set cmd "package require ade::milos::$mod"
            eval $cmd
            set cmd "[string toupper $mod] $mod [namespace current]::ml"
            eval $cmd
        }
    }

    return $num_inst
}

proc appname { } {
    return "Milos"
}

proc rx { args } {
    package require ade::milos::rte 0.1
    set cmd "RTE::rnode $args"
    eval $cmd
}

## currently inactive, mirrored by VOB::new_rmt
proc RMT { name {async 0} {rmt_app "milos"} } {
    variable CUR_INST
    variable SLAVE
    variable MYLICENSE

    set lic [::eval ::zr.lic Checkout $MYLICENSE rr]
    if { $lic != 0 } {
        error "Invalid command name \"RMT\"" "" -1
    }

    set name [string trim $name]
    if { $name eq "" } {
        puts "Error: Remote Task name cannot be blank!"
        return 1
    }
    set ns [cur_ns]
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
        puts "Error: Remote Node $name already exists in work space $CUR_INST"
        return 1
    }
    if { ![info exists ${ns}::TM_INST(MSH)] } {
        msh::init
        set ${ns}::TM_INST(MSH) ""
    }
    set a [::RMT::new $n2 $async $rmt_app]
    set ${ns}::RSLAVES($a) $name
    set ${ns}::RSTATUS($a) 0
    set ${ns}::WRMT($a) { }
    set ${ns}::WME($a) { }

    interp alias $SLAVE $name "" ${a}
}

proc init_sta_cmds { slave } {
    variable STA_CMDS 
    variable STA_EXCEPT 
    variable EXT_CMDS
    variable INTERP_CMDS 

    ## to avoid a problem on startup..
    eval "proc ::debug \{ args \} \{ \} "
    
    set INTERP_CMDS [concat [interp eval $slave "info commands"] $EXT_CMDS $STA_EXCEPT]

    set STA_TMP [sta "info command"]

    set STA_CMDS { }
    foreach arg $STA_TMP {

        set idx [lsearch $INTERP_CMDS $arg]
        if { $idx == -1 } {
            lappend STA_CMDS $arg
        }
    }
    set STA_CMDS [lsort -increasing $STA_CMDS]
}

proc add_sta { cmd } {
    set sa [state sta_cmds]
    lappend sa $cmd
    set_state sta_cmds $sa
}

proc sta_proc { args } {
    variable STA_CMDS
    variable mycf
    variable dup_cnt

    set dup_plugin_cmd ""
    set dup_fname ""
    set cmd [string trim [lindex $args 0]]

    if { $args != "" } {
        set cmd "sta $args"
        set flag 0
        set rl 0
        set idx [string first "report_timing" $cmd]
        if { $idx >= 0 } {
            set rl 1
            set myarg [join $args]
            set idx [string first "report_timing" $myarg]
            if { [string first ">" $cmd] >= 0 } {
                ## have a report redirected to file
                set flag 1

                ## cut off "sta "
                set idx [string first ">" $myarg]
                set dup_plugin_cmd [string range $myarg 0 [expr {$idx - 1}]]
                #puts "dup_plugin_cmd: $dup_plugin_cmd ($myarg)"
                set dup_fname [string range $myarg [expr {$idx + 2 }] end ]

                set dup_fname "[string trim $dup_fname]_dup"
            } else {
                set dup_plugin_cmd [string range $cmd 4 end]
                set dup_fname "plugin_dup_${dup_cnt}.rpt"
                incr dup_cnt
            }
        } elseif { [string first "read_sdc" $cmd] >= 0 } {
            set rl 2
            ## capture everything but report_timing
            add_sta $cmd
        } else {
            ## capture everything but report_timing
            add_sta $cmd
        }

        if { [$mycf cf_get_state plugin_active] == 0 } {
            set dup_plugin_cmd 0
        }

        ## update sta_cmd list
        if { $dup_plugin_cmd ne "" } {

            set dup_plugin_cmd "$dup_plugin_cmd > $dup_fname"
            #puts "DUP cmd ($mycf): $dup_plugin_cmd"
            $mycf send_plugin_cmd TE $dup_plugin_cmd
        }

        if { $flag == 1 } {
            ::zr.zlog configure -enabled 0
        }

        set rc [ catch { ::eval $cmd } output ]

        if { $flag == 1 } {
            ::zr.zlog configure -enabled 1
        }

        if { $dup_plugin_cmd ne "" } {
            variable mycf
            $mycf get_plugin_result TE
        }

        if { $rl == 1 } {
            $mycf cmd_rlog "End: sta report_timing"
        } elseif { $rl == 2 } {
            $mycf cmd_rlog "End: sta read_sdc"
        }
            
        if { $rc != 0 } {
            error $output "" -1
        } else {
            return $output
        }
    } else {
        puts "Usage: sta cmd \[options]"
        puts "    Valid STA commands are: "
        foreach arg $STA_CMDS {
            puts $arg
        }

    }

}




#
# -- end of namespace apps
}
