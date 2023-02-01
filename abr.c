#include <stdlib.h>
#include <stdio.h>

#include "abr.h"

static int max(int a, int b) {
    return a > b ? a : b;
}

void abrAfficherRec(NoeudABR* racine, int profondeur) {
    if (racine == NULL) return;
    abrAfficherRec(racine->gauche, profondeur+1);
    for (int i = 0; i < profondeur; i++) {
        for (int j = 0; j < 8; j++) printf(" ");
    }
    printf("(%2f) \n", valeur(racine->valeur));
    abrAfficherRec(racine->droite, profondeur+1);
}

void abrAfficher(NoeudABR* racine) {
    printf("----------------\n");
    abrAfficherRec(racine, 0);
    printf("----------------\n");
}

NoeudABR* noeudAbrCreer(Valeur* valeur) {
	NoeudABR* node = (NoeudABR*) malloc(sizeof(NoeudABR));
	node->valeur = valeur;
	node->gauche = NULL;
	node->droite = NULL;
	return node;
}

NoeudABR* abrNouvelEnfant(NoeudABR* parent, Valeur* valeur) {
    if (parent == NULL) {
        NoeudABR* nouveau = noeudAbrCreer(valeur);
        return nouveau;
    }

    if (comparer(valeur, parent->valeur) == 1) {
        parent->droite = abrNouvelEnfant(parent->droite, valeur);
    } else {
        parent->gauche = abrNouvelEnfant(parent->gauche, valeur);
    }
    return parent;
}

NoeudABR* abrCreer(Liste* liste) {
    if (liste->taille > 0) {
        NoeudABR* nouveau = noeudAbrCreer(&liste->buffer[0]);
        for (int i = 1; i < liste->indice; i++) {
            nouveau = abrNouvelEnfant(nouveau, &liste->buffer[i]);
            abrAfficher(nouveau);
        }
        return nouveau;
    } else {
        return NULL;
    }
}

void abrElements(NoeudABR* noeud, Liste* out) {
    if (noeud == NULL) return;
    abrElements(noeud->gauche, out);
    listeAjouter(out, noeud->valeur);
    abrElements(noeud->droite, out);
}

void abrDesallouer(NoeudABR* noeud) {
    if (noeud == NULL) return;
    abrDesallouer(noeud->gauche);
    abrDesallouer(noeud->droite);
    free(noeud);
}
