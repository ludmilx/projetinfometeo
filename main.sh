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
    echo "  --avl : tri effectué à l'aide d'une structure de type AVL (par défaut)"
    echo "  -f FILE : fichier d'entrée. Cette option est obligatoire"
    echo "  --help : affiche cette aide détaillée"
    echo "  -d : date des données"
    echo "  -F : France métropolitaine et Corse"
    echo "  -G : Guyane française"
    echo "  -Q : Antarctique"
    echo "  -S : St Pierre et Miquelon"
    echo "  -A : Antilles"
    echo "  -O : océan Indien"
}

erreur() {
  echo -e "$1" #"\e[31m$1\e[0m"
  exit 1
}

verifier_doublon () {
  if [[ "${!1}" ]];
  then
    erreur "vous ne pouvez pas mettre deux fois le même paramètre"
  fi
}

selection_latitude() {
  if [[ "$latmin" || "$latmax" ]];
  then
    erreur "la latitude existe déjà"
  fi
  latmin="$1"
  latmax="$2"
}

selection_longitude() {
  if [[ "$longmin" || "$longmax" ]];
  then
    erreur "la longitude existe déjà"
  fi
  longmin="$1"
  longmax="$2"
}

verifier_mode() {
  if [[ "$1" -ne "1" && "$2" -ne "2" && "$3" -ne "3" ]];
  then
    erreur "ce mode n'existe pas"
  fi
}

while [[ $# -gt 0 ]]
do
  case "$1" in
    --help)
      help_message
      ;;
    -f) #fichier d'entrée
      verifier_doublon "filename"
      filename="$2"
      shift
      ;;
    -r)
      verifier_doublon "inversion"
      inversion="true"
      ;;
    -F) #France et Corse
      selection_longitude '2.347699' '3.171137'
      selection_latitude '42.224831' '50.872278'
      ;;
    -O) #océan indien
      selection_longitude '64.740824' '73.887425'
      selection_latitude '-50.746884' '20.617361'
      ;;
    -A) #Antilles
      selection_longitude '-61.464237' '-60.869146'
      selection_latitude '14.378691' '16.495874'
      ;;
    -S) #St Pierre et Miquelon
      selection_longitude '-56.165470' '-56.221811'
      selection_latitude '46.749731' '46.814618'
      ;;
    -G) #Guyane française
      selection_longitude '-54.416198' '-53.866522'
      selection_latitude '2.210536' '5.979497'
      ;;
    -Q) #Antarctique
      selection_longitude '-57.056397' '-39.103781'
      selection_latitude '-85.463275' '-63.508916'
      ;;
    -w) #vent
      verifier_doublon "w"
      w="a"
      ;;
    -h) #h
      verifier_doublon "h"
      h="a"
      ;;
    -m) #humidité
      verifier_doublon "m"
      m="a"
      ;;
    -g) #longitude
      selection_longitude "$2" "$3"
      shift
      shift
      ;;
    -a) #latitude
      selection_latitude "$2" "$3"
      shift
      shift
      ;;
    -d)
      min_date="$2"
      if ! grep -qE '^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$' <<< "$min_date"; then
    echo "Date de format invalide : ${min_date}"
    exit 1
    fi
      max_date="$3"
      if ! grep -qE '^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$' <<< "$max_date"; then
    echo "Date de format invalide : ${max_date}"
    exit 1
    fi
      shift
      shift
      ;;
    -t) #temperature
      verifier_doublon "t"
      verifier_mode "$2"
      t="$2"
      shift
      ;;
    -p) #pression atmosphérique
      verifier_doublon "p"
      verifier_mode "$2"
      p="$2"
      shift
      ;;
    --avl) #mode de tri
      verifier_doublon "tri"
      tri="avl"
      ;;
    --tab) #mode de tri
      verifier_doublon "tri"
      tri="tab"
        ;;
    --abr) #mode de tri
      verifier_doublon "tri"
      tri="abr"
      ;;
  esac
  shift #passe au prochain argument
done

if [[ ! "$filename" ]];
then
  erreur "l'argument -f est obligatoire"
fi

if [[ ! "$t" && ! "$p" && ! "$w" && ! "$m" && ! "$h" ]];
then
  erreur "veuillez sélectionner au moins une colonne"
fi

if [[ ! "$tri" ]];
then
  tri="avl"
fi

if [[ ! "$reverse" ]];
then
  reverse="false"
fi


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


# Fonction pour trier les données en fonction de la date
tri_par_date() {
  local date=$1
  local data_file=$2

  # Extraire les lignes du fichier qui correspondent à la date choisie
  local sorted_data=$(grep $date $data_file)

  # Afficher les données triées
  echo "$sorted_data"
}

# Fonction pour trier les données en fonction du lieu
tri_par_lieu() {
  local lieu=$1
  local data_file=$2

  case $location in
    -F) location="France";;
    -G) location="Guyane";;
    -S) location="St Pierre et Miquelon";;
    -A) location="Antilles";;
    -O) location="Océan Indien";;
    -Q) location="Antarctique";;
  esac

  # Extraire les lignes du fichier qui correspondent au lieu choisi
  local sorted_data=$(grep $location $data_file)

  # Afficher les données triées
  echo "$sorted_data"
}
# Definition des options
options=":t:p:w:h:m:"

# analyse des options
while getopts $options opt; do
  case $opt in
    t)
      t=$OPTARG
      ;;
    p)
      p=$OPTARG
      ;;
    w)
      w=1
      ;;
    h)
      h=1
      ;;
    m)
      m=1
      ;;
    d) sort_by_date $OPTARG $filename
      ;;
    l) sort_by_location $OPTARG $filename
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
if [[ -z $t && -z $p && -z $w && -z $h && -z $m ]]; then
  echo "Au moins une option (-t, -p, -w, -h, ou -m) doit être choisie." >&2
  exit 1
fi

# filtrage et sortie des options choisies
if [[ -n $t ]]; then
  if [[ $t -eq 1 ]];
  then
# Extraction des donnees de temperature
  temp_data=$(awk -F; '{if ($2 == "temperature") print $0}' $filename)

# Calcul des valeurs minimales, maximales et moyennes par station
echo "station,min_temp,avg_temp,max_temp" > temperatures_mode1.csv
while read line; do
    station=$(echo $line | awk -F; '{print $1}')
    temp=$(echo $line | awk -F; '{print $3}')
    min_temp=$(echo "$temp" | awk 'BEGIN {min = 999999} {if ($1 < min) min = $1} END {print min}')
    max_temp=$(echo "$temp" | awk 'BEGIN {max = -999999} {if ($1 > max) max = $1} END {print max}')
    avg_temp=$(echo "$temp" | awk '{sum += $1} END {print sum/NR}')
    echo "$station,$min_temp,$avg_temp,$max_temp" >> temperatures_mode1.csv
done <<< "$temp_data"

# Trie des données par ordre croissant du numéro de station
./app -f temperatures_mode1.csv -o temperatures_mode1.csv

elif [[ $t -eq 2 ]]; then

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
  done < $filename
  average=$(echo "scale=2; $sum / $count" | bc)
  echo "$date_time, $average"
}

# Supprimer le fichier de sortie s'il existe déjà
if [ -f "average_temperatures.csv" ]; then
  rm average_temperatures.csv
fi

# Obtenir la liste des dates/heures uniques dans le fichier de données
date_times=$(cut -d',' -f1 $filename | sort -u)

# Pour chaque date/heure, obtenir la moyenne de la température
for date_time in $date_times; do
  average_temp $date_time >> temperatures_mode2.csv
done

# Trier le fichier de sortie en fonction de la date/heure
./app -f temperatures_mode2.csv -o temperatures_mode2.csv

