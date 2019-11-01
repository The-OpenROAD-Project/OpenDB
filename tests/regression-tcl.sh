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

echo "[5] Read def"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/05-read_def_test.tcl
echo "SUCCESS!"
echo ""

echo "[6] Dump nets"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/06-dump_nets_test.tcl
echo "SUCCESS!"
echo ""

echo "[7] Write def file"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/07-write_def_test.tcl
echo "SUCCESS!"
echo ""

echo "[8] Database LEF access tests"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/08-lef_data_access_test.tcl
echo "SUCCESS!"
echo ""

echo "[9] Database DEF access tests (1)"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/09-gcd_def_access_test.tcl
echo "SUCCESS!"
echo ""

echo "[10] Database DEF access tests (2)"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/10-gcd_pdn_def_access_test.tcl
echo "SUCCESS!"
echo ""

echo "[11] Database edit DEF tests"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/11-edit_def_test.tcl
echo "SUCCESS!"
echo ""

echo "[12] Wire encoder test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/12-wire_encoder.tcl
echo "SUCCESS!"
echo ""

# Test 13 is disabled until issue #25 is fixed
echo "[13] Edit via params test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/13-edit_via_params.tcl
echo "SUCCESS!"
echo ""

echo "[14] Database row create test"
./OpenDB/build/src/swig/tcl/opendbtcl ./OpenDB/tests/tcl/14-opendb_row_settings_test.tcl
echo "SUCCESS!"
echo ""
