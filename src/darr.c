
/*
    File Type   : C Source
    Description : Dynamic array.

    TODO :

    - Should be allowed to insert anywhere within total memory in alloc
    - Append should append to end of total, always increasing the size
    - test_darr_insert
    - rows_insert
*/

#include "darr.h"

void darr_free(DArr *d) { if (d != NULL) { alloc_free(d->block); free(d); } }

void *darr_index(DArr *d, size_t idx) { 
    return (darr_isEmpty(d) || idx >= darr_len(d)) ? 
        NULL : 
        alloc_index(d->block, idx, d->itemSize); 
}

void darr_insert(DArr *d, void *item, size_t idx) {
    size_t len = darr_len(d);
    if (d == NULL || idx > len) return;
    alloc_insert(d->block, item, idx, d->itemSize);
    d->len++;
}

bool darr_isEmpty(DArr *d) { return darr_len(d) == 0; }

size_t darr_len(DArr *d) { return (d == NULL) ? 0 : d->len; }

DArr *darr_new(size_t size, size_t itemSize, AllocStrategy strat) {
    DArr *d = (DArr*)mem_alloc(sizeof(DArr));
    d->block = alloc_new(size, strat);
    d->itemSize = itemSize;
    d->len = 0;
    return d;
}
