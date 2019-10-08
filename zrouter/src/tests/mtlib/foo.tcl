
set master [ADEMaster #auto -port 1235 -debug 1]
parray env
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
msh::init

spawn t1 {
# -- create  a slave
#set slave [ADESlave #auto -debug 1 -tag slave1 -server 1235@localhost]
set slave [ADESlave #auto -debug 1 -tag slave1]
\$slave OnEvent {
    connected {
	puts "connected to master"
    }
    hello {
	global slave
	\$slave Send "howdy"
    }
}
vwait forever
}

