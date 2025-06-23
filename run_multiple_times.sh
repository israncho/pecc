#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Use: $0 <algorithm> <directory_path> [veces=50]"
    echo "example: $0 memetic_euc_tsp test_inputs/saturn 30"
    exit 1
fi

ALG_TYPE=$1
TEST_DIR=$2
RUNS=${3:-75} # default value 50 
ALG_BIN="./bin/pecc"

# Validaciones básicas
[ ! -d "$TEST_DIR" ] && echo "Error: Directory $TEST_DIR does not exist." && exit 1
[ ! -f "$ALG_BIN" ] && echo "Error: Binary $ALG_BIN not found" && exit 1

# exec for every test file
for input_file in "$TEST_DIR"/*.txt; do
    echo "--Executing $ALG_TYPE with the parameters in $(basename "$input_file") $RUNS times..."
    for ((i=1; i<=RUNS; i++)); do
        "$ALG_BIN" "$ALG_TYPE" "$input_file"
        echo "Execution $i done." 
    done
done

echo "Finished: $ALG_TYPE executed $RUNS times per file."