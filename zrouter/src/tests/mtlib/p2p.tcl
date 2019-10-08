package require Itcl
lappend auto_path $env(HOME)/work/zrouter/src/mtlib

switch $argv {
    slave {
	set env(ADESLAVETAG) slave1
	set env(ADEADDRESS) 1234@localhost
	# -- create  a slave
	set slave [ADESlave #auto -debug 1 -tag slave1 -server 1234@localhost]
	$slave OnEvent {
	    connected {
		puts "connected to master"
	    }
	    hello {
		global slave
		$slave Send "howdy"
	    }
	}
    }
    master {
	set master [ADEMaster #auto -port 1234 -debug 1]
	$master OnEvent {
	    slave1.connected {
		puts "slave1 has been connected"
		global master
		$master Send slave1 hello
	    }
	    slave1.howdy {
		puts "slave1 has replied"
	    }
	}
    }
    default {
	puts "$argv0 master|slave"
	exit 1
    }
}
vwait forever
}

exit
}
