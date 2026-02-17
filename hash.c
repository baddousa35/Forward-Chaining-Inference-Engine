#include "hash.h"
#include <stdlib.h>
#include <string.h>

/*
 * ------------------------------------------------------------
 * Fonction : strdup_safe
 * ------------------------------------------------------------
 * Rôle :
 *  Duplique une chaîne de caractères de manière portable,
 *  sans dépendre de la fonction strdup (non standard en C).
 *
 * Paramètres :
 *  - s : pointeur vers la chaîne de caractères à dupliquer
 *
 * Valeur de retour :
 *  - pointeur vers une nouvelle chaîne allouée dynamiquement
 *  - NULL si la chaîne est NULL ou si l’allocation échoue
 *
 * Variables locales :
 *  - n : taille de la chaîne à copier (caractère nul inclus)
 *  - p : pointeur vers la zone mémoire allouée
 */
static char *strdup_safe(const char *s) {
    // Vérifie que la chaîne source existe
    if (!s) {
        return NULL;
    }

    // Calcul de la taille nécessaire pour la copie
    size_t n = strlen(s) + 1;

    // Allocation dynamique de la mémoire
    char *p = (char *)malloc(n);
    if (!p) {
        return NULL;
    }

    // Copie du contenu de la chaîne source vers la nouvelle zone mémoire
    memcpy(p, s, n);
    return p;
}

/*
 * ------------------------------------------------------------
 * Fonction : hash_function
 * ------------------------------------------------------------
 * Rôle :
 *  Calcule l’indice de hachage associé à une chaîne de caractères.
 *  Utilise une fonction de hachage simple basée sur une
 *  accumulation multiplicative.
 *
 * Paramètres :
 *  - str : chaîne de caractères à hacher
 *
 * Valeur de retour :
 *  - indice compris entre 0 et HASH_SIZE - 1
 *
 * Variables locales :
 *  - hash : valeur intermédiaire du calcul de hachage
 */
static size_t hash_function(const char *str) {
    size_t hash = 0;

    // Parcours de chaque caractère de la chaîne
    while (*str) {
        hash = (hash * 31) + (unsigned char)(*str++);
    }

    // Réduction de la valeur pour rester dans les limites de la table
    return hash % HASH_SIZE;
}

/*
 * ------------------------------------------------------------
 * Fonction : hash_table_init
 * ------------------------------------------------------------
 * Rôle :
 *  Initialise une table de hachage en mettant tous les
 *  pointeurs de listes à NULL.
 *
 * Paramètres :
 *  - ht : pointeur vers la table de hachage à initialiser
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - i : indice utilisé pour parcourir la table
 */
void hash_table_init(HashTable *ht) {
    // Vérifie que la table existe
    if (!ht) return;

    // Initialisation de chaque case de la table
    for (size_t i = 0; i < HASH_SIZE; i++) {
        ht->table[i] = NULL;
    }
}

/*
 * ------------------------------------------------------------
 * Fonction : hash_table_insert
 * ------------------------------------------------------------
 * Rôle :
 *  Insère une proposition dans la table de hachage.
 *  Les collisions sont gérées par chaînage.
 *
 * Paramètres :
 *  - ht          : pointeur vers la table de hachage
 *  - proposition: chaîne de caractères à insérer
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - index    : indice calculé par la fonction de hachage
 *  - new_node : nouveau nœud inséré dans la table
 */
void hash_table_insert(HashTable *ht, const char *proposition) {
    // Vérification des paramètres
    if (!ht || !proposition) return;

    // Calcul de l’indice de hachage
    size_t index = hash_function(proposition);

    // Allocation d’un nouveau nœud
    HashNode *new_node = (HashNode *)malloc(sizeof(HashNode));
    if (!new_node) return;

    // Duplication de la proposition
    new_node->proposition = strdup_safe(proposition);
    if (!new_node->proposition) {
        free(new_node);
        return;
    }

    // Insertion du nœud en tête de la liste chaînée
    new_node->next = ht->table[index];
    ht->table[index] = new_node;
}

/*
 * ------------------------------------------------------------
 * Fonction : hash_table_contains
 * ------------------------------------------------------------
 * Rôle :
 *  Vérifie si une proposition est déjà présente
 *  dans la table de hachage.
 *
 * Paramètres :
 *  - ht          : pointeur vers la table de hachage
 *  - proposition: chaîne de caractères recherchée
 *
 * Valeur de retour :
 *  - true  : la proposition est trouvée
 *  - false : la proposition est absente
 *
 * Variables locales :
 *  - index   : indice de hachage correspondant à la proposition
 *  - current : pointeur pour parcourir la liste chaînée
 */
bool hash_table_contains(const HashTable *ht, const char *proposition) {
    // Vérification des paramètres
    if (!ht || !proposition) return false;

    // Calcul de l’indice de hachage
    size_t index = hash_function(proposition);
    const HashNode *current = ht->table[index];

    // Parcours de la liste chaînée associée à l’indice
    while (current) {
        if (strcmp(current->proposition, proposition) == 0) {
            return true;
        }
        current = current->next;
    }

    // Proposition non trouvée
    return false;
}

/*
 * ------------------------------------------------------------
 * Fonction : hash_table_clear
 * ------------------------------------------------------------
 * Rôle :
 *  Libère toute la mémoire utilisée par la table de hachage
 *  et remet toutes les cases à NULL.
 *
 * Paramètres :
 *  - ht : pointeur vers la table de hachage à libérer
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - i       : indice pour parcourir la table
 *  - current : pointeur sur le nœud courant
 *  - next    : pointeur sur le nœud suivant
 */
void hash_table_clear(HashTable *ht) {
    // Vérifie que la table existe
    if (!ht) return;

    // Parcours de chaque case de la table
    for (size_t i = 0; i < HASH_SIZE; i++) {
        HashNode *current = ht->table[i];

        // Libération de la liste chaînée
        while (current) {
            HashNode *next = current->next;
            free(current->proposition);
            free(current);
            current = next;
        }

        // Réinitialisation de la case
        ht->table[i] = NULL;
    }
}
