
/*
    File Type   : C Header
    Description : Memory Allocator
*/

#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED

#include <assert.h>
#include <stdlib.h>

#include "math.h"
#include "mem.h"

// TODO - ALLOC_STRAT_CHUNKS - Fixed size chunks allocated/deallocated when needed/not needed
typedef enum {
    ALLOC_STRAT_DYNAMIC,
    ALLOC_STRAT_BUDDY
} AllocStrategy;

typedef struct {
    void *block;
    size_t avail, used, total;
    AllocStrategy strat;
} AllocBlock;

/**
 * @brief Append an item into the AllocBlock.
 * 
 * @param b AllocBlock object.
 * @param item Pointer to item (item is copied).
 * @param itemSize Size of an single item (bytes).
 */
void alloc_append(AllocBlock *b, void *item, size_t itemSize);

/**
 * @brief Free AllocBlock object.
 * 
 * @param b AllocBlock object.
 */
void alloc_free(AllocBlock *b);

/**
 * @brief Available memory of the AllocBlock.
 * 
 * @param b AllocBlock object.
 * @return Available memory (bytes).
 */
size_t alloc_getAvail(AllocBlock *b);

/**
 * @brief Pointer to actual memory block in the AllocBlock.
 * 
 * @param  b AllocBlock object.
 * @return void* Pointer to actual memory block.
 */
void *alloc_getBlock(AllocBlock *b);

/**
 * @brief Size of actual memory block in the AllocBlock.
 * 
 * @param  b AllocBlock object.
 * @return Size of actual memory block (bytes).
 */
size_t alloc_getSize(AllocBlock *b);

/**
 * @brief Allocation strategy used by the AllocBlock.
 * 
 * @param b AllocBlock object.
 * @return Allocation strategy.
 */
AllocStrategy alloc_getStrat(AllocBlock *b);

/**
 * @brief Used memory of the AllocBlock.
 * 
 * @param b AllocBlock object.
 * @return Used memory (bytes).
 */
size_t alloc_getUsed(AllocBlock *b);

/**
 * @brief Index into AllocBlock.
 * 
 * @param b AllocBlock object.
 * @param idx Item index in AllocBlock.
 * @param itemSize Size of an single item (bytes).
 * @return Pointer to item at given index.
 */
void *alloc_index(AllocBlock *b, size_t idx, size_t itemSize);

/**
 * @brief Create a new AllocBlock.
 * 
 * @param size Initial size of block (bytes).
 * @param strat Allocation strategy.
 * @return AllocBlock object.
 */
AllocBlock *alloc_new(size_t size, AllocStrategy strat);

/**
 * @brief Resize a memory block. 
 * 
 * @param b AllocBlock object.
 * @param size New size (bytes). This is the required size, but actual size could be larger 
 * depending on allocation strategy.
 */
void alloc_resize(AllocBlock *b, size_t size);

/**
 * @brief Set the Allocation strategy.
 * 
 * @param b AllocBlock object. 
 * @param strat New allocation strategy.
 */
void alloc_setStrat(AllocBlock *b, AllocStrategy strat);

#endif
