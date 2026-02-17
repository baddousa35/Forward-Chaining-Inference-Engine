#include "inference.h"
#include "hash.h"
#include "kb.h"
#include "rule.h"
#include "list.h"
#include <stdio.h>

/*
 * ------------------------------------------------------------
 * Fonction : inference
 * ------------------------------------------------------------
 * Rôle :
 *  Cette fonction synchronise la table de hachage avec la base
 *  de faits courante. Elle vide complètement la table de hachage
 *  puis y réinsère tous les faits présents dans la base de faits.
 *
 * Paramètres :
 *  - BF : pointeur vers la base de faits contenant les faits actuels
 *  - ht : pointeur vers la table de hachage à synchroniser
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - p : pointeur permettant de parcourir la liste chaînée des faits
 */
void inference(BaseConnaissances *BF, HashTable *ht){
    // Suppression de tous les éléments actuellement stockés dans la table
    hash_table_clear(ht);

    // Réinitialisation de la table de hachage (table vide et prête à l’usage)
    hash_table_init(ht);

    // Parcours de tous les faits présents dans la base de faits
    for (ListNode *p = BF->head; p != NULL; p = p->next) {
        // Insertion de chaque fait dans la table de hachage
        hash_table_insert(ht, p->s);
    }
}

/*
 * ------------------------------------------------------------
 * Fonction : toutes_premisses_vraies
 * ------------------------------------------------------------
 * Rôle :
 *  Vérifie si toutes les prémisses d’une règle sont présentes
 *  dans la base de faits.
 *
 * Paramètres :
 *  - R  : pointeur vers la règle dont on veut tester les prémisses
 *  - BF : pointeur vers la base de faits actuelle
 *
 * Valeur de retour :
 *  - true  : toutes les prémisses sont présentes dans la base de faits
 *  - false : au moins une prémisse est absente
 *
 * Variables locales :
 *  - p : pointeur permettant de parcourir la liste des prémisses
 */
bool toutes_premisses_vraies(const Regle *R, const BaseFaits *BF) {
    // Parcours de toutes les prémisses de la règle
    for (ListNode *p = R->premisses.head; p; p = p->next) {
        // Si une prémisse n’est pas trouvée dans la base de faits,
        // la règle ne peut pas être appliquée
        if (!liste_contient_rec(BF, p->s)) return false;
    }

    // Toutes les prémisses sont présentes
    return true;
}

/*
 * ------------------------------------------------------------
 * Fonction : moteur_inference
 * ------------------------------------------------------------
 * Rôle :
 *  Applique un moteur d’inférence chaînage avant.
 *  Tant que de nouveaux faits peuvent être déduits,
 *  le moteur parcourt l’ensemble des règles et ajoute
 *  les conclusions valides à la base de faits.
 *
 * Paramètres :
 *  - BC : pointeur vers la base de connaissances (ensemble des règles)
 *  - BF : pointeur vers la base de faits à enrichir
 *  - ht : pointeur vers la table de hachage utilisée pour éviter
 *         les doublons de faits
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - nouveau : booléen indiquant si un nouveau fait a été ajouté
 *  - n       : pointeur pour parcourir les règles
 *  - R       : règle courante analysée
 *  - c       : conclusion de la règle courante
 */
void moteur_inference(const BaseConnaissances *BC, BaseFaits *BF, HashTable *ht) {
    // Indique si une nouvelle déduction a été faite
    bool nouveau = true;

    // Boucle principale : continue tant que de nouveaux faits sont déduits
    while (nouveau) {
        nouveau = false;

        // Parcours de toutes les règles de la base de connaissances
        for (BCNode *n = BC->head; n != NULL; n = n->next) {
            const Regle *R = &n->regle;

            // Récupération de la conclusion associée à la règle
            const char *c = regle_obtenir_conclusion(R);
            if (!c) continue;

            // Vérifie si la règle est applicable :
            //  - toutes les prémisses sont vraies
            //  - la conclusion n’est pas déjà connue
            if (toutes_premisses_vraies(R, BF) && !hash_table_contains(ht, c)) {

                // Ajout de la nouvelle conclusion à la base de faits
                liste_ajouter_en_queue(BF, c);

                // Insertion dans la table de hachage pour éviter les doublons
                hash_table_insert(ht, c);

                // Affichage de la nouvelle déduction
                printf(">> Nouvelle déduction : %s\n", c);

                // Indique qu’un nouveau fait a été ajouté
                nouveau = true;
            }
        }
    }

    // Fin du processus d’inférence
    printf("Inférence terminée.\n");
}
