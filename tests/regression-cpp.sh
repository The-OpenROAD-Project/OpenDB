
set -e

BASE_DIR=$(dirname $0)

files=$(find $BASE_DIR/../build/tests/cpp -maxdepth 1 -name "Test*")
for file in $files
do
    name=$(echo $file | awk -F"/" '{print $NF}')
    echo "$name"
    echo ""
    $BASE_DIR/../build/tests/cpp/$name
    echo ""
done
