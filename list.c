#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * ------------------------------------------------------------
 * Fonction : xmalloc
 * ------------------------------------------------------------
 * Rôle :
 *  Alloue dynamiquement un bloc mémoire de taille donnée.
 *  En cas d’échec de l’allocation, le programme est interrompu
 *  avec un message d’erreur.
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
 * Fonction : xstrdup
 * ------------------------------------------------------------
 * Rôle :
 *  Duplique une chaîne de caractères en allouant dynamiquement
 *  la mémoire nécessaire.
 *
 * Paramètres :
 *  - s : chaîne de caractères à copier
 *
 * Valeur de retour :
 *  - pointeur vers la nouvelle chaîne copiée
 *
 * Variables locales :
 *  - n : taille de la chaîne (caractère nul inclus)
 *  - p : pointeur vers la chaîne allouée
 */
static char *xstrdup(const char *s) {
    // Calcul de la taille nécessaire pour la copie
    size_t n = strlen(s) + 1;

    // Allocation de la mémoire
    char *p = (char *)xmalloc(n);

    // Copie de la chaîne source
    memcpy(p, s, n);
    return p;
}

/*
 * ------------------------------------------------------------
 * Fonction : liste_init
 * ------------------------------------------------------------
 * Rôle :
 *  Initialise une liste chaînée vide.
 *
 * Paramètres :
 *  - L : pointeur vers la liste à initialiser
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
void liste_init(Liste *L) {
    L->head = NULL;
    L->tail = NULL;
    L->size = 0;
}

/*
 * ------------------------------------------------------------
 * Fonction : liste_est_vide
 * ------------------------------------------------------------
 * Rôle :
 *  Indique si la liste est vide.
 *
 * Paramètres :
 *  - L : pointeur constant vers la liste
 *
 * Valeur de retour :
 *  - true  : la liste est vide
 *  - false : la liste contient au moins un élément
 */
bool liste_est_vide(const Liste *L) {
    return L->head == NULL;
}

/*
 * ------------------------------------------------------------
 * Fonction : liste_ajouter_en_queue
 * ------------------------------------------------------------
 * Rôle :
 *  Ajoute une chaîne de caractères à la fin de la liste chaînée.
 *
 * Paramètres :
 *  - L : pointeur vers la liste
 *  - s : chaîne de caractères à ajouter
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - n : nouveau nœud ajouté à la liste
 */
void liste_ajouter_en_queue(Liste *L, const char *s) {
    // Allocation et initialisation du nouveau nœud
    ListNode *n = (ListNode *)xmalloc(sizeof(ListNode));
    n->s = xstrdup(s);
    n->next = NULL;

    // Insertion du nœud en fin de liste
    if (L->tail == NULL) {
        L->head = L->tail = n;
    } else {
        L->tail->next = n;
        L->tail = n;
    }

    // Mise à jour de la taille de la liste
    L->size++;
}

/*
 * ------------------------------------------------------------
 * Fonction : contient_rec_node
 * ------------------------------------------------------------
 * Rôle :
 *  Recherche récursivement une chaîne de caractères dans
 *  une liste chaînée à partir d’un nœud donné.
 *
 * Paramètres :
 *  - node : pointeur vers le nœud courant
 *  - s    : chaîne de caractères recherchée
 *
 * Valeur de retour :
 *  - true  : la chaîne est trouvée
 *  - false : la chaîne est absente
 */
static bool contient_rec_node(const ListNode *node, const char *s) {
    // Cas de base : fin de liste
    if (!node) return false;

    // Comparaison de la valeur du nœud courant
    if (strcmp(node->s, s) == 0) return true;

    // Appel récursif sur le nœud suivant
    return contient_rec_node(node->next, s);
}

/*
 * ------------------------------------------------------------
 * Fonction : liste_contient_rec
 * ------------------------------------------------------------
 * Rôle :
 *  Vérifie récursivement si une chaîne de caractères
 *  est présente dans la liste.
 *
 * Paramètres :
 *  - L : pointeur constant vers la liste
 *  - s : chaîne de caractères recherchée
 *
 * Valeur de retour :
 *  - true  : la chaîne est présente
 *  - false : la chaîne est absente
 */
bool liste_contient_rec(const Liste *L, const char *s) {
    return contient_rec_node(L->head, s);
}

/*
 * ------------------------------------------------------------
 * Fonction : liste_supprimer_premiere
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime la première occurrence d’une chaîne de caractères
 *  dans la liste chaînée.
 *
 * Paramètres :
 *  - L : pointeur vers la liste
 *  - s : chaîne de caractères à supprimer
 *
 * Valeur de retour :
 *  - true  : un élément a été supprimé
 *  - false : la chaîne n’a pas été trouvée
 *
 * Variables locales :
 *  - prev : pointeur vers le nœud précédent
 *  - cur  : pointeur vers le nœud courant
 */
bool liste_supprimer_premiere(Liste *L, const char *s) {
    ListNode *prev = NULL;
    ListNode *cur = L->head;

    // Parcours de la liste
    while (cur) {
        if (strcmp(cur->s, s) == 0) {
            // Suppression du nœud courant
            if (prev) prev->next = cur->next;
            else L->head = cur->next;

            // Mise à jour de la fin de liste si nécessaire
            if (cur == L->tail) L->tail = prev;

            // Libération de la mémoire
            free(cur->s);
            free(cur);
            L->size--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }

    // Élément non trouvé
    return false;
}

/*
 * ------------------------------------------------------------
 * Fonction : liste_vider
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime tous les éléments de la liste et libère
 *  la mémoire associée.
 *
 * Paramètres :
 *  - L : pointeur vers la liste
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - cur : pointeur pour parcourir les nœuds
 *  - nxt : pointeur vers le nœud suivant
 */
void liste_vider(Liste *L) {
    ListNode *cur = L->head;

    // Parcours et libération de tous les nœuds
    while (cur) {
        ListNode *nxt = cur->next;
        free(cur->s);
        free(cur);
        cur = nxt;
    }

    // Réinitialisation de la liste
    liste_init(L);
}

/*
 * ------------------------------------------------------------
 * Fonction : liste_tete
 * ------------------------------------------------------------
 * Rôle :
 *  Retourne la chaîne de caractères contenue
 *  dans le premier élément de la liste.
 *
 * Paramètres :
 *  - L : pointeur constant vers la liste
 *
 * Valeur de retour :
 *  - pointeur vers la chaîne du premier élément
 *  - NULL si la liste est vide
 */
const char *liste_tete(const Liste *L) {
    return L->head ? L->head->s : NULL;
}

/*
 * ------------------------------------------------------------
 * Fonction : liste_afficher
 * ------------------------------------------------------------
 * Rôle :
 *  Affiche le contenu de la liste, chaque élément étant
 *  précédé d’un préfixe donné.
 *
 * Paramètres :
 *  - L      : pointeur constant vers la liste
 *  - prefix : chaîne affichée avant chaque élément
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - p : pointeur pour parcourir les éléments de la liste
 */
void liste_afficher(const Liste *L, const char *prefix) {
    // Parcours et affichage de chaque élément
    for (ListNode *p = L->head; p; p = p->next) {
        printf("%s%s\n", prefix, p->s);
    }
}
