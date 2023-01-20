#include <stdlib.h>
#include <stdio.h>

#include "avl.h"

NoeudAVL* creerAVL(Liste* listeDepart) {
    if (listeDepart->taille > 0) {
        NoeudAVL* nouveau = nouveauNoeud(listeDepart->buffer[0]);
        for (int i = 1; i < listeDepart->taille; i++) {
            // printf("+ %d\n", listeDepart->buffer[i]);
            if (listeDepart->buffer[i] > listeDepart->buffer[0]) {
                nouveau->droite = nouvelEnfant(nouveau->droite, listeDepart->buffer[i]);
            } else {
                nouveau->gauche = nouvelEnfant(nouveau->gauche, listeDepart->buffer[i]);
            }
            print2D(nouveau);
        }
        return nouveau;
    } else {
        return NULL;
    }
}

NoeudAVL* nouveauNoeud(Valeur* valeur) {
	NoeudAVL* node = (NoeudAVL*) malloc(sizeof(NoeudAVL));
	node->valeur = valeur;
	node->gauche = NULL;
	node->droite = NULL;
	return node;
}

// NoeudAVL* enfantInferieur(NoeudAVL* noeud);
// NoeudAVL* enfantSuperieur(NoeudAVL* noeud);
// NoeudAVL* parent(NoeudAVL* noeud);

NoeudAVL* nouvelEnfant(NoeudAVL* parent, Valeur* valeur) {
    if (parent == NULL) {
        NoeudAVL* nouveau = nouveauNoeud(valeur);
        return nouveau;
    }

    if (comparer(valeur, parent->valeur) == 1) {
        parent->droite = nouvelEnfant(parent->droite, valeur);
    } else {
        parent->gauche = nouvelEnfant(parent->gauche, valeur);
    }

    int eq = equilibre(parent);

    //printf("1\n");
    if (eq > 1 && comparer(valeur, parent->gauche->valeur) == -1) {
        return rotationDroite(parent);
    }

    //printf("2\n");
    if (eq < -1 && comparer(valeur, parent->droite->valeur) == 1) {
        return rotationGauche(parent);
    }

    //printf("3\n");
    if (eq > 1 && comparer(valeur, parent->gauche->valeur) == 1) {
        parent->gauche = rotationGauche(parent->gauche);
        return rotationDroite(parent);
    }

    //printf("4\n");
    if (eq < -1 && comparer(valeur, parent->droite->valeur) == -1) {
        parent->gauche = rotationDroite(parent->droite);
        return rotationGauche(parent);
    }

    return parent;
}

//        noeud
//      /
//  gauche
//      \
//      racine

//      racine
//      / \
//  gauche  noeud

NoeudAVL* rotationDroite(NoeudAVL* noeud) {
	NoeudAVL*x = noeud->gauche;
	NoeudAVL*T2 = x->droite;

	// Perform rotation
	x->droite = noeud;
	noeud->gauche = T2;

	// Return new root
	return x;
}

//     noeud
//        \
//        droite = racine
//            \
//            droite2
// ->
//        racine
//         /   \
//      noeud   droite2

//     noeud
//        \
//        droite
//        /    \
//     racine   droite2
// ->
//      racine
//      / \
//  noeud droite
//          \
//          droite2

NoeudAVL* rotationGauche(NoeudAVL* noeud) {
	NoeudAVL* y = noeud->droite;
	NoeudAVL*T2 = y->gauche;

	// Perform rotation
	y->gauche = noeud;
	noeud->droite = T2;

	// Return new root
	return y;
}

int equilibre(NoeudAVL* noeud) {
    if (noeud == NULL) return 0;
    return hauteur(noeud->gauche) - hauteur(noeud->droite); // plus à gauche -> nbr positif, equilibre -> 0 ou 1
}

int hauteur(NoeudAVL* noeud) {
    if (noeud == NULL) return 0;

    int hg = hauteur(noeud->gauche);
    int hd = hauteur(noeud->droite);
    return hg > hd ? hg + 1 : hd + 1; // opérateur ternaire
}

// void elements(NoeudAVL* noeud, Liste* out);

void desallouerAVL(NoeudAVL* noeud) {
    if (noeud == NULL) return;
    desallouerAVL(noeud->gauche);
    desallouerAVL(noeud->droite);
    free(noeud);
}

void print2DUtil(NoeudAVL* root, int space)
{
    if (root == NULL)
        return;

    space += 5;

    print2DUtil(root->droite, space);

    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%d\n", root->valeur->colonnes[root->valeur->focusComparaison]);

    print2DUtil(root->gauche, space);
}

void print2D(NoeudAVL* root)
{
    printf("----------------\n");
    print2DUtil(root, 0);
}
