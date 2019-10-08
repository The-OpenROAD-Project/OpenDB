if [file exists e.db] {
   puts "reading db..."
   db read -file e.db
} else {
   db load_lef -file /u/wfs/demo/tiles/data/e.lef
   db load_def -file /u/wfs/demo/tiles/data/e.def
   db order_wires
   db write -file e.db
}

TE te
te make_regular -nx 3 -ny 2
te snap_tiles
te make_tile_blocks
te get_tile_block_names -string= tblocks

Ext x
x load_model

foreach bname $tblocks {
  puts $bname
  db find -block $bname -object= blk
  x set_block -block $blk
  x extract
}

te assemble

db get -top -object= topblk
x set_block -block $topblk
x write_spef -file all.spef

x report_total_cap -ref e.ctot

Tmg tmg
tmg make_rcmodels -no_pin_caps

exit
