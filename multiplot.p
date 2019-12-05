set termoption dashed
set tics font "Helvetica,7"
set ylabel "Wartość Sygnału"
set xlabel ""

set grid
set yrange[0:2500]
set xrange[0:10000]
set multiplot layout 4, 1 title "" font ",14"
set tmargin 2
set title "Mikrofon 1" font ",12"
unset key
plot 'syg1.dat' with lines lw 2 lc "black"
#
set title "Mikrofon 2"
unset key
plot 'syg2.dat'  with lines lw 2 lc "black"
#
#
set title "Mikrofon 3"
unset key
plot 'syg3.dat' with lines lw 2 lc "black"
#
#
set title "Mikrofon 4"
unset key
plot 'syg4.dat' with lines  lw 2 lc "black"
#

unset multiplot
