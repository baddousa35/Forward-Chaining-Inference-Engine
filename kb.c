#include "kb.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * ------------------------------------------------------------
 * Fonction : xmalloc
 * ------------------------------------------------------------
 * Rôle :
 *  Alloue dynamiquement un bloc mémoire de taille donnée.
 *  En cas d’échec de l’allocation, le programme est arrêté
 *  proprement avec un message d’erreur.
 *
 * Paramètres :
 *  - n : taille (en octets) de la mémoire à allouer
 *
 * Valeur de retour :
 *  - pointeur vers la zone mémoire allouée
 *
 * Variables locales :
 *  - p : pointeur vers la mémoire allouée
 */
static void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return p;
}

/*
 * ------------------------------------------------------------
 * Fonction : bc_init
 * ------------------------------------------------------------
 * Rôle :
 *  Initialise une base de connaissances vide.
 *  Tous les pointeurs sont mis à NULL et la taille est remise à zéro.
 *
 * Paramètres :
 *  - BC : pointeur vers la base de connaissances à initialiser
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
void bc_init(BaseConnaissances *BC) {
    BC->head = NULL;
    BC->tail = NULL;
    BC->size = 0;
}

/*
 * ------------------------------------------------------------
 * Fonction : bc_est_vide
 * ------------------------------------------------------------
 * Rôle :
 *  Indique si la base de connaissances est vide.
 *
 * Paramètres :
 *  - BC : pointeur constant vers la base de connaissances
 *
 * Valeur de retour :
 *  - true  : la base de connaissances est vide
 *  - false : elle contient au moins une règle
 */
bool bc_est_vide(const BaseConnaissances *BC) {
    return BC->head == NULL;
}

/*
 * ------------------------------------------------------------
 * Fonction : bc_ajouter_regle_en_queue
 * ------------------------------------------------------------
 * Rôle :
 *  Ajoute une règle à la fin de la base de connaissances.
 *  Une copie complète de la règle est créée (prémisses et conclusion).
 *
 * Paramètres :
 *  - BC : pointeur vers la base de connaissances
 *  - R  : pointeur vers la règle à ajouter
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - n : nouveau nœud contenant la règle copiée
 *  - p : pointeur pour parcourir les prémisses de la règle source
 */
void bc_ajouter_regle_en_queue(BaseConnaissances *BC, const Regle *R) {
    // Allocation d’un nouveau nœud de base de connaissances
    BCNode *n = (BCNode *)xmalloc(sizeof(BCNode));

    // Initialisation de la règle contenue dans le nœud
    regle_init(&n->regle);

    // Copie de toutes les prémisses de la règle source
    for (ListNode *p = R->premisses.head; p; p = p->next) {
        regle_ajouter_premisse(&n->regle, p->s);
    }

    // Copie de la conclusion si elle existe
    if (R->conclusion)
        regle_definir_conclusion(&n->regle, R->conclusion);

    // Insertion du nœud en fin de liste
    n->next = NULL;
    if (BC->tail == NULL) {
        BC->head = BC->tail = n;
    } else {
        BC->tail->next = n;
        BC->tail = n;
    }

    // Mise à jour du nombre de règles
    BC->size++;
}

/*
 * ------------------------------------------------------------
 * Fonction : bc_tete
 * ------------------------------------------------------------
 * Rôle :
 *  Retourne la première règle de la base de connaissances.
 *
 * Paramètres :
 *  - BC : pointeur constant vers la base de connaissances
 *
 * Valeur de retour :
 *  - pointeur vers la première règle
 *  - NULL si la base est vide
 */
const Regle *bc_tete(const BaseConnaissances *BC) {
    return BC->head ? &BC->head->regle : NULL;
}

/*
 * ------------------------------------------------------------
 * Fonction : bc_supprimer_regle_index
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime une règle située à un indice donné dans la
 *  base de connaissances.
 *
 * Paramètres :
 *  - BC  : pointeur vers la base de connaissances
 *  - idx : indice de la règle à supprimer
 *
 * Valeur de retour :
 *  - true  : la suppression a été effectuée
 *  - false : l’indice est invalide
 *
 * Variables locales :
 *  - prev : pointeur vers le nœud précédent
 *  - cur  : pointeur vers le nœud courant
 */
bool bc_supprimer_regle_index(BaseConnaissances *BC, size_t idx) {
    // Vérification de la validité de l’indice
    if (idx >= BC->size) return false;

    BCNode *prev = NULL;
    BCNode *cur = BC->head;

    // Parcours jusqu’au nœud à supprimer
    for (size_t i = 0; i < idx; i++) {
        prev = cur;
        cur = cur->next;
    }

    // Réorganisation des pointeurs
    if (prev) prev->next = cur->next;
    else BC->head = cur->next;

    // Mise à jour de la fin de liste si nécessaire
    if (cur == BC->tail) BC->tail = prev;

    // Libération de la règle et du nœud
    regle_detruire(&cur->regle);
    free(cur);

    // Mise à jour de la taille
    BC->size--;
    return true;
}

/*
 * ------------------------------------------------------------
 * Fonction : bc_vider
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime toutes les règles de la base de connaissances
 *  et libère la mémoire associée.
 *
 * Paramètres :
 *  - BC : pointeur vers la base de connaissances
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - cur : pointeur pour parcourir les nœuds
 *  - nxt : pointeur vers le nœud suivant
 */
void bc_vider(BaseConnaissances *BC) {
    BCNode *cur = BC->head;

    // Parcours et destruction de tous les nœuds
    while (cur) {
        BCNode *nxt = cur->next;
        regle_detruire(&cur->regle);
        free(cur);
        cur = nxt;
    }

    // Réinitialisation de la base de connaissances
    bc_init(BC);
}

/*
 * ------------------------------------------------------------
 * Fonction : bc_afficher
 * ------------------------------------------------------------
 * Rôle :
 *  Affiche toutes les règles de la base de connaissances
 *  avec leur indice.
 *
 * Paramètres :
 *  - BC : pointeur constant vers la base de connaissances
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - i : indice de la règle affichée
 *  - n : pointeur pour parcourir les règles
 */
void bc_afficher(const BaseConnaissances *BC) {
    size_t i = 0;

    // Parcours de toutes les règles pour affichage
    for (BCNode *n = BC->head; n; n = n->next) {
        printf("[%zu] ", i++);
        regle_afficher(&n->regle);
    }
}
