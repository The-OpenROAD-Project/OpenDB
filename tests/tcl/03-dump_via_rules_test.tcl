set db [dbDatabase_create]
set lef_parser [new_lefin $db true]
set tech [lefin_createTech $lef_parser ./OpenDB/tests/data/gscl45nm.lef]
if {$tech == "NULL"} {
    exit 1
}
set via_rules [dbTech_getViaRules $tech]
exit 0