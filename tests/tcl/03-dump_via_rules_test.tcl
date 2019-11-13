set db [dbDatabase_create]
set lib [odb_read_lef $db ./OpenDB/tests/data/gscl45nm.lef]
if {$lib == "NULL"} {
    exit 1
}
set tech [$lib getTech]
if {$tech == "NULL"} {
    exit 1
}
set via_rules [$tech getViaRules]
exit 0