
/*
    File        : darr.h
    Description : Dynamic Array Library for managing and resizing arrays of any single type.
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
 * @brief Append items into the DArr at the end.
 * 
 * @param d DArr object.
 * @param items Pointer to items (will be copied).
 * @param count Number of items to insert.
 * @return true if append succeeded, false otherwise.
 */
bool darr_append(DArr *d, const void *items, size_t count);

/**
 * @brief Clears all data in the DArr but keeps the allocated memory.
 * 
 * @param d DArr object.
 */
void darr_clear(DArr *d);

/**
 * @brief Creates a copy of a DArr, including its data.
 * 
 * @param d DArr object to copy.
 * @return A new DArr containing a copy of the items in the original DArr (NULL if failure).
 */
DArr *darr_copy(const DArr *d);

/**
 * @brief Expand a DArr. Size cannot be less that the current length of the array. 
 * 
 * @param d DArr object.
 * @param size New number of allocated item slots. This is the required size, but actual size could 
 * be larger depending on allocation strategy.
 * @return true if expand succeeded, false otherwise.
 */
bool darr_expand(DArr *d, size_t size);

/**
 * @brief Get a pointer to first item in DArr.
 * 
 * @param d DArr object.
 * @return Pointer to first item (NULL if it does not exist).
 */
void *darr_first(DArr *d);

/**
 * @brief Free DArr object. Does not free contained items.
 * 
 * @param d DArr object.
 */
void darr_free(DArr *d);

/**
 * @brief Get a pointer to an item in DArr at the given index.
 * 
 * @param d DArr object.
 * @param idx Index of item in DArr.
 * @return Pointer to desired item (NULL if it does not exist).
 */
void *darr_index(DArr *d, size_t idx);

/**
 * @brief Insert items into the DArr at the given index.
 * 
 * @param d DArr object.
 * @param items Pointer to items (will be copied).
 * @param idx Index to insert at.
 * @param count Number of items to insert.
 * @return true if insert succeeded, false otherwise.
 */
bool darr_insert(DArr *d, const void *items, size_t idx, size_t count);

/**
 * @brief Is the DArr object empty?
 * 
 * @param d DArr object.
 * @return true if the DArr contains no items, false otherwise.
 */
bool darr_isEmpty(DArr *d);

/**
 * @brief Return the itemSize of a single item in the DArr.
 * 
 * @param d DArr object.
 * @return itemSize of a single item.
 */
size_t darr_itemSize(DArr *d);

/**
 * @brief Get a pointer to last item in DArr.
 * 
 * @param d DArr object.
 * @return Pointer to last item (NULL if it does not exist).
 */
void *darr_last(DArr *d);

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
 * @return DArr object (or NULL if failure).
 */
DArr *darr_new(size_t size, size_t itemSize, AllocStrategy strat);

/**
 * @brief Remove items from the DArr at the given index.
 * 
 * @param d DArr object.
 * @param idx Index to remove items from.
 * @param count Number of items to remove.
 * @return true if remove succeeded, false otherwise.
 */
bool darr_remove(DArr *d, size_t idx, size_t count);

/**
 * @brief Resize a DArr. If size is less that the current array length, the array is truncated. 
 * 
 * @param d DArr object.
 * @param size New number of allocated item slots. This is the required size, but actual size could 
 * be larger depending on allocation strategy.
 * @return true if resize succeeded, false otherwise.
 */
bool darr_resize(DArr *d, size_t size);

/**
 * @brief Set items directly into the DArr at a specified index (overwrites).
 * 
 * @param d DArr object.
 * @param items Pointer to items (will be copied).
 * @param idx Index to insert at.
 * @param count Number of items to insert.
 * @return true if set succeeded, false otherwise.
 */
bool darr_setAt(DArr *d, const void *items, size_t idx, size_t count);

/**
 * @brief Return the size (number of total item slots) of the DArr.
 * 
 * @param d DArr object.
 * @return Size.
 */
size_t darr_size(DArr *d);

/**
 * @brief Splits a DArr into two separate DArrs. Original DArr (d) is free'd.
 * 
 * @param d DArr object that will be split. 
 * @param ld Address to store the left DArr in.
 * @param rd Address to store the right DArr in.
 * @param idx Index to split at.
 * @return true if split succeeded, false otherwise.
 */
bool darr_split(DArr *d, DArr **ld, DArr **rd, size_t idx);

#endif // DARR_H_INCLUDED
