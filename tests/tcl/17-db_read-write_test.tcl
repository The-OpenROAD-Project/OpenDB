set chip [read_design $db ./OpenDB/tests/data/gscl45nm.lef ./OpenDB/tests/data/design.def]
if {$chip == "NULL"} {
    puts "Read DEF Failed"
    exit 1
}

set export_result [export_db $db ./OpenDB/build/export.db]
if {$export_result != 1} {
    puts "Export DB failed"
    exit 1
}

set new_db [dbDatabase_create]
import_db $new_db ./OpenDB/build/export.db
if {$new_db == "NULL"} {
    puts "Import DB Failed"
    exit 1
}
set diff_file [fopen ./OpenDB/build/db-export-import-diff.txt w]
set diff_rc [dbDatabase_diff $db $new_db $diff_file 4]
fclose $diff_file
if {$diff_rc != "0"} {
    puts "Database diff failed"
    exit 1
}
if {[exec cat ./OpenDB/build/db-export-import-diff.txt] != ""} {
    puts "Differences found between exported and imported db"
    exit 1
}
exit 0