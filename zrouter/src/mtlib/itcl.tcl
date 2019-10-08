package require Itcl
package provide mtlib 1.0


#
# ITCL related utilities
#
proc itcl_frame {obj win args} {
    rename $obj $obj-tmp-
    eval "frame $win $args"
    catch {rename $win-win- ""}
    rename $win $win-win-
    rename $obj-tmp- $obj
}

# generate a window with the same name as the underlined object
# be careful with name collisions. Effectively, rename the widget command
# to .win-win-
proc itcl_toplevel {obj win args} {
    # save the current object command ...
    rename $obj $obj-tmp-
    eval "toplevel $win $args"
    catch {rename $win-win- ""}
    rename $win $win-win-
    rename $obj-tmp- $obj
}

proc itcl_cleanup { object } {
    catch {destroy $object }
    catch { rename $object-win- "" }
}

proc itcl_exists { object } {
    expr { [itcl_info objects $object] != "" }
}

proc itcl_readonly {var {message "can not modify"}} {
    error "READONLY $var: $message"
}

proc itcl_unimplemented {proc args} {
    Log itcl "Unimplemented ${proc}($args)"
}

proc itcl_gensym { prefix } {
    global gItclSym
    if { [catch { set s [incr gItclSym($prefix)] }] } {
        set s [set gItclSym($prefix) 0]
    }
    return $prefix$s
}

# -- bridge changes to a TCL public variable to a command
# -- the command will be called with one additional argument, the value
# -- of the public variable
proc LBridge {pvar cmd} {
    upvar this this
    set call [list LBridge1 $this $pvar $cmd]

    # -- remove previous bindings
    foreach bnd [uplevel trace vinfo $pvar] {
        foreach {op command} $bnd { break }
        switch $op {
            w { set op write }
        }
        uplevel [list trace remove variable $pvar $op $command]
    }


    # -- install a variable trace
    uplevel [list trace add variable $pvar write $call]

    # -- invoke command to estanblish initial value
    LBridge1 $this $pvar $cmd
}

proc LBridge1 {this pvar cmd args} {
    set command $cmd
    lappend command [$this cget -$pvar]
    uplevel #0 $command 
}

# -- propagate a public variable change to one of the specified object
proc LPropagateVar {var object {newvar ""}} {
    if { $newvar == "" } { set newvar $var }
    uplevel [list LBridge $var [list $object configure -$newvar]]
}


# sleep, but keep processing events (ms) 
proc iwait {ms} {
    global vsleep
    if ![info exists vsleep] {
        set vsleep 1
    }
    after $ms "set vsleep 1"
    vwait vsleep
}


# Export a(n optional list of) public methods as simple procs at the toplevel
# Exclude lower case methods and standard methods as well.
# An optional prefix will be prepended to all commands exported
#
proc LExportMethods {this {methods "-"} {prefix ""}} {
    # if methods were not specified, filter through public methods of class
    if  { $methods == "-" } {
        set methods ""
        foreach fn [$this info function] {
            # -- skip non public functions
            if { [eval $this info function $fn -protection] != "public" } {
                continue
            }
            set cmd [namespace tail $fn]
            switch -glob -- $cmd {
                destructor - constructor - cget - configure - delete - isa {
                    continue
                }
                [a-z]* - Export { continue }
            }
            lappend methods $fn
        }
    }
    # we still need to restrict some functions from being exported
    foreach fn $methods {
        # -- skip non public functions
        if { [eval $this info function $fn -protection] != "public" } {
            continue
        }
        set cmd [namespace tail $fn]
        switch -glob -- $cmd {
            destructor - constructor - cget - configure - delete - isa {
                continue
            }
            default {
                interp alias {} ${prefix}$cmd {} $this $cmd
            }
        }
    }
}

# generate a list of public methods of an object
# excluding constructor and destructor
proc LPublicMethods {obj} {
    set res ""
    foreach fn [$obj info function] {
        set fn [namespace tail $fn]
        switch $fn -- { constructor - destructor { continue } }
        lappend res $fn
    }
    set res
}

# -- create a restricted proxy of an object, bound to a slave interpeter
# slave: slave interpreter
# lcmd: slave interpreter alias of the command
# main: main interpeter
# mcmd: main interpeter object command
# blacklist: a list of methods not to call
#
# usage: LObjectProxy slave foo main foo "info delete"
# will restrict the slave interpeter from running info or delete on foo
#
proc LObjectProxy {slave lcmd main mcmd {blacklist ""}} {
    interp alias $slave $lcmd $main LObjectProxy1 $mcmd $blacklist
}

# -- used by LMakeProxy
proc LObjectProxy1 {obj blacklist method args} {
    if { [lsearch $blacklist $method] != -1 } {
        global errorInfo
        error "$method $args: call not allowed"
    }
    set cmd [concat $obj $method $args]
    uplevel $cmd
}
