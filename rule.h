#ifndef RULE_H
#define RULE_H

#include "list.h"
#include <stdbool.h>

typedef struct {
    Liste premisses;
    char *conclusion; // NULL si non définie
} Regle;

void regle_init(Regle *R);
void regle_ajouter_premisse(Regle *R, const char *p);
bool regle_supprimer_premisse(Regle *R, const char *p);
bool regle_premisses_vide(const Regle *R);

void regle_definir_conclusion(Regle *R, const char *c);
const char *regle_obtenir_conclusion(const Regle *R);

void regle_detruire(Regle *R);
void regle_afficher(const Regle *R);

#endif
