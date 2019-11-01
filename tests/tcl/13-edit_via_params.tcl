set db [dbDatabase_create]
set lef_parser [new_lefin $db false]
set def_parser [new_defin $db]
set def_writer [new_defout]
set lib [$lef_parser createTechAndLib gscl45nm ./OpenDB/tests/data/Nangate45/NangateOpenCellLibrary.mod.lef]
set chip [$def_parser createChip [$db getLibs] ./OpenDB/tests/data/gcd/gcd_pdn.def]
if {$chip == "NULL"} {
    puts "Read DEF Failed"
    exit 1
}
set block [$chip getBlock]
set vias [$block getVias]
set via [lindex $vias 0]
puts $via
set params [$via getViaParams]
puts $params
$params setNumCutRows 3
$via setViaParams $params
puts $num_cut_rows