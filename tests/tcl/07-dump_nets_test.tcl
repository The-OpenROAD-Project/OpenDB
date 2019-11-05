set db [dbDatabase_create]
set chip [read_design $db ./OpenDB/tests/data/gscl45nm.lef ./OpenDB/tests/data/design.def]
set block [$chip getBlock]
set nets [$block getNets]
foreach net $nets {
    puts "Net: [$net getName]"
}
exit [expr [llength $nets] != 24]
