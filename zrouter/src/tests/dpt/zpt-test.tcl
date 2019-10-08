#
# Generate a data set and a picture using dtest
#
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

proc dump {file args} {
	set fp [open "| bzip2 -9 > $file" w]
	puts $fp [uplevel $args]
	flush $fp
	close $fp
}



# -- initialize dtest
proc dpt_init {xy bb slices out} {
	set fp [open "| ../../dcr/dptn/dtest | cjpeg -quality 80 > $out" w]
	puts $fp "c 2048 2048 $bb $slices"
	puts $fp "C 0 ffff00"
	puts $fp "C 1 00ffcc"
	puts $fp "C 2 ccffcc"
	puts $fp "C 3 00ff00"
	puts $fp "C 4 ff0000"
	puts $fp "C 5 0000ff"
	puts $fp "C 6 aaaacc"
	puts $fp "C 7 ccccaa"
	return $fp
}

# -- load dtest from the object list
proc dpt_load {objs maxlayer fp} {
	foreach obj $objs {
		array set L $obj
		# -- ignore layer > $maxlayer
		if { $L(layer) < $maxlayer } {
			puts $fp "b $L(bb) $L(layer)"
		}
		unset L
	}
	puts $fp "w 0"
	if [catch {close $fp} rc] {
		puts stderr "$rc"
	}
}

# -- generate an object list, use dtest to render it. produce a jpeg image
proc dpt_test {img} {
	# -- small bb (8k objcts)
	set bb "1681156 206957 1843710 349526"

	# -- large bb (49k objects)
	set bb "1656874 580167 1895085 789089"

	set layers 8
	set cmd "chip_get -chip all -bbox {$bb} -layer {4 0 5 1 6 2 7 3} -hierarchy {} -type {inst/pin inst/bbox inst/obs} -gx 1562. -gy 1562."
	set fp [dpt_init "2048 2048" $bb $layers $img]
	dpt_load [eval $cmd] $layers $fp
}

dpt_test set0.jpg
