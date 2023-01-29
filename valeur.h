#ifndef H_VALEUR
#define H_VALEUR

#define NB_COLONNES 15
#define TAILLE_LIGNE 1024

typedef struct {
    double colonnes[NB_COLONNES];
    int focusComparaison;
    char ligne[TAILLE_LIGNE];
} Valeur;

void copier(Valeur* val, Valeur* out);
int comparer(Valeur* a, Valeur* b);
double valeurFocus(Valeur* v);

#endif
