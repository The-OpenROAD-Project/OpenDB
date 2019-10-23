set db [dbDatabase_create]
set lef_parser [new_lefin $db true]
set tech [lefin_createTech $lef_parser ./OpenDB/tests/data/gscl45nm.lef]