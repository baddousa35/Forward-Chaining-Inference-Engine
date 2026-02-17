#include "rule.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * ------------------------------------------------------------
 * Fonction : xmalloc
 * ------------------------------------------------------------
 * Rôle :
 *  Alloue dynamiquement un bloc mémoire de taille donnée.
 *  En cas d’échec de l’allocation, le programme est arrêté
 *  avec un message d’erreur explicite.
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
 *  Duplique une chaîne de caractères en allouant
 *  dynamiquement la mémoire nécessaire.
 *
 * Paramètres :
 *  - s : chaîne de caractères à dupliquer
 *
 * Valeur de retour :
 *  - pointeur vers la nouvelle chaîne copiée
 *
 * Variables locales :
 *  - n : taille de la chaîne (caractère nul inclus)
 *  - p : pointeur vers la chaîne allouée
 */
static char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)xmalloc(n);
    memcpy(p, s, n);
    return p;
}

/*
 * ------------------------------------------------------------
 * Fonction : regle_init
 * ------------------------------------------------------------
 * Rôle :
 *  Initialise une règle vide en préparant la liste
 *  de prémisses et en mettant la conclusion à NULL.
 *
 * Paramètres :
 *  - R : pointeur vers la règle à initialiser
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
void regle_init(Regle *R) {
    liste_init(&R->premisses);
    R->conclusion = NULL;
}

/*
 * ------------------------------------------------------------
 * Fonction : regle_ajouter_premisse
 * ------------------------------------------------------------
 * Rôle :
 *  Ajoute une prémisse à la règle.
 *
 * Paramètres :
 *  - R : pointeur vers la règle
 *  - p : chaîne de caractères représentant la prémisse
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
void regle_ajouter_premisse(Regle *R, const char *p) {
    liste_ajouter_en_queue(&R->premisses, p);
}

/*
 * ------------------------------------------------------------
 * Fonction : regle_supprimer_premisse
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime la première occurrence d’une prémisse
 *  dans la règle.
 *
 * Paramètres :
 *  - R : pointeur vers la règle
 *  - p : chaîne de caractères représentant la prémisse
 *
 * Valeur de retour :
 *  - true  : la prémisse a été supprimée
 *  - false : la prémisse n’a pas été trouvée
 */
bool regle_supprimer_premisse(Regle *R, const char *p) {
    return liste_supprimer_premiere(&R->premisses, p);
}

/*
 * ------------------------------------------------------------
 * Fonction : regle_premisses_vide
 * ------------------------------------------------------------
 * Rôle :
 *  Indique si la règle ne contient aucune prémisse.
 *
 * Paramètres :
 *  - R : pointeur constant vers la règle
 *
 * Valeur de retour :
 *  - true  : aucune prémisse
 *  - false : au moins une prémisse présente
 */
bool regle_premisses_vide(const Regle *R) {
    return liste_est_vide(&R->premisses);
}

/*
 * ------------------------------------------------------------
 * Fonction : regle_definir_conclusion
 * ------------------------------------------------------------
 * Rôle :
 *  Définit ou modifie la conclusion de la règle.
 *  Toute conclusion précédente est libérée.
 *
 * Paramètres :
 *  - R : pointeur vers la règle
 *  - c : chaîne de caractères représentant la conclusion
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
void regle_definir_conclusion(Regle *R, const char *c) {
    // Libération de l’ancienne conclusion si elle existe
    free(R->conclusion);

    // Duplication et affectation de la nouvelle conclusion
    R->conclusion = xstrdup(c);
}

/*
 * ------------------------------------------------------------
 * Fonction : regle_obtenir_conclusion
 * ------------------------------------------------------------
 * Rôle :
 *  Retourne la conclusion associée à la règle.
 *
 * Paramètres :
 *  - R : pointeur constant vers la règle
 *
 * Valeur de retour :
 *  - pointeur vers la chaîne de caractères de la conclusion
 *  - NULL si aucune conclusion n’est définie
 */
const char *regle_obtenir_conclusion(const Regle *R) {
    return R->conclusion;
}

/*
 * ------------------------------------------------------------
 * Fonction : regle_detruire
 * ------------------------------------------------------------
 * Rôle :
 *  Libère toutes les ressources associées à une règle :
 *  prémisses et conclusion.
 *
 * Paramètres :
 *  - R : pointeur vers la règle à détruire
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
void regle_detruire(Regle *R) {
    // Libération de la liste des prémisses
    liste_vider(&R->premisses);

    // Libération de la conclusion
    free(R->conclusion);
    R->conclusion = NULL;
}

/*
 * ------------------------------------------------------------
 * Fonction : regle_afficher
 * ------------------------------------------------------------
 * Rôle :
 *  Affiche la règle sous une forme lisible :
 *  IF <premisse1> AND <premisse2> ... THEN <conclusion>
 *
 * Paramètres :
 *  - R : pointeur constant vers la règle à afficher
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - p : pointeur pour parcourir les prémisses
 */
void regle_afficher(const Regle *R) {
    printf("IF ");

    // Affichage de toutes les prémisses
    for (ListNode *p = R->premisses.head; p; p = p->next) {
        printf("%s", p->s);
        if (p->next) printf(" AND ");
    }

    // Affichage de la conclusion
    printf(" THEN %s\n", R->conclusion ? R->conclusion : "(aucune)");
}
