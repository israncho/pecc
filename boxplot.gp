# Basic conf 
set title "Execution time"
set xlabel "Threads"
set ylabel "Seconds"
set style data boxplot
set xtics ("1" 1, "2" 2, "3" 3, "4" 4, "5" 5, "6" 6, "7" 7, "8" 8)  # x axis labels 

# Draw columns (usando columnas 1, 2, 3, 4, 5, 6, 7, 8)
plot "executions.dat" using (1):1 notitle, "" using (2):2 notitle, "" using (3):3 notitle, "" using (4):4 notitle, "" using (5):5 notitle, "" using (6):6 notitle, "" using (7):7 notitle, "" using (8):8 notitle

# Save as PNG
set terminal pngcairo enhanced
set output "boxplot_saturno.png"
replot