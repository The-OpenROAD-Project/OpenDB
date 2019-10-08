# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB
package provide mtlib 1.0

#       Class: Exec
# Description: Execute and manage a server
#      Author: Manolis M. Tsangaris -- mmt@karpathos.net
#   Copyright: Manolis M. Tsangaris, 2004
#
# Exec runs a program in the background and collects all of its outputs
# When its state changes, it will generate an event and will update the
# state variable
#
# states:
# config
# queue
# run
# exit
#
itcl::class Exec {
    inherit LBinder
    # Event <<newstate>>  state
    # Event <<error>>     error
    # Event <<output>>    msg
    # -------------------------------------------------- Public Variables --
    public variable dprogram "sleep 86400" ;# default program to run
    public variable env ""                 ;# the environment
    public variable stdin ""               ;# limited stdin
    public variable program ""
    public variable name "server"
    public variable active 1               ;# if false the server will not start
    public variable status  ""
    public variable state   ""
    public variable rtime 0
    public variable restarts 0
    public variable pid 0
    public variable EXIT 0                  ;# exit code
    public variable SHELL "/bin/ksh"        ;# the KSH to use
    public variable MAGIC  "@__exit__@"     ;# mark end of job output
    public variable MAGIC1 "@_input_@"      ;# mark end of job input
    # -------------------------------------------------- Public Methods ----
    # -- start/restart the program
    public method Start {{timeout 0}} {
        if {$active} {
            exec_run  $timeout
        } else {
            return 0
        }
    }
    public method Stop {} {
	notice exec "$this: stopping"
        # -- do I really have to stop?
        if { $state == "exit" } { return 0 }
        if { $fp == "" } { return 0 }
        kill_tree $pid
        catch {fconfigure $fp -blocking no}
        catch {close $fp}
        set pid 0
        set fp ""
        set rtime [expr [clock seconds] - $stime]
        set status "killed"
        newstate exit killed
        return 1
    }
    public method TreePrint {{tab ""}} {
        exec_tree_print $tab
    }
    public method Status {} {
        switch $state {
            run   { set rt [expr [clock seconds] - $stime] }
            queue { set rt 0 }
            exit  { set rt $rtime }
            config { set rt 0 }
        }
        if { $active } { 
            set ast *
        } else {
            set ast ""
        }
        format "%8s %6d %8s %8s %5s %6d %7s" $name $pid $state $status\
            $rt $restarts $ast
    }
    public method State {} {
        list exit $EXIT state $state status $status name $name pid $pid
    }
    proc StatusHeader {} {
        format "%8s %6s %8s %8s %5s %7s" \
            Name PID State Status Time ACTIVE
    }
    # -- return the state of a server
    public method GetState {} {
        list pid $pid name $name active $active program $program
    }
    # -------------------------------------------------- Virtual Methods ---
    public method eDone {fp} {
        if [catch {gets $fp line} err]  {
            Event <<error>> $err
            return
        }
        if ![eof $fp] {
            if [string match "*$MAGIC*" $line] {
                set EXIT [lindex $line 1]
                set status "done"
                newstate exit process-done
                return
            }
            Event <<output>> $line
            return
        }
        if ![catch {close $fp} ec] {
            set ec "OK"
        }
        set fp ""
        set rtime [expr [clock seconds] - $stime]
        set status $ec
        newstate exit shell-died
    }
    public method eRun {} {
        set task ""
        if { $state != "queue" } {
            warning exec "false start since we are in $state state"
            return
        }
        if { $program == "" } {
            set command $dprogram
        } else {
            set command $program
        }

        set stime  [clock seconds]
        array set V $env
        set fp     [open "| $SHELL" r+]

        # -- remove dangerous env variables
        catch {unset V(IFS)}

        # setup the environment
        foreach v [array names V] {
            puts $fp   "export $v='$V($v)'"
        }
        if ![info exists V(SHELL)] {
            set V(SHELL) $SHELL
        }
        if ![info exists V(CWD)] {
            set V(CWD) /tmp
        }

        # -- find out the "do not read profile" flag
        switch -glob $V(SHELL) {
            */sh  - */ksh  { set p -p }
            */bash         { set p --noprofile }
            */csh - */tcsh { set p -f }
            default        { set p "" }
        }

        #set ncmd "(cd $V(CWD) && $V(SHELL) $p -c '$command') 2>&1"
        set ncmd "(cd $V(CWD) && $command) 2>&1"
        if { $stdin == "" } {
            append ncmd " </dev/null"
        } else {
            append ncmd " <<EOF\n"
            append ncmd "$stdin"
            append ncmd "\nEOF\n"
        }
        puts $fp   $ncmd
        puts $fp   "echo $MAGIC $?"
        flush $fp
        set pid    [pid $fp]
	debug exec "run $command (shell pid $pid])"
        set status "running"
        newstate run
        fileevent $fp r "$this eDone $fp"

        incr restarts
    }
    protected method newstate {st {info ""}} {
        set state $st
        set task1 [after idle "$this Event <<newstate>> $st"]
    }
    # -------------------------------------------------- Protected Variables 
    protected variable fp ""
    protected variable stime 0
    protected variable task ""
    protected variable task1 ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        newstate config
    }
    destructor {
	notice exec "$this: starting destructor"
        Stop
        catch {after cancel $task1}
        catch {after cancel $task}
    }
    public method delete {} { itcl::delete object $this }
    # -------------------------------------------------- Private Methods ---
    protected method exec_run {timeout} {
        switch $state {
            run    { warning exec "$name running already" ; return }
            queued { warning exec "$name in queue already" ; return }
        }
        if {$task != "" } {
            after cancel $task
        }
        set pid 0 ; set status "queued"
        set EXIT -
        newstate queue
        if { $timeout == "0" } {
            set tm idle
        }  else {
            set tm [expr $timeout * 1000]
        }
        set task [after $tm "$this eRun"]
    }
    
    # kill a process and its entire tree
    # (well, just a simple kill for the time being)
    protected method kill_tree {pid} {
	debug exec "kill tree $pid (children: [procwalk $pid])"

        if { $pid == 0 } { return }
        # -- try to kill them with HUP
        procwalk -childrenfirst $pid p {
	    debug exec "killing -HUP $p(pid)"
            catch {exec kill -HUP  $p(pid)}
        }
        # -- try to kill them with TERM
        procwalk -childrenfirst $pid p {
	    debug exec "killing -TERM $p(pid)"
            catch {exec kill -TERM $p(pid)}
        }
        # -- try to kill them with 9
        procwalk -childrenfirst $pid p {
	    debug exec "killing -9 $p(pid)"
            catch {exec kill -9    $p(pid)}
        }
        if { [procwalk $pid] != "" } {
            warning exec "children of $pid survived!"
            return 0
        }
        return 1
    }
    protected variable PIDS
    protected variable CMD

    # --- print out the process tree
    protected method exec_tree_print {tab} {
        catch {unset PIDS}
        catch {unset CMD}
        set out "Process List\n"
	# -- collect all processes
        procwalk $pid P {
            set cm "$P(args)"
            if {[string length $cm] > 20} { set cm [string range $cm 0 17].. }
            set CMD($P(pid)) $cm
            lappend PIDS($P(ppid)) $P(pid)
        }
        append out [exec_tree1 $pid $tab]
        set out
    }

    protected method exec_tree1 {pid tab} {
        if ![info exists CMD($pid)] {
            set CMD($pid) ?(DEAD)
        }
        set st [string toupper [string range $state 0 0]]
        if { $active } {
            set st ${st}*
        } else {
            set st ${st}x
        }
        set out [format "%-40s:%2s:%s\n" ${tab}$pid $st $CMD($pid)]

        # -- no more children?
        if ![info exists PIDS($pid)] { return $out }
        foreach p $PIDS($pid) {
            append out [exec_tree1 $p "$tab  "]
        }
        set out
    }
    # ----------------------------------------------- Interface Implementation
    # -------------------------------------------------- Common Initialization
}
