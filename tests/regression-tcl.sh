set -e

echo "Running tests .."
echo "================"

echo "[1] Importing opendbtcl package"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/01-import_package_test.tcl
echo "SUCCESS!"
echo ""

echo "[2] Reading lef files"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/02-read_lef_test.tcl
echo "SUCCESS!"
echo ""

echo "[3] Dump via rules"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/03-dump_via_rules_test.tcl
echo "SUCCESS!"
echo ""

echo "[4] Dump vias"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/04-dump_vias_test.tcl
echo "SUCCESS!"
echo ""

echo "[5] Read DEF"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/05-read_def_test.tcl
echo "SUCCESS!"
echo ""

echo "[6] Read design"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/06-read_design_test.tcl
echo "SUCCESS!"
echo ""

echo "[7] Dump nets"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/07-dump_nets_test.tcl
echo "SUCCESS!"
echo ""

echo "[8] Write LEF/DEF file"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/08-write_lef_and_def_test.tcl
echo "SUCCESS!"
echo ""

echo "[9] Database LEF access tests"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/09-lef_data_access_test.tcl
echo "SUCCESS!"
echo ""

echo "[10] Database DEF access tests (1)"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/10-gcd_def_access_test.tcl
echo "SUCCESS!"
echo ""

echo "[11] Database DEF access tests (2)"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/11-gcd_pdn_def_access_test.tcl
echo "SUCCESS!"
echo ""

echo "[12] Database edit DEF tests"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/12-edit_def_test.tcl
echo "SUCCESS!"
echo ""

echo "[13] Wire encoder test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/13-wire_encoder_test.tcl
echo "SUCCESS!"
echo ""

echo "[14] Edit via params test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/14-edit_via_params_test.tcl
echo "SUCCESS!"
echo ""

echo "[15] Database row create test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/15-row_settings_test.tcl
echo "SUCCESS!"
echo ""

echo "[16] Wrtie def versions test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/16-write_def_versions_test.tcl
echo "SUCCESS!"
echo ""

echo "[17] Database read/write test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/17-db_read-write_test.tcl
echo "SUCCESS!"
echo ""

echo "[18] Check routing tracks test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/18-check_routing_tracks.tcl
echo "SUCCESS!"
echo ""