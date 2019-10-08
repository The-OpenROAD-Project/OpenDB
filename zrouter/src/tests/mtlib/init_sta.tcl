
set MIN_DERATE 0.4
set MAX_DERATE 0.2

set DEBUG_LOG debug_log
set RMT_NAME MIN_MAX
set SDC /home/nefelus/z/zrouter/test/tmg/buf1/buf1.sdc
set VERILOG milos.v
set SPEF_MAX spef.max.gz
set SPEF_MIN spef.min.gz
set SPEF_LIST [list " "]
set LIB_MAX /fs/designs/nec_CB90BLOCK/time_libs/CB90M_WIDE2_1V_M9_MAX_PRIM.lib
set LIB_MIN /fs/designs/nec_CB90BLOCK/time_libs/CB90M_WIDE2_1V_M9_MIN_PRIM.lib
set LIB_LIST [list " "]

set DBS [list " "]
set RESTORE_DIR [list " "]
run_cmd start_init [list $DEBUG_LOG $RMT_NAME]
run_cmd open_eco eco.$RMT_NAME

set ARGS "0.4  2.0  $SDC  $VERILOG $SPEF_LIST {$SPEF_MAX}  {$SPEF_MIN} $LIB_LIST \
 {$LIB_MAX} {$LIB_MIN}  0 top $DBS $RESTORE_DIR "

run_cmd init_netlist $ARGS

