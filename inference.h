#ifndef INFERENCE_H
#define INFERENCE_H

#include <stdbool.h>
#include "kb.h"
#include "list.h"
#include "hash.h"

/* BaseFaits = une liste de faits (strings) */
typedef Liste BaseFaits;

bool toutes_premisses_vraies(const Regle *R, const BaseFaits *BF);
void moteur_inference(const BaseConnaissances *BC, BaseFaits *BF, HashTable *ht);

#endif


