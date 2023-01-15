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
      inversion="a"
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

filtrage_coordonnees(){
  if [[ "$latmin" && "$latmax" && "$longmin" && "$longmax" ]]
  then
    head "$entree" | awk "\$10 >= $latmin && \$10 <= $latmax && \$11 >= $longmin && \$11 <= $longmax" FS='[;,]' > "$sortie"
  fi

}


filtrage_coordonnees
