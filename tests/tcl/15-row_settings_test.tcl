source [file join [file dirname [info script]] "test_helpers.tcl"]
set current_dir [file dirname [file normalize [info script]]]
set tests_dir [find_parent_dir $current_dir]
set opendb_dir [find_parent_dir $tests_dir]
set data_dir [file join $tests_dir "data"]

set db [dbDatabase_create]
set chip [odb_read_design $db  $data_dir/Nangate45/NangateOpenCellLibrary.mod.lef $data_dir/gcd/floorplan.def]
set lib [lindex [$db getLibs] 0]


set block [$chip getBlock]
set tech [[$block getDataBase] getTech]
set lib [lindex [concat {*}[[$block getDataBase] getLibs]] 0]
set site [lindex [set sites [$lib getSites]] 0]
set rt [dbRow_create $block ROW_test $site 0 380 "MX" "HORIZONTAL" 420 380]

check "row name" {$rt getName} ROW_test
check "row origin" {$rt getOrigin} "0 380"
check "row site" {[$rt getSite] getName} [$site getName]
check "row direction" {$rt getDirection} "HORIZONTAL"
check "row orientation" {$rt getOrient} "MX"
check "row spacing" {$rt getSpacing} 380
check "row site count" {$rt getSiteCount} 420

set rt1 [dbRow_create $block ROW_test $site 0 380 "R0" "HORIZONTAL" 420 380]

check "row direction" {$rt1 getDirection} "HORIZONTAL"
check "row orientation" {$rt1 getOrient} "R0"

set rt2 [dbRow_create $block ROW_test $site 0 380 "R0" "VERTICAL" 420 380]

check "row direction" {$rt2 getDirection} "VERTICAL"
check "row orientation" {$rt2 getOrient} "R0"

exit_summary
