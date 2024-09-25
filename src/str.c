
/*
    File        : str.c
    Description : String operations.
*/

#include "str.h"

char *str_join(const char *l, const char *r) {
    if (l == NULL || r == NULL) return NULL;

    size_t len = strlen(l) + strlen(r) + 1; // +1 for the null terminator

    char *s = (char*)malloc(len * sizeof(char));
    if (s == NULL) return NULL;

    strcpy(s, l);
    strcat(s, r);
    return s;
}
