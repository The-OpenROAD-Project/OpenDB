set db [dbDatabase_create]
set lib [odb_read_lef $db ./OpenDB/tests/data/gscl45nm.lef]
if {$lib == "NULL"} {
    puts "Failed to read LEF file"
    exit 1
}
exit 0