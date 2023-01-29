#include <stdlib.h>
#include <stdio.h>

#include "avl.h"

static int max(int a, int b) {
    return a > b ? a : b;
}

int avlHauteur(NoeudAVL* noeud) {
    if (noeud == NULL) return 0;
    return noeud->hauteur;
}

void avlAfficherRec(NoeudAVL* racine, int profondeur) {
    if (racine == NULL) return;
    avlAfficherRec(racine->gauche, profondeur+1);
    for (int i = 0; i < profondeur; i++) {
        for (int j = 0; j < 8; j++) printf(" ");
    }
    printf("(%2d;%2f) \n", racine->hauteur, valeurFocus(racine->valeur));
    avlAfficherRec(racine->droite, profondeur+1);
}

void avlAfficher(NoeudAVL* racine) {
    printf("----------------\n");
    avlAfficherRec(racine, 0);
    printf("----------------\n");
}

NoeudAVL* rotationDroite(NoeudAVL* noeud) {
	NoeudAVL*racine = noeud->gauche;
	NoeudAVL*inter = racine->droite;

	racine->droite = noeud;
	noeud->gauche = inter;

    noeud->hauteur = max(avlHauteur(noeud->droite), avlHauteur(noeud->gauche)) + 1;
    racine->hauteur = max(avlHauteur(racine->droite), avlHauteur(racine->gauche)) + 1;

	return racine;
}

NoeudAVL* rotationGauche(NoeudAVL* noeud) {
	NoeudAVL* racine = noeud->droite;
	NoeudAVL* inter = racine->gauche;

	racine->gauche = noeud;
	noeud->droite = inter;

    noeud->hauteur = max(avlHauteur(noeud->droite), avlHauteur(noeud->gauche)) + 1;
    racine->hauteur = max(avlHauteur(racine->droite), avlHauteur(racine->gauche)) + 1;

	return racine;
}

int equilibre(NoeudAVL* noeud) {
    if (noeud == NULL) return 0;
    return avlHauteur(noeud->gauche) - avlHauteur(noeud->droite); // plus à gauche -> nbr positif, equilibre -> 0 ou 1
}

NoeudAVL* noeudAvlCreer(Valeur* valeur) {
	NoeudAVL* node = (NoeudAVL*) malloc(sizeof(NoeudAVL));
	node->valeur = valeur;
	node->gauche = NULL;
	node->droite = NULL;
    node->hauteur = 1;
	return node;
}

NoeudAVL* avlNouvelEnfant(NoeudAVL* parent, Valeur* valeur) {
    if (parent == NULL) {
        NoeudAVL* nouveau = noeudAvlCreer(valeur);
        return nouveau;
    }

    if (comparer(valeur, parent->valeur) == 1) {
        parent->droite = avlNouvelEnfant(parent->droite, valeur);
    } else {
        parent->gauche = avlNouvelEnfant(parent->gauche, valeur);
    }
    parent->hauteur = max(avlHauteur(parent->droite), avlHauteur(parent->gauche)) + 1;

    int eq = equilibre(parent);

    if (eq > 1 && comparer(valeur, parent->gauche->valeur) == -1) {
        return rotationDroite(parent);
    }

    if (eq < -1 && comparer(valeur, parent->droite->valeur) == 1) {
        return rotationGauche(parent);
    }

    if (eq > 1 && comparer(valeur, parent->gauche->valeur) == 1) {
        parent->gauche = rotationGauche(parent->gauche);
        return rotationDroite(parent);
    }

    if (eq < -1 && comparer(valeur, parent->droite->valeur) == -1) {
        parent->gauche = rotationDroite(parent->droite);
        return rotationGauche(parent);
    }

    return parent;
}

NoeudAVL* avlCreer(Liste* liste) {
    if (liste->taille > 0) {
        NoeudAVL* nouveau = noeudAvlCreer(&liste->buffer[0]);
        for (int i = 1; i < liste->indice; i++) {
            nouveau = avlNouvelEnfant(nouveau, &liste->buffer[i]);
            avlAfficher(nouveau);
        }
        return nouveau;
    } else {
        return NULL;
    }
}

void avlElements(NoeudAVL* noeud, Liste* out) {
    if (noeud == NULL) return;
    avlElements(noeud->gauche, out);
    listeAjouter(out, noeud->valeur);
    avlElements(noeud->droite, out);
}

void avlDesallouer(NoeudAVL* noeud) {
    if (noeud == NULL) return;
    avlDesallouer(noeud->gauche);
    avlDesallouer(noeud->droite);
    free(noeud);
}
