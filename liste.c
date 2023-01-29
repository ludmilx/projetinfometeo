#include <stdlib.h>
#include <stdio.h>
#include "liste.h"

Liste* listeCreer(int taille) {
    Liste* nouvelleListe = malloc(sizeof(Liste));
    nouvelleListe->indice = 0;
    nouvelleListe->taille = taille;
    nouvelleListe->buffer = malloc(sizeof(Valeur) * taille);
    return nouvelleListe;
}

// on prend le struct Valeur par valeur car on veut
// avoir la responsabilité de le supprimer ensuite
void listeAjouter(Liste *liste, Valeur* valeur) {
    if (liste->taille == liste->indice) {
        liste->taille *= 2;
        liste->buffer = realloc(liste->buffer, sizeof(Valeur) * liste->taille);
    }

    copier(valeur, &liste->buffer[liste->indice]);

    liste->indice++;
}

// on a alloué les valeurs
// il faut aussi les désaouller
void listeDesallouer(Liste *liste) {
    free(liste->buffer);
    liste->taille = 0;
    free(liste);
}

void listeAfficher(Liste *liste) {
    printf("[ ");
    for (int i = 0; i < liste->indice; i++) {
        printf("%f, ", valeurFocus(&liste->buffer[i]));
    }
    printf("] utilisé=%d alloué=%d\n", liste->indice, liste->taille);
}

void listeEchanger(Liste *liste, int a, int b) {
    Valeur buffer = {0};
    copier(&liste->buffer[a], &buffer);
    copier(&liste->buffer[b], &liste->buffer[a]);
    copier(&buffer, &liste->buffer[b]);
}

void listeTrierRec(Liste *liste, int debut, int fin) {
    int i, j, pivot;
    if(debut < fin){
        pivot = debut;
        i = debut;
        j = fin;
        while(i < j){
            int valComp = comparer(&liste->buffer[i], &liste->buffer[pivot]);
            while((valComp == -1 || valComp == 0) && i < fin) {
                i++;
                valComp = comparer(&liste->buffer[i], &liste->buffer[pivot]);
            }

            valComp = comparer(&liste->buffer[j], &liste->buffer[pivot]);
            while(valComp == 1) {
                j--;
                valComp = comparer(&liste->buffer[j], &liste->buffer[pivot]);
            }

            if(i < j){
                listeEchanger(liste, i, j);
            }
        }
        listeEchanger(liste, pivot, j);
        listeTrierRec(liste, debut, j-1);
        listeTrierRec(liste, j+1, fin);
    }
}

void listeTrier(Liste *liste) {
    listeTrierRec(liste, 0, liste->indice - 1);
}
