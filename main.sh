#!/usr/bin/env bash

# initialisation des variables
tri_method="avl"
filename=""

# fonction d'affichage de l'aide
help_message() {
    echo "Usage: $0 [OPTION] -f FILE"
    echo "Les commandes suivantes permettent de trier les données dans le fichier meteo_filtered_data_v1 avec les options suivantes :"
    echo "  --tab : tri effectué à l'aide d'une structure linéaire (tableau ou liste chainée)"
    echo "  --abr : tri effectué à l'aide d'une structure de type ABR"
    echo "  --avl : tri effectué à l'aide d'une structure de type AVL (défaut)"
    echo "  -f FILE : fichier d'entrée. Cette option est obligatoire"
    echo "  --help : affiche cette aide détaillée"
}

# boucle d'analyse des options
while [ $# -gt 0 ]; do
    case "$1" in
        --tab)
            tri_method="tab"
            ;;
        --abr)
            tri_method="abr"
            ;;
        --avl)
            tri_method="avl"
            ;;
        -f)
            filename="$2"
            shift
            ;;
        --help)
            help_message
            exit 0
            ;;
        *)
            echo "Erreur : option ou argument non valide : $1"
            help_message
            exit 1
            ;;
    esac
    shift
done

# vérification de l'existence du fichier
if [ -z "$filename" ]; then
    echo "Erreur : le nom du fichier d'entrée n'a pas été fourni avec l'option -f"
    help_message
    exit 1
fi

if [ ! -f "$filename" ]; then
    echo "Erreur : le fichier $filename n'existe pas"
    help_message
    exit 1
fi

# Definition des options
options=":t:p:w:h:m:"

# analyse des options
while getopts $options opt; do
  case $opt in
    t)
      temp_mode=$OPTARG
      ;;
    p)
      pressure_mode=$OPTARG
      ;;
    w)
      wind=1
      ;;
    h)
      altitude=1
      ;;
    m)
      humidity=1
      ;;
    \?)
      echo "Option invalide : -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG nécessite un argument." >&2
      exit 1
      ;;
  esac
done

# Vérifie si au moins une option a été choisie
if [[ -z $temp_mode ]] && [[ -z $pressure_mode ]] && [[ -z $wind ]] && [[ -z $altitude ]] && [[ -z $humidity ]]; then
  echo "Au moins une option (-t, -p, -w, -h, or -m) doit être choisie." >&2
  exit 1
fi

# filtrage et sortie des options choisies
if [[ -n $temp_mode ]]; then
  if [[ $temp_mode -eq 1 ]];
  then
# Extraction des donnees de temperature
  temp_data=$(awk -F, '{if ($2 == "temperature") print $0}' meteo_filtered_data_v1.csv)

# Calcul des valeurs minimales, maximales et moyennes par station
echo "station,min_temp,avg_temp,max_temp" > temperatures_mode1.csv
while read line; do
    station=$(echo $line | awk -F, '{print $1}')
    temp=$(echo $line | awk -F, '{print $3}')
    min_temp=$(echo "$temp" | awk 'BEGIN {min = 999999} {if ($1 < min) min = $1} END {print min}')
    max_temp=$(echo "$temp" | awk 'BEGIN {max = -999999} {if ($1 > max) max = $1} END {print max}')
    avg_temp=$(echo "$temp" | awk '{sum += $1} END {print sum/NR}')
    echo "$station,$min_temp,$avg_temp,$max_temp" >> temperatures_mode1.csv
done <<< "$temp_data"

# Trie des donnees par ordre croissant du numero de station
sort -t, -k1 -n temperatures_mode1.csv -o temperatures_mode1.csv

elif [[ $temp_mode -eq 2 ]]; then

# Fonction pour obtenir la moyenne de la température pour une date/heure donnée
function average_temp {
  date_time=$1
  sum=0
  count=0
  while read line; do
    line_date_time=$(echo $line | cut -d',' -f1)
    if [ "$line_date_time" == "$date_time" ]; then
      temp=$(echo $line | cut -d',' -f2)
      sum=$(echo "$sum + $temp" | bc)
      count=$((count + 1))
    fi
  done < meteo_filtered_data_v1.csv
  average=$(echo "scale=2; $sum / $count" | bc)
  echo "$date_time, $average"
}

# Supprimer le fichier de sortie s'il existe déjà
if [ -f "average_temperatures.csv" ]; then
  rm average_temperatures.csv
fi

# Obtenir la liste des dates/heures uniques dans le fichier de données
date_times=$(cut -d',' -f1 meteo_filtered_data_v1.csv | sort -u)

# Pour chaque date/heure, obtenir la moyenne de la température
for date_time in $date_times; do
  average_temp $date_time >> temperatures_mode2.csv
done

# Trier le fichier de sortie en fonction de la date/heure
sort -t',' -k1 temperatures_mode2.csv -o temperatures_mode2.csv

elif [[ $temp_mode -eq 3 ]]; then

  # Extraction des données de température
  awk -F"," '{print $1 "," $2 "," $3}' meteo_filtered_data_v1.csv > temp_data.csv

  # Trier les données par date/heure puis par identifiant de station
  sort -t "," -k 2,2 -k 1,1 temp_data.csv > temperatures_mode3.csv

  # Afficher les données triées
  cat temperatures_mode3.csv

  else
    echo "Mode invalide pour -t: $temp_mode" >&2
    exit 1
  fi
fi

