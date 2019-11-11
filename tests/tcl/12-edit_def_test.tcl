set db [dbDatabase_create]
set chip [odb_read_design $db ./OpenDB/tests/data/gscl45nm.lef ./OpenDB/tests/data/design.def]
set lib [lindex [$db getLibs] 0]
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
