#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>

bool lire_ligne(const char *prompt, char *buf, size_t cap);
bool lire_entier(const char *prompt, int *out);
void pause_console(void);

#endif
