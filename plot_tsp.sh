#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Use: $0 <archivo.tsp>"
    exit 1
fi

INPUT_TSP="$1"
OUTPUT_PNG="${INPUT_TSP%.tsp}.png"
TEMP_FILE="temp_formatted.tsp"

echo "Giving format to '$INPUT_TSP' with Python..."
python3 format_for_plotting.py "$INPUT_TSP" "$TEMP_FILE" || {
    echo "Error while processing .tsp with Python."
    exit 1
}

echo "Generating plot con GNUPlot..."
gnuplot <<- EOF
    set terminal pngcairo enhanced font "Arial,12"
    set output "$OUTPUT_PNG"
    set title "TSP Solution: $(basename "$INPUT_TSP")"
    plot "$TEMP_FILE" using 1:2 with linespoints lc "blue" pt 7 ps 0.5 title "Route"
EOF

rm -f "$TEMP_FILE"
echo "¡All done! plot saved in '$OUTPUT_PNG'"