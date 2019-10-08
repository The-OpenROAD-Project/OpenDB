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
proc run_cmd { args } {

set rcmd [lindex $args 0]
puts "$args =============== $rcmd"

	pm Send $::cmd {set rc [echo_args {a b c}  ]; puts $rc ; flush stdout}
	pm Send $::cmd {set rc [ [$rcmd] $args ]; puts $rc ; flush stdout}
}



# -- send a few commands to the server
pm Send $cmd {set rc [source remote_scripts.tcl]; puts $rc ; flush stdout}

pm Send $cmd {set rc [echo_args "12 23 45"]; puts $rc ; flush stdout}
run_cmd echo_args {12 23 45}
pm Send $cmd {set rc [source papi_psta.tcl ]; puts $rc ; flush stdout}
pm Send $cmd {set rc [source /home/nefelus/DEMO/1buf1/scripts/psta.tcl ]; puts $rc ; flush stdout}
pm Send $cmd {set rc [all_clocks]; puts $rc ; flush stdout}
pm Send $cmd {set rc [get_hostname]; puts $rc ; flush stdout}
pm Send $cmd {set rc [echo_args "12 23 45"]; puts $rc ; flush stdout}
run_cmd echo_args {12 23 45}
pm Send $cmd {set rc [echo_args {a b c}]; puts $rc ; flush stdout}
run_cmd {echo_args {a b c}}
pm Send $cmd {set rc [info level]; puts $rc ; flush stdout}
#pm Send $cmd {set rc [exit] ; puts $rc ; flush stdout}
# -- the notifier should now print the result of those commands
#vwait forever
