source [file join [file dirname [info script]] "test_helpers.tcl"]
set current_dir [file dirname [file normalize [info script]]]
set tests_dir [find_parent_dir $current_dir]
set opendb_dir [find_parent_dir $tests_dir]
set data_dir [file join $tests_dir "data"]

set db [dbDatabase_create]
set chip [odb_read_design $db $data_dir/gscl45nm.lef $data_dir/design.def]
set block [$chip getBlock]
foreach version "DEF_5_3 DEF_5_4 DEF_5_5 DEF_5_6" {
    set result [odb_write_def $block $opendb_dir/build/test.def $version]
    if {[expr $result != 1]} {
        exit 1
    }
    set db [dbDatabase_create]
    set def_parser [new_defin $db]
    set lef_parser [new_lefin $db true]
    set lib [lefin_createTechAndLib $lef_parser gscl45nm $data_dir/gscl45nm.lef]
    set chip [$def_parser createChip {$lib} $opendb_dir/build/test.def]
    if {$chip == "NULL"} {
        puts "Failed to read written def"
        exit 1
    }
    set written_content [exec cat $opendb_dir/build/test.def]
    regexp {VERSION\s+(\d+)\.(\d+)\s+?;} $written_content full_match major minor

    if {![info exists major] || ![info exists minor] || [expr {"DEF_${major}_${minor}" != "$version"}]} {
        puts "Invalid written DEF for version $version" 
        exit 1
    }
}
# Test different syntax
foreach version "5.3 5.4 5.5 5.6" {
    set result [odb_write_def $block $opendb_dir/build/test.def $version]
    if {[expr $result != 1]} {
        exit 1
    }
    set db [dbDatabase_create]
    set def_parser [new_defin $db]
    set lef_parser [new_lefin $db true]
    set lib [lefin_createTechAndLib $lef_parser gscl45nm $data_dir/gscl45nm.lef]
    set chip [$def_parser createChip {$lib} $opendb_dir/build/test.def]
    if {$chip == "NULL"} {
        puts "Failed to read written def"
        exit 1
    }
    set written_content [exec cat $opendb_dir/build/test.def]
    regexp {VERSION\s+(\d+)\.(\d+)\s+?;} $written_content full_match major minor

    if {![info exists major] || ![info exists minor] || [expr {"${major}.${minor}" != "$version"}]} {
        puts "Invalid written DEF for version $version" 
        exit 1
    }
}
exit 0
