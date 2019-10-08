if [file exists e.db] {
   puts "reading db..."
   db read -file e.db
} else {
   db load_lef -file /u/wfs/demo/tiles/data/e.lef
   db load_def -file /u/wfs/demo/tiles/data/e.def
   db order_wires
   db write -file e.db
}
db connect_ztop

proc write_zroute_config { b filename } {
    set fileId [open $filename w 0664]
    puts $fileId "db load_lef -file /u/wfs/demo/tiles/data/e.lef"
    puts $fileId "db load_def -file $b.def"
    puts $fileId "db order_wires"
    puts $fileId "Ext x"
    puts $fileId "x load_model"
    puts $fileId "x extract -cc_model 5"
    puts $fileId "x write_spef -file $b.spef"
    puts $fileId "x report_total_cap -file $b.ctot"
    puts $fileId "exit"
    close $fileId
}

TE te
te make_regular -nx 3 -ny 1
te snap_tiles
te make_tile_blocks
te get_tile_block_names -string= tblocks

foreach bname $tblocks {
  puts $bname
  db find -block $bname -object= blk
  db save_def -block $blk -use_ids -file scratch/$bname.def
  write_zroute_config $bname scratch/$bname.tcl
}

exit
