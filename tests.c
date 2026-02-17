#include "tests.h"
#include "list.h"
#include "rule.h"
#include "hash.h"
#include "kb.h"
#include "inference.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

/*
 * ------------------------------------------------------------
 * Variable globale : tests_echoues
 * ------------------------------------------------------------
 * Rôle :
 *  Compte le nombre total de tests échoués durant la phase
 *  de tests. Cette variable est remise à zéro au début
 *  de chaque phase de tests.
 */
static int tests_echoues = 0;

/*
 * ------------------------------------------------------------
 * Fonction : test_result
 * ------------------------------------------------------------
 * Rôle :
 *  Affiche le résultat d’un test unitaire sous une forme
 *  standardisée et met à jour le compteur d’échecs.
 *
 * Paramètres :
 *  - nom : nom descriptif du test
 *  - ok  : résultat du test (true si réussi, false sinon)
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
void test_result(const char *nom, bool ok) {
    if (ok) {
        printf("[TEST] %-40s OK\n", nom);
    } else {
        printf("[TEST] %-40s FAIL\n", nom);
        tests_echoues++;
    }
}

/*
 * ------------------------------------------------------------
 * Fonction : tests_liste
 * ------------------------------------------------------------
 * Rôle :
 *  Teste l’ensemble des fonctionnalités principales du
 *  module liste :
 *   - initialisation
 *   - ajout d’éléments
 *   - recherche
 *   - suppression
 *   - vidage
 *
 * Paramètres :
 *  - Aucun
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Structures testées :
 *  - Liste
 */
void tests_liste(void) {
    printf("\n--- Tests LISTE ---\n");

    // Initialisation de la liste
    Liste L;
    liste_init(&L);

    // Tests sur l’état initial
    test_result("liste_init -> vide", liste_est_vide(&L));
    test_result("liste_init -> taille = 0", L.size == 0);

    // Ajout de deux éléments
    liste_ajouter_en_queue(&L, "A");
    liste_ajouter_en_queue(&L, "B");

    // Vérifications après ajout
    test_result("ajout -> non vide", !liste_est_vide(&L));
    test_result("ajout -> taille = 2", L.size == 2);
    test_result("ajout -> tete = A", strcmp(liste_tete(&L), "A") == 0);

    // Recherche d’éléments
    test_result("contient A", liste_contient_rec(&L, "A"));
    test_result("contient Z (absent)", !liste_contient_rec(&L, "Z"));

    // Suppression d’un élément
    test_result("suppression A", liste_supprimer_premiere(&L, "A"));
    test_result("suppression -> taille = 1", L.size == 1);

    // Vidage complet de la liste
    liste_vider(&L);
    test_result("vider -> liste vide", liste_est_vide(&L));
}

/*
 * ------------------------------------------------------------
 * Fonction : tests_regle
 * ------------------------------------------------------------
 * Rôle :
 *  Vérifie le bon fonctionnement du module règle :
 *   - initialisation
 *   - gestion des prémisses
 *   - définition de la conclusion
 *   - suppression de prémisses
 *
 * Paramètres :
 *  - Aucun
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Structures testées :
 *  - Regle
 */
void tests_regle(void) {
    printf("\n--- Tests REGLE ---\n");

    // Initialisation de la règle
    Regle R;
    regle_init(&R);

    // Tests de l’état initial
    test_result("init -> premisses vides", regle_premisses_vide(&R));
    test_result("init -> conclusion NULL", regle_obtenir_conclusion(&R) == NULL);

    // Ajout de prémisses
    regle_ajouter_premisse(&R, "A");
    regle_ajouter_premisse(&R, "B");

    test_result("ajout premisses", !regle_premisses_vide(&R));

    // Définition de la conclusion
    regle_definir_conclusion(&R, "C");
    test_result("definir conclusion", strcmp(regle_obtenir_conclusion(&R), "C") == 0);

    // Suppression de prémisses
    test_result("supprimer premisse B", regle_supprimer_premisse(&R, "B"));
    test_result("supprimer premisse Z (absent)", !regle_supprimer_premisse(&R, "Z"));

    // Libération des ressources
    regle_detruire(&R);
}

/*
 * ------------------------------------------------------------
 * Fonction : tests_hash
 * ------------------------------------------------------------
 * Rôle :
 *  Teste le module table de hachage :
 *   - initialisation
 *   - insertion
 *   - recherche
 *   - nettoyage
 *
 * Paramètres :
 *  - Aucun
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Structures testées :
 *  - HashTable
 */
void tests_hash(void) {
    printf("\n--- Tests HASH ---\n");

    // Initialisation de la table de hachage
    HashTable ht;
    hash_table_init(&ht);

    // Test sur table vide
    test_result("table vide -> absent", !hash_table_contains(&ht, "A"));

    // Insertion d’éléments
    hash_table_insert(&ht, "A");
    hash_table_insert(&ht, "B");

    // Recherche d’éléments
    test_result("contient A", hash_table_contains(&ht, "A"));
    test_result("contient B", hash_table_contains(&ht, "B"));
    test_result("absent C", !hash_table_contains(&ht, "C"));

    // Nettoyage de la table
    hash_table_clear(&ht);
    test_result("clear -> A absent", !hash_table_contains(&ht, "A"));
}

/*
 * ------------------------------------------------------------
 * Fonction : tests_inference
 * ------------------------------------------------------------
 * Rôle :
 *  Vérifie le bon fonctionnement du moteur d’inférence :
 *   - application d’une règle simple
 *   - déduction correcte d’un nouveau fait
 *
 * Paramètres :
 *  - Aucun
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Modules testés :
 *  - Base de connaissances
 *  - Base de faits
 *  - Moteur d’inférence
 */
void tests_inference(void) {
    printf("\n--- Tests INFERENCE ---\n");

    // Initialisation des structures
    BaseConnaissances BC;
    BaseFaits BF;
    HashTable ht;

    bc_init(&BC);
    liste_init(&BF);
    hash_table_init(&ht);

    // Création d’une règle : IF A THEN B
    Regle R;
    regle_init(&R);
    regle_ajouter_premisse(&R, "A");
    regle_definir_conclusion(&R, "B");
    bc_ajouter_regle_en_queue(&BC, &R);
    regle_detruire(&R);

    // Ajout du fait initial A
    liste_ajouter_en_queue(&BF, "A");

    // Lancement du moteur d’inférence
    moteur_inference(&BC, &BF, &ht);

    // Vérification de la déduction
    test_result("inference -> B deduit", liste_contient_rec(&BF, "B"));

    // Nettoyage des structures
    bc_vider(&BC);
    liste_vider(&BF);
    hash_table_clear(&ht);
}

/*
 * ------------------------------------------------------------
 * Fonction : phase_tests
 * ------------------------------------------------------------
 * Rôle :
 *  Lance l’ensemble des tests unitaires du projet,
 *  affiche les résultats détaillés et fournit un
 *  résumé global du nombre de tests échoués.
 *
 * Paramètres :
 *  - Aucun
 *
 * Valeur de retour :
 *  - Aucune (void)
 */
void phase_tests(void) {
    // Réinitialisation du compteur d’échecs
    tests_echoues = 0;

    printf("\n=== PHASE DE TESTS ===\n");

    // Lancement des tests par module
    tests_liste();
    tests_regle();
    tests_hash();
    tests_inference();

    // Résumé final
    printf("\n=== FIN DES TESTS ===\n");
    printf("Tests echoues : %d\n", tests_echoues);

    // Pause pour permettre la lecture des résultats
    pause_console();
}
