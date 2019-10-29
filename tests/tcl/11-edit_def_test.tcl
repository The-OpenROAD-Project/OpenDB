set db [dbDatabase_create]
set lef_parser [new_lefin $db true]
set def_parser [new_defin $db]
set lib [lefin_createTechAndLib $lef_parser gscl45nm ./OpenDB/tests/data/gscl45nm.lef]
set chip [$def_parser createChip {$lib} ./OpenDB/tests/data/design.def]
if {$chip == "NULL"} {
    puts "Read DEF Failed"
    exit 1
}
set block [$chip getBlock]
set net [dbNet_create $block "w1"]
$net setSigType "POWER"
set swire [dbSWire_create $net "ROUTED"]
if {[$swire getNet] != "$net"} {
    exit 1
}
set site [lindex [$lib getSites] 0]
set row [dbRow_create $block "row0" $site 0 0 "RO" "HORIZONTAL" 1 10]
if {$row == "NULL"} {
    exit 1
}
puts $net
puts $row
puts $swire
exit 0
