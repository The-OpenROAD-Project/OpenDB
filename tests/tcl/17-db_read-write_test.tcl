set tcl_dir [file dirname [file normalize [info script]]]
set tests_dir [file dirname $tcl_dir]
set data_dir [file join $tests_dir "data"]
set opendb_dir [file dirname $tests_dir]
source [file join $tcl_dir "test_helpers.tcl"]

set db [dbDatabase_create]
set chip [odb_read_design $db $data_dir/gscl45nm.lef $data_dir/design.def]
if {$chip == "NULL"} {
    puts "Read DEF Failed"
    exit 1
}

set export_result [odb_export_db $db $opendb_dir/build/export.db]
if {!$export_result} {
    puts "Export DB failed"
    exit 1
}

set new_db [dbDatabase_create]
odb_import_db $new_db $opendb_dir/build/export.db

if { [db_diff $db $new_db] } {
  puts "Differences found between exported and imported db"
  exit 1
}

exit 0
