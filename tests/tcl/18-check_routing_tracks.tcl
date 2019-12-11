source [file join [file dirname [info script]] "test_helpers.tcl"]
set current_dir [file dirname [file normalize [info script]]]
set tests_dir [find_parent_dir $current_dir]
set opendb_dir [find_parent_dir $tests_dir]
set data_dir [file join $tests_dir "data"]

set db [dbDatabase_create]
set chip [odb_read_design $db $data_dir/Nangate45/NangateOpenCellLibrary.mod.lef $data_dir/gcd/floorplan.def]

set tech [$db getTech]

if {$tech == "NULL"} {
    puts "Read tech Failed"
    exit 1
}

set block [$chip getBlock]

if {$block == "NULL"} {
    puts "Read block Failed"
    exit 1
}

set routing_layers_count [$tech getRoutingLayerCount]

for {set l 1} {$l <= $routing_layers_count} {incr l} {
    set routingLayer [$tech findRoutingLayer $l]

    if {$routingLayer == "NULL"} {
        puts "Read routing layer Failed"
        exit 1
    }

    set routingTrack [$block findTrackGrid $routingLayer]
    if {$routingTrack == "NULL"} {
        puts "Read routing track for layer $l Failed"
        exit 1
    }
}