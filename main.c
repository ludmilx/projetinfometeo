#include <stdlib.h>
#include <stdio.h>
#include "valeur.h"
#include "liste.h"
#include "avl.h"

// rendre le 3 février max

// typedef struct _NoeudABR {
//     int valeur;
//     struct _NoeudABR* gauche;
//     struct _NoeudABR* droite;
// } NoeudABR;

int main(int argc, char** argv) {
    Liste* l = creerListe(10);
    Valeur v = {
        .colonnes = { 0, 1, 0, 8 },
        .focusComparaison = 0
    };
    listeAjouter(l, v);
    v.colonnes[0] = 1;
    listeAjouter(l, v);
    v.colonnes[0] = 2;
    listeAjouter(l, v);
    v.colonnes[0] = -19;
    listeAjouter(l, v);
    v.colonnes[0] = 3;
    listeAjouter(l, v);
    v.colonnes[0] = 0;
    listeAjouter(l, v);
    v.colonnes[0] = 7;
    listeAjouter(l, v);
    v.colonnes[0] = -6;
    listeAjouter(l, v);
    v.colonnes[0] = 9;
    listeAjouter(l, v);
    v.colonnes[0] = 11;
    listeAjouter(l, v);
    NoeudAVL* avl = creerAVL(l);
    printf("-----------------------\n");
    printf("%d\n", equilibre(avl));



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
