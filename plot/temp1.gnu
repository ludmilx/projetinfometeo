set datafile separator ";"
set ylabel "Température (en °C)"
set xlabel "Identifiant de la station"
set title "Diagramme de barres d'erreur - Températures par station (mode 1)"
plot "temperatures_mode1.csv" using 1:2:3:4 with errorbars title "Température"
