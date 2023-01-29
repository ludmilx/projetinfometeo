#include "valeur.h"

int comparer(Valeur* a, Valeur* b) {
    if (valeurFocus(a) > valeurFocus(b)) return 1;
    if (valeurFocus(a) < valeurFocus(b)) return -1;
    return 0;
}

double valeurFocus(Valeur* v) {
    return v->colonnes[v->focusComparaison];
}

void copier(Valeur* val, Valeur* out) {
    for (int i = 0; i < NB_COLONNES; i++)
        out->colonnes[i] = val->colonnes[i];
    out->focusComparaison = val->focusComparaison;
    for (int i = 0; i < TAILLE_LIGNE; i++)
        out->ligne[i] = val->ligne[i];
}
