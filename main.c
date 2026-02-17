#include <stdio.h>
#include "inference.h"
#include "utils.h"
#include "hash.h"
#include "tests.h"

/*
 * ------------------------------------------------------------
 * Fonction : menu_afficher
 * ------------------------------------------------------------
 * Rôle :
 *  Affiche le menu principal de l’application permettant
 *  à l’utilisateur d’interagir avec le moteur d’inférence.
 *
 * Paramètres :
 *  - Aucun
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
static void menu_afficher(void) {
    printf("\n=== Moteur d'inférence ===\n");
    printf("1) Ajouter une règle\n");
    printf("2) Ajouter un fait\n");
    printf("3) Lancer l'inférence\n");
    printf("4) Afficher les règles\n");
    printf("5) Afficher les faits\n");
    printf("6) Supprimer une règle (index)\n");
    printf("7) Supprimer un fait (nom)\n");
    printf("8) Supprimer toutes les règles\n");
    printf("9) Supprimer tous les faits\n");
    printf("10) Supprimer une prémisse d'une règle\n");
    printf("11) Phase de test\n");
    printf("0) Quitter\n");
}

/*
 * ------------------------------------------------------------
 * Fonction : ajouter_regle
 * ------------------------------------------------------------
 * Rôle :
 *  Permet à l’utilisateur de saisir une règle complète
 *  (ensemble de prémisses + conclusion) et de l’ajouter
 *  à la base de connaissances.
 *
 * Paramètres :
 *  - BC : pointeur vers la base de connaissances
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - R   : règle temporaire construite à partir des entrées utilisateur
 *  - buf : tampon utilisé pour la saisie des chaînes de caractères
 */
static void ajouter_regle(BaseConnaissances *BC) {
    Regle R;
    regle_init(&R);

    char buf[256];

    // Saisie des prémisses jusqu’à une ligne vide
    for (;;) {
        if (!lire_ligne("Prémisse (vide pour terminer): ", buf, sizeof(buf))) {
            regle_detruire(&R);
            return;
        }
        if (buf[0] == '\0') break;
        regle_ajouter_premisse(&R, buf);
    }

    // Saisie obligatoire de la conclusion
    for (;;) {
        if (!lire_ligne("Conclusion (obligatoire): ", buf, sizeof(buf))) {
            regle_detruire(&R);
            return;
        }
        if (buf[0] != '\0') break;
        printf("Conclusion vide.\n");
    }
    regle_definir_conclusion(&R, buf);

    // Ajout de la règle à la base de connaissances
    bc_ajouter_regle_en_queue(BC, &R);

    // Libération de la règle temporaire
    regle_detruire(&R);
    printf("Règle ajoutée.\n");
}

/*
 * ------------------------------------------------------------
 * Fonction : ajouter_fait
 * ------------------------------------------------------------
 * Rôle :
 *  Permet à l’utilisateur d’ajouter un fait à la base de faits
 *  s’il n’est pas déjà présent.
 *
 * Paramètres :
 *  - BF : pointeur vers la base de faits
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - buf : tampon de saisie du fait
 */
static void ajouter_fait(BaseFaits *BF) {
    char buf[256];
    if (!lire_ligne("Fait: ", buf, sizeof(buf))) return;
    if (buf[0] == '\0') return;

    // Vérifie l’unicité du fait avant insertion
    if (!liste_contient_rec(BF, buf)) {
        liste_ajouter_en_queue(BF, buf);
        printf("Fait ajouté.\n");
    } else {
        printf("Déjà présent.\n");
    }
}

/*
 * ------------------------------------------------------------
 * Fonction : supprimer_regle
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime une règle de la base de connaissances à partir
 *  de son indice.
 *
 * Paramètres :
 *  - BC : pointeur vers la base de connaissances
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - idx : indice de la règle à supprimer
 */
static void supprimer_regle(BaseConnaissances *BC) {
    int idx;
    if (!lire_entier("Index: ", &idx) || idx < 0) {
        printf("Index invalide.\n");
        return;
    }

    // Suppression de la règle si l’indice est valide
    if (bc_supprimer_regle_index(BC, (size_t)idx))
        printf("Règle supprimée.\n");
    else
        printf("Index hors limites.\n");
}

