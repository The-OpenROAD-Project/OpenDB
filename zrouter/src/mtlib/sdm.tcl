

package provide mtlib 1.0

#
#       Class: LSDM
# Description: Simple dependency manager
#
# SDM keeps a simple dependency graph of "abstract variables"
# A variable may depend on other variables and so on.
# If a variable changes, its dependants are notified, one by one
# with a simple upcall mechanism. Notification could be delayed, on idle,
# or prohibited.
#
itcl::class LSDM {
    inherit LObject
    # -------------------------------------------------- Public Variables --
    # -- how often to propagate changes (in msec or idle)
    public variable timeout 1000
    public variable debug 0
    # -------------------------------------------------- Public Methods ----
    # -- a depends on b ...
    public method Depend {a args} {
	foreach b $args {
	    if { $a == $b }  { continue }
	    sdm_depend_add $a $b
	}
    }
    # -- return dependencies of node
    public method Deps {node} {
	set DEPENDS($node)
    }
    # -- which command to run when all a dependants are done 
    public method Bind {a cmd} {
	$binder Bind $a $this eVarChanged $a $cmd
    }
    # -- variables have changed
    public method Set {args} {
	foreach {var value} $args {
	    Set! $var $value
	    sdm_changed $var 1
	}
        # -- schedule update, subject to mode
        sdm_schedule
    }

    # -- set the value of a variable, no events are produced
    public method Set! {var val} {
	notice "set $var $val"
	if [info exists VALUE($var)] {
	    set OLD($var) $VALUE($var)
	}
	set VALUE($var) $val
    }
    # -- get the current value
    public method Get {var} {
	set VALUE($var)
    }
    public method GetAll {} {
	array get VALUE
    }
    # -- return old value
    public method GetOld {var} {
	set OLD($var)
    }
    # -- get dependant values of var (old & new)
    public method GetDepValues {var old new} {
	upvar $old O
	upvar $new N
	array set O {}
	array set N {}
	set list $DEPENDS($var) 
	lappend list $var
	foreach d $list {
	    if [info exists OLD($d)] {
		set O($d) $OLD($d)
	    }
	    if [info exists VALUE($d)] {
		set N($d) $VALUE($d)
	    }
	}
    }

    # -- propagate changes:
    # --          : force propagation on the current mode
    # --   delayed: after timeout
    # --   on-idle: idle loop
    # --   stop   : no propagation
    public method Propagate {{nmode ""}} {
	if { $nmode == "" } { set nmode $omode }
        set omode $mode
        set mode $nmode
        sdm_schedule
        set omode
    }
    # -- force propagation as soon as possible
    # -- do not change the current mode
    public method Update {} {
        sdm_schedule now
    }

    # -- reset all changes
    public method Reset {} {
        foreach v [array names MARK] {
            set MARK($v) 0
            set CHANGED($v) 0
        }
    }
    # -- compute changed dependants
    public method GetChanged {node} {
	set out ""
	foreach n $DEPENDS($node) {
	    if { $CHANGED($n) }  { lappend out $n }
	}
	set out
    }
    # -------------------------------------------------- Protected Variables 
    protected variable DEPEND                   ;# reverse dependencies
    protected variable DEPENDS                  ;# forward dependencies
    protected variable CHANGED                  ;# mark changed nodes
    protected variable MARK                     ;# mark nodes to be updated
    protected variable VALUE                    ;# remember current value
    protected variable OLD                      ;# remember old value
    protected variable task ""
    protected variable binder
    protected variable mode ""
    # -------------------------------------------------- Common Variables --
    # -------------------------------------------------- Configuration -----
    constructor {args} {
        eval configure $args
        set binder [LBinder $this.binder]
    }
    destructor {
        $binder delete
        catch {after cancel $task}
    }
    public method delete {} { itcl::delete object $this }
    # -------------------------------------------------- Private Methods ---
    # ----------------------------------------------- Interface Implementation
    # -- A variable has changed. Mark & propagate changes if necessary
    # -- If really is set, the variable has really changed by user
    protected method sdm_changed {var {really 0}} {
        notice "changed $var"
        if ![info exists MARK($var)] {
            set DEPEND($var) ""
            set MARK($var)    0
        }
	if { $really } {
	    set CHANGED($var) 1
	}
        # mark changes
        # -- is that variable already marked as changed?
        if { $MARK($var) } { return }
        set MARK($var) 1
        foreach d $DEPEND($var) {
            sdm_changed $d
        }
    }
    protected method sdm_depend_add {a b} {
	set CHANGED($a) 0
        lappend DEPEND($b) $a
	lappend DEPENDS($a) $b
	foreach v [list $a $b] {
	    if ![info exists MARK($v)] { set MARK($v) 0 }
            if ![info exists DEPEND($v)] { set DEPEND($v) "" }
            if ![info exists DEPENDS($v)] { set DEPENDS($v) "" }
            if ![info exists CHANGED($v)] { set CHANGED($v) 0 }
	}
    }
    # -- schedule or cancel a change propagation
    protected method sdm_schedule {{nmode ""}} {
	if { $nmode == "" } { set nmode $mode }
        notice "sdm_schedule: $mode"
        if { $task != "" } {
            after cancel $task
	    set task ""
        }
        switch $mode {
            delayed {
                set task [after $timeout "$this eScheduled"]
            } 
            on-idle - now {
                set task [after idle     "$this eScheduled"]
            }
            stop { } 
            none { } 
        }
    }
    # -- propagate all changes ...
    protected method sdm_propagate_all {} {
	# -- during propagation, disable any further propagations
        set omode [Propagate none]
	set order [tsort DEPENDS]
        foreach node $order {
            if !$MARK($node) { continue }
            set MARK($node) 0
            notice "notify: $node"
            $binder Event $node
        }
	foreach node [array names CHANGED] {
	    set CHANGED($node) 0
	}
        set mode $omode
    }
    # ----------------------------------------------------- 
    # -- simple recursive topological sort
    # -- DAG: array of depedencies
    proc tsort {dag} {
	upvar $dag DAG
	set OUT ""
	foreach node [array names DAG] {
	    visit $node DAG TMARK OUT
	}
	set OUT
    }
    # -- visit all nodes, for the purposes of tsort
    proc visit {node dag mark out} {
	upvar $dag DAG
	upvar $mark TMARK
	upvar $out OUT
	# -- already visited?
	if [info exists TMARK($node)] { return }

	# -- visit dependants if any
	if [info exists DAG($node)] {
	    foreach n $DAG($node) {
		visit $n DAG TMARK OUT
	    }
	}
	set TMARK($node) 1
	lappend OUT $node
    }

    # ----------------------------------------------------- 
    method notice {msg} {
        if {$debug} {
            puts stderr ">>> dm: $msg"
        }
    }
    # -------------------------------------------------- Upcalls -----
    # called by the after to propagate changes
    public method eScheduled {} {
        set task ""
        sdm_propagate_all
    }
    public method eVarChanged {var upcall} {
	notice "$var: running: $upcall"
        uplevel #0 $upcall
    }
    # -------------------------------------------------- Common Initialization
}

if 0 {
source ../mtlib/object.tcl
source ../mtlib/binder.tcl
SDM dm -debug 1
dm Depend b1 b {
    puts b1-changed
} 
dm Depend b2 b
dm Depend c1 c
dm Depend c2 c
dm Depend b  a
dm Depend c  a
dm Depend b1 c
dm Propagate delayed

dm Changed c new-c
vwait forever
}
