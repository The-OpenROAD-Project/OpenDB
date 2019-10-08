package require Itcl
lappend auto_path $env(HOME)/work/zrouter/src/mtlib

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
pm Send $cmd {set rc [info hostname]; puts $rc ; flush stdout}
pm Send $cmd {set rc [info level]; puts $rc ; flush stdout}
pm Send $cmd {set rc [exit] ; puts $rc ; flush stdout}
# -- the notifier should now print the result of those commands
vwait forever
