set db [dbDatabase_create]
set lib [odb_read_lef $db ./OpenDB/tests/data/gscl45nm.lef]
set tech [$lib getTech]
set vias [$tech getVias]
foreach via $vias {
    puts [$via getName]
}

exit [expr [llength $vias] != 14]
