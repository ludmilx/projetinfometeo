#include <stdlib.h>
#include "liste.h"


Liste* creerListe(int taille) {
    Liste* nouvelleListe = malloc(sizeof(Liste));
    nouvelleListe->indice = 0;
    nouvelleListe->taille = taille;
    nouvelleListe->buffer = malloc(sizeof(Valeur*) * taille);
    return nouvelleListe;
}

// on prend le struct Valeur par valeur car on veut
// avoir la responsabilité de le supprimer ensuite
void listeAjouter(Liste *liste, Valeur valeur) {
    Valeur* nouvelle_valeur = malloc(sizeof(Valeur));

    // si ajout de champs dans Valeur revenir ici
    for (int i = 0; i < NB_COLONNES; i++)
        nouvelle_valeur->colonnes[i] = valeur.colonnes[i];
    nouvelle_valeur->focusComparaison = valeur.focusComparaison;

    if (liste->taille == liste->indice) {
        liste->taille *= 2;
        liste->buffer = realloc(liste->buffer, sizeof(Valeur*) * liste->taille);
    }
    liste->buffer[liste->indice] = nouvelle_valeur;
    liste->indice++;
}

// on a alloué les valeurs
// il faut aussi les désaouller
void desallouerListe(Liste *liste) {
    for (int i = 0; i < liste->taille; i++) free(liste->buffer[i]);
    free(liste->buffer);
    liste->taille = 0;
    free(liste);
}

void echanger(Liste *liste, int a, int b) {
    Valeur* buffer = liste->buffer[a];
    liste->buffer[a] = liste->buffer[b];
    liste->buffer[b] = buffer;
}
