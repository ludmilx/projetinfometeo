#include <stdlib.h>
#include <stdio.h>

// rendre le 3 février max

// typedef struct {
//     int valeurs[4];
//     time_t date,
//     ...
// }

typedef struct {
    int* buffer;
    int taille;
    int indice;
} Liste;

Liste* creerListe(int taille);
void listeAjouter(Liste *liste, int n);
void desallouerListe(Liste *liste);

typedef struct _NoeudABR {
    int valeur;
    struct _NoeudABR* gauche;
    struct _NoeudABR* droite;
} NoeudABR;

typedef struct _NoeudAVL {
    int valeur;
    struct _NoeudAVL* gauche;
    struct _NoeudAVL* droite;
} NoeudAVL;

NoeudAVL* creerAVL(Liste* listeDepart);
NoeudAVL* nouveauNoeud(int element);
NoeudAVL* enfantInferieur(NoeudAVL* noeud);
NoeudAVL* enfantSuperieur(NoeudAVL* noeud);
NoeudAVL* rotationDroite(NoeudAVL* noeud);
NoeudAVL* rotationGauche(NoeudAVL* noeud);

// NoeudAVL* parent(NoeudAVL* noeud);
NoeudAVL* nouvelEnfant(NoeudAVL* parent, int element);
int equilibre(NoeudAVL* noeud);
int hauteur(NoeudAVL* noeud);
void elements(NoeudAVL* noeud, Liste* out);
void desallouerAVL(NoeudAVL* noeud);

int main(int argc, char** argv) {
    Liste* l = creerListe(10);
    listeAjouter(l, 0);
    listeAjouter(l, 1);
    listeAjouter(l, 3);
    listeAjouter(l, 4);
    listeAjouter(l, -3);
    listeAjouter(l, 2);
    listeAjouter(l, 8);
    listeAjouter(l, 9);
    NoeudAVL* avl = creerAVL(l);
    // printf("%d\n", equilibre(avl));



    // // truc.csv
    // if (argc <= 1) {
    //     return 1;
    // }

    // char c;
    // FILE *fp = fopen(argv[1], "r");

    // if (fp == NULL) {
    //     // message d'erreur
    //     return 1;
    // }

    // int nblignes = 0;
    // int nbcolonne = 0;
    // char buffer[100] = {0};
    // int curseurBuffer = 0;
    // int n;

    // Liste* liste = creerListe(2);

    // while ((c = fgetc(fp)) != EOF) { // pas oublier dernière ligne
    //     if (nblignes >= 1) {
    //         if (nbcolonne == 2) {
    //             if (c == ';') {
    //                 n = atoi(buffer);
    //                 listeAjouter(liste, n);
    //                 printf("%d\n", n);
    //                 buffer[0] = '\0';
    //                 curseurBuffer = 0;
    //             } else {
    //                 buffer[curseurBuffer] = c;
    //                 curseurBuffer++;
    //             }
    //         }
    //     }

    //     if (c == '\n') {
    //         nbcolonne = 0;
    //         nblignes++;
    //     }

    //     if (c == ';') {
    //         nbcolonne++;
    //     }
    // }

    // desallouerListe(liste);

    // fclose(fp);

    // return 0;
}

Liste* creerListe(int taille) {
    Liste* nouvelleListe = malloc(sizeof(Liste));
    nouvelleListe->indice = 0;
    nouvelleListe->taille = taille;
    nouvelleListe->buffer = malloc(sizeof(int) * taille);
    return nouvelleListe;
}

void listeAjouter(Liste *liste, int n) {
    if (liste->taille == liste->indice) {
        liste->taille *= 2;
        liste->buffer = realloc(liste->buffer, sizeof(int) * liste->taille);
    }
    liste->buffer[liste->indice] = n;
    liste->indice++;
}

void desallouerListe(Liste *liste) {
    free(liste->buffer);
    liste->taille = 0;
    free(liste);
}

void echanger(Liste *liste, int a, int b) {
    int buffer = liste->buffer[a];
    liste->buffer[a] = liste->buffer[b];
    liste->buffer[b] = buffer;
}

