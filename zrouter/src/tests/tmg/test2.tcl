if [file exists e.db] {
   puts "reading db..."
   db read -file e.db
} else {
   db load_lef -file /u/wfs/demo/tiles/data/e.lef
   db load_def -file /u/wfs/demo/tiles/data/e.def
   db order_wires
   db write -file e.db
}

# Ext x
# x load_model
# x extract

set d  /fs/designs/nec2/libs

Tmg tmg
tmg set_slew_def -lo 0 -hi 100
tmg load_lib -max -file "
  $d/CB130M_CMOS_12V_H9_MAX_DRIVE.lib
  $d/CB130M_CMOS_12V_H9_MAX_SCAN.lib
  $d/CB130M_CMOS_12V_H9_MAX_GATING.lib
  $d/CB130M_CMOS_12V_H9_MAX_SPECIAL.lib
  $d/CB130M_CMOS_12V_H9_MAX_IO.lib
  $d/CB130M_PC5_12V-M_COM_MAX.lib
  $d/CB130M_CMOS_12V_H9_MAX_PRIM.lib "
tmg load_lib -min -file "
  $d/CB130M_CMOS_12V_H9_MIN_DRIVE.lib
  $d/CB130M_CMOS_12V_H9_MIN_SCAN.lib
  $d/CB130M_CMOS_12V_H9_MIN_GATING.lib
  $d/CB130M_CMOS_12V_H9_MIN_SPECIAL.lib
  $d/CB130M_CMOS_12V_H9_MIN_IO.lib
  $d/CB130M_PC5_12V-M_COM_MIN.lib
  $d/CB130M_CMOS_12V_H9_MIN_PRIM.lib "

# tmg check_lib -verbose
tmg check_lib

tmg report_delay -master TC2BUFXN 
tmg report_delay -master TC2BUFXC 
tmg report_delay -master TC2BUFXC -in_pin A -out_pin Y  -in_slew 0.2 -cload 0.175 -test 2

exit