elif [[ $t -eq 3 ]]; then

  # Extraction des données de température
  awk -F";" '{print $1 "," $2 "," $3}' $filename > temp_data.csv

  # Trier les données par date/heure puis par identifiant de station
  sort -t "," -k 2,2 -k 1,1 temp_data.csv > temperatures_mode3.csv

  # Afficher les données triées
  cat temperatures_mode3.csv

  else
    echo "Mode invalide pour -t: $t" >&2
    exit 1
  fi
fi

if [[ -n $p ]]; then
  if [[ $p -eq 1 ]];
    then
  # Extraction des donnees de pression
    pressure_data=$(awk -F; '{if ($2 == "pressure") print $0}' $filename)

  # Calcul des valeurs minimales, maximales et moyennes par station
  echo "station,min_pressure,avg_pressure,max_pressure" > p1.csv
  while read line; do
      station=$(echo $line | awk -F, '{print $1}')
      pressure=$(echo $line | awk -F, '{print $7}')
      min_pressure=$(echo "$pressure" | awk 'BEGIN {min = 999999} {if ($1 < min) min = $1} END {print min}')
      max_pressure=$(echo "$pressure" | awk 'BEGIN {max = -999999} {if ($1 > max) max = $1} END {print max}')
      avg_pressure=$(echo "$pressure" | awk '{sum += $1} END {print sum/NR}')
      echo "$station,$min_pressure,$avg_pressure,$max_pressure" >> p1.csv
  done <<< "$pressure_data"

  # Trie des données par ordre croissant du numéro de station
  ./app -f p1.csv -o p1.csv

elif [[ $p -eq 2 ]]; then
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
      done < $filename
      average=$(echo "scale=2; $sum / $count" | bc)
      echo "$date_time, $average"
    }

    # Supprimer le fichier de sortie s'il existe déjà
    if [ -f "average_pressure.csv" ]; then
      rm average_pressure.csv
    fi

    # Obtenir la liste des dates/heures uniques dans le fichier de données
    date_times=$(cut -d';' -f1 $filename | sort -u)

    # Pour chaque date/heure, obtenir la moyenne de la température
    for date_time in $date_times; do
      average_pressure $date_time >> p2.csv
    done

    # Trier le fichier de sortie en fonction de la date/heure
    sort -t';' -k1 p2.csv -o p2.csv

  elif [[ $p -eq 3 ]]; then

    # Extraction des données de pression
    awk -F";" '{print $1 "," $2 "," $7}' $filename > pressure_data.csv

    # Trier les données par date/heure puis par identifiant de station
    sort -t ";" -k 2,2 -k 1,1 pressure_data.csv > p3.csv

    # Afficher les données triées
    cat p3.csv

    else
      echo "Mode invalide pour -p: $p" >&2
      exit 1
    fi
  fi

if [[ -n $w ]]; then
  # Extraire les colonnes contenant les informations de vent
  cut -d ";" -f 3,4 $filename > w_data.csv

  # Calculer les moyennes de vitesse et d'orientation pour chaque station
  awk -F ";" '{
    w_speed_sum[$1] += sqrt($2^2 + $3^2);
    w_dir_sum[$1] += atan2($3, $2);
    w_count[$1]++;
  } END {
    for (station in w_speed_sum) {
      avg_w_speed = w_speed_sum[station] / w_count[station];
      avg_w_dir = w_dir_sum[station] / w_count[station];
      printf("%s,%.2f,%.2f\n", station, avg_w_speed, avg_w_dir);
    }
  }' w_data.csv | sort -n -t ";" -k 1 > avg_w.csv

  # Supprimer le fichier temporaire
  rm w_data.csv
fi
if [[ -n $h ]]; then
  awk -F";" '{print $1","$14}' $filename | sort -t"," -k2,2nr > h_sorted_by_station.csv
fi

if [[ -n $m ]]; then
  awk -F";" '{print $1","$6}' $filename | sort -t"," -k2,2nr > humidite_sorted_by_station.csv
fi
