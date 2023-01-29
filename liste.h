#ifndef H_LISTE
#define H_LISTE

#include "valeur.h"

typedef struct {
    Valeur* buffer;
    int taille;
    int indice;
} Liste;

Liste* listeCreer(int taille);
void listeAjouter(Liste *liste, Valeur *valeur);
void listeDesallouer(Liste *liste);
void listeAfficher(Liste *liste);
void listeTrier(Liste *liste);

#endif
