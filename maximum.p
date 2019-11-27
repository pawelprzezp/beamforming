
set ylabel "Suma sygnałów" font "Helvetica,20"
set xlabel "Kąt" font "Helvetica,15"
set tics font "Helvetica,12"
set grid
set size 1,0.96
set title "Maksimum korelacji" font "Helvetica,20"

plot 'max.dat' with lines lw 3 lc "black"
