
/*
    File        : alloc.c
    Description : Dynamic Memory Allocator Library for managing and resizing 
                  memory blocks, with support for different allocation strategies.
*/

#include "alloc.h"

/**
 * @brief Convert a size to its nearest (ceiling) power of 2 value.
 * 
 * @param size Memory block size to convert (bytes).
 * @return Converted memory block size (bytes).
 */
static size_t _buddySize(size_t size) {
    if (size == 0) return 0;
    size_t newSize = 1;
    while (newSize < size) newSize *= 2;
    return newSize;
}

/**
 * @brief Convert a size based on the allocation strategy.
 * 
 * @param strat Allocation strategy.
 * @param size Memory block size to convert (bytes).
 * @return Converted memory block size (bytes).
 */
static size_t _convertSize(AllocStrategy strat, size_t size) {
    if (size == 0) return 0;
    switch (strat) {
        case ALLOC_STRAT_BUDDY:
            return _buddySize(size);
        default:
            return size;
    }
}

bool alloc_append(AllocBlock *b, const void *data, size_t size) {
    return alloc_insert(b, data, b->used, size);
}

void alloc_clear(AllocBlock *b) { if (b != NULL) b->used = 0; }

AllocBlock *alloc_copy(const AllocBlock *b) {
    if (b == NULL) return NULL;

    AllocBlock *copy = alloc_new(b->total, b->strat);

    if (b->used > 0 && b->block != NULL) {
        memcpy(copy->block, b->block, b->used);
        copy->used = b->used;
    }

    return copy;
}

void alloc_free(AllocBlock *b) { 
    if (b != NULL) { 
        if (b->block != NULL) {
            free(b->block);
            b->block = NULL; 
        }
        free(b); 
    } 
}

size_t alloc_getAvail(const AllocBlock *b) { return b ? b->total - b->used : 0; }

void *alloc_getBlock(const AllocBlock *b) { return b ? b->block : NULL; }

size_t alloc_getSize(const AllocBlock *b) { return b ? b->total : 0; }

AllocStrategy alloc_getStrat(const AllocBlock *b) { return b ? b->strat : ALLOC_STRAT_DYNAMIC; }

size_t alloc_getUsed(const AllocBlock *b) { return b ? b->used : 0; }

void *alloc_index(AllocBlock *b, size_t byteIdx) {
    if (b == NULL || byteIdx >= b->used) return NULL;
    return (char*)b->block + byteIdx;
}

bool alloc_insert(AllocBlock *b, const void *data, size_t byteIdx, size_t size) {
    if (b == NULL || data == NULL || size == 0) return false;
    
    if (byteIdx > b->used) return false; // Index out of bounds
    
    // Resize the block if necessary
    size_t newSize = b->used + size;
    if (newSize > b->total) 
        if (!alloc_resize(b, newSize)) return false;
    
    // Shift memory to make space for the new items, if necessary
    if (byteIdx < b->used) 
        memmove(
            (char*)b->block + byteIdx + size, 
            (char*)b->block + byteIdx, 
            b->used - byteIdx
        );
    
    memcpy((char*)b->block + byteIdx, data, size);
    b->used = newSize;

    return true;
}

bool alloc_isEmpty(const AllocBlock *b) { return b->used == 0; }

AllocBlock *alloc_new(size_t size, AllocStrategy strat) {
    AllocBlock *b = (AllocBlock*)malloc(sizeof(AllocBlock));
    if (b == NULL) return NULL;
    
    b->block = size ? malloc(size) : NULL;
    if (size != 0 && b->block == NULL) {
        b->block = malloc(size);
        if (b->block == NULL) { free(b); return NULL; }
    }
    
    b->used = 0;
    b->total = size;
    alloc_setStrat(b, strat);
    
    return b;
}

bool alloc_remove(AllocBlock *b, size_t byteIdx, size_t size) {
    if (b == NULL || size == 0) return false;

    size_t endByte = byteIdx + size;
    if (byteIdx >= b->used || endByte > b->used) return false; // Out of bounds

    // Shift remaining items to fill the gap
    memmove((char*)b->block + byteIdx, (char*)b->block + endByte, b->used - endByte);

    b->used -= size;
    alloc_resize(b, b->used);

    return true;
}

bool alloc_resize(AllocBlock *b, size_t size) {
    if (b == NULL) return false;

    size = _convertSize(b->strat, size);
    if(size == b->total) return true;

    if (size == 0 && b->block != NULL) {
        free(b->block);
        b->block = NULL;
    } else if (size > 0) {
        void *new_block = realloc(b->block, size);
        if (new_block == NULL) return false;
        b->block = new_block;
    }

    b->total = size;
    b->used = math_min(b->total, b->used);

    return true;
}

bool alloc_setAt(AllocBlock *b, const void *data, size_t byteIdx, size_t size) {
    if (b == NULL || data == NULL || size == 0) return false;

    if (byteIdx > b->used) return false; // Index out of bounds

    // Resize the block if necessary
    size_t newSize = byteIdx + size;
    if (newSize > b->total) alloc_resize(b, newSize);

    memcpy((char*)b->block + byteIdx, data, size);

    // Update used size if it was expanded
    if (newSize > b->used) b->used = newSize;

    return true;
}

void alloc_setStrat(AllocBlock *b, AllocStrategy strat) {
    if (b == NULL || strat <= _ALLOC_STRAT_MIN || _ALLOC_STRAT_MAX <= strat) return;
    b->strat = strat;
    alloc_resize(b, b->total);
}

bool alloc_split(AllocBlock *b, AllocBlock **lb, AllocBlock **rb, size_t byteIdx) {
    if (b == NULL || lb == NULL || rb == NULL || byteIdx > b->used) return false;

    size_t leftUsed = byteIdx;
    size_t rightUsed = b->used - byteIdx;

    *lb = alloc_new(leftUsed, b->strat);
    *rb = alloc_new(rightUsed, b->strat);

    if (leftUsed > 0) {
        if (*lb == NULL) return false;
        if (!alloc_append(*lb, b->block, leftUsed)) return false;
    }

    if (rightUsed > 0) {
        if (*rb == NULL) return false;
        if (!alloc_append(*rb, (char *)b->block + byteIdx, rightUsed)) return false;
    }

    alloc_free(b);

    return true;
}
