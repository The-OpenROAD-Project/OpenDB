foreach lib { } {

	read_lib $lib

}

foreach lib {/fs/designs/nec_CB90BLOCK/time_libs/CB90M_WIDE2_1V_M9_MAX_PRIM.lib} {

	read_lib -max $lib

}

foreach lib {/fs/designs/nec_CB90BLOCK/time_libs/CB90M_WIDE2_1V_M9_MIN_PRIM.lib} {

	read_lib -min $lib

}

read_verilog milos.v

link_design top

sta::set_path_min_max min_max

set timing_save_pin_arrival_and_slack true

read_sdc /home/nefelus/z/zrouter/test/tmg/buf1/buf1.sdc

foreach s { } {
	read_parasitics -analysis_point min_max $s
}
foreach s {spef.max.gz} {
	read_parasitics -analysis_point max $s
}
foreach s {spef.min.gz} {
	read_parasitics -analysis_point min $s
}
set timing_remove_clock_reconvergence_pessimism true
create_net _MILOS_DUMMYX
set_timing_derate -early 0.4
set_timing_derate -late 2.0
