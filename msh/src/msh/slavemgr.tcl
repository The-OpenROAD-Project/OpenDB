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
#       Class: SlaveMGR
# Description: Slave Manager
#      Author: Manolis M. Tsangaris  -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
# This object manages all remote slaves and runs in the msh process
#
itcl::class SlaveMGR {
    inherit LObject LBinder
    # Event <<slave-requested>>
    # Event <<slave-released>>
    # -------------------------------------------------- Public Variables --
    public variable class class
    public variable nproc    1  ;# -- how many slaves to enable
    public variable pslaves  0  ;# -- how many slaves to preallocate
    public variable port 8753   ;# -- port to listen to by slaves
    public variable queue       ;# -- task queue
    public variable logdir "logs/msh"   ;# -- log directory
    public variable RJE         ;# -- RJE interface
    public variable msh  "msh"  ;# -- the slave command
    public variable slavetimeout 0  ;# -- slave idle timeout
    public variable slaveDebugLevel notice  ;#
    # -------------------------------------------------- Public Methods ----
    public method SystemStatus {} {
        slave_status
    }
    # -- a slave has been granted
    # -- return false if the slave is (no longer) needed
    public method GrantedSlave {} {
        # -- nothing to run?
        if { [$queue Peek] == "" } { return 0 }

        # -- proceed to use an extra slave
	slave_granted
    }
    # -- number of slaves needed to achieve maximum parallelism (to nproc level)
    public method SlavesNeeded {} {
	slaves_needed
    }
    # -- return statistics
    public method Stats {} {
	list active $ASLAVES requested $RSLAVES free [array size FREE] \
	    totslaves $NSLAVES queue [$queue Size]
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable sfp
    protected variable SLAVE
    protected variable FREE        ;# set for each free slave
    protected variable JSLAVE      ;# where a task runs
    protected variable RSLAVES 0   ;# requested slaves (not active)
    protected variable ASLAVES 0   ;# active slaves
    protected variable NSLAVES 0   ;# number of slaves allocated
    protected variable SLAVE_TASK  ;# check slave health periodically
    protected variable PSLAVE      ;# keep track of pending slaves
    protected variable PSLAVE_TAG  ;# slave tag by job id
    protected variable PTIMEOUT 30000 ;# how often to check slaves
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set port [server_setup $port 100]
        if { $port == "" } { 
            error "failed to allocate port for listening"
        }
        # -- delay eSchecduleJob
        $queue Bind <<new>> after idle $this eSchecduleJob

	# -- see if we have to preallocate
	if { $pslaves } {
	    slave_request $pslaves
	}
	set SLAVE_TASK [LSTask $this.slave_check \
	    -timeout $PTIMEOUT -cmd "$this eSlaveCheck"]
	$SLAVE_TASK Start
    }
    destructor {
	notice slavemgr "shutting down control socket"
	$SLAVE_TASK delete

	# -- close the control socket
	catch {close $sfp}


	# -- disconect all sockets to remote slaves
	slave_goodbye

	notice slavemgr "waiting for remote slaves to cleanup"
	after 5000
        foreach slave [array names SLAVE] {
	    slave_release $slave "cleanup"
        }
	notice slavemgr "cleanup complete"
    }
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- slave registry
    protected method server_setup {port range} {
        for {set n 0} { $n < $range } { incr n } {
            set p [expr $port + $n]
            if ![catch {socket -server "$this eSlaveConnect" $p} sfp] {
                debug slavemgr "listening at $p"
                return $p
            }
        }
        warning slavemgr "failed to find listening socket"
        return ""
    }
    # -- create a slave using an RJE
    protected method slave_request {ns} {
	Event <<slave-requested>>
    }

    # -- Now that a slave slot is granted, run an msh slave on it.
    protected method slave_granted {} {
	# -- remember # of granted slaves
	incr RSLAVES
	slave_submit
    }

    # -- (re)submit a slave to rje
    protected method slave_submit {{oldtag ""}} {
	if { ($slavetimeout < 0) && ($slavetimeout != -1) } {
	    warning slavemgr "$class: slave_timeout=$slavetimeout"
	    set slave_timeout 60
	    warning slavemgr "$class: adjusting to $slavetimeout"
	}
	incr NSLAVES
	set tag $class.$NSLAVES
	set   cm  ""
	append cm "$msh -slave -l $logdir"
	append cm " -master $port@[info hostname]"
	append cm " -st $slavetimeout"
	append cm " -d $slaveDebugLevel"
	append cm " -tag $tag"
	notice slavemgr "$tag: executing: $cm"

	set jid [$RJE Run "$cm"]
	set PSLAVE($tag)    $jid
	set PSLAVE_TAG($jid) $tag

	if { $oldtag == "" } { 
	    notice slavemgr "$tag: slave was submitted as $jid"
	} else {
	    notice slavemgr "$tag: slave was resubmitted as $jid (for $oldtag)"
	}
	return 1
    }

    # -- a remote slave just connected to us
    # -- a remote slave has registered -- keep the connection open
    protected method slave_register {fp address rhost tag} {
	# -- no need to remember this slave anymore
	unset PSLAVE_TAG($PSLAVE($tag))
	unset PSLAVE($tag)
	set cmd "VRCSlave #auto -address $address -fp $fp -host $rhost"
	lappend cmd -tag $tag
	if [catch $cmd rc] {
	    global errorInfo
	    warning slavemgr "failed to allocate slave proxy: $errorInfo"
	    return 0
	}
        set SLAVE($fp) $rc
        set FREE($fp) 1
	$rc configure -idletimeout $slavetimeout 
	# -- note the number of active slaves & requested slaves
	incr ASLAVES
	incr RSLAVES -1

        notice slavemgr "$fp: slave registered ($rhost)"
        # --- oops! this does not work!
        ###$SLAVE($fp) Bind <<free>> $this eSlaveFree $fp

        $SLAVE($fp) Bind <<idle-timeout>> $this eSlaveIdled $fp

	# -- attempt to schedule a job there
	$this eSchecduleJob
	return 1
    }

    # -- compute slave status
    protected method slave_status {} {
        set s ""
	append s "-- SLAVES\n"
	append s "  $ASLAVES active -- $RSLAVES requested"
	append s " - [$queue Size] queue - $NSLAVES allocated\n"
        foreach sl [array names SLAVE] {
	    if [catch {$SLAVE($sl) Status} rc] {
		append s "$SLAVE($sl): ? ($rc)"
	    }  else {
		append s $rc
	    }
	    if { [info exists FREE($sl)] } {
		append s " IDLE"
	    } else {
		append s " BUSY"
	    }
	    append s "\n"
	}
	set s
    }

    # -- compute the number of extra slaves needed, in order
    # -- to achieve max parallelism for this class
    protected method slaves_needed {} {
	notice slavemgr "slaves_needed: [Stats]"
	set all_slaves [expr $RSLAVES + $ASLAVES]

	# -- available extra parallelism
	set extra_slaves [expr $nproc - $all_slaves]

	# -- no more slaves should be allocated?
	if { $extra_slaves <= 0 } { return 0 }

	# -- I could possibly run all tasks in the queue in parallel
	# -- need to subtract the ones I have requested
	set nq [expr [$queue Size] - $RSLAVES]

	# -- have I requested enough slaves?
	if { $nq <= 0 } { return 0 }

	# -- but they should not be more than avail. extra parallelism
	if { $extra_slaves > $nq } { set extra_slaves $nq}
	set extra_slaves
    }
    # ----------------------------------------------- slave control
    # -- run a task on a slave
    protected method slave_run {task} {
        # -- select a slave to run
        foreach sl [array names FREE] {
	    # -- have I failed to run the remote task?
            if [catch {$SLAVE($sl) Run $task} rc] {
		warning slavemgr "$sl: slave has failed to run task ($rc)"
		slave_cleanup [$SLAVE($sl) cget -fp]
		continue
	    }
            unset FREE($sl)
            set JSLAVE($task) $sl
	    notice slavemgr "$sl: running task $task"
	    return
        }
	# -- at this stage, the task did not succeed to run
	# -- just requeue
	$queue Requeue $task
    }

    # -- send a goodbye message to each slave and close the connection
    # -- to all servers
    protected method slave_goodbye {} {
	foreach fp [array names SLAVE] {
	    debug smgr "closing channel $fp with slave"
	    if [catch {puts $fp "**GOODBYE!"} rc] {
		notice smgr "goodby $fp: $rc"
	    }
	    catch {close $fp}
	}
    }

    # -- gracefully release a slave
    protected method slave_release {slave reason} {
	notice slavemgr "$slave: releasing slave (due to $reason)"
	$SLAVE($slave) delete
    }

    # -- cleanup after a slave has disconnected (fp is the remote socket)
    protected method slave_cleanup {fp} {
	$SLAVE($fp) Disconnected
	$SLAVE($fp) delete
	notice slavemgr "$fp: slave disconnected"
	incr ASLAVES -1
	unset SLAVE($fp)
	catch {unset FREE($fp)}
	catch {close $fp}
	Event <<slave-released>>
    }
    # -- make sure the slave is rescheduled, if needed
    # -- we should probably check for too many resubmissions
    protected method slave_reschedule {jid tag} {
	notice slavemgr "resubmitting $jid"
	# -- cleanup state
	unset PSLAVE($tag)
	unset PSLAVE_TAG($jid)
	# -- resubmit a slave
	slave_submit $tag
    }
    # ----------------------------------------------------------- slave messages
    # -- slave reverse connection
    public method eSlaveConnect {fp host port} {
	fconfigure $fp -buffering line
        fileevent $fp r "$this eSlaveMessage $fp"
    }

    # -- handle slave message
    public method eSlaveMessage {fp} {
        if { [gets $fp line] == -1 } { 
	    slave_cleanup $fp
            return
        }
        switch -glob -- $line {
            slave*  { 
                eval slave_register $fp [lrange $line 1 end]
            }
            keepalive*   {
		$SLAVE($fp) IsAlive $fp
	    }
            free*   { eSlaveFree $fp }
            default { warning slavemgr "unexpected message : $line"     }
        }
    }

    # -- a slave is free
    # -- read the rest of the message (output)
    public method eSlaveFree {slave} {
        set res ""
        while { [gets $slave line] != -1 } {
            if { $line == "." } { break }
            append res $line \n
        }
        # -- notify slave the task is done
        $SLAVE($slave) TaskDone $res
        notice slavemgr "$slave: slave FREE"
        set FREE($slave) 1

	# -- see if we should release this slave right now
	if { [$SLAVE($slave) cget -idletimeout] == 0 } {
	    eSlaveIdled $slave
	} else {
	    # -- see if we can run anything!
	    $this eSchecduleJob
	}
    }

    # -- a slave should be released
    public method eSlaveIdled {slave} {
	notice slavemgr "$slave: removed from the pool as idled"
	slave_cleanup $slave
    }

    # ----------------------------------------------- queue events
    # -- see if we can schedule a job
    public method eSchecduleJob {} {
        set task [$queue Peek]
	if { $task == "" } { return }

        # -- is there any free slave?
        if { [array size FREE] == 0 } {
	    # -- should we allocate some more?
	    if { $RSLAVES >= $nproc   } {
		notice slavemgr \
		    "$class: reached class MPL limit ($RSLAVES slaves req)"
		return
	    } 
	    # -- let's allocate a slave
	    slave_request 1
            return
        }
        # -- is there a new task to run?
        set task [$queue Deq]
	# -- just in case!
        if { $task == "" } { return }
        slave_run $task
    }

    # -- periodically check pending slaves (i.e. slaves submitted but not conn)
    public method eSlaveCheck {} {
	# -- collect the status of all pending slaves
	set jids [array names PSLAVE_TAG]
	if { $jids == "" } { 
	    $SLAVE_TASK Start
	    return
	}
	foreach {jid st} [$RJE Status $jids] {
	    array set S $st
	    notice slavemgr \
		"monitoring $jid ($PSLAVE_TAG($jid)): status $S(status)"
	    switch -- $S(status) {
		running - pending { }
		default {
		    warning "slavemgr" "$jid: ($PSLAVE_TAG($jid)): lost"
		    foreach tag [lsort [array names S]] {
			warning "slavemgr" [format {%10s: %s} $tag $S(tag)]
		    }
		    slave_reschedule $jid $PSLAVE_TAG($jid)
		}
	    }
	    unset S
	}
	# -- reschedule the slave task
	$SLAVE_TASK Start
    }
    # ----------------------------------------------- queue events
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
