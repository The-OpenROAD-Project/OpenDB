source [file join [file dirname [info script]] "test_helpers.tcl"]
set current_dir [file dirname [file normalize [info script]]]
set tests_dir [find_parent_dir $current_dir]
set data_dir [file join $tests_dir "data"]

set db [dbDatabase_create]
set chip [odb_read_design $db $data_dir/gscl45nm.lef $data_dir/design.def]
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
