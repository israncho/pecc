#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Use: $0 <file.tsp>"
    exit 1
fi

INPUT_TSP="$1"
OUTPUT_PNG="${INPUT_TSP%.tsp}.png"
TEMP_FILE="temp_formatted.tsp"

echo "Giving format to '$INPUT_TSP' with Python..."
python3 python/format_for_plotting_solution.py "$INPUT_TSP" "$TEMP_FILE" || {
    echo "Error while processing .tsp with Python."
    exit 1
}

echo "Generating plot with GNUPlot..."
gnuplot <<- EOF
    set terminal pngcairo enhanced font "Arial,12"
    set output "$OUTPUT_PNG"
    set title "TSP Solution: $(basename "$INPUT_TSP")"
    plot "$TEMP_FILE" using 1:2 with linespoints lc "blue" pt 5 ps 0.25 title "Route"
EOF

rm -f "$TEMP_FILE"
echo "¡All done! plot saved in '$OUTPUT_PNG'"