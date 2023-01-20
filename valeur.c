#include "valeur.h"

int comparer(Valeur* a, Valeur* b) {
    if (a->colonnes[a->focusComparaison] > b->colonnes[a->focusComparaison]) return 1;
    if (a->colonnes[a->focusComparaison] < b->colonnes[a->focusComparaison]) return -1;
    return 0;
}
