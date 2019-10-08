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

TE te
te make_regular -nx 3 -ny 1
te snap_tiles
te make_tile_blocks
te get_tile_block_names -string= tblocks

Ext x
x load_model

foreach bname $tblocks {
  puts $bname
  db find -block $bname -object= blk
  x set_block -block $blk
  x read_spef -use_ids -file scratch/$bname.spef
}

te assemble

db get -top -object= topblk
x set_block -block $topblk

x report_total_cap -ref flat.ctot -file ctot.diff
puts ""

Tmg tmg
tmg make_rcmodels -no_pin_caps
tmg report_elmore -ref flat.elmore -file elmore.diff

exit
