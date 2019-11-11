set db [dbDatabase_create]
set chip [odb_read_design $db ./OpenDB/tests/data/gscl45nm.lef ./OpenDB/tests/data/design.def]
set lib [lindex [$db getLibs] 0]
set tech [$db getTech]
if {$chip == "NULL"} {
    puts "Read DEF Failed"
    exit 1
}
set vias [$tech getVias]
set via1 [lindex $vias 0]
set layer1 [$via1 getBottomLayer]
set via2 [lindex $vias 1]
set via3 [lindex $vias 2]

set block [$chip getBlock]
set net [dbNet_create $block "w1"]
set wire [dbWire_create $net]
set wire_encoder [dbWireEncoder]

# Encoding
$wire_encoder begin $wire
# void newPath( dbTechLayer * layer, dbWireType type )
$wire_encoder newPath $layer1 "ROUTED" 
# int addPoint( int x, int y, uint property = 0 );
$wire_encoder addPoint 2000 2000
set jid1 [$wire_encoder addPoint 10000 2000]
$wire_encoder addPoint 18000 2000
$wire_encoder newPath $jid1
# int addTechVia( dbTechVia * via );
$wire_encoder addTechVia $via1
set jid2 [$wire_encoder addPoint 10000 10000]
$wire_encoder addPoint 10000 18000
$wire_encoder newPath $jid2
set jid3 [$wire_encoder addTechVia $via2]
$wire_encoder end

set result [write_def $block ./OpenDB/build/wire_encoder.def]
exit [expr $result != 1]