set chip [read_design $db ./OpenDB/tests/data/gscl45nm.lef ./OpenDB/tests/data/design.def]
if {$chip == "NULL"} {
    puts "Read DEF Failed"
    exit 1
}
puts $chip
exit 0