set pm3d map
set xrange [<min_longitude>:<max_longitude>]
set yrange [<min_latitude>:<max_latitude>]
splot 'humidite_sorted_by_station.csv' using 1:2:3 with pm3d$
