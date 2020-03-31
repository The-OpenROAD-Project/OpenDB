set tcl_dir [file dirname [file normalize [info script]]]
source [file join $tcl_dir "test_helpers.tcl"]

set ps1 [odb::newSetFromRect 0 0 20 10]
set ps2 [odb::newSetFromRect 0 0 10 20]
set or [odb::orSet $ps1 $ps2]

set test []
foreach poly [odb::getPolygons $or] {
    foreach pt [odb::getPoints $poly] {
        lappend test [$pt getX]
    }
}

check "polygon points" {list $test} {{0 20 20 10 10 0}}
