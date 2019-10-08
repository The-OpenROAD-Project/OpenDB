

msh::init

set var 100

Publish var.var
Subscribe -async var.1.var

spawn t1 {
    msh::init
    puts "waiting for var to be received"
    Subscribe var.var
    set var [expr \$var +1]
    Publish  var.1.var
    puts "waiting for var to be requested"
    Wait var.1.var
    return \$var
}

puts var=$var
puts "waiting for var to change"
vwait var
puts newvar=$var
Unpublish var.var
Unpublish var.1.var
