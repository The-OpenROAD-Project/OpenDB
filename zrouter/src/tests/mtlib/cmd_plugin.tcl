package require Itcl
#lappend auto_path $env(HOME)/work/zrouter/src/mtlib
lappend auto_path $env(HOME)/z/zrouter/src/mtlib

global Buffer
set Buffer ""
global pathCnt 
set pathCnt -1
proc notifier {args} {
	if { [lindex $args 0] eq "notice:" } {
		return
	}
	if { [lindex $args 0] eq "result:" } {
		#puts "$args"
		

		if {[llength $args]<3 } {
			return
		}
		set line [lindex $args 2]
		if { [lindex $line 0] != "output:" } {
			if { [lindex $line 0] == "result" } {
				set objLine [list "\"TimingLine\": \"\" "]
				puts "    \]"
				puts "  \}"
				puts " \]"
				puts "\}"
				return
			}
			if { [lindex $line 0] == "Startpoint:" } {
				incr ::pathCnt
				if { $::pathCnt > 0 } {
					set objLine [list "\"TimingLine\": \"\" "]
					puts "     $objLine,"
					puts "   \]"
					puts "  \} "
				}
				if { $::pathCnt == 0 } {
					puts "\{\"Status\": \"OK\" , \"ReportType\": \"Full\" , "
					puts "  \"Paths\": \[ "
				}
				puts "   \{ \"Path\": $::pathCnt , \"ReportPath\": \[ "
			}
			set objLine [list "\"TimingLine\": \"$line\" "]
			puts "     $objLine,"
		}
	}
}

# -- create a plugin
set pm [LPlugin pm]
# -- bind its events
pm Bind <<done>> notifier   done:
pm Bind <<notice>> notifier notice:
pm Bind <<result>> notifier result:

# -- start a "server"
set rmt [pm Start "sta"]

# run_cmd foo 1 2 3
proc run_cmd_bak { args } {
    set fmt {set r [catch {%s} rc ];puts [list result $r $rc]; flush stdout}
    set rcmd [format $fmt $args]
    puts "remote cmd=$rcmd"
    pm Send $::rmt $rcmd
}
proc run_cmd { cmd_name arg_list  } {
	set ::pathCnt -1
	set args $arg_list
    puts "run_cmd args=$cmd_name $args"
    set fmt {set r [catch { %s %s} rc ];puts [list result $r $rc]; flush stdout}
    set rcmd [format $fmt $cmd_name $args]
    puts "remote cmd=$rcmd"
    pm Send $::rmt $rcmd
}

# -- send a few commands to the server

#pm Send $rmt {set rc [source remote_scripts.tcl]; puts $rc ; flush stdout}
pm Send $rmt {set rc [source papi_psta.tcl]; puts $rc ; flush stdout}
pm Send $rmt {set rc [echo_args "12 23 45"]; puts $rc ; flush stdout}

set A "13 aaa 15"
run_cmd echo_args $A

#run_cmd echo_args {a b c}



source init_sta.tcl


set args "-delay_type min_max"
run_cmd report_timing $args

run_cmd write_cterms cterms.dat

