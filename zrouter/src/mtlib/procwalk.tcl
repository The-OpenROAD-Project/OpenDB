# -- NOTICE THIS FILE IS PART OF THE FREELY AVAILABLE OPENSOURCE
# -- STD LIBRARY MTLIB
package provide mtlib 1.0

#-------------------------------------------------------------------------
# procwalk
#-------------------------------------------------------------------------
# This proc walks the tree of children processes of a given process
# and runs a script on each one of them
#
# Usage:
#   procwalk ?options? ppid varName command
# Example:
#   procwalk -childrenfirst 1 p {
#       puts "P= $p(pid)"
#   }
#
# ARGUMENTS:
#   ppid   : the parent process. This proc will walk the tree of its
#            children
#   varName: the variable name
#   command: the body of the command to execute.
#
#   Options:
#   -parentfirst
#       Return the list of pids (and apply any scripts)
#       first to the parent and then to its children.
#       No ordering on siblings though!
#       This is the default.
#   -childrenfirst
#       The opposite of "-parentfirst". First do the children,
#       and then the parent.
#   -noinclude
#       Do not include the process id given as the first argument
#       to the list of returned processes and do not apply the
#       script to it.
#
#-------------------------------------------------------------------------
proc procwalk {args} {

    # Parse arguments
    # There must be zero or more options followed by 3 arguments:
    # the process id, the variable name, and the body to execute.
    #
    set nargs [llength $args]
    if {($nargs < 3) && ($nargs != 1)} {
        set msg "wrong # args: should be "
        append msg "\"procwalk ?options? ppid varName commad\""
        append msg "\"procwalk  ppid\""
        error $msg
    }

    # Parse options...
    #
    set nopts [expr {$nargs-3}]
    set reverse_order 0     ;# parent first
    set include_parent 1    ;# include parent pid
    for {set i 0} {$i < $nopts} {incr i} {
    set arg [lindex $args $i]
    if {$arg == "-parentfirst"} {
        set reverse_order 0
    } elseif {$arg == "-childrenfirst"} {
        set reverse_order 1
    } elseif {$arg == "-noinclude"} {
        set include_parent 0
    } else {
        set msg "Invalid option \"$arg\". Valid options are:"
        append msg " -parentfirst, -childrenfirst, -noinclude."
        error $msg
    }
    }

    if { $nargs == 1 } {
        set parent_pid [lindex $args 0]
        set varName ""
    } else {
        set parent_pid [lindex $args $nopts]
        set varName [lindex $args [expr {$nopts + 1}]]
        set body [lindex $args [expr {$nopts + 2}]]
    }
    if {![regexp {^[0-9]+$} $parent_pid]} {
        error "Invalid pid \"$parent_pid\""
    }

    # Run the ps command to collect the graph or parent/children processes.
    #
    # The following unix command will return the pid as its first
    # argument and the parent pid as its second. We must discard
    # the first line which contains a header.
    #
    set unix_cmd "ps -o pid -o ppid -o etime -o comm -o args -e"
    if {[catch {eval exec $unix_cmd} unix_result]} {
        set msg "procwalk: \"$unix_cmd\": $unix_result"
        error $msg
    }

    # Parse the result of the "ps" command.
    # Construct the array "PIDS" which for each process
    # contains a list with all its children.
    #
    array set PIDS {}
    # -- just in case, since orphans get init (0 or 1) as their parent
    array set PPID  {0 0 1 1}
    array set COMM  {0 0 - -}
    array set PARGS {0 0 - -}
    array set ETIME {0 0 1 0}

    foreach line [split $unix_result "\n"] {
        foreach {pid ppid etime comm} $line { break }
        if [catch {lrange $line 4 end} pargs] {
            set pargs ...
        }
        if { $pid == "PID" } { continue }
        set PPID($pid) $ppid
        set COMM($pid) $comm
        set PARGS($pid) $pargs
        set ETIME($pid) $etime
        lappend PIDS($ppid) $pid
    }
    # -- now exclude 0 and 1 from the PIDS
    catch {unset PIDS(0)}
    catch {unset PIDS(1)}
    
    # Does the pid given as our argument exists?
    # If not, what shall we do ???
    #
    if ![info exists PIDS($parent_pid)] {
        return ""
    }

    # Now traverse the graph and find all processes, parent first.
    set pid_list [list $parent_pid]

    set n 0
    while {1} {
        set cur_pid [lindex $pid_list $n]
        if {$cur_pid == ""} {
            # we have reached the end of list. Exit the loop
            break
        }
        if {[info exists PIDS($cur_pid)]} {
            set pid_list [concat $pid_list $PIDS($cur_pid)]
        }
        incr n
    }
    if {!$include_parent} {
        # Exclude the given pid (this is the first item in our list)
        set pid_list [lrange $pid_list 1 end]
    }

    # If we need to reverse the order of children/parents do it
    #
    if {$reverse_order} {
        set tmp $pid_list
        set n [llength $tmp]
        set pid_list {}
        for {set i [expr {$n - 1}]} {$i >= 0} {incr i -1} {
            lappend pid_list [lindex $tmp $i]
        }
    }

    # Now it is time to execute the body for each pid
    if { $varName != "" } {
        upvar $varName p
        foreach p(pid) $pid_list {
            set p(ppid) $PPID($p(pid))
            set p(comm) $COMM($p(pid))
            set p(etime) $ETIME($p(pid))
            set p(args) $PARGS($p(pid))
            uplevel $body
        }
    }
    return $pid_list
}

#procwalk -childrenfirst [pid] p { puts "$p(ppid)->$p(pid) ($p(comm) $p(etime))" }
#puts [procwalk [pid]]


# return process information
proc LProcInfo {pid pp} {
    upvar $pp P
    global ::tcl_platform
    set cmd "ps -p $pid -o pid -o ppid -o etime -o comm -o args"

    switch $::tcl_platform(platform) {
        unix {
            if [catch {eval exec $cmd} rc] {
                set P(info) $rc
                return 0
            }
            set P(info) [lindex [split $rc \n] 1]
            return 1
        }
        default { return 0 }
    }
}
