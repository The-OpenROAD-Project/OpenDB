set -e

export PYTHONPATH=/OpenROAD/build/src/swig/python:$PYTHONPATH

echo "Running tests .."
echo "================"

echo "[1] Importing opendbpy package"
python3 /OpenDB/tests/python/import_package_test.py
echo "SUCESS!"
echo ""

echo "[2] Reading lef files"
python3 /OpenDB/tests/python/read_lef_test.py
echo "SUCESS!"
echo ""