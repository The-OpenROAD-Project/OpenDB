
set d /fs/designs/s1

db_schema -v 1

if [file exists s1.db] {
	db read -file s1.db
} else {
	db load_lef -file $d/lef_verify
	db load_def -file $d/def
	db write -file s1.db
}
db connect_ztop

Ext x
x define_process_corner -ext_model_index 0 -name X

TE te
te make_regular -nproc 8
te snap_tiles
te make_tile_blocks
te get_tile_block_names -string= blocks

# -- $blocks now contains the list of blocks
