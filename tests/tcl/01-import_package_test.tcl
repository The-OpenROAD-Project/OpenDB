set db [dbDatabase_create]
if {$db == "NULL"} {
    puts "Create DB failed"
    exit 1
}
exit 0
