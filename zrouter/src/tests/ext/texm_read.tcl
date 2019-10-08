#
# texm_read.tcl: multi-processing extraction, reading spef on remote node
#
#

msh::init

global BLOCKS 

proc do_load {} {
	global env
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
}

proc do_tiling {} {
    global BLOCKS
    TE te
    te set_corner_count -count 3
    te make_regular -nx 3 -ny 1
    te snap_tiles
    te make_tile_blocks
    te get_tile_block_names -string= BLOCKS
    puts $BLOCKS
}

proc rnode { b } {
	Subscribe chip.tech
	Subscribe chip.libs
	Subscribe $b.block

    Ext x
    x load_model
    x read_spef -use_ids -file [file join scratch $b.spef]

	Publish  -dbargs "-parasitics" -wait $b.res.block
}

do {

load {
	do_load
}

tiling {
	do_tiling
}

tile_extract {
	Publish chip.tech
	Publish chip.libs
	foreach b $BLOCKS {
		Publish $b.block
	}

	foreach b $BLOCKS {
		Subscribe  -dbargs "-parasitics" -async $b.res.block
	}

    exec mkdir -p scratch

	set tasks ""
	foreach b $BLOCKS {
		spawn t$b "
			source texm_read.tcl
			rnode $b
		"
		lappend tasks t$b
	}

	monitor $tasks {
      done {
        set itile [string trimleft $task(task) "tT"]
        puts "T$itile DONE"
        te assemble -tile $itile
      }
    }

}

runall {
  do load
  do tiling
  do tile_extract
}

post {
  Ext x
  x report_total_cap -ref e.ctot
  x write_spef -use_ids -preserve_caps -file all_read.spef
  Tmg tmg
  tmg make_rcmodels -no_pin_caps
}

}

