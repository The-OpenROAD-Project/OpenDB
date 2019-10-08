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

package require mtlib 1.0
package provide MSH 1.0
package require mshlic 1.0

#       Class: MSH
# Description: 
#      Author: Manolis M. Tsangaris
#
itcl::class MSH {
    inherit LProgram
    # -------------------------------------------------- Public Variables --
    public variable cf "" ;# configuration file
    # -------------------------------------------------- Public Methods ----
    public method quit { } {
        after idle "$this Signal 0"
    }
    public method echo {args} {
        return $args
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable server  ""     ;# the controller or the slave servers
    protected variable cli ""         ;# the cli user input
    protected variable progname
    protected variable rpc ""         ;# the client rpc based server
    protected variable mdsmonitor ""  ;# MDS monitor
    protected variable MDS            ;# MDS attribute information
    protected variable lic ""         ;# MSH license
    protected variable EXIT_TIMEOUT 60;# forced exit(1) after this time on quit
    protected variable slave ""
    protected variable MAXSLAVES 30   ;# HARD LIMIT to MPL
    protected variable MPL            ;# Desired MPL
    protected variable smpxmonitor "" ;# smpx monitor
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        option cli        -i          "run in interactive mode"
        option server     -s          "force msh server mode (default)"
        option slave      -slave      "start msh slave"
        option check      -w          "wait and exit when stdin is closed"
        option logdir     -l    dir   "where to put logs" logs/msh
        option info       -info       "print server address and other info"

        option profile    -p  profile "MSH profile" "default"
        option rje        -rje  auto|lsf|sh|ssh  "RJE type" auto
        option mpl        -N  int     "number of slaves to use (max)" ""
        option queue      -q  queue   "rje queue to use" ""
        option rjeopts    -ro  opts   "rje options (to submit command)"

        option address    -a  port@host "msh server address" 6740-@
        option logging   -d level:level:.. "MSHD logging level" "notice:warning"
        option mdlogging -dm level:level.. "MDSD logging level" "notice:warning"
        option cpath      -cp  dir:dir:.. "MDSD cache path" /tmp:.
        option mdsopts    -mds  options "MDSD options" ""
        option hosts      -hosts  "hostlist" "lists of hosts to use" ""
        option lic        -L license  "license file" ""
        option slave_timeout   -st time  "slave idletime timeout (sec)" 60
        option master   -master port@host  "master address" ""
        option tag     -tag tag "symbolic name for the slave" slave
        option smpx     -smpx "spawn an SMPX server"
    }
    destructor {
        LLog msh notice "$this: shutting down [pid] on [info hostname]"
        foreach f "cli rpc mdsmonitor server slave smpxmonitor" {
            if { [set $f] != "" } {
		LLog msh debug "deleting $f"
                [set $f] delete
            }
        }
        notice msh ".----"
    }
    # -------------------------------------------------- Private Methods ---
    # -- setup the mds server and our rpc server
    protected method servers_setup {opt cf} {
        upvar $opt OPT
	upvar $cf CONF

        # -- start the mds server
        set mcmd [list mdsd -w -i -l $OPT(logdir)/mds.log -d $OPT(mdlogging)]
        lappend mcmd -c $OPT(cpath)
        set mcmd [concat $mcmd $OPT(mdsopts)]
        mds_start $mcmd

        # -- start our rpc server
        set rpc [LRPCServer $this.cs -address $OPT(address) -restricted 0]
        $rpc configure -debug 0

        # -- start the msh controller
        set server [VRCMain $this.vrc \
	    -default $OPT(profile) -cf [array get CONF]\
	    -slavetimeout $OPT(slave_timeout) -logdir $OPT(logdir)\
	    -slaveDebugLevel $OPT(logging) -mpl $MPL]

	if { $OPT(smpx) } {
	    set smpxcmd "smpx -w -info -log $OPT(logdir)/smpx.log"
	    smpx_start $smpxcmd
	}

        set server
    }

    protected method mds_start {mdscmd} {
        LLog msh notice "starting mdsd: $mdscmd"

        set mdsmonitor [LProcMonitor #auto -wait 1]
        # start mds and wait for it to send attributes
        set mds [$mdsmonitor Start $mdscmd]
        array set MDS [$mdsmonitor Info $mds]
	set MDS(smpx) {}
        set MDS(fp) $mds
    }
    protected method smpx_start {smpxcmd} {
        set smpxmonitor [LProcMonitor $this.smpxmon -wait 1]
        # -- start smpx and wait for it to send attributes
        set smpx [$smpxmonitor Start $smpxcmd]
        array set SMPX [$smpxmonitor Info $smpx]
	set MDS(smpx) $SMPX(address)
    }

    public method cli_print {level msg} {
        if { $level != "-"  } {
            puts -nonewline "$level: "
        }
        puts $msg
    }

    # -- initialize the license manager
    protected method lic_check {_lic} {
        upvar $_lic LIC
	set LIC(mpl) ""

        if { $lic == "" } {
            set lic [ADELicense $this.lic]
        }

        if { [$lic Init R] != 0 } {
            puts stderr "$R(msg)"
            return 1
        }
	set facility ade.msh
        set r [$lic Checkout $facility R]
        if { $r != 0 } {
	    # -- checkout failed
            puts $R(msg)
            return $r
        }
        # -- request the MPL
        set r  [$lic Checkout ade.msh.mpl R]
        if { $r != 0 } {
            puts stderr $R(msg)
            return $r
        }
	# -- is there an MPL defined? 
	if [info exists R(param)] {
	    set LIC(mpl) $R(param)
	}
        set r
    }

    # -- read & process configuration file
    # -- return the msh portion of it
    protected method getconf {cf} {
	# -- default values
	array set CONF "rje auto queue normal nproc {} rjeopts {}"
	array set CONF "profiles {} hosts localhost mpl {}"
	array set CONF "slave_timeout 60"

	# -- is there a real configuration file?
        if ![file exists $cf] { 
	    notice msh "$cf: config file does not exist. Using defaults"
 	    return [array get CONF]
	}

	# -- read conf file
        if { [catch { array set CC [readconf $cf] } rc ]  } {
            LLog msh warning "$cf: errors: $rc"
        }
	# -- get the msh section
	if [info exists CC(msh)] {
	    if [catch {array set CONF $CC(msh)} rc] {
		LLog msh warning "$cf: errors: $rc"
	    }
	}
	array get CONF
    }

    # -- read configuration file and remove comments
    protected method readconf {cf} {
	set out [exec cat $cf]
	regsub -all {#[^\n]*\n} $out "\n" out
	set out
    }

    # -- Process the profile information based on command line opts
    # -- 1. User options about profile settings (queue, rje, etc),
    # --    only apply on the default profile.
    # -- 2. The rest of the profiles is unaffected.
    # -- CONF: the configuration file settings
    # -- OPT: the command line options (which override default profile)
    protected method profile_apply {opt cf} {
	upvar $opt OPT
	array set CONF $cf

	array set P $CONF(profiles)

	unset CONF(profiles)     ;# what is left is only global profile settings
	# -- set Q to the global profile
	array set Q [array get CONF]
	
	# -- apply the configuration file options to the global profile
	foreach opt "rje queue rjeopts mpl slave_timeout" {
	    if [OptionSet $opt] {
		set Q($opt) $OPT($opt) 
		notice msh "setting $opt to $OPT($opt) (command line)"
	    }
	}
	if { $Q(nproc) == "" } { set Q(nproc) $MAXSLAVES }
	# -- merge the profiles to the global one
	foreach class [array names P] {
	    # -- start with the global profile
	    array set P1 [array get Q]
	    # -- override with profile definition
	    array set P1 $P($class)
	    if { $P1(nproc) == "" } { set P1(nproc) $MAXSLAVES }
	    set P($class) [array get P1]
	    unset P1
	}
	# -- remember the global (default) profile
	set P(default) [array get Q]

	# -- return the new profile definitions
	array get P
    }

    # -- manipulate the number of slaves
    # -- np : number of slaves set in the command line/profile
    # -- mpl: licensing limit of the number of slaves
    protected method mpl_adjust {np lpl} {
	notice msh "adjusting mpl: cmd/conf=$np license=$lpl"

	# -- adjust license mpl first
	# -- not specified in license file?
	if { $lpl == "" } { set lpl $MAXSLAVES }
	# -- too much?
	if { $lpl > $MAXSLAVES } { set lpl $MAXSLAVES }

	# -- adjust default value from the command line
	if { $np == "" } { set np $MAXSLAVES }
        if { $np <= 0 } { set np $lpl }

	# -- exceeding licensed limit?
        if { $np > $lpl } {
            set np $lpl
        }
	set np
    }
    # ----------------------------------------------- Interface Implementation
    # ----------------------------------------------- CallBacks
    protected method vRun {opt} {
        upvar $opt OPT

	# -- check for license , get MPL and other info
	if { [lic_check LIC] != 0 } {
            Signal 1
	    return
	}

	# --- IS IT A SLAVE?
        if { $OPT(slave) } {
            set slave [Slave #auto -opts [array get OPT]]
            $slave Bind <<done>> $this eExit
            return
        }
	# -- MAIN MSH SERVER STARTS HERE

	# -- setup the logs
        exec mkdir -p $OPT(logdir)
        LLogger::Auto -logfile $OPT(logdir)/msh.log
        LLogEnable $OPT(logging)
        notice msh "-------------------------"

        # monitor stdin for eof. In that case exit
        if {$OPT(check)} { 
            fileevent stdin r "$this eChannelCheck stdin"
        }
	# -- read & apply the profile
	array set CF [getconf $cf]
	array set CONF [profile_apply OPT [array get CF]]
	array set D $CONF(default)
	set MPL [mpl_adjust $D(mpl) $LIC(mpl)]

        notice msh "msh server starting"
    	notice msh "mpl set to '$MPL'"
	notice msh "options: [array get OPT]"

        set server [servers_setup OPT CONF]

        if { $OPT(cli) } {
            set cli [LSafeCLI $this.cli -ps1 "msh> " -oneof exit]
            $cli Bind exit   "$this Signal 0"
            $cli Bind quit   "$this Signal 0"
            $cli Bind status "$server Info"
            $cli Bind test   "$server Test"
            $cli Bind run    "$server Run"
            $cli Bind source "$server Source"
            $cli Bind tasks  "$server Tasks"
            $cli Bind ts     "$server TaskStatus"
            $cli Bind js     "$server Status"
            $cli Alias help  "info commands"
            $cli Alias ?     "set errorInfo"
            $cli Start
        }

	set sign [list msh [$rpc Address] mds $MDS(address) \
	    mrc $server nproc $MPL smpx $MDS(smpx)]
	notice msh "server info: $sign"

        if { $OPT(info) } {
            puts $sign
            flush stdout
        }
    }
    # check for end of file status and exit on EOF
    public method eChannelCheck {fp} {
        if { [gets $fp line] == -1 } {
            LLog msh notice "cli peer exited. Quiting"
            catch {close $fp}
            Signal 0
        }
    }
    # -- try to exit cleanly, but schedule a foreful exit as well
    public method eExit {} {
	$this Signal 0
	after [expr $EXIT_TIMEOUT*1000] exit
    }
    # -------------------------------------------------- Common Initialization
}

proc bgerror {err} {
    global errorInfo
    if [catch {LLog msh warning bgerror:$err:$errorInfo} rc] {
        puts stderr "bgerror: ($err:$errorInfo) $rc"
        flush stderr
    }
}
