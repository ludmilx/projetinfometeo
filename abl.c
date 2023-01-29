#include <stdlib.h>
#include <stdio.h>

#include "abl.h"

static int max(int a, int b) {
    return a > b ? a : b;
}

void ablAfficherRec(NoeudABL* racine, int profondeur) {
    if (racine == NULL) return;
    ablAfficherRec(racine->gauche, profondeur+1);
    for (int i = 0; i < profondeur; i++) {
        for (int j = 0; j < 8; j++) printf(" ");
    }
    printf("(%2f) \n", valeurFocus(racine->valeur));
    ablAfficherRec(racine->droite, profondeur+1);
}

void ablAfficher(NoeudABL* racine) {
    printf("----------------\n");
    ablAfficherRec(racine, 0);
    printf("----------------\n");
}

NoeudABL* noeudAblCreer(Valeur* valeur) {
	NoeudABL* node = (NoeudABL*) malloc(sizeof(NoeudABL));
	node->valeur = valeur;
	node->gauche = NULL;
	node->droite = NULL;
	return node;
}

NoeudABL* ablNouvelEnfant(NoeudABL* parent, Valeur* valeur) {
    if (parent == NULL) {
        NoeudABL* nouveau = noeudAblCreer(valeur);
        return nouveau;
    }

    if (comparer(valeur, parent->valeur) == 1) {
        parent->droite = ablNouvelEnfant(parent->droite, valeur);
    } else {
        parent->gauche = ablNouvelEnfant(parent->gauche, valeur);
    }
    return parent;
}

NoeudABL* ablCreer(Liste* liste) {
    if (liste->taille > 0) {
        NoeudABL* nouveau = noeudAblCreer(&liste->buffer[0]);
        for (int i = 1; i < liste->indice; i++) {
            nouveau = ablNouvelEnfant(nouveau, &liste->buffer[i]);
            ablAfficher(nouveau);
        }
        return nouveau;
    } else {
        return NULL;
    }
}

void ablElements(NoeudABL* noeud, Liste* out) {
    if (noeud == NULL) return;
    ablElements(noeud->gauche, out);
    listeAjouter(out, noeud->valeur);
    ablElements(noeud->droite, out);
}

void ablDesallouer(NoeudABL* noeud) {
    if (noeud == NULL) return;
    ablDesallouer(noeud->gauche);
    ablDesallouer(noeud->droite);
    free(noeud);
}