int partition(Liste *liste, int premier, int dernier, int pivot) {
    pivot = liste->buffer[(premier+dernier)/2];
    int i = premier - 1;
    int j = dernier - 1;

    // do {

    // } while();
}

NoeudAVL* creerAVL(Liste* listeDepart) {
    if (listeDepart->taille > 0) {
        NoeudAVL* nouveau = nouveauNoeud(listeDepart->buffer[0]);
        for (int i = 1; i < listeDepart->taille; i++) {
            printf("+ %d\n", listeDepart->buffer[i]);
            if (listeDepart->buffer[i] > listeDepart->buffer[0]) {
                nouveau->droite = nouvelEnfant(nouveau->droite, listeDepart->buffer[i]);
            } else {
                nouveau->gauche = nouvelEnfant(nouveau->gauche, listeDepart->buffer[i]);
            }
        }
        return nouveau;
    } else {
        return NULL;
    }
}

NoeudAVL* nouveauNoeud(int element) {
    NoeudAVL* nouveau = malloc(sizeof(NoeudAVL));
    nouveau->droite = NULL;
    nouveau->gauche = NULL;
    nouveau->valeur = element;
    return nouveau;
}

// NoeudAVL* enfantInferieur(NoeudAVL* noeud);
// NoeudAVL* enfantSuperieur(NoeudAVL* noeud);
// NoeudAVL* parent(NoeudAVL* noeud);

//
//
//
//

NoeudAVL* nouvelEnfant(NoeudAVL* parent, int element) {
    if (parent == NULL) {
        NoeudAVL* nouveau = nouveauNoeud(element);
        return nouveau;
    }

    if (element > parent->valeur) {
        parent->droite = nouvelEnfant(parent->droite, element);
    } else {
        parent->gauche = nouvelEnfant(parent->gauche, element);
    }

    int eq = equilibre(parent);

    printf("1\n");
    if (eq > 1 && parent->gauche != NULL && element < parent->gauche->valeur) {
        return rotationDroite(parent);
    }

    printf("2\n");
    if (eq < -1 && parent->droite != NULL && element > parent->droite->valeur) {
        return rotationGauche(parent);
    }

    printf("3\n");
    if (eq > 1 && parent->gauche != NULL && element > parent->gauche->valeur) {
        parent->gauche = rotationGauche(parent);
        return rotationDroite(parent);
    }

    printf("4\n");
    if (eq < -1 && parent->droite != NULL && element < parent->droite->valeur) {
        parent->gauche = rotationDroite(parent);
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
    NoeudAVL * gauche = noeud->gauche;
    NoeudAVL * racine = gauche->droite;

    if (racine == NULL) {
        racine = gauche;
    }

    racine->droite = noeud;

    if (racine->gauche == NULL) {
        racine->gauche = gauche;
    }

    return racine;
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
    printf("1.1\n");
    NoeudAVL * droite = noeud->droite;
    NoeudAVL * racine = droite->gauche;

    if (racine == NULL) {
        racine = droite;
    }

    printf("1.3 %d\n", (void*) noeud);
    printf("1.4 %d\n", (void*) racine);
    racine->gauche = noeud;
    printf("1.5 %d\n", (void*) droite);

    if (racine->droite == NULL) {
        racine->droite = droite;
    }

    return racine;
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

/*
// Sorts a (portion of an) array, divides it into partitions, then sorts those
algorithm quicksort(A, lo, hi) is
  if lo >= 0 && hi >= 0 && lo < hi then
    p := partition(A, lo, hi)
    quicksort(A, lo, p) // Note: the pivot is now included
    quicksort(A, p + 1, hi)

// Divides array into two partitions
algorithm partition(A, lo, hi) is
  // Pivot value
  pivot := A[ floor((hi + lo) / 2) ] // The value in the middle of the array

  // Left index
  i := lo - 1

  // Right index
  j := hi + 1

  loop forever
    // Move the left index to the right at least once and while the element at
    // the left index is less than the pivot
    do i := i + 1 while A[i] < pivot

    // Move the right index to the left at least once and while the element at
    // the right index is greater than the pivot
    do j := j - 1 while A[j] > pivot

    // If the indices crossed, return
    if i >= j then return j

    // Swap the elements at the left and right indices
    swap A[i] with A[j]
*/
