#ifndef H_ABR
#define H_ABR

#include "valeur.h"
#include "liste.h"

typedef struct _NoeudABR {
    Valeur* valeur;
    struct _NoeudABR* gauche;
    struct _NoeudABR* droite;
} NoeudABR;

NoeudABR* abrCreer(Liste* liste);
void abrAfficher(NoeudABR* root);
void abrElements(NoeudABR* noeud, Liste* out);
void abrDesallouer(NoeudABR* noeud);

#endif
