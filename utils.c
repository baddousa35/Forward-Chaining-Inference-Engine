#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * ------------------------------------------------------------
 * Fonction : trim
 * ------------------------------------------------------------
 * Rôle :
 *  Supprime les espaces inutiles d’une chaîne de caractères :
 *  - espaces, retours chariot et sauts de ligne en fin de chaîne
 *  - espaces en début de chaîne
 *
 * Paramètres :
 *  - s : chaîne de caractères à nettoyer (modifiée sur place)
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - n : longueur de la chaîne
 *  - i : indice utilisé pour repérer le début du texte utile
 */
static void trim(char *s) {
    size_t n = strlen(s);

    // Suppression des espaces et retours à la ligne en fin de chaîne
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || isspace((unsigned char)s[n-1]))) {
        s[n-1] = '\0';
        n--;
    }

    // Recherche du premier caractère non blanc
    size_t i = 0;
    while (s[i] && isspace((unsigned char)s[i])) i++;

    // Décalage de la chaîne vers la gauche si nécessaire
    if (i > 0)
        memmove(s, s + i, strlen(s + i) + 1);
}

/*
 * ------------------------------------------------------------
 * Fonction : lire_ligne
 * ------------------------------------------------------------
 * Rôle :
 *  Lit une ligne de texte depuis l’entrée standard,
 *  affiche éventuellement un message d’invite, et nettoie
 *  la chaîne lue.
 *
 * Paramètres :
 *  - prompt : message affiché avant la saisie (peut être NULL)
 *  - buf    : tampon dans lequel stocker la ligne lue
 *  - cap    : capacité maximale du tampon
 *
 * Valeur de retour :
 *  - true  : lecture réussie
 *  - false : erreur de lecture (EOF ou autre)
 */
bool lire_ligne(const char *prompt, char *buf, size_t cap) {
    // Affichage du message d’invite si fourni
    if (prompt) printf("%s", prompt);

    // Lecture de la ligne
    if (!fgets(buf, (int)cap, stdin)) return false;

    // Nettoyage de la chaîne lue
    trim(buf);
    return true;
}

/*
 * ------------------------------------------------------------
 * Fonction : lire_entier
 * ------------------------------------------------------------
 * Rôle :
 *  Lit un entier depuis l’entrée standard en s’assurant
 *  que la saisie est valide.
 *
 * Paramètres :
 *  - prompt : message affiché avant la saisie
 *  - out    : pointeur vers l’entier de sortie
 *
 * Valeur de retour :
 *  - true  : entier correctement lu et converti
 *  - false : saisie invalide
 *
 * Variables locales :
 *  - buf : tampon de lecture
 *  - end : pointeur utilisé pour vérifier la conversion
 *  - v   : valeur entière lue sous forme longue
 */
bool lire_entier(const char *prompt, int *out) {
    char buf[64];

    // Lecture de la ligne contenant l’entier
    if (!lire_ligne(prompt, buf, sizeof(buf))) return false;
    if (buf[0] == '\0') return false;

    // Conversion de la chaîne en entier
    char *end = NULL;
    long v = strtol(buf, &end, 10);

    // Vérification de la validité de la conversion
    if (end == buf || *end != '\0') return false;

    *out = (int)v;
    return true;
}

/*
 * ------------------------------------------------------------
 * Fonction : pause_console
 * ------------------------------------------------------------
 * Rôle :
 *  Met le programme en pause jusqu’à ce que l’utilisateur
 *  appuie sur la touche Entrée.
 *
 * Paramètres :
 *  - Aucun
 *
 * Valeur de retour :
 *  - Aucune (void)
 *
 * Variables locales :
 *  - buf : tampon utilisé pour consommer l’entrée utilisateur
 */
void pause_console(void) {
    char buf[8];

    printf("\n(Appuyez sur Entrée pour continuer)");
    fflush(stdout);

    // Attente d’une entrée utilisateur
    fgets(buf, (int)sizeof(buf), stdin);
}
