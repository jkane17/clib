
/*
    File        : mem.c
    Description : Dynamic memory management operations.
*/

#include "mem.h"

void *mem_alloc(size_t size) {
    if (size == 0) return NULL;
    void *p = malloc(size);
    if (p == NULL) {
        fprintf(stderr, "Error: Memory allocation of %zu bytes failed.\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *mem_calloc(size_t num, size_t size) {
    if (num == 0 || size == 0) return NULL;
    void *p = calloc(num, size);
    if (p == NULL) {
        fprintf(stderr, "Error: Memory allocation for %zu elements of %zu bytes each failed.\n", num, size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *mem_realloc(void *p, size_t size) {
    if (size == 0) { if (p != NULL) free(p); return NULL; }
    void *np = realloc(p, size);
    if (np == NULL) {
        fprintf(stderr, "Error: Memory reallocation of %zu bytes failed.\n", size);
        exit(EXIT_FAILURE);
    }
    return np;
}
