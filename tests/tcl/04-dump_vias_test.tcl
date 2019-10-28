set db [dbDatabase_create]
set lef_parser [new_lefin $db true]
set tech [lefin_createTech $lef_parser ./OpenDB/tests/data/gscl45nm.lef]
set vias [dbTech_getVias $tech]
foreach via $vias {
    puts [dbTechVia_getName $via]
}

exit [expr [llength $vias] != 14]
