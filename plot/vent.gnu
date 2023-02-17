set xlabel "Longitude (Ouest-Est)"
set ylabel "Latitude (Sud-Nord)"
plot data using 1:2:(0):3 with vectors head filled lc rgb "red"
