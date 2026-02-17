 #ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ListNode {
    char *s;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    ListNode *tail;
    size_t size;
} Liste;

void liste_init(Liste *L);
bool liste_est_vide(const Liste *L);

void liste_ajouter_en_queue(Liste *L, const char *s);
bool liste_contient_rec(const Liste *L, const char *s);
bool liste_supprimer_premiere(Liste *L, const char *s);

void liste_vider(Liste *L);

const char *liste_tete(const Liste *L);

void liste_afficher(const Liste *L, const char *prefix);

#endif
