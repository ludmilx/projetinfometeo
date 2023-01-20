#ifndef H_VALEUR
#define H_VALEUR

#define NB_COLONNES 4

typedef struct {
    int colonnes[NB_COLONNES];
    int focusComparaison;
} Valeur;

int comparer(Valeur* a, Valeur* b);

#endif
