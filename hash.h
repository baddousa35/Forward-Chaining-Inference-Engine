#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>

#define HASH_SIZE 1000

typedef struct HashNode {
    char *proposition;
    struct HashNode *next;
} HashNode;

typedef struct HashTable {
    HashNode *table[HASH_SIZE];
} HashTable;

void hash_table_init(HashTable *ht);
void hash_table_insert(HashTable *ht, const char *proposition);
bool hash_table_contains(const HashTable *ht, const char *proposition);
void hash_table_clear(HashTable *ht);

#endif

