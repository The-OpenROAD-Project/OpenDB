set chip [read_design $db ./OpenDB/tests/data/gscl45nm.lef ./OpenDB/tests/data/design.def]
set lib [lindex [$db getLibs] 0]
set block [$chip getBlock]
set lef_write_result [write_lef $lib ./OpenDB/build/test.lef]
if {$lef_write_result != 1} {
    exit 1
}
set def_write_result [write_def $block ./OpenDB/build/test.def]
if {$def_write_result != 1} {
    exit 1
}
exit 0