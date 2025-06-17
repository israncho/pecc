#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Use: $0 <file.tsp>"
    exit 1
fi

INPUT_TSP="$1"
OUTPUT_PNG="${INPUT_TSP%.tsp}_performance.png"
TEMP_FILE="temp_performance_data.dat"

echo "Processing performance data..."
python3 format_for_plotting_performance.py "$INPUT_TSP" "$TEMP_FILE" || {
    echo "Error while processing .tsp with Python."
    exit 1
}

echo "Generating performance plot with GNUPlot..."
gnuplot <<- EOF
    set terminal pngcairo enhanced font "Arial,12"
    set output "$OUTPUT_PNG"
    set title "Genetic Algorithm Performance: $(basename "$INPUT_TSP")"
    set xlabel "Generation"
    set ylabel "Fitness"
    set grid
    plot "$TEMP_FILE" using 0:1 with lines lc "red" title "Population avg fitness", \
         "$TEMP_FILE" using 0:2 with lines lc "blue" title "Fitness of best individual"
EOF

rm -f "$TEMP_FILE"
echo "¡All done! plot saved in '$OUTPUT_PNG'"