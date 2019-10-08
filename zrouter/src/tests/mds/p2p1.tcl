#
#
# Test streaming interface using p2p
#
# NOTE: this does not work, since the db interface has changed
db_schema -v 1

#
# spawn a command to process 'tile' t
#

proc do_tile {b} {
    set task [string tolower $b]
    spawn $task "
        zr.zlog enable_debug_log -module ZTC
        msh::init
        Subscribe chip.tech
        Subscribe chip.libs
        Subscribe -p2p 1 $b.block
        after 1000
        Publish -wait -p2p 1 $b.res.block
    "
}

#set tasks ""
zr.zlog enable_debug_log -module ZTC
msh::init
do {
    load {
        source s1.tcl
    }

    local {
        Publish chip.tech
        Publish chip.libs
	foreach b $blocks {
	    Publish -p2p 1 $b.block
	}

	foreach b $blocks {
            Subscribe -p2p 1 -async $b.res.block
        }
    }
    spawn {
	set tasks ""
        foreach b $blocks {
            lappend tasks [do_tile $b]
        }
    }
    monitor {
	monitor $tasks {
	    done {
		puts "$task(tid): '$task(return)'"
		if { $task(error) != "" } {
		    puts "   error:$task(error)"
		}
	    }
	}
    }
}
