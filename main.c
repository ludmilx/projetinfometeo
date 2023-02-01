#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "valeur.h"
#include "liste.h"
#include "avl.h"
#include "abr.h"

#define MODE_TRI_TAB 0
#define MODE_TRI_ABR 1
#define MODE_TRI_AVl 2

int main(int argc, char** argv) {
    if (argc <= 5) {
        return 1;
    }

    char* nomFichierIn = NULL;
    char* nomFichierOut = NULL;
    int decroissant = 0;
    int modeTri = -1;

    char* argPrec = "";
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];

        if (strcmp(argPrec, "-o") == 0) {
            if (arg[0] == '-') {
                return 1;
            }
            nomFichierOut = arg;
            argPrec = "";
        } else if (strcmp(argPrec, "-f") == 0) {
            if (arg[0] == '-') {
                return 1;
            }
            nomFichierIn = arg;
            argPrec = "";
        } else if (strcmp(arg, "--tab") == 0) {
            if (modeTri != -1) return 1;
            modeTri = MODE_TRI_TAB;
        } else if (strcmp(arg, "--avl") == 0) {
            if (modeTri != -1) return 1;
            modeTri = MODE_TRI_TAB;
        } else if (strcmp(arg, "--abr") == 0) {
            if (modeTri != -1) return 1;
            modeTri = MODE_TRI_TAB;
        } else if (strcmp(arg, "-r") == 0) {
            decroissant = 1;
        } else if (strcmp(arg, "-o") == 0 || strcmp(arg, "-f") == 0) {
            argPrec = arg;
        }
    }

    if (modeTri == -1) modeTri = MODE_TRI_AVl;

    char c;
    FILE *fichierEntree = fopen(nomFichierIn, "r");

    if (fichierEntree == NULL) {
        return 2;
    }

    char buffer[100] = {0};
    int curseurBuffer = 0;
    double n;

    Liste* liste = listeCreer(2);
	Valeur valeur = {0};

    while ((c = fgetc(fichierEntree)) != EOF) {
        if (c == '\n') {
            buffer[curseurBuffer] = '\0';
            valeur.valeur = decroissant ? atof(buffer) * -1 : atof(buffer);
            listeAjouter(liste, &valeur);
            curseurBuffer = 0;
        } else {
            buffer[curseurBuffer] = c;
            curseurBuffer++;
        }
    }

	// listeAfficher(liste);
	Liste *listeTriee;

	if (modeTri == MODE_TRI_TAB) { // tri rapide sur liste directement
		listeTrier(liste);
		listeTriee = liste;
	} else if (modeTri == MODE_TRI_ABR) { // tri avec arbre binaire
		NoeudABR* abr = abrCreer(liste);
    	listeTriee = listeCreer(2);
		abrElements(abr, listeTriee);
	} else if (modeTri == MODE_TRI_AVl) { // tri avec avl
		NoeudAVL* avl = avlCreer(liste);
    	listeTriee = listeCreer(2);
		avlElements(avl, listeTriee);
	}

	// listeAfficher(listeTriee);

    fclose(fichierEntree);

	FILE *fichierSortie = fopen(nomFichierOut, "w");

    if (fichierSortie == NULL) {
        return 3;
    }

	for (int i = 0; i < listeTriee->indice; i++) {
        char buffer[255] = "";
        sprintf(buffer, "%f\n", decroissant ? listeTriee->buffer[i].valeur * -1 : listeTriee->buffer[i].valeur);
        fputs(buffer, fichierSortie);
	}

	fclose(fichierSortie);

	if (listeTriee != liste) listeDesallouer(listeTriee);
	listeDesallouer(liste);

    return 0;
}
