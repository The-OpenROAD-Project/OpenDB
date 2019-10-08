#
# use the supplied msh.conf
#
msh::init

set tasks ""

foreach t "1 2 3 4 5 6 7 8 9 0" {
    foreach {task class} "t1 demo1 t2 demo2 t3 demo1 t4 demo2" {
    	lappend tasks $task-$t
    	spawn $task-$t "
		after 1000;
		list \[info hostname\] \[set env(LSB_QUEUE)\]" -class $class
    }
}

monitor $tasks {
    done {
	puts "$task(task) : $task(state) ($task(return))"
    }
}

status
