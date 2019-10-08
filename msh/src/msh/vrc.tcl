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

#       Class: VRCMain
# Description: Main Virtual RJE controller
#      Author: Manolis M. Tsangaris
# This object manages the Virtual RJE
#
# VRC implements the 'virtual rje', i.e. a group of servers requested by
# a real RJE to run jobs
#
itcl::class VRCMain {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    public variable default "default"  ;# -- default class
    public variable cf "" { array set CF $cf }
    public variable env  ""            ;# -- environment to use by default
    public variable logdir  "logs/msh" ;# -- log directory
    public variable slavetimeout 0     ;# -- idle timeout for slaves
    public variable slaveDebugLevel notice
    public variable mpl 30             ;# -- maximum # of slaves supported
    # -------------------------------------------------- Public Methods ----
    public method Run {task {stdin ""} {_env ""} {argv ""}} {
        if { $_env == "" } { set _env $env }
	set class [snoop_class $argv]
	if { $class == ""} { set class $default }
	set vrc [get_vrc $class]
	if { $vrc == "" } {
	    error "run : $task: $class: no such class"
	}
        set jid t[incr NJID]
        set jid [$VRC($class) Run $jid $task $stdin $_env $argv]
	set CLASS($jid) $class
	set jid
    }
    public method Status {jid} {
        $VRC($CLASS($jid)) Status $jid
    }
    public method Tasks {} {
	set res ""
	foreach class [array names VRC] {
	    set res [concat $res [$VRC($class) Tasks]
	}
	set res
    }
    public method TaskStatus {tid} {
        $VRC($CLASS($tid)) TaskStatus $tid
    }
    public method TasksStatus {tids} {
        set res ""
        foreach tid $tids {
            lappend res $tid [TaskStatus $tid]
        }
        set res
    }
    public method Source {file} {
        source $file
    }
    public method Info {} {
        append s "MSH -- STATUS\n"
	if ![array size VRC] {
	    append s "-- no classes used --\n"
	    append s "profiles defined: [array names CF]\n"
	}
	foreach class [array names VRC] {
	    append s " --- CLASS: $class\n"
	    append s [$VRC($class) Info "  "]
	}
	append s " --- CONFIG --- \n"
	array set op [array get CF]
	set op(slavetimeout) $slavetimeout
	set op(mpl) $mpl
	foreach var [array names op] {
	    append s [format "%10s %s\n" $var $op($var)]
	}
        set s
    }
    # -- generate a set of test commands (for all defined classes)
    public method Test {{nt 10} {profiles ""}} {
	if { $profiles == "" } { set profiles [array names CF] }
	set n [llength $profiles]
        for {set i 0} {$i < $nt} { incr i } {
	    set cl [lindex $profiles [expr $i % $n]]
            lappend ts [Run "sleep 15" "" "" "-class $cl"]
        }
        return "[llength $ts] tasks dispatched:\n$ts"
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable VRC           ;# virtual controller per class
    protected variable CF            ;# pass information to RJE
    protected variable CLASS         ;# class of a job
    protected variable NJID 0
    protected variable MPL  0       ;# current MPL
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
    }
    destructor {
	foreach class [array names VRC] {
	    $VRC($class) delete
	}
    }
    # -------------------------------------------------- Private Methods ---
    # -- allocate a new vrc per class, if necessary
    protected method get_vrc {class} {
	if [info exists VRC($class)] {
	    return $VRC($class)
	}
	if ![info exists CF($class)] { return "" }

	array set P $CF($class)
	set vrc [VRC $this.vrc#auto -class $class -env $env -logdir $logdir \
	    -slavetimeout $P(slave_timeout) -slaveDebugLevel $slaveDebugLevel\
	    -cf $CF($class)]
	set VRC($class) $vrc
	$vrc Bind <<slave-released>>  $this eSlaveReleased $class
	$vrc Bind <<slave-requested>> $this eSlaveRequested $class
	set vrc
    }
    # -- grant a slave to a class
    protected method grant_slave {cl} {
	# -- notify the class it can grant a slave
	if [$VRC($cl) GrantedSlave] {
	    LLog msh notice "granting a slave to `$cl` (at MPL=$MPL)"
	    incr MPL
	    return 1
	}
	return 0
    }
    # -- snoop the arguments, extract the class
    # -- we assume binary arguments
    protected method snoop_class {argv} {
	foreach {a v} $argv {
	    if { $a == "-class" } { return $v }
	}
	return ""
    }
    # -------------------------------------------------- Callbacks
    # -- a slave has just been released. Offer that slot to another class
    # -- prefer the class requested the maximum # of slaves
    # -- in case the class does not need the slave, try another one
    public method eSlaveReleased {class} {
	# -- decrease the current MPL
	incr MPL -1
	while 1 {
	    # -- create the list of requested slaves
	    set list ""
	    foreach cl [array names VRC] {
		set n [$VRC($cl) SlavesNeeded]
		if { $n == 0 } { continue }
		notice vrc "$cl: $n (more) slaves needed"
		lappend list [list $n  $cl]
	    }
	    # -- no requests
	    if { $list == "" }  { return }
	    set slist [lsort -integer -decreasing -index 0 $list]
	    set cl [lindex [lindex $slist 0] 1]

	    # -- try to grant a slave to the selected class
	    if { [grant_slave $cl] } { break }
	}
    }
    # -- a VRC of a given class is requesting a slave
    # -- notify it, if one becomes available
    public method eSlaveRequested {cl} {
	# -- can we allocate more slaves?
	if { $MPL >= $mpl } {
	    # -- note the request
	    LLog msh notice "grant request of `$cl` delayed"
	    return
	}
	grant_slave $cl
    }
    # -------------------------------------------------- Common Initialization
}

#       Class: VRC
# Description: Virtual RJE controller per class
#      Author: Manolis M. Tsangaris
# This object manages the Virtual RJE
#
# VRC implements the 'virtual rje', i.e. a group of servers requested by
# a real RJE to run jobs
#
itcl::class VRC {
    inherit LObject LBinder
    # Event <<slave-requested>>    # notify for request a new slave
				   # return 0 if none granted
    # Event <<slave-released>>     # notify of a release of slave
    # -------------------------------------------------- Public Variables --
    public variable class "default"    ;# vrc class
    public variable env  ""            ;# -- environment to use by default
    public variable logdir  "logs/msh" ;# -- log directory
    public variable slavetimeout 0         ;# -- idle timeout for slaves
    public variable cf "" { array set CF $cf }
    public variable slaveDebugLevel notice
    # -------------------------------------------------- Public Methods ----
    # run a remote command and optionally, use the env set
    public method Run {jid task {stdin ""} {_env ""} {argv ""}} {
        if { $_env == "" } { set _env $env }
        $vrje Run $jid $task $stdin $_env $argv
    }
    public method Status {jid} {
        $vrje Status $jid
    }
    public method Tasks {} {
        $vrje Tasks
    }
    public method TaskStatus {tid} {
        $vrje TaskStatus $tid
    }
    public method TasksStatus {tids} {
        set res ""
        foreach tid $tids {
            lappend res $tid [$vrje TaskStatus $tid]
        }
        set res
    }
    public method Source {file} {
        source $file
    }
    public method Info {{tab ""}} {
        append s "${tab}RJE: $CF(rje) (mpl: $CF(nproc))\n"
        append s "${tab}[$smgr SystemStatus]"
        append s "${tab}[$vrje SystemStatus]"
        #append s "${tab}*** JOB QUEUE\n"
        #append s "${tab}[$queue SystemStatus]"
        append s "\n"
        set s
    }
    # -- a slave has been granted
    public method GrantedSlave {} {
	$smgr GrantedSlave
    }

    # -- queue size
    public method QueueSize {} { $queue Size }

    # -- # of slaves needed (for full multiprocessing)
    public method SlavesNeeded {} {
	$smgr SlavesNeeded
    }
    # -------------------------------------------------- Virtual Methods ---
    # -------------------------------------------------- Protected Variables 
    protected variable vrje          ;# virtual rje exported
    protected variable smgr          ;# slave manager
    protected variable queue         ;# queue to hold jobs
    protected variable RJE           ;# generic RJE interface
    protected variable CF            ;# pass information to RJE
    protected variable MAXSLAVES 30  ;# canot support more than those
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args

	LLog vrc notice "$class: [array get CF]"
        set RJE   [LRJE::Create $CF(rje) $this.rje [array get CF] \
	    -stderr $logdir/%u.err -stdout $logdir/%u.out]

        set queue [LSQUEUE $this.q -policy fifo]
        set smgr  [SlaveMGR $this.smgr -nproc  $CF(nproc)  -class $class\
            -queue $queue -RJE $RJE -logdir $logdir -slavetimeout $slavetimeout\
	    -slaveDebugLevel $slaveDebugLevel]
	$smgr Redirect <<slave-requested>> $this
	$smgr Redirect <<slave-released>>  $this
        set vrje  [VRJE $this.vrje -queue $queue]
    }
    destructor {
	notice vrc "cleaning up (class $class)"
	catch {
	    $smgr delete
	    $vrje delete
	    $queue delete
	    $RJE   delete
	} rc
	notice vrc "cleaning up done: ($rc)"
    }
    # -------------------------------------------------- Private Methods ---
    # -------------------------------------------------- Common Initialization
}
