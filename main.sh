#!/usr/bin/env bash

aide() {
  echo "-p : pression atmosphérique"
  echo "-t : température"
  echo "-m : humidité"
  echo "-h : altitude"
  echo "-w : vent"
  echo "-f : entrée"
  echo "-o : sortie"
  echo "-F : France métropolitaine et Corse"
  echo "-G : Guyane française"
  echo "-S : Saint-Pierre et Miquelon"
  echo "-A : Antilles"
  echo "-O : océan Indien"
  echo "-Q : Antarctique"
  echo "--help : affichage les paramètres"
  echo "-r : inverser"
  echo "-g : longitude"
  echo "-a : latitude"
}

erreur() {
  echo -e "$1" #"\e[31m$1\e[0m"
  exit 1
}

verifier_doublon () {
  if [[ "${!1}" ]]
  then
    erreur "vous ne pouvez pas mettre deux fois le même paramètre"
  fi
}

selection_latitude() {
  if [[ "$latmin" || "$latmax" ]]
  then
    erreur "la latitude existe déjà"
  fi
  latmin="$1"
  latmax="$2"
}

selection_longitude() {
  if [[ "$longmin" || "$longmax" ]]
  then
    erreur "la longitude existe déjà"
  fi
  longmin="$1"
  longmax="$2"
}

verifier_mode() {
  if [[ "$1" -ne "1" && "$2" -ne "2" && "$3" -ne "3" ]]
  then
    erreur "ce mode n'existe pas"
  fi
}

while [[ $# -gt 0 ]]
do
  case "$1" in
    --help)
      aide
      ;;
    -f) #fichier d'entrée
      verifier_doublon "entree"
      entree="$2"
      shift
      ;;
    -o) #ficher de sortie
      verifier_doublon "sortie"
      sortie="$2"
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
    -h) #altitude
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
    -t) #température
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

if [[ ! "$entree" ]]
then
  erreur "l'argument -f est obligatoire"
fi

if [[ ! "$sortie" ]] #vérifier que -o existe
then
  erreur "l'argument -o est obligatoire"
fi

if [[ ! "$t" && ! "$p" && ! "$w" && ! "$m" && ! "$h" ]]
then
  erreur "veuillez sélectionner au moins une colonne"
fi

if [[ ! "$tri" ]]
then
  tri="avl"
fi

if [[ ! "$reverse" ]]
then
  reverse="false"
fi

filtrage_coordonnees() {
  if [[ "$latmin" && "$latmax" && "$longmin" && "$longmax" ]]
  then
    head "$entree" | awk "\$10 >= $latmin && \$10 <= $latmax && \$11 >= $longmin && \$11 <= $longmax" FS='[;,]' > "$sortie"
  elif [[ "$latmin" && "$latmax" ]]
  then
    head "$entree" | awk "\$10 >= $latmin && \$10 <= $latmax" FS='[;,]' > "$sortie"
  elif [[ "$longmin" && "$longmax" ]]
  then
    head "$entree" | awk "\$11 >= $longmin && \$11 <= $longmax" FS='[;,]' > "$sortie"
  else
    head "$entree" > "coord_${1}_$sortie"
  fi
}

filtrage_colonnes() {
  cut -f "$2" -d ";" "coord_${1}_$sortie" > "colonne_${1}_$sortie"
}

if [[ "$t" ]] #température
then
  filtrage_coordonnees "t$t"
  filtrage_colonnes "t$t" "1,2,11,12,13"
  ./app "colonne_t$t_$sortie" "$sortie" "t$t" "$tri" "$reverse"
fi

if [[ "$p" ]] #pression
then
  filtrage_coordonnees "p$p"
  filtrage_colonnes "p$p" "1,2,3,7"
  ./app "colonne_p$p_$sortie" "$sortie" "p$p" "$tri" "$reverse"
fi

if [[ "$w" ]] #vent
then
  filtrage_coordonnees "w"
  filtrage_colonnes "w" "1,2,4,5"
  ./app "colonne_w_$sortie" "$sortie" "w" "$tri" "$reverse"
fi

if [[ "$h" ]] #altitude
then
  filtrage_coordonnees "h"
  filtrage_colonnes "h" "1,2,14"
  ./app "colonne_h_$sortie" "$sortie" "h" "$tri" "$reverse"
fi

if [[ "$m" ]] #humidité
then
  filtrage_coordonnees "m"
  filtrage_colonnes "m" "1,2,6"
  ./app "colonne_m_$sortie" "$sortie" "m" "$tri" "$reverse"
fi
