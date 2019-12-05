source [file join [file dirname [info script]] "test_helpers.tcl"]
set current_dir [file dirname [file normalize [info script]]]
set tests_dir [find_parent_dir $current_dir]
set opendb_dir [find_parent_dir $tests_dir]
set data_dir [file join $tests_dir "data"]

set db [dbDatabase_create]
set chip [odb_read_design $db $data_dir/gscl45nm.lef $data_dir/design.def]
if {$chip == "NULL"} {
    puts "Read DEF Failed"
    exit 1
}

set export_result [odb_export_db $db $opendb_dir/build/export.db]
if {$export_result != 1} {
    puts "Export DB failed"
    exit 1
}

set new_db [dbDatabase_create]
odb_import_db $new_db $opendb_dir/build/export.db
if {$new_db == "NULL"} {
    puts "Import DB Failed"
    exit 1
}
set diff_file [fopen $opendb_dir/build/db-export-import-diff.txt w]
set diff_rc [dbDatabase_diff $db $new_db $diff_file 4]
fclose $diff_file
if {$diff_rc != "0"} {
    puts "Database diff failed"
    exit 1
}
if {[exec cat $opendb_dir/build/db-export-import-diff.txt] != ""} {
    puts "Differences found between exported and imported db"
    exit 1
}
exit 0