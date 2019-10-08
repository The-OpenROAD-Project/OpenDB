# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB

package require Itcl
package provide mtlib 1.0

#
#       Class: LRJE
# Description: Interface to the unterlined RJE, to start & maintain slaves
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LRJE {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable stderr /dev/null
    public variable stdout /dev/null
    public variable type ""           ;# type of rje: sh, lsf, vnc
    public variable hosts "localhost" ;# hosts to use (honored by LRJE_SSH)
    public variable nproc   10 ;# maximum concurrency level
    public variable queue "" ;# which queue to use
    public variable rjeopts "" ;# additional rje options
    # -------------------------------------------------- Public Methods ----
    public method Run {cmd} {
        $this rje_run $cmd
    }
    # -- Multi run, or group mode. Return an array of job ids
    public method MRun {cmd {count 1}} {
        $this rje_mrun $cmd $count
    }
    public method Kill {jid} {
        $this rje_kill $jid
    }
    # -- obtain the status of a list of job ids
    public method Status {jids} {
        $this rje_status $jids
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable NJOBS 0  ;# job number used for stdout/err logs
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # --expand logs
    # %u expands to a unique name for the task
    protected method fixlogs {ff} {
        upvar $ff F
        set u [info hostname].[incr NJOBS]
        foreach e "stderr stdout" {
            set F($e) [set $e]
            regsub -all %u $F($e) $u F($e)
        }
    }
    # ----------------------------------------------- Interface Implementation
    # -- run a command - return job id
    protected method rje_run {cmd} {
        fixlogs F
    }
    # -- simulate this, using multiple runs
    protected method rje_mrun {cmd count} {
	set res ""
	for {set i 0} { $i < $count} { incr i } {
	    lappend res [Run $cmd]
	}
	set res
    }
    # -- kill a job
    protected method rje_kill {jid} {
    }
    # -- job status
    protected method rje_status {jid} {
        error "unsupported: status $jid"
    }
    # -------------------------------------------------- Common Initialization
    # -- create an RJE based on type
    public proc Create {type name cf args} {
        if { $type == "auto" } {
            set type [Auto]
        }
        switch $type {
            lsf { set class LRJE_LSF }
            vnc { set class LRJE_VNC }
            sh  { set class LRJE_SH  }
            ssh { set class LRJE_SSH  }
            default {
                error "unknown RJE type : $type" 
            }
        }
        array set C $cf
        set argv ""
        # -- pass the queue to the constructor
        foreach v "queue rjeopts hosts nproc" {
            if [info exists C($v)] {
                lappend argv -$v $C($v)
            }
        }
        foreach a $args { lappend argv $a }
        set obj [namespace eval :: $class $name $argv]
        set obj
    }
    # guess an RJE
    public proc Auto {} {
        if ![catch {exec lsf ls}] {
            set type lsf
        } elseif ![catch {exec bsub "sleep 1"}] {
            set type lsf
        } elseif ![catch {exec vnc cmd info}] {
            return vnc
        } else {
            set type sh
        }
    }
}


#
#       Class: LRJE_SH
# Description: Interface to the unterlined RJE, to start & maintain slaves
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LRJE_SH {
    inherit LRJE
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -- run a command - return job id
    protected method rje_run {cmd} {
        fixlogs F
        exec sh -c "$cmd" &
    }
    # -- kill a job
    protected method rje_kill {jid} {
        catch {exec kill $jid}
    }
    # -- job status
    protected method rje_status {jids} {
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LRJE_SSH
# Description: Use SSH in a round robin fashion
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LRJE_SSH {
    inherit LRJE
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable HOST ;# where each job runs
    protected variable PID  ;# the pid
    protected variable JID 100
    protected variable FP   ;# open pipes to the remote host
    protected variable ENV
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        global env
        array set ENV [array get env]
        set ENV(CWD) [exec pwd]
        catch {unset ENV(IFS)}
    }
    destructor { }
    # -------------------------------------------------- Private Methods ---
    protected method rexec {host ENV cmd jid} {
        upvar $ENV env
	set sopts "-x -o StrictHostKeyChecking=false"
        set scmd "ssh $rjeopts $sopts $host /bin/ksh"
        set fp [open "| $scmd" r+]
        set FP($jid) $fp
        fconfigure $fp -buffering line
        puts $fp "exec 2>&1"
        foreach v [array names env] {
            puts $fp "export $v='$env($v)'"
        }
        puts $fp "cd $env(CWD)"
        puts $fp "($cmd) &"
        puts $fp "pid=\$!; echo @pid=\$pid"
        puts $fp "wait \$pid; echo @exit=\$?"
        fileevent $fp r "$this eOutput $fp $jid"
        LLog rje:ssh info "$fp: running $cmd"
    }
    public method eOutput {fp jid} {
        if { [gets $fp line] == -1 } {
            cleanup $jid
            return
        }
        LLog rje:ssh info "output:$jid: $line"
        switch -glob -- $line {
            @pid=*  { set PID($jid) [string range $line 4 end] }
            @exit=* { set EXIT($jid) [string range $line 5 end] }
            default {
                LLog notice rje:ssh: "unexpected output: $line"
            }
        }
    }
    # ----------------------------------------------- Interface Implementation
    # -- run a command - return job id
    protected method rje_run {cmd} {
        fixlogs F
        incr JID
        # -- pick the host in round robin 
        set HOST($JID) [lindex $hosts [expr $JID % [llength $hosts]]]
        set PID($JID)  [rexec $HOST($JID) ENV $cmd $JID]
        set JID
    }
    # -- kill a job
    # -- FIX ME: probably need to run killtree on the remote machine
    # -- to kill other processes
    protected method rje_kill {jid} {
        puts $fp "kill $PID($jid)"
        cleanup $jid
    }
    protected method cleanup {jid} {
        if ![info exists PID($jid)] { return }

        catch {puts $FP($jid) "kill $PID($jid)"}
        if [catch {close $FP($jid) } rc] {
            warning rje:ssh "cleanup $jid on $HOST($jid)"
            warning rje:ssh "cleanup: $rc"
        }
        unset PID($jid)
        unset HOST($jid)
        unset FP($jid)
    }
    # -- job status
    protected method rje_status {jids} {
	set out ""
	foreach jid $jids {
	    if [!info exists PID($jid)] {
		lappend out $jid invalid
		continue
	    }
	    lappend out $jid [procinfo $PID($jid)]
	}
	set out
    }
    protected method procinfo {pid} {
	set cmd "ps -t $pid -o {pid status}"
	if [catch {exec ps -t $pid -o "pid status"} rc] {
	    return unknown
	}
	set info [lindex [split $rc \n] 1]
	if { $info == "" } { 
	    return exit
	}
	foreach {_pid status}  $info { break }
	if { $_pid != $pid } { return unknown }
	switch $status {
	    D -  W - R  - S  - T { return running }
	    X    { return exit }
	    Z    { return exit }
	    default { return unknown }
	}
    }
    # -------------------------------------------------- Common Initialization
}

#
#       Class: LRJE_LSF
# Description: Use LSF to submit jobs
#      Author: Manolis M. Tsangaris  -- mtlib@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
itcl::class LRJE_LSF {
    inherit LRJE
    # -------------------------------------------------- Public Variables --
    # -------------------------------------------------- Public Methods ----
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable JOB      ;# submitted jobs
    protected variable NHOSTS 0 ;# number of hosts available
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        global env
        catch {unset env(IFS)}
        if { [catch {lsf bhosts} rc] } {
            error "rje:lsf: no LSF on '[info hostname]'"
        }
        set NHOSTS [llength [split $rc \n]]
        notice rje:lsf "LSF: detected $NHOSTS nodes"
    }
    destructor {
        foreach j [array names JOB] {
            rje_kill $j
        }
    }
    # -------------------------------------------------- Private Methods ---
    # run an LSF command
    protected method lsf {cmd} {
        set r [eval exec lsf $cmd]
        return $r
    }
    # ----------------------------------------------- Interface Implementation
    # -- run a command - return job id
    protected method rje_run {cmd} {
        set com "lsf bsub"

        fixlogs F
        lappend com -o $F(stdout) -e $F(stderr)

	# -- append any additional rje options
	if { $rjeopts != "" } {
	    foreach opt $rjeopts {
		lappend com $opt
	    }
	}

	# -- append the queue name
        if { $queue != "" } {
            lappend com -q $queue
        }

        set com [concat $com $cmd]

        if [catch {lsf $com} rc] {
            error "run: $com: $rc"
        }

        # collect the Job ID
        set rc [string trim $rc]
        if {![regexp {Job <([0-9]+)>} $rc _ JID]} {
            error "run: $cmd: $rc: cannot parse Job ID"
        }
        LLog rje:lsf info "run: id $JID: $com"
        LLog rje:lsf info "result: $rc"
        set JOB($JID) ""
        set JID
    }
    # -- submit $count jobs to LSF, for concurrent (group) execution
    protected method rje_mrun {cmd count} {
        set com "lsf bsub -n $count"

        fixlogs F
	# -- FIX ME: all jobs will overwrite their logs
        lappend com -o $F(stdout) -e $F(stderr)

	# -- append any additional rje options
	if { $rjeopts != "" } {
	    foreach opt $rjeopts {
		lappend com $opt
	    }
	}

	# -- append the queue name
        if { $queue != "" } {
            lappend com -q $queue
        }

        set com [concat $com $cmd]

        if [catch {lsf $com} rc] {
            error "run: $com: $rc"
        }

        # collect the Job ID
        set rc [string trim $rc]
        if {![regexp {Job <([0-9]+)>} $rc _ JID]} {
            error "run: $cmd: $rc: cannot parse Job ID"
        }
        LLog rje:lsf info "run: id $JID: $com"
        LLog rje:lsf info "result: $rc"
        set JOB($JID) ""
	set JIDS ""
	for {set c 0} { $c < $count} {incr c} {
	    lappend JIDS $JID
	}
	# -- return an array of JIDS (all equal)
        set JIDS
    }
    # -- kill a job
    protected method rje_kill {jid} {
        if [catch {lsf "bkill $jid"} rc] {
            warning rje:lsf "kill: $jid: $rc"
        }
        unset JOB($jid)
    }
    # -- job status
    protected method rje_status {jids} {
	# -- -w: to show full array specification
        if [catch {lsf "bjobs -w $jids"} rc] {
	    # mask errors
        }
	set res ""
	foreach line [split $rc \n] {
	    # -- skip the header
	    if { [string match JOBID* $line] } { continue }
	    # -- parse the line
	    switch -glob -- $line {
		"* not found*" - "* Illegal job*" {
		    # -- deal with unknown jobs
		    foreach {_ jid} $line { break }
		    set jid [string map {< "" > ""} $jid]
		    set r [list status notfound]
		    lappend res $jid $r
		    continue
		}
		{[0-9]*} {
		    foreach {jid user status queue shost ehost name} \
			$line {break}
		    lappend res $jid [list \
			user $user queue $queue shost $shost ehost $ehost\
			name $name\
			status [map_status $status]]
		}
		"* Illegal job*" {
		    set jid [lindex [split $line :] 0]
		    set r [list status illegal]
		    lappend res $jid $r
		    continue
		}
		"No unfinished*" { }
		* {
		    puts stderr "rje_status $jids: warning: $line"
		}
	    }
	}
	set res
    }
    protected method map_status {st} {
	switch $st {
	    RUN { return running }
	    PEND { return pending }
	    DONE { return done }
	    EXIT { return exit }
	    default { return $st }
	}
    }
    # -------------------------------------------------- Common Initialization
}
