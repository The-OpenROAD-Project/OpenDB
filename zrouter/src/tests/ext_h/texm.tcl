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

proc write_zroute_config { b filename } {
    set fileId [open $filename w 0666]
    puts $fileId "db load_lef -file /u/wfs/demo/tiles/data/e.lef"
    puts $fileId "db load_def -file $b.def"
    puts $fileId "db order_wires"
    puts $fileId "Ext x"
    puts $fileId "x load_model"
    puts $fileId "x extract -cc_model 5"
    puts $fileId "x write_spef -file $b.spef"
    puts $fileId "exit"
    close $fileId
}

proc rnode { b } {
	Subscribe chip.tech
	Subscribe chip.libs
	Subscribe $b.block
    db save_def -use_ids -file scratch/$b.def
    write_zroute_config $b scratch/$b.tcl

    cd scratch
    exec zroute < $b.tcl > $b.log
    cd ..

    Ext x
    x read_spef -use_ids -file scratch/$b.spef

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
			source texm.tcl
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
  te assemble -wrap
  Ext x
  x report_total_cap -ref flat.ctot -file ctot.diff
  Tmg tmg
  tmg make_rcmodels -no_pin_caps
  tmg report_elmore -ref flat.elmore -file elmore.diff
}

}

