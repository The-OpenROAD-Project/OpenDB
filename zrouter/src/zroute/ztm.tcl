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

package provide ade::main 1.0

#
#      Author: Manolis M. Tsangaris
#
# Generate the wrapper classes for the zroute technology modules
#

namespace eval zroute::ztm {
    variable TMCLASSES "" 

# generate ITCL classes based on the C++ API exported *_tm classes

proc init {} {
    variable TMCLASSES
    set ns ::zroute
    set tmcmds [namespace eval $ns info commands *_tm]
    foreach tm $tmcmds { 
        set usage [lindex [eval ${ns}::${tm}_usage] 0]
        set def   [mkztm ${ns}::$tm $usage class]

        # -- check if for some reason this class exists already
        if { [itcl::find class ::$class] != "" }  {
            warning  mktm "TM $class: already exists. Conflict?"
            continue
        }
        # now evaluate the command to define the class
        uplevel #0 $def
        lappend TMCLASSES $class
    }
}

proc classes {} {
    variable TMCLASSES
    set TMCLASSES
}

# ------------------------------------------------------------------------
# generate a itcl class definition for each tech module
proc mkztm {tm usage cl} {
    upvar $cl CLASS
    set CLASS [string range [namespace tail $tm] 0 end-3]
    set CLASS_tm $tm
    set def {
itcl::class $CLASS {
    # NOTE: THIS IS AN AUTOMATICALLY GENERATED CLASS
    # FROM ${CLASS}_tm ZROUTE C++ API
    inherit ZTM
    # ---- public variables
$PUBLICVARS

    destructor {
       \$TM delete
    }
    # ---- public methods
$METHODS
}
}

    set PUBLICVARS [mkpublic  $tm $usage]
    set METHODS    [mkmethods $tm $usage]

    subst -nocommands $def
}

# generate the public variables
proc mkpublic {tm usage} {
    array set USAGE $usage
    if ![info exists USAGE(configure)] { 
        return ""
    }
    array set N $USAGE(configure)
    set def ""
    foreach VAR $N(inputs) {
        array set A $VAR
        set var $A(var)
        append def "   public variable $var {$A(default)}"
        append def " { ztm_config $var \$$var ; set $var }\n"
        unset A
    } 
    append def "   public method cget {{var {}}} {\n"
    append def "      ztm_cget [list $USAGE(configure)] \$var\n"
    append def "   }\n"
    set def
}

# generate the public methods
proc mkmethods {tm usage} {
    array set USAGE $usage
    set def ""
    set methods "one two three"
    set usage {""}
    foreach method [array names USAGE] {
        # skip the constructor usage for now
        switch $method {
            configure - cget { continue }
        }
        # -- add the result variable option
        array set US $USAGE($method)
        lappend US(inputs) \
            {default "" type var var "result" usage "result array"}

        # append output variable settings (-var=)
        foreach ovar $US(outputs) {
            lappend US(inputs) [mk_ovar $ovar "%s="]
        }

        # --- protect the list from eval with an extra list
        set us [list [array get US]]
        unset US

        if { $method == "constructor" } {
            append def "
    $method {args} {
           set TM \$this.tm 
       set usage $us
           tm_constructor $tm \$TM \$usage \$args
    }
"
         } else {
            append def "
    public method $method {args} {
        set usage $us
        ztm_dispatch $method \$usage \$args
    }
"
        }
    } 
    set def
}

# generate a usage line for each output variable
proc mk_ovar {option {nvar "%s="}} {
    array set U $option
    set var $U(var)
    set nv [format $nvar $var]
    list var $nv default "" type var usage "output var: $U(usage)"
}

# -- END OF NAMESPACE EVAL
}

