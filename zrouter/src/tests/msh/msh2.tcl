#
# test: large spawn
#
msh::init
#
# send a null job on a large number of hosts
# check if we can go through
#
proc check {N {cmd "after 5000 ; list 1"}} {
    set tasks ""
    array set HOST {}

    # -- spawn N test tasks
    for {set i 0} { $i < $N} { incr i } {
        lappend tasks [spawn t$i $cmd]
    }

    #-- monitor the tasks
    monitor $tasks {
        done {
            if { $task(error) != "" } {
                set HOST($task(host)) "trouble ($task(error))"
            } else {
                set HOST($task(host)) ok
            }
        }
    }

    # -- cleanup the tasks
    foreach t $tasks {
        $t delete
    }
    foreach h [array names HOST] {
        puts [format "%-22s: %s" $h $HOST($h)]
    }
}

check 40
