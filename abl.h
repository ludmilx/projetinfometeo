#ifndef H_ABL
#define H_ABL

#include "valeur.h"
#include "liste.h"

typedef struct _NoeudABL {
    Valeur* valeur;
    struct _NoeudABL* gauche;
    struct _NoeudABL* droite;
} NoeudABL;

NoeudABL* ablCreer(Liste* liste);
void ablAfficher(NoeudABL* root);
void ablElements(NoeudABL* noeud, Liste* out);
void ablDesallouer(NoeudABL* noeud);

#endif