#
#       Class: ZTM
# Description: Generic ZROUTE Techology Module
#      Author: Manolis M. Tsangaris
#
# ZTM encapsulates a c++ technology object and provides all necessary
# TCL services, to make their implementation easier.
# In particular, it converts the usual "-option value" argument convention
# to a slotted array representation.
#
itcl::class ZTM {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable evmgr ""   ; # the event manager to receive events
    common zsession  ""        ; # ZROUTE session object
    # -------------------------------------------------- Public Methods ----
    public method ztest {} {
        foreach x {1 2 3 4 5} {
            zevent ztest x $x time [clock seconds]
        }
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable TM ""   ;# the technology module instance
    protected variable CCF     ;# constructor configuration
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
    }
    # -------------------------------------------------- Private Methods ---
    # ---------------------------------------------------------- CONSTRUCTOR
    # -- call the C++ constructor. Pass the right arguments--keep configuration
    protected method tm_constructor {tmclass instance usage argv} {
        array set U $usage
        array set Ain {}
        arg_parse constructor $usage $argv Ain

        set a [getArray Ain]
        if { [info command debug] == "debug" } {
            debug ZTC:A "ZTC:A [string range $tmclass 0 end-3] [string range $instance 0 end-3] $argv"
            debug ZTM:A "ZTM:A [string range $tmclass 0 end-3] [string range $instance 0 end-3] $a"
        } 

        # if the user wants to receive the return result ...
        if { [info exists Ain(result)]  && ( $Ain(result) != "" ) } {
            # -- create a var at the caller
            upvar 2 $Ain(result) Aout
            unset Ain(result)
        }

        # pass two more default arguments (db & session)
        # -- pass some "environment" variables
        set Ain(db)        [$zsession Resolve db]
        set Ain(session)   [$zsession Resolve session]

        # install default values for Aout
        set Aout(session) ""
        set Aout(db) ""

        $zsession NewTM $this
        # now call the constructor
        $tmclass $instance Ain Aout

        # take care of any return variables
        foreach flag [array names Ain *=] {
            upvar 2 $Ain($flag) V
            set nv [string range $flag 0 end-1]
            # set the user defined variable
            if [catch {set V $Aout($nv) } nrc] {
                set nrc "setting output var $nv (-$flag option):\n$rc"  
                return -code error -errorinfo $errorInfo "$this: $method: $nrc"
            }
        }

        # -- register any values returned by the constructor
        # -- this will overwrite the values of the name server though!
        foreach v "session db" {
            if {$Aout($v) != "" } {
                $zsession Register $v $Aout($v)
            }
        }
        # now register this tm to the session manager

        set instance
    }
        
    protected method getArray { name } {
        upvar $name arr
        set nm [array names arr]
        set aval ""
        foreach arg $nm {
            if { $arg == "args" } { continue }
            if { $arg == "result" } {
                if { [lindex [array get arr $arg] 1] eq "" } { continue } 
            }
            #append aval "[format "%-30s = " "$name\($arg\)"]"
            #append aval "[lindex [array get arr $arg] 1]\n"
            append aval "-$arg [lindex [array get arr $arg] 1] "
        }
        append aval "\n"
        return [string range $aval 0 1024]
    }


    # ---------------------------------------------------------- DISPATCH
    # call a c++ method. Pass Ain and Aout
    protected method ztm_dispatch {method usage argv} {
        array set U $usage
        array set Ain {}
        arg_parse $method $usage $argv Ain


        # if the user wants to receive the return result ...
        if { [info exists Ain(result)]  && ( $Ain(result) != "" ) } {
            # -- create a var at the caller
            upvar 2 $Ain(result) Aout
            unset Ain(result)
        }
        array set Aout {}

        if { [info command debug] == "debug" } {
            set a [getArray Ain]
            debug ZTC:A "ZTC:A [string range $TM 0 end-3] $method $argv"
            debug ZTM:A "ZTM:A [string range $TM 0 end-3] $method $a"
        } 

        if { ([info exists ::env(DRYRUN)]) && ($::env(DRYRUN) == 1) } {
            ## DRY RUN mode - set return variables to 0 so they exist
            foreach flag [array names Ain *=] {
                upvar 2 $Ain($flag) V
                set V 0
            }
            return 0
        } 

        global errorInfo
        set zt [ ::zr.zlog get_debug_log_config -module ZTIMER]
        if { $zt } {
            set timer_idx [::sys::startTimer]
        }
        if { [catch {$TM $method Ain Aout} rc ] } {
            if { $zt } {
                ::sys::showTimer "${TM}::${method}" ZTIMER A $timer_idx
            }
            return -code error -errorinfo $errorInfo "$this: $method: $rc"
        }
        # take care of any return variables
        foreach flag [array names Ain *=] {
            upvar 2 $Ain($flag) V
            set nv [string range $flag 0 end-1]
            # set the user defined variable
            if [catch {set V $Aout($nv) } nrc] {
                set nrc "setting output var $nv (-$flag option):\n$rc"  
                return -code error -errorinfo $errorInfo "$this: $method: $nrc"
            }
        }
        if { $zt } {
            ::sys::showTimer "${TM}::${method}" ZTIMER A $timer_idx
        }
        set rc
    }
    # ---------------------------------------------------------- configure
    # call the config method of tm
    # we should probably check for type here ..
    protected method ztm_config {var val} {
        set method configure
        array set Ain {}
        set Ain($var) $val  

        array set Aout {}

        global errorInfo
        if { [catch {$TM $method Ain Aout} rc ] } {
            return -code error -errorinfo $errorInfo "$this: $method: $rc"
        }
        return
    }
    # ---------------------------------------------------------- cget
    # -- call the config tm method
    protected method ztm_cget {usage flag} {
        set method cget
        set var [cget_check_usage $usage $flag]
        set Ain($var) ""
        array set Aout {}
        global errorInfo
        if { [catch {$TM $method Ain Aout} rc ] } {
            return -code error -errorinfo $errorInfo "$this: $method: $rc"
        }
        return "$Aout($var)"
    }
    # -- check the usage of the cget command
    private method cget_check_usage {usage flag} {
        array set U1 $usage
        set var $flag
        # remove the leading "-"
        regsub -- {^-} $var "" var
        set vars ""
        foreach option $U1(inputs) {
            array set U $option
            set nv $U(var)
            if { $nv == $var } { return $var }
            set V($nv) $U(default)
            set USAGE($nv) [array get U]
            unset U
        }
        error [usage_make cget USAGE "unknown option $flag"]
    }
    # ----------------------------------------------------------------------
    # each c++ method is described by the usage string
    private method arg_parse {method usage av v} {
        upvar $v V

        # normalize the usage
        usage_norm $usage USAGE V

        # N points to the part of the string to be parsed
        set N 0
        while { "@[set flag [lindex $av $N]]" != "@" } {
            if { ![string match -* $flag]  } { break }
            if { "$flag" == "-?"  } {
                error [usage_make $method USAGE ""]
            }

            # find all matching options to this flag
            set options [option_match $flag USAGE]

            # ambiguity?
            switch [llength $options] {
                0 { error [usage_make $method USAGE "unknown option $flag"] }
                1 { set var [lindex $options 0] }
                default {
                    error [usage_make $method USAGE "ambiguous option $flag"]
                }
            }
                # box  {
                    # 4 element list
                #   incr N 1
                #   set arg [lrange $av $N [expr $N+3]]
                #   incr N 4
                #   if { ![type_check V($var) $OPTION(type) $arg] } {
                #       error "$method: $flag: arg '$arg' not $OPTION(type)"
                #   }
                # } 
                # point  {
                    # 2 element list
                #   incr N 1
                #   set arg [lrange $av $N [expr $N+1]]
                #   incr N 2
                #   if { ![type_check V($var) $OPTION(type) $arg] } {
                #       error "$method: $flag: arg '$arg' not $OPTION(type)"
                #   }
                #} 
            array set OPTION $USAGE($var)
            switch $OPTION(type) {
                bool - boolean {
                    incr V($var)
                }
                default {
                            incr N
                    set arg [lindex $av $N]
                    if { ![type_check V($var) $OPTION(type) $arg] } {
                        error "$method: $flag: arg '$arg' not $OPTION(type)"
                    }
                }
            }
            incr N
        }
        set V(args) [lrange $av $N end]
        return 1
    }

    # -- do minimum prerix match -- handle the corner case
    private method option_match {flag uu} {
        upvar $uu USAGE
        set _opt [string range $flag 1 end]
        if { [info exists USAGE($_opt)] } { return $_opt }
        array names USAGE ${_opt}*
    }

    proc type_check {var type val} {
        upvar $var V
        switch $type {
            int            { scan $val "%d" V }
            double - float { scan $val "%f" V }
            string         { set V $val ; return 1 }
            box            { 
                # 4 coordinates
                set V $val
                if { [scan $val "%d %d %d %d" a b c d] != 4 } {
                    return 0
                }
                return 1
            }
            point          {
                # 2 coordinates
                set V $val
                if { [scan $val "%d %d" a b] != 2 } {
                    return 0
                }
                return 1
            }
            default { set V $val ; return 1}
        }
    }
    
    private method usage_make {method uu {msg ""}} {
        upvar $uu USAGE
        global argv0
        set res ""
        set me [file tail [file root $argv0]]
        if { $msg != "" } { append res $msg \n }
        append res "$method: usage: " \n
        set sep ""
        foreach var [lsort [array names USAGE]] {
            append res $sep
            array set OPTION $USAGE($var)

            set arg "$var ($OPTION(type))"
            set def "($OPTION(default))"
            if { "@$OPTION(default)" == "@" } { set def {("")} }

            switch $OPTION(type) {
                bool {
                    set arg ""
                    set def "false"
                }
                var  {
                    set arg "var"
                }
            }
            append res  [usage_print_opt -$var $arg $OPTION(usage) $def]
            unset OPTION
            set sep "\n"
        }
        set res
    }

    # -- print the option usage
    private method usage_print_opt {flag arg desc def} {
        if { [string length $arg] > 20 } {
            format "%12s %-15s \n%12s: %-27s %s" $flag $arg "" $desc $def
        } else {
            format "%12s %-15s : %-30s %s" $flag $arg $desc $def
        }
    }
    # -- generate the proper argument list for the tm constructor
    # -- assume that argv is just -attr val pairs
    private method mkconstr_args {argv aa} {
        upvar $aa A
        set n 0
        while { [set arg [lindex $argv $n]] != "" } {
            if { ![string match -* $arg] } { break }
            incr n
            set A([string range $arg 1 end]) [lindex $argv $n]
            incr n
        }
        set A(args) [lrange $argv $n end]
    }
    # -- normalize usage
    private method usage_norm {usage uu vv} {
        upvar $uu USAGE
        upvar $vv V
        array set US $usage
        # scan all inputs -- fix boolean defaults
        foreach option $US(inputs) {
            array set U $option ; set var $U(var)
            switch $U(type) {
                bool - boolean { 
                    set U(default) 0
                } 
            }
            set V($var) $U(default)
            set USAGE($var) [array get U]
            unset U
        }
    }
    # ----------------------------------------------- Interface Implementation
    # this routing is called by the C++ TM.h method (event(event, ...))
    # to post events, related with this tech module
    # the module itself, will post the event to the event manager
    protected method zevent {args} {
        if { $evmgr == "" } { 
            puts "zevent: $args"
        } else {
            lappend args tm $this
            $evmgr Post $args
        }
    }
    # ----------------------------------------------- CallBacks
    # -------------------------------------------------- Common Initialization
}

