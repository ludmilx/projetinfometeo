#ifndef H_AVL
#define H_AVL

#include "valeur.h"
#include "liste.h"

typedef struct _NoeudAVL {
    Valeur* valeur;
    int hauteur;
    struct _NoeudAVL* gauche;
    struct _NoeudAVL* droite;
} NoeudAVL;

NoeudAVL* avlCreer(Liste* liste);
void avlAfficher(NoeudAVL* root);
void avlElements(NoeudAVL* noeud, Liste* out);
void avlDesallouer(NoeudAVL* noeud);

#endif
