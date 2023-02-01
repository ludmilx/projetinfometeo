#ifndef H_VALEUR
#define H_VALEUR

#define NB_COLONNES 15
#define TAILLE_LIGNE 1024

typedef struct {
    double valeur;
} Valeur;

void copier(Valeur* val, Valeur* out);
int comparer(Valeur* a, Valeur* b);
double valeur(Valeur* v);


#endif
