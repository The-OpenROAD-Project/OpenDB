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

Ext x
x load_model
x extract -cc_model 5
x report_total_cap -file flat.ctot

Tmg tmg
tmg make_rcmodels -no_pin_caps
tmg report_elmore -file flat.elmore

exit
