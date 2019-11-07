set db [dbDatabase_create]
set lib [read_lef $db ./OpenDB/tests/data/gscl45nm.lef]
set chip [read_def $lib "./OpenDB/tests/data/design.def"]
if {$chip == "NULL"} {
    puts "Read DEF Failed"
    exit 1
}
puts $chip
exit 0