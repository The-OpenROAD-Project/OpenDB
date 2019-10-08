proc get_hostname { } {
	set rc [info hostname]
	puts "hostname=$rc"
	flush stdout
}

proc echo_args { args } {
	puts "args=$args"
	flush stdout
}

puts "Finished remote_scripts"

