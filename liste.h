#ifndef H_LISTE
#define H_LISTE

#include "valeur.h"

typedef struct {
    Valeur** buffer;
    int taille;
    int indice;
} Liste;

Liste* creerListe(int taille);
void listeAjouter(Liste *liste, Valeur valeur);
void desallouerListe(Liste *liste);

#endif
