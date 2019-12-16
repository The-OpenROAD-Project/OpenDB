set tcl_dir [file dirname [file normalize [info script]]]
set tests_dir [file dirname $tcl_dir]
set data_dir [file join $tests_dir "data"]
source [file join $tcl_dir "test_helpers.tcl"]

set db [dbDatabase_create]
set chip [odb_read_design $db $data_dir/gscl45nm.lef $data_dir/design.def]
set block [$chip getBlock]
set nets [$block getNets]
foreach net $nets {
    puts "Net: [$net getName]"
}
exit [expr [llength $nets] != 24]
