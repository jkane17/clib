
/*
    File Type   : C Header
    Description : Dynamic array.
*/

#ifndef DARR_H_INCLUDED
#define DARR_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

#include "alloc.h"

typedef struct { 
    AllocBlock *block;
    size_t itemSize, len;
} DArr;

/**
 * @brief Free DArr object. Does not free items themselves.
 * 
 * @param d DArr object.
 */
void darr_free(DArr *d);

/**
 * @brief Get pointer to item in DArr at the given index.
 * 
 * @param d DArr object.
 * @param idx Index of item in DArr.
 * @return Pointer to desired item (NULL if it does not exist).
 */
void *darr_index(DArr *d, size_t idx);

/**
 * @brief Is the DArr object empty?
 * 
 * @param d DArr object.
 * @return true or false.
 */
bool darr_isEmpty(DArr *d);

/**
 * @brief Return the number of items in the DArr.
 * 
 * @param d DArr object.
 * @return Number of items.
 */
size_t darr_len(DArr *d);

/**
 * @brief Create a new DArr object.
 * 
 * @param size Number of allocated item slots.
 * @param itemSize Size of a single item (bytes).
 * @param strat Allocation strategy.
 * @return DArr object.
 */
DArr *darr_new(size_t size, size_t itemSize, AllocStrategy strat);

#endif
