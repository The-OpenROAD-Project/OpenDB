set tcl_dir [file dirname [file normalize [info script]]]
set tests_dir [file dirname $tcl_dir]
set data_dir [file join $tests_dir "data"]
set opendb_dir [file dirname $tests_dir]
source [file join $tcl_dir "test_helpers.tcl"]

set db [dbDatabase_create]
set chip [odb_read_design $db $data_dir/gscl45nm.lef $data_dir/design.def]
set lib [lindex [$db getLibs] 0]
set block [$chip getBlock]
set lef_write_result [odb_write_lef $lib $opendb_dir/build/test.lef]
if {$lef_write_result != 1} {
    exit 1
}
set def_write_result [odb_write_def $block $opendb_dir/build/test.def]
if {$def_write_result != 1} {
    exit 1
}
exit 0
