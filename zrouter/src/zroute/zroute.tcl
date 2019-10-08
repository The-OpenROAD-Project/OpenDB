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
# zrouter library
#
# This file provides some initialization code
# User accessible commands are in zcmds.tcl
#

namespace eval zroute  {
    variable MSHC ""
    variable PORT ""
    variable ZROUTE ""  ;# ZROUTE object
    variable OPT        ;# options

    variable TID    ""  ;# task ID passed by MSH
    variable SID    ""  ;# SESSION ID
    variable BREAK      ;# should we break (suspend) this tid?

    variable EPOCH  1072908000 ;# 1/1/2004

    variable SLAVE      ;# keep track of slaves

    variable PARENT ""  ;# Parent address
    variable REGISTERED 0 ;# have we registered
    variable SERVER  ""  ;# RPC server handing the zui


    # technology modules
    variable TMODULES "db tiler ext"

    namespace export Init quit pause cont zdb
    namespace export whoami
    namespace export RegisterSlave UnRegisterSlave Break slaves mark

# --------------------------------------------------------------------------
# -- initialize zroute. Return non 0 if there is trouble
proc Init {opt} {
    upvar $opt OPT1
    variable OPT
    array set OPT [array get OPT1]

    variable PORT
    variable TMODULES
    variable ZROUTE
    variable TID
    variable SERVER
    global env

    # collect  the msh Task ID
    if [info exists env(MSHTID)] { set TID $env(MSHTID) }

    set port $OPT(port)
    set ZROUTE $OPT(zroute)

    # start a server for remote access
    set srv [LRPCServer srv -address $port -restricted 0]
    set SERVER [namespace origin $srv]

    # -- get back the actual port used
    set PORT [srv cget -port]

    # can we register this session?
    if { $OPT(nosession) == 0 } {
        if { ![session_register  $PORT] } {
            return 2
        }
    }

    # -- initialize the Session Manager
    set zs [namespace eval :: ZSession sm -evmgr $OPT(evmgr)]

    # HACK!!!
    # initalize packages 
    zroute::db::init_cmd

    # initialize documentation
    LMAN::export

    # load & initialize all technology modules
    foreach tm $TMODULES {
        # initialize the tm namespace, if necessary
        ${tm}::init
        # import commands from the tm namespace
        namespace import ${tm}::*
        # export each tm command
        foreach cmd [namespace eval ${tm} {namespace export}] {
            namespace export $cmd
        }
    }

    # pass options to msh (in case it comes up)
    msh::SetOpt [array get OPT]

    # initialize the debugger
    ::LDebug::init
    namespace eval :: namespace import ::LDebug::*

    # initialize the gdb debugging stuff (my become optional sometime)
    package require ade::gdb 1.0
    ::ade::gdb::init
    ::ade::gdb::init
    # -- import all gdb commands
    namespace eval :: namespace import ::ade::gdb::*

    return 0
}

proc cleanup {} {
    session_unregister
}

# -- mark this momemnt as a state
proc mark {state} {
    variable OPT
    if { !$OPT(mark) } { return }
    set now [clock clicks -milliseconds]
    set ts [expr ($now - $OPT(starttime))/1000.0]
    puts [format "~%.2f %s" $ts $state]
}

# ------------------------------------------- ERROR HANDLING
proc zerror {msg} {
    error zroute:$msg
}
proc quit {{exit_code 0}} {
    variable ZROUTE
    $ZROUTE Quit $exit_code
    # -- once more, just in case ...
    $ZROUTE Signal $exit_code
}
proc pause {{reason pausing}} {
    variable ZROUTE
    $ZROUTE Pause $reason
}
proc cont {} {
    variable ZROUTE
    $ZROUTE Cont
}
# ------------------------------------------- debugging
proc zdb {args} {
    variable OPT
    uplevel $OPT(dbg) [list Run $args]
}
# ------------------------------------------- USER ACCESSIBLE MSH functions
proc msh {args} {
    variable MSHC
    eval $MSHC $args
}

# ------------------------------------------- SLAVE REGISTRATION
proc RegisterSlave {tid sid {info ""}} {
    variable SLAVE
    variable SID
    variable BREAK

    if { $sid != $SID } {
        error "$sid: this session is different (SID=$SID)"
    }
    set SLAVE($tid) $info
    if ![info exists BREAK($tid)] {
        set BREAK($tid) 0
    }
    array set S $info
    debug ZR:R "$tid: remote slave registered"
    return OK
}

# mark a task breakable
proc Break {tid} {
    variable BREAK
    if ![info exists BREAK($tid)] {
        set BREAK($tid) 0
    }
    incr BREAK($tid)
}

proc UnRegisterSlave {tid sid} {
    variable SLAVE
    variable SID
    variable BREAK
    if { $sid != $SID } {
        error "$sid: this session is different (SID=$SID)"
    }
    if ![info exists SLAVE($tid)] {
        error "$tid: slave not registered"
    }
    # do not allow the task to unregister, if break has been requested
    if { $BREAK($tid) } {
        incr BREAK($tid)
        debug ZR:R "$tid: remote slave is ready for commands"
        return BREAK
    }
    unset BREAK($tid)
    unset SLAVE($tid)
    debug ZR:R "$tid: remote slave unregistered"
    return OK
}

proc slaves {{match *}} {
    variable SLAVE
    set res ""
    foreach tid [array names SLAVE $match] {
        lappend res $tid $SLAVE($tid)
    }
    set res
}
# ---------------------------------------------------------------------------

# ---------------------------------------------------------- COMMANDS

# ----------------------------------------------------- FILE BROWSING
# -- produce a list of files
proc files_list {{dir .}} {
    glob -nocomplain [file join $dir *]
}

# -- return the cwd
proc files_pwd {} {
    pwd
}

# ----------------------------------------------------- SESSION INFO
# -- return who am I and some session info
proc whoami {} {
    variable PORT
    variable SID
    variable TID
    variable OPT
    global env
    global argv0
    
    list host [lindex [split [info hostname] .] 0]  \
	port $PORT user [LPlatform user] \
        cwd [pwd] \
	pid [pid]  \
	argv0 $argv0 sid $SID tid $TID stime $OPT(sstarttime)
}
# -------------------------------------------------------------------------
# register this process to the parent of the session
# the session id contains the session name and the address of the server
proc session_register1 {zs1 port _sid _parent} {
    variable TID
    variable REGISTERED
    if { [ catch {
            set zs [split $zs1 :] ;
            set parent [lindex $zs 0] ;
            set sid    [lindex $zs 1] ;
        } rc] || ($sid == "") } {
        warning zroute "$zs1: malformed ZSESSION variable"
        warning zroute "cannot register this session"
        return BADVAR
    }
    set rpc [LRPCClient #auto]
    if {[catch {$rpc Connect $parent} rc]} {
        warning zroute "$parent: cannot connect to master: $rc"
        return DEADSERVER
    }
    set info [whoami]
    if [catch {$rpc Run RegisterSlave $TID $sid $info} rc] {
        $rpc delete
        warning zroute "$parent/$sid: failed to register to $parent"
        warning zroute "$parent/$sid: $rc"
        warning zroute "I cannot register this session"
        return REGFAIL
    }
    $rpc delete
    # return the session id
    upvar $_sid sid1
    set sid1 $sid
    upvar $_parent p ; set p $parent
    set REGISTERED 1
    return OK
}

# attempt to register our session
# return the success
proc session_register {port} {
    global env
    variable SID
    variable PORT
    variable PARENT

    # is there a need to register?
    if [info exists env(ZSESSION)] {
        set zs $env(ZSESSION)
        set rc [session_register1 $zs $PORT sid parent]
        # have we succeeded to register?
        switch $rc {
            OK {
                set SID    $sid 
                set PARENT $parent
                return 1
            }
            BADVAR  {                ;# ignore a bad variable
            }
            default {return 0}
        }
    }
    # I gues we are a new top level zroute. So we will create our own SID
    set SID [mksessionid]
    set env(ZSESSION) $port@[lindex [split [info hostname] .] 0]:$SID
    return 1
}

# try unregister our session. 
# master may refuse our unregistration, if debugging has been 
# requested by the user
proc session_unregister {} {
    variable PARENT
    variable SID
    variable TID
    variable REGISTERED

    if { !$REGISTERED } { return OK}

    if { $PARENT == "" } { return OK}
    if { $TID == "" } { return  OK}

    set rpc [LRPCClient #auto]
    if {[catch {$rpc Connect $PARENT} rc]} {
        set REGISTERED 0
        return OK
    }
    if [catch {$rpc Run UnRegisterSlave $TID $SID} rc ] {
        warning zroute "$TID/$SID: failed to unregister with $PARENT"
        warning zroute "$rc"
        set REGISTERED 0
        return OK
    }
    $rpc delete
    if { $rc == "OK" } {
        set REGISTERED 0
    }
    return $rc
}


# Send a command to the master
proc master_run {cmd} {
    variable PARENT

    if { $PARENT == "" } { error "not registered to master yet" }

    set rpc [LRPCClient #auto]
    if {[catch {$rpc Connect $PARENT} rc]} {
	$rpc delete
	error $rc
    }
    if [catch {eval $rpc Run $cmd} rc] {
	$rpc delete
	error $rc
    }
    $rpc delete
    return $rc
}

proc mksessionid {} {
    variable EPOCH
    format "%x%x" [expr [clock seconds] - $EPOCH] [pid]
}

}
