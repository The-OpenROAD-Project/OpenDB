package require Itcl
#lappend auto_path $env(HOME)/work/zrouter/src/mtlib
lappend auto_path $env(HOME)/z/zrouter/src/mtlib

proc notifier {args} {
   # puts "pm:notified $args"
   puts " -------------------------------------- $args"
	if { [lindex $args 0] == "result:" } {
		puts "$args"
	}
}

# -- create a plugin
set pm [LPlugin pm]
# -- bind its events
pm Bind <<done>> notifier   done:
pm Bind <<notice>> notifier notice:
pm Bind <<result>> notifier result:

# -- start a "server"
set cmd [pm Start "sta"]




# run_cmd foo 1 2 3
proc run_cmd { args } {
    set fmt {set r [catch {%s} rc ];puts [list result $r $rc]; flush stdout}
    set rcmd [format $fmt $args]
    puts "remote cmd=$rcmd"
    pm Send $rcmd
}

# -- send a few commands to the server

pm Send $cmd {set rc [echo_args "12 23 45"]; puts $rc ; flush stdout}

set A "13 aaa 15"
#pm Send $cmd {set rc [echo_args $A]; puts $rc ; flush stdout}
run_cmd echo_args $A

run_cmd echo_args {12 23 45}
