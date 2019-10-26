set -e

echo "Running tests .."
echo "================"

echo "[1] Importing opendbtcl package"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/import_package_test.tcl
echo "SUCCESS!"
echo ""

echo "[2] Reading lef files"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/read_lef_test.tcl
echo "SUCCESS!"
echo ""

echo "[3] Dump via rules"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/dump_via_rules_test.tcl
echo "SUCCESS!"
echo ""

echo "[4] Dump vias"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/dump_vias.tcl
echo "SUCCESS!"
echo ""

echo "[5] Read def"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/read_def_test.tcl
echo "SUCCESS!"
echo ""

echo "[6] Dump nets"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/dump_nets.tcl
echo "SUCCESS!"
echo ""

echo "[7] Write def file"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/dump_nets.tcl
echo "SUCCESS!"
echo ""

echo "[8] Database access tests"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/lef_data_access_test.tcl
echo "SUCCESS!"
echo ""
