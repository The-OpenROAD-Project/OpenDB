set -e

BASE_DIR=$(dirname $0)

export PYTHONPATH=$BASE_DIR/../build/src/swig/python:$PYTHONPATH

echo "Running tests .."
echo "================"

echo "[1] dbNet Unit Test"
python3 $BASE_DIR/unitTestsPython/TestNet.py
echo ""
