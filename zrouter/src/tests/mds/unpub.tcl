#
#
# Test streaming interface
#
# NOTE: this does not work, since the db interface has changed
db_schema -v 1

#
# spawn a command to process 'tile' t
#

proc do_tile {t} {
    spawn t$t "
        msh::init
        Subscribe b1.block
        Wait db
        after 1000
        Publish $t.res.block
        Wait $t.res.block
    "
}

# how many tiles to use
set tiles "1 2 3 4 5 6"

msh::init
do {
    load {
        source s1.tcl
    }

    local {
        Publish b1.block

        foreach t $tiles {
            Subscribe -async b$t.res.block
        }
    }
    spawn {
        foreach t $tiles {
            do_tile $t
        }
    }
    monitor {
	monitor $tiles {
	    done {
		parray task
		# puts "$task(id):$task(status)"
	    }
	}
    }
    all {
	do load
	do local
	do spawn
	do monitor
    }
}
