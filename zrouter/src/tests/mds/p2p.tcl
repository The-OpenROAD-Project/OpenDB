

msh::init

set var 100

Publish   -p2p 1          var.var
Subscribe -p2p 1 -async var.1.var

spawn t1 {
    msh::init
    puts "waiting for var to arrive"
    Subscribe -p2p 1 var.var
    set var [expr \$var +1]
    Publish   -p2p 1 var.1.var
    puts "waiting for var to be fetched"
    Wait var.1.var
    return \$var
}

puts var=$var
puts "waiting for var to change"
vwait var
puts newvar=$var
Unpublish var.var
Unpublish var.1.var
