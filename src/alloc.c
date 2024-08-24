
/*
    File Type   : C Source
    Description : Memory Allocator.
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

void alloc_append(AllocBlock *b, void *item, size_t itemSize) {
    alloc_insert(b, item, b->used / itemSize, itemSize);
} 

void alloc_free(AllocBlock *b) { free(b->block); free(b); }

size_t alloc_getAvail(AllocBlock *b) { return b ? b->avail : 0; }

void *alloc_getBlock(AllocBlock *b) { return b ? b->block : NULL; }

size_t alloc_getSize(AllocBlock *b) { return b ? b->total : 0; }

AllocStrategy alloc_getStrat(AllocBlock *b) {
    assert(b && "NULL AllocBlock");
    return b->strat;
}

size_t alloc_getUsed(AllocBlock *b) { return b ? b->used : 0; }

void *alloc_index(AllocBlock *b, size_t idx, size_t itemSize) {
    size_t byteOffset = idx * itemSize;
    if (b == NULL || byteOffset >= b->total || itemSize == 0) return NULL;
    return byteOffset + (char*)b->block;
}

void alloc_insert(AllocBlock *b, void *item, size_t idx, size_t itemSize) {
    size_t byteOffset = idx * itemSize;
    if (b == NULL || item == NULL || byteOffset > b->used || itemSize == 0) return;
    size_t newSize = b->used + itemSize, total = b->total;
    if (newSize > total) alloc_resize(b, newSize);
    if (byteOffset < total) mem_shift(b->block, b->total / itemSize, itemSize, idx, 1);
    memcpy(byteOffset + (char*)b->block, item, itemSize);
    b->used += itemSize;
    b->avail -= itemSize;
}

AllocBlock *alloc_new(size_t size, AllocStrategy strat) {
    AllocBlock *b = (AllocBlock*)mem_alloc(sizeof(AllocBlock));
    b->block = mem_alloc(size);
    b->avail = size;
    b->used = 0;
    b->total = size;
    alloc_setStrat(b, strat);
    return b;
}

void alloc_resize(AllocBlock *b, size_t size) {
    if (b == NULL) return;
    size = _convertSize(b->strat, size);
    if(size == b->total) return;

    b->block = mem_realloc(alloc_getBlock(b), size);
    if (size) assert(b && "Failed to resize AllocBlock");
    b->total  = size;
    b->used  = math_min(b->total, b->used);
    b->avail = size - b->used;
}

void alloc_setStrat(AllocBlock *b, AllocStrategy strat) {
    if (b) {
        b->strat = strat;
        alloc_resize(b, b->total);
    }
}