/*
 * ------------------------------------------------------------
 * Fonction : supprimer_fait
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime un fait de la base de faits à partir de son nom.
 *
 * Paramètres :
 *  - BF : pointeur vers la base de faits
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - buf : tampon contenant le nom du fait à supprimer
 */
static void supprimer_fait(BaseFaits *BF) {
    char buf[256];
    if (!lire_ligne("Fait à supprimer: ", buf, sizeof(buf))) return;
    if (buf[0] == '\0') return;

    if (liste_supprimer_premiere(BF, buf))
        printf("Fait supprimé.\n");
    else
        printf("Introuvable.\n");
}

/*
 * ------------------------------------------------------------
 * Fonction : supprimer_premisse
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime une prémisse spécifique d’une règle donnée,
 *  identifiée par son indice dans la base de connaissances.
 *
 * Paramètres :
 *  - BC : pointeur vers la base de connaissances
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - idx  : indice de la règle concernée
 *  - node : pointeur vers le nœud contenant la règle
 *  - buf  : tampon contenant la prémisse à supprimer
 */
static void supprimer_premisse(BaseConnaissances *BC) {
    int idx;
    if (!lire_entier("Index de la règle: ", &idx) || idx < 0) {
        printf("Index invalide.\n");
        return;
    }
    if ((size_t)idx >= BC->size) {
        printf("Index hors limites.\n");
        return;
    }

    // Accès à la règle ciblée
    BCNode *node = BC->head;
    for (int i = 0; i < idx; i++) node = node->next;

    char buf[256];
    if (!lire_ligne("Prémisse à supprimer (texte exact): ", buf, sizeof(buf))) return;
    if (buf[0] == '\0') return;

    // Suppression de la prémisse dans la règle
    if (regle_supprimer_premisse(&node->regle, buf))
        printf("Prémisse supprimée.\n");
    else
        printf("Prémisse introuvable.\n");
}

/*
 * ------------------------------------------------------------
 * Fonction : main
 * ------------------------------------------------------------
 * Rôle :
 *  Point d’entrée du programme. Initialise les structures
 *  principales, gère la boucle interactive et appelle
 *  le moteur d’inférence selon les choix de l’utilisateur.
 *
 * Paramètres :
 *  - Aucun
 *
 * Valeur de retour :
 *  - 0 à la fin normale du programme
 *
 * Variables principales :
 *  - BC : base de connaissances (règles)
 *  - BF : base de faits
 *  - ht : table de hachage utilisée pour optimiser l’inférence
 */
int main(void) {
    BaseConnaissances BC;
    bc_init(&BC);

    BaseFaits BF;
    liste_init(&BF);

    // Initialisation de la table de hachage pour les faits
    HashTable ht;
    hash_table_init(&ht);

    // Boucle principale du menu interactif
    for (;;) {
        menu_afficher();
        int choix;

        if (!lire_entier("> ", &choix)) {
            printf("Entrée invalide.\n");
            continue;
        }

        switch (choix) {
            case 1: ajouter_regle(&BC); break;
            case 2: ajouter_fait(&BF); break;

            case 3:
                if (bc_est_vide(&BC)) {
                    printf("BC vide.\n");
                    pause_console();
                    break;
                }
                moteur_inference(&BC, &BF, &ht);
                printf("Inférence terminée.\n");
                pause_console();
                break;

            case 4:
                printf("=== Règles ===\n");
                if (bc_est_vide(&BC)) printf("(aucune règle)\n");
                else bc_afficher(&BC);
                pause_console();
                break;

            case 5:
                printf("=== Faits ===\n");
                if (liste_est_vide(&BF)) printf("(aucun fait)\n");
                else liste_afficher(&BF, "- ");
                pause_console();
                break;

            case 6: supprimer_regle(&BC); break;
            case 7: supprimer_fait(&BF); break;

            case 8:
                bc_vider(&BC);
                printf("Toutes les règles supprimées.\n");
                break;

            case 9:
                liste_vider(&BF);
                hash_table_clear(&ht);
                hash_table_init(&ht);
                printf("Tous les faits supprimés.\n");
                break;

            case 10: supprimer_premisse(&BC); break;
            case 11:
                phase_tests();
                break;
            case 0:
                bc_vider(&BC);
                liste_vider(&BF);
                hash_table_clear(&ht);
                printf("Bye.\n");
                return 0;

            default:
                printf("Choix invalide.\n");
        }
    }
}
