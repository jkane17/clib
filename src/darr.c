
/*
    File        : darr.c
    Description : Dynamic Array Library for managing and resizing arrays of any single type.
*/

#include "darr.h"

bool darr_append(DArr *d, const void *items, size_t count) {
    return darr_insert(d, items, darr_len(d), count);
}

void darr_clear(DArr *d) { if (d != NULL) { alloc_clear(d->block); d->len = 0; }; }

DArr *darr_copy(const DArr *d) {
    if (d == NULL) return NULL;
    DArr *copy = darr_new(d->len, d->itemSize, alloc_getStrat(d->block));
    copy->block = alloc_copy(d->block);
    copy->len = d->len;
    return copy;
}

bool darr_expand(DArr *d, size_t size) {
    if (d == NULL || size < d->len) return false;
    return darr_resize(d, size);
}

void *darr_first(DArr *d) { return darr_index(d, 0); }

void darr_free(DArr *d) { if (d != NULL) { alloc_free(d->block); free(d); } }

void *darr_index(DArr *d, size_t idx) { 
    if (d == NULL || darr_isEmpty(d) || idx >= darr_len(d)) return NULL; 
    return alloc_index(d->block, idx * d->itemSize); 
}

bool darr_insert(DArr *d, const void *items, size_t idx, size_t count) {
    if (d == NULL || items == NULL || idx > darr_len(d) || count == 0) return false;
    if (!alloc_insert(d->block, items, idx * d->itemSize, count * d->itemSize)) 
        return false;
    d->len += count;
    return true;
}

bool darr_isEmpty(DArr *d) { return darr_len(d) == 0; }

size_t darr_itemSize(DArr *d) { return d ? d->itemSize : 0; }

void *darr_last(DArr *d) { return darr_index(d, darr_len(d) - 1); }

size_t darr_len(DArr *d) { return d ? d->len : 0; }

DArr *darr_new(size_t size, size_t itemSize, AllocStrategy strat) {
    if (itemSize == 0) return NULL;

    DArr *d = (DArr*)malloc(sizeof(DArr));
    if (d == NULL) return NULL;

    d->block = alloc_new(size * itemSize, strat);
    if (d->block == NULL) { free(d); return NULL; }

    d->itemSize = itemSize;
    d->len = 0;

    return d;
}

bool darr_remove(DArr *d, size_t idx, size_t count) {
    if (d == NULL || darr_len(d) <= idx || darr_len(d) < idx + count || count == 0 ) return false;
    if (false == alloc_remove(d->block, idx * d->itemSize, count * d->itemSize)) return false;
    d->len -= count;
    return true;
}

bool darr_resize(DArr *d, size_t size) {
    if (d == NULL) return false;
    if (size < d->len) d->len = size;
    return alloc_resize(d->block, size * d->itemSize);
}

bool darr_setAt(DArr *d, const void *items, size_t idx, size_t count) {
    if (d == NULL || items == NULL || idx + count > darr_len(d) || count == 0) return false;
    return alloc_setAt(d->block, items, idx * d->itemSize, count * d->itemSize);
}

size_t darr_size(DArr *d) { return d ? alloc_getSize(d->block) / d->itemSize : 0; }

bool darr_split(DArr *d, DArr **ld, DArr **rd, size_t idx) {
    if (d == NULL || ld == NULL || rd == NULL || idx > d->len) return false;

    *ld = darr_new(idx, d->itemSize, alloc_getStrat(d->block));
    *rd = darr_new(d->len - idx, d->itemSize, alloc_getStrat(d->block));

    if (*ld == NULL || *rd == NULL) {
        if (*ld) darr_free(*ld);
        if (*rd) darr_free(*rd);
        return false;
    }

    // Copy the data into the left DArr
    if (idx > 0) {
        if (!darr_append(*ld, alloc_getBlock(d->block), idx)) {
            darr_free(*ld);
            darr_free(*rd);
            return false;
        }
    }

    // Copy the data into the right DArr
    if (idx < d->len) {
        if (!darr_append(*rd, alloc_index(d->block, idx * d->itemSize), d->len - idx)) {
            darr_free(*ld);
            darr_free(*rd);
            return false;
        }
    }

    darr_free(d);

    return true;
}
