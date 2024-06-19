
/*
    File Type   : C Source
    Description : Memory Operations
*/

#include "mem.h"

void *mem_alloc(size_t size) {
    void *p = malloc(size);
    assert(p && "Memory allocation failed");
    return p;
}

void *mem_realloc(void *p, size_t size) {
    void *newp = realloc(p, size);
    if(size) assert(newp && "Memory reallocation failed");
    return newp;
}

void mem_shift(void *arr, size_t len, size_t itemSize, size_t idx, int n) {
    if (
        n == 0 || 
        idx >= len ||
        (n > 0 && (size_t)n > (len - idx) - 1) || 
        (n < 0 && idx < (size_t)math_abs(n)) 
    ) return;
    size_t byteOffset = idx * itemSize;
    memmove(
        (char *) arr + byteOffset + n * itemSize, 
        (char *) arr + byteOffset, 
        itemSize * (len - idx) - (math_abs(n) - 1)
    );
}
