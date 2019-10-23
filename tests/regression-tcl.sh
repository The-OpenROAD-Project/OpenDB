set -e

echo "Running tests .."
echo "================"

echo "[1] Importing opendbtcl package"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/import_package_test.tcl
echo "SUCESS!"
echo ""

echo "[2] Reading lef files"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/read_lef_test.tcl
echo "SUCESS!"
echo ""

echo "[3] Dump via rules"
./OpenDB/build/src/swig/tcl/opendbtcl < ./OpenDB/tests/tcl/dump_via_rules_test.tcl
echo "SUCESS!"
echo ""