#ifndef KB_H
#define KB_H

#include "rule.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct BCNode {
    Regle regle;
    struct BCNode *next;
} BCNode;

typedef struct {
    BCNode *head;
    BCNode *tail;
    size_t size;
} BaseConnaissances;

void bc_init(BaseConnaissances *BC);
bool bc_est_vide(const BaseConnaissances *BC);

void bc_ajouter_regle_en_queue(BaseConnaissances *BC, const Regle *R); // copie profonde
const Regle *bc_tete(const BaseConnaissances *BC);

bool bc_supprimer_regle_index(BaseConnaissances *BC, size_t idx);
void bc_vider(BaseConnaissances *BC);

void bc_afficher(const BaseConnaissances *BC);

#endif
