msh::init

puts "*** sending one task to the selected profile slave"
spawn t1 "after 5000 ;array get env"
spawn t2 "after 5000 ;array get env"
spawn t3 "after 5000 ;array get env"
spawn t4 "after 5000 ;array get env"

monitor "t1 t2 t3 t4" {
done {
    array set e $task(return)
    puts "*** task run on: $e(LSB_MCPU_HOSTS) $e(LSB_QUEUE)"
}
}
t1 delete
t2 delete
t3 delete
t4 delete
