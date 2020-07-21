set -e

BASE_DIR=$(dirname $0)

export PYTHONPATH=$BASE_DIR/../build/src/swig/python:$PYTHONPATH

echo "Running tests .."
echo ""
files=$(find $BASE_DIR/unitTestsPython -name "Test*.py")
for file in $files
do
    name=$(echo $file | awk -F"/" '{print $NF}')
    echo "$name"
    echo ""
    python3 $BASE_DIR/unitTestsPython/$name
    echo ""
done

# echo "Running tests .."
# echo "================"

# echo "[1] dbNet Unit Test"
# python3 $BASE_DIR/unitTestsPython/TestNet.py
# echo ""

# echo "[2] dbInst Unit Test"
# python3 $BASE_DIR/unitTestsPython/TestInst.py
# echo ""

# echo "[5] Destroy Integration Test"
# python3 $BASE_DIR/unitTestsPython/TestDestroy.py
# echo ""