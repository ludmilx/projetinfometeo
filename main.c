#include <stdlib.h>
#include <stdio.h>
#include "valeur.h"
#include "liste.h"
#include "avl.h"
#include "abl.h"

// typedef struct _NoeudABR {
//     int valeur;
//     struct _NoeudABR* gauche;
//     struct _NoeudABR* droite;
// } NoeudABR;

void testTriRapide() {
    Liste* l = listeCreer(2);
    Valeur v = {
        .colonnes = { 0, 1, 0, 8 },
        .focusComparaison = 0
    };
    listeAjouter(l, &v);
    v.colonnes[0] = 1;
    listeAjouter(l, &v);
    v.colonnes[0] = 2;
    listeAjouter(l, &v);
    v.colonnes[0] = -19;
    listeAjouter(l, &v);
    v.colonnes[0] = 3;
    listeAjouter(l, &v);
    v.colonnes[0] = 0;
    listeAjouter(l, &v);
    v.colonnes[0] = 7;
    listeAjouter(l, &v);
    v.colonnes[0] = 4;
    listeAjouter(l, &v);
    v.colonnes[0] = 9;
    listeAjouter(l, &v);
    v.colonnes[0] = 11;
    listeAjouter(l, &v);

    listeAfficher(l);
    listeTrier(l);
    listeAfficher(l);
}

void testAvl() {
    Liste* l = listeCreer(2);
    Valeur v = {
        .colonnes = { 0, 1, 0, 8 },
        .focusComparaison = 0
    };
    listeAjouter(l, &v);
    v.colonnes[0] = 1;
    listeAjouter(l, &v);
    v.colonnes[0] = 2;
    listeAjouter(l, &v);
    v.colonnes[0] = -19;
    listeAjouter(l, &v);
    v.colonnes[0] = 3;
    listeAjouter(l, &v);
    v.colonnes[0] = 0;
    listeAjouter(l, &v);
    v.colonnes[0] = 7;
    listeAjouter(l, &v);
    v.colonnes[0] = 4;
    listeAjouter(l, &v);
    v.colonnes[0] = 9;
    listeAjouter(l, &v);
    v.colonnes[0] = 11;
    listeAjouter(l, &v);
    NoeudAVL* avl = avlCreer(l);
    Liste* lTriee = listeCreer(2);
    listeAfficher(l);
    avlElements(avl, lTriee);
    listeAfficher(lTriee);
    avlDesallouer(avl);
    listeDesallouer(l);
    listeDesallouer(lTriee);
}

void testAbl() {
    Liste* l = listeCreer(2);
    Valeur v = {
        .colonnes = { 0, 1, 0, 8 },
        .focusComparaison = 0
    };
    listeAjouter(l, &v);
    v.colonnes[0] = 1;
    listeAjouter(l, &v);
    v.colonnes[0] = 2;
    listeAjouter(l, &v);
    v.colonnes[0] = -19;
    listeAjouter(l, &v);
    v.colonnes[0] = 3;
    listeAjouter(l, &v);
    v.colonnes[0] = 0;
    listeAjouter(l, &v);
    v.colonnes[0] = 7;
    listeAjouter(l, &v);
    v.colonnes[0] = 4;
    listeAjouter(l, &v);
    v.colonnes[0] = 9;
    listeAjouter(l, &v);
    v.colonnes[0] = 11;
    listeAjouter(l, &v);
    NoeudABL* abl = ablCreer(l);
    Liste* lTriee = listeCreer(2);
    listeAfficher(l);
    ablElements(abl, lTriee);
    listeAfficher(lTriee);
    ablDesallouer(abl);
    listeDesallouer(l);
    listeDesallouer(lTriee);
}

int main(int argc, char** argv) {
    // truc.csv
    // 0 1 -> id, date
    // 2 3 4 5 6 7 -> donnees
    // 8 -> coordonnees
    // 9 10 11 -> donnees
    // 12 13 -> alt/communes

    if (argc <= 4) {
        return 1;
    }

    char c;
    FILE *fichierEntree = fopen(argv[1], "r");

    if (fichierEntree == NULL) {
        return 1;
    }

    int nblignes = 0;
	int nbcharligne = 0;
    int nbcolonne = 0;
    char buffer[100] = {0};
    int curseurBuffer = 0;
	char headerTableau[360] = {0};
    double n;

    Liste* liste = listeCreer(2);
	Valeur valeur = {0};
	valeur.focusComparaison = atoi(argv[3]);

    while ((c = fgetc(fichierEntree)) != EOF) {
		if (nblignes == 0) {
			headerTableau[nbcharligne++] = c;
			headerTableau[nbcharligne] = '\0';
		}

		if (nblignes >= 1) {
			valeur.ligne[nbcharligne++] = c;
			valeur.ligne[nbcharligne] = '\0';

			if (c == ';' || c == '\n') {
				n = atof(buffer);
				valeur.colonnes[nbcolonne] = n;
				for (int i = 0; i < 100; i++) buffer[i] = 0;
				curseurBuffer = 0;

				if (nbcolonne == 14) {
					listeAjouter(liste, &valeur);
				}
			} else {
				buffer[curseurBuffer] = c;
				curseurBuffer++;
			}
        }

        if (c == '\n') {
            nbcolonne = 0;
            nblignes++;
			nbcharligne = 0;
        }

        if (c == ';') {
            nbcolonne++;
        }
    }

	listeAfficher(liste);
	Liste *listeTriee;

	int typeTri = atoi(argv[4]);
	if (typeTri == 0) { // tri rapide sur liste directement
		listeTrier(liste);
		listeTriee = liste;
	} else if (typeTri == 1) { // tri avec arbre binaire
		NoeudABL* abl = ablCreer(liste);
    	listeTriee = listeCreer(2);
		ablElements(abl, listeTriee);
	} else if (typeTri == 2) { // tri avec avl
		NoeudAVL* avl = avlCreer(liste);
    	listeTriee = listeCreer(2);
		avlElements(avl, listeTriee);
	}

	listeAfficher(listeTriee);

    fclose(fichierEntree);

	FILE *fichierSortie = fopen(argv[2], "w");

    if (fichierSortie == NULL) {
        return 1;
    }

	fputs(headerTableau, fichierSortie);
	for (int i = 0; i < listeTriee->indice; i++) {
		fputs(listeTriee->buffer[i].ligne, fichierSortie);
	}

	fclose(fichierSortie);

	if (listeTriee != liste) listeDesallouer(listeTriee);
	listeDesallouer(liste);

    return 0;
}
