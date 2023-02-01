#include "valeur.h"

int comparer(Valeur* a, Valeur* b) {
    if (valeur(a) > valeur(b)) return 1;
    if (valeur(a) < valeur(b)) return -1;
    return 0;
}

double valeur(Valeur* v) {
    return v->valeur;
}

void copier(Valeur* val, Valeur* out) {
    out->valeur = val->valeur;
}
