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

package provide MSH 1.0

#
# This object implements the slave running on the node
#

#
#       Class: Slave
# Description: Slave server process
#      Author: Manolis M. Tsangaris -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
# This object implements a slave server which accepts commands
# and runs remote jobs
#
itcl::class Slave {
    inherit LBinder LObject
    # Event <<done>>
    # -------------------------------------------------- Public Variables --
    public variable address "8786@"  ;# port we are listening to
                                     ;# @ will force INADDR_ANY
    public variable SHELL   "ksh"    ;#
    public variable tid     "-"      ;# job id being run
    public variable opts "" { array set OPT $opts }
    # -------------------------------------------------- Public Methods ----
    public method Configure {args} {
	eval configure $args
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable server         ;# local RPC server
    protected variable exec ""
    protected variable sfp ""         ;# server socket
    protected variable out ""         ;# task output
    protected variable me ""
    protected variable TASK ""        ;# timeout task
    protected variable REASON ""      ;# reason for termination
    protected variable OPT            ;# options
    protected variable KTASK          ;# keepalive task
    protected variable KTIMEOUT 30    ;# keepalive timeout
    protected variable LDIRTIMEOUT 10 ;# log dir check timeout
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set me [lindex [split $address .] 0]
	set TASK  [LSTask $this.timeout  -start 0 -cmd "$this eTimeout"]
	set KTASK [LSTask $this.kalive  -start 0 -cmd "$this eKeepAlive"]
	$KTASK configure -timeout [expr $KTIMEOUT * 1000]

        if ![slave_setup reason] {
            LLog slave warning "failed to setup ($reason)"
	    error "failed to setup ($reason)"
	}
    }

    destructor {
        LLog slave notice "going down"
        $server delete
        catch {$exec TreePrint} rc
        LLog slave notice "slave process tree:\n$rc"
        $exec delete
	$TASK delete
	$KTASK delete
	if { $sfp != "" } { catch {close $sfp} }
        LLog slave notice "exiting at [clock format [clock seconds]]"
    }
    # -------------------------------------------------- Private Methods ---
    # -- register this slave to master vrc
    protected method register {master rr} {
	upvar $rr reason
        if { $OPT(master) == "" } { return }
        foreach {port host} [split $OPT(master) @] { break }
	# -- can you connect back to your parent?
	if [catch {socket $host $port} rc ] {
	    set reason "failed to connect to $host@$port"
	    append reason "($rc - $host: is there a firewall/server died?)"
	    return 0
	}
	set sfp $rc
        fconfigure $sfp -buffering line
        fileevent $sfp r "$this eMasterResponse $sfp"

        # -- send a status line
        puts $sfp "slave $address [info hostname] $OPT(tag)"
        LLog slave notice "registered to $OPT(master)"

	# -- now start the keepalive task
	$KTASK Start

        set reason "registered to $OPT(master)"
	return 1
    }

    # -- setup all slave services
    protected method slave_setup {rr} {
	upvar $rr reason

        # -- start the slave RPC server -- try a few sockets
        set server ""
        # give the rpc server the flexibility to bind any address
        if [catch {LRPCServer $this.s -address $address} res] {
            set reason "$res:\nfailed to bind slave socket: $address"
	    return 0
        }
        set server $res

        # -- get the real port & hostname
        set address [$server Address]

        # -- setup the logs (now that server socket is bound)
        set logfile $OPT(logdir)/$address
        regsub {\.$} $logfile "" logfile       ;# remove trailing .

	# -- check if the logfile can be created
	set logfile [check_log $logfile.log]

	# -- setup the logger
        set log [LLogger $this.log -logfile $logfile]
	$log Export
        $log Enable $OPT(logging)

        notice slave "-------------------------------------------"
        notice slave "slave starting on [info hostname]: $address"
        notice slave "slave starting at [clock format [clock seconds]]"
        notice slave "slave log at '$logfile'"

        # -- locate SHELL & create exec object
        global env
        foreach dir [split $env(PATH) :] {
            set s [file join $dir $SHELL]
            if [file executable $s] {
                set exec [Exec $this.exec -SHELL $s]
                LLog slave notice "found shell $s"
                break
            }
        }
        if { $exec == "" } {
            die "cannot find any suitable: '$SHELL'"
        }

        $exec Bind <<newstate>> $this eNewState
        $exec Bind <<error>>    $this eOutput
        $exec Bind <<output>>   $this eOutput

        # -- bind RPC 
        $server Bind test   $this eRCMDTest
        $server Bind start  $this eRCMDStart
        $server Bind configure $this Configure
        $server Bind status $exec  Status
        $server Bind state  $exec  State

	# -- see if we can register to master
        register $OPT(master) reason
    }

    # -- check if the log file can be written. Normally the master
    # -- will create a directory where the logs should reside.
    # -- On high traffic servers, this may not be the case due to
    # -- NFS delays.
    protected method check_log {logfile} {
	global env
	set logf   [file tail $logfile]
	set logdirs [list [file dirname $logfile] $env(HOME) /tmp]
	foreach logdir $logdirs {
	    # -- try 3 times if the directory has shown up there
	    for {set n 0} { $n < 6 } {incr n } {
	    	if [file exists $logdir] {
		    set logfile [file join $logdir $logf]
		    return $logfile
		}
		# -- wait for the directory to appear
		after [expr $LDIRTIMEOUT* 1000]
	    }
	    # -- try the next alternative
	}
	puts stderr "warning: slave: failed to create log in '$logdirs'"
	set logfile /dev/null
	puts stderr "warning: slave: suppressing logging"
	set logfile
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- vrc remote commands
    # -- perform a local test
    public method eRCMDTest {} {
        return "ok $address"
    }
    public method eRCMDStart {cmd {tid 0000} {stdin ""} {env ""} {timeout 0}} {
        array set S [$exec State]
	set REASON ""

        LLog slave notice "$me: run <$cmd> $tid stdin=<$stdin>"
        LLog slave notice "$me: env"
	array set e $env
	foreach v [array names e] {
	    LLog slave notice [format "%16s: %s" $v $e($v)]
	}
        switch $S(state) {
            run - queue {
                error "cannot start: node in $S(state) mode"
            }
        }
        configure -tid $tid
        $exec configure -program $cmd
        $exec configure -env $env
        $exec configure -stdin $stdin
        $exec Start 0
	if { $timeout != 0 } {
	    LLog slave notice "$tid: set timeout of $timeout sec"
	    $TASK configure -timeout [expr $timeout*1000]
	    $TASK Start
	}
        return ""
    }
    # -------------------------------------------------- exec events
    # -- a response from the master
    public method eMasterResponse {fp} {
        if { [gets $fp line] == -1 } {
            LLog slave warning "server $OPT(master) died"
            catch {close $fp}
            Event <<done>>
            return
        } 
        LLog slave notice "unexpected master repsonse: $line"
    }

    # -- job getting to a new state
    public method eNewState {state} {
        # -- is the job done?
        LLog slave notice "$tid: newstate: $state"
        if { $state == "exit" } {
	    # -- cancel pending timeout
	    $TASK Cancel

	    # -- notify server
            puts $sfp "free $address $tid"
            puts $sfp "$out"
	    if { $REASON != "" } { puts $sfp $REASON }
            puts $sfp "."
            set out ""
	    set REASON ""
        }
    }

    # -- Output generated from the job
    public method eOutput {line} {
        append out $line \n
        LLog slave notice "job output: $line"
    }

    # -- handle task timeout
    public method eTimeout {} {
	set TASK ""
	set REASON "TIMEOUT"
	LLog slave notice "$tid: timed out"
	LLog slave notice [$exec TreePrint]
	$exec Stop
	LLog slave notice [$exec TreePrint]
	# -- eNewState will take care of it
    }
    # -- called to send a keep alive message
    public method eKeepAlive {} {
	notice slave "sending keepalive"
        if ![catch {puts $sfp "keepalive"}] {
	    $KTASK Start
	    return
	}
	notice slave "master died"
	Event <<done>>
    }

    # -------------------------------------------------- test
    # -------------------------------------------------- Common Initialization
}
