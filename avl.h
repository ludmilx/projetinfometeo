#ifndef H_AVL
#define H_AVL

#include "valeur.h"
#include "liste.h"

typedef struct _NoeudAVL {
    Valeur* valeur;
    struct _NoeudAVL* gauche;
    struct _NoeudAVL* droite;
} NoeudAVL;

NoeudAVL* creerAVL(Liste* listeDepart);
NoeudAVL* nouveauNoeud(Valeur* valeur);
NoeudAVL* rotationDroite(NoeudAVL* noeud);
NoeudAVL* rotationGauche(NoeudAVL* noeud);
void print2D(NoeudAVL* root);
NoeudAVL* nouvelEnfant(NoeudAVL* parent, Valeur* valeur);
int equilibre(NoeudAVL* noeud);
int hauteur(NoeudAVL* noeud);
void elements(NoeudAVL* noeud, Liste* out);
void desallouerAVL(NoeudAVL* noeud);

#endif
