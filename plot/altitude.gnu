set xlabel "Longitude (Ouest-Est)"
set ylabel "Latitude (Sud-Nord)"
set cblabel "Altitude (m)"
set title "Carte interpolée et colorée de l'altitude"
set pm3d
set palette defined (0 "blue", 1 "green", 2 "red")
splot 'altitude_sorted_by_station.csv' using 1:2:3 with pm3d
