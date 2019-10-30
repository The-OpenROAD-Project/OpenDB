set db [dbDatabase_create]
set lef_parser [new_lefin $db true]
set tech [$lef_parser createTech ./OpenDB/tests/data/gscl45nm.lef]
set vias [$tech getVias]
foreach via $vias {
    puts [$via getName]
}

exit [expr [llength $vias] != 14]
