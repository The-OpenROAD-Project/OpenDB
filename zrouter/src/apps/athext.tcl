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
package provide ade::athext 1.0

namespace eval apps { 
    variable cur_app "::apps::athext"
}

namespace eval apps::athext {

variable num_inst 0
variable PREFIX "::apps::athext"

variable MYLICENSE "milos.debug"
variable SLAVE ""
variable CUR_INST 0

variable EXT_CMDS "cd encoding exec fconfigure file glob load open pwd source"

variable INTERP_CMDS { }
variable STA_CMDS { }
variable STA_EXCEPT { auto_execok auto_import auto_load auto_load_index exit \
    open socket unknown }

variable MODS { ext pdb }

variable tstamp

variable dup_cnt 0
variable mycf
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


    # -- expose the read command
    package require ade::milos::vob 0.1
    package require ade::milos::dobj 0.1
    package require ade::milos::cf 0.1

    set SLAVE $slave

    ## setup I/O stuff
    $SLAVE eval "set errorInfo \"\""
    $SLAVE eval "set errorCode \"\""

    $SLAVE hide puts
    interp alias $SLAVE puts "" apps::athext::puts
    interp alias $SLAVE help "" apps::mod_help

    #stdout and stderr not really available..
    #foreach chan {stdin stdout stderr} 

    ## stdin could be useful for prompting in user scripts
    foreach chan {stdin} {
        interp share "" $chan $SLAVE
    }


    ## other setup stuff
    interp alias $SLAVE exit "" ::apps::athext::exit
    interp alias $SLAVE quit "" ::apps::athext::exit


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
        if { [file channels $fp] != $fp } {
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
            if { [file channels $fp] != $fp } {
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

proc exit { args } {
    variable mycf
    $mycf sync_nodes 1 "Exit requested - waiting for Remote Nodes to finish."
    if { [string length [string trim $args]] == 0 } {
        ::quit 0
    } else {
        ::quit $args
    }
}

proc setinst { cnt } {
    variable num_inst 
    variable SLAVE
    variable CUR_INST
    variable MODS
    variable mycf

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

        if { $apps::athext::num_inst == 1 } {
            set TM_INST(DB) "db"
        } 

        VOB ml "" $::ts
        CF cf [namespace current]::ml
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
    return "AthExtract"
}

#
# -- end of namespace apps
}
