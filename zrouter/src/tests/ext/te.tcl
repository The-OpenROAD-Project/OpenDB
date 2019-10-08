if [file exists e.db] {
   puts "reading db..."
   db read -file e.db
} else {
   db load_lef -file /u/wfs/demo/tiles/data/e.lef
   db load_def -file /u/wfs/demo/tiles/data/e.def
   db order_wires
   db write -file e.db
}

Ext x
x load_model
x extract
x report_total_cap -file e.ctot
#x write_spef -file e_names.spef
x write_spef -use_ids -file e.spef

Tmg tmg
tmg make_rcmodels -no_pin_caps

exit
db connect_ztop
