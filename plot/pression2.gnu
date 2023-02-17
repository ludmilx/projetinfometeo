set xdata time
set timefmt "%Y-%m-%d %H:%M:%S"
set format x "%d/%m\n%H:%M"

plot 'pressure_mode2.csv' using 1:3 with lines title "Pressions en mode 2"