if [[ -n $pressure_mode ]]; then
  if [[ $pressure_mode=-p1 ]];
    then
  # Extraction des donnees de pression
    pressure_data=$(awk -F, '{if ($2 == "pressure") print $0}' meteo_filtered_data_v1.csv)

  # Calcul des valeurs minimales, maximales et moyennes par station
  echo "station,min_pressure,avg_pressure,max_pressure" > pressure_mode1.csv
  while read line; do
      station=$(echo $line | awk -F, '{print $1}')
      pressure=$(echo $line | awk -F, '{print $7}')
      min_pressure=$(echo "$pressure" | awk 'BEGIN {min = 999999} {if ($1 < min) min = $1} END {print min}')
      max_pressure=$(echo "$pressure" | awk 'BEGIN {max = -999999} {if ($1 > max) max = $1} END {print max}')
      avg_pressure=$(echo "$pressure" | awk '{sum += $1} END {print sum/NR}')
      echo "$station,$min_pressure,$avg_pressure,$max_pressure" >> temperatures_mode1.csv
  done <<< "$pressure_data"

  # Trie des donnees par ordre croissant du numero de station
  sort -t, -k1 -n pressure_mode1.csv -o pressure_mode1.csv

elif [[ $pressure_mode=-p2 ]]; then
    # Fonction pour obtenir la moyenne de la pression pour une date/heure donnée
    function average_pressure {
      date_time=$1
      sum=0
      count=0
      while read line; do
        line_date_time=$(echo $line | cut -d',' -f1)
        if [ "$line_date_time" == "$date_time" ]; then
          pressure=$(echo $line | cut -d',' -f2)
          sum=$(echo "$sum + $pressure" | bc)
          count=$((count + 1))
        fi
      done < meteo_filtered_data_v1.csv
      average=$(echo "scale=2; $sum / $count" | bc)
      echo "$date_time, $average"
    }

    # Supprimer le fichier de sortie s'il existe déjà
    if [ -f "average_pressure.csv" ]; then
      rm average_pressure.csv
    fi

    # Obtenir la liste des dates/heures uniques dans le fichier de données
    date_times=$(cut -d';' -f1 meteo_filtered_data_v1.csv | sort -u)

    # Pour chaque date/heure, obtenir la moyenne de la température
    for date_time in $date_times; do
      average_pressure $date_time >> pressure_mode2.csv
    done

    # Trier le fichier de sortie en fonction de la date/heure
    sort -t';' -k1 pressure_mode2.csv -o pressure_mode2.csv

  elif [[ $pressure_mode=-p3 ]]; then

    # Extraction des données de pression
    awk -F";" '{print $1 "," $2 "," $7}' meteo_filtered_data_v1.csv > pressure_data.csv

    # Trier les données par date/heure puis par identifiant de station
    sort -t ";" -k 2,2 -k 1,1 pressure_data.csv > pressure_mode3.csv

    # Afficher les données triées
    cat pressure_mode3.csv

    else
      echo "Mode invalide pour -p: $pressure_mode" >&2
      exit 1
    fi
  fi

if [[ -n $wind ]]; then
  # Extraire les colonnes contenant les informations de vent
  cut -d ";" -f 3,4 meteo_filtered_data_v1.csv > wind_data.csv

  # Calculer les moyennes de vitesse et d'orientation pour chaque station
  awk -F ";" '{
    wind_speed_sum[$1] += sqrt($2^2 + $3^2);
    wind_dir_sum[$1] += atan2($3, $2);
    wind_count[$1]++;
  } END {
    for (station in wind_speed_sum) {
      avg_wind_speed = wind_speed_sum[station] / wind_count[station];
      avg_wind_dir = wind_dir_sum[station] / wind_count[station];
      printf("%s,%.2f,%.2f\n", station, avg_wind_speed, avg_wind_dir);
    }
  }' wind_data.csv | sort -n -t ";" -k 1 > avg_wind.csv

  # Supprimer le fichier temporaire
  rm wind_data.csv

if [[ -n $altitude ]]; then
  awk -F";" '{print $1","$14}' meteo_filtered_data_v1.csv | sort -t"," -k2,2nr > altitude_sorted_by_station.csv
fi

if [[ -n $humidity ]]; then
  awk -F";" '{print $1","$6}' meteo_filtered_data_v1.csv | sort -t"," -k2,2nr > humidite_sorted_by_station.csv
fi

# Diagramme température et pression mode 1
set datafile separator ";"
set ylabel "Température (en °C)"
set xlabel "Identifiant de la station"
set title "Diagramme de barres d'erreur - Températures par station (mode 1)"
plot "temperatures_mode1.csv" using 1:2:3:4 with errorbars title "Température"

set datafile separator ";"
set ylabel "Pression"
set xlabel "Identifiant de la station"
set title "Diagramme de barres d'erreur - Pressions par station (mode 1)"
plot "pressure_mode1.csv" using 1:2:3:4 with errorbars title "Pression"

# diagramme température et pression mode 2
set xdata time
set timefmt "%Y-%m-%d %H:%M:%S"
set format x "%d/%m\n%H:%M"

plot 'temperatures_mode2.csv' using 1:3 with lines title "Température en mode 2"

set xdata time
set timefmt "%Y-%m-%d %H:%M:%S"
set format x "%d/%m\n%H:%M"

plot 'pressure_mode2.csv' using 1:3 with lines title "Pressions en mode 2"

# diagramme altitude
set xlabel "Longitude (Ouest-Est)"
set ylabel "Latitude (Sud-Nord)"
set cblabel "Altitude (m)"
set title "Carte interpolée et colorée de l'altitude"
set pm3d
set palette defined (0 "blue", 1 "green", 2 "red")
splot 'altitude_sorted_by_station.csv' using 1:2:3 with pm3d

# digramme vent

# diagramme humidité
set pm3d map
set xrange [<min_longitude>:<max_longitude>]
set yrange [<min_latitude>:<max_latitude>]
splot 'humidite_sorted_by_station.csv' using 1:2:3 with pm3d
