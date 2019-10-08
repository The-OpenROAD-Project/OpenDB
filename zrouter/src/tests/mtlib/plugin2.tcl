package require Itcl
#lappend auto_path $env(HOME)/work/zrouter/src/mtlib
lappend auto_path $env(HOME)/z/zrouter/src/mtlib

proc notifier {args} {
    puts "pm:notified $args"
}

# -- create a plugin
set pm [LPlugin pm]
# -- bind its events
pm Bind <<done>> notifier   done:
pm Bind <<notice>> notifier notice:
pm Bind <<result>> notifier result:

# -- start a "server"
set cmd [pm Start "tclsh"]
# -- send a few commands to the server
pm Send $cmd {set rc [source remote_scripts.tcl]; puts $rc ; flush stdout}
pm Send $cmd {set rc [get_hostname]; puts $rc ; flush stdout}
pm Send $cmd {set rc [echo_args "12 23 45"]; puts $rc ; flush stdout}
pm Send $cmd {set rc [echo_args {a b c}]; puts $rc ; flush stdout}
pm Send $cmd {set rc [info level]; puts $rc ; flush stdout}
#pm Send $cmd {set rc [exit] ; puts $rc ; flush stdout}
# -- the notifier should now print the result of those commands
vwait forever
