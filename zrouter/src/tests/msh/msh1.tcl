#
# test: simple msh test
#

msh::init

do { 
	spawn {
		spawn t1 {
			list [info hostname] [pid] [info name]
		}
		spawn t2 {
			# after 2000
			list [info hostname] [pid] [info name]
		}
		spawn t3 {
			expr 1/0
		}
	}

	monitor {
		monitor "t1 t2 t3" {
			done {
				if { $task(error) != "" } {
					puts "$task(task) failed: $task(error)"
				} else {
					puts "$task(task) output: $task(return)"
				}
			}
			check {
				puts "$task(task) checked $task(state)"
			}
		}
	}
	test3 {
		spawn t3 "array get env" 
		monitor t3 {
			done {
				puts "$task(task) done: <<$task(output)>>"
			}
		}
	}
}
