
/*
    File        : alloc.h
    Description : Dynamic Memory Allocator Library for managing and resizing 
                  memory blocks, with support for different allocation strategies.
*/

#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "math.h"

// TODO - ALLOC_STRAT_CHUNKS - Fixed size chunks allocated/deallocated when needed/not needed
typedef enum {
    _ALLOC_STRAT_MIN,
    ALLOC_STRAT_DYNAMIC,
    ALLOC_STRAT_BUDDY,
    _ALLOC_STRAT_MAX
} AllocStrategy;

typedef struct {
    void *block;
    size_t used, total;
    AllocStrategy strat;
} AllocBlock;

/**
 * @brief Append data into the AllocBlock at the end.
 * 
 * @param b AllocBlock object.
 * @param data Pointer to data (will be copied).
 * @param size Size (bytes) of data.
 * @return true if append succeeded, false otherwise.
 */
bool alloc_append(AllocBlock *b, const void *data, size_t size);

/**
 * @brief Clears all data in the AllocBlock but keeps the allocated memory.
 * 
 * @param b AllocBlock object.
 */
void alloc_clear(AllocBlock *b);

/**
 * @brief Creates a copy of an AllocBlock, including its data.
 * 
 * @param b AllocBlock object.
 * @return Copy of AllocBlock.
 */
AllocBlock *alloc_copy(const AllocBlock *b);

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
 * @return Available memory (bytes). Zero if NULL block.
 */
size_t alloc_getAvail(const AllocBlock *b);

/**
 * @brief Pointer to actual memory block in the AllocBlock.
 * 
 * @param  b AllocBlock object.
 * @return void* Pointer to actual memory block. NULL if NULL block.
 */
void *alloc_getBlock(const AllocBlock *b);

/**
 * @brief Size of actual memory block in the AllocBlock.
 * 
 * @param b AllocBlock object.
 * @return Size of actual memory block (bytes). Zero if NULL block.
 */
size_t alloc_getSize(const AllocBlock *b);

/**
 * @brief Allocation strategy used by the AllocBlock. ALLOC_STRAT_DYNAMIC if NULL block.
 * 
 * @param b AllocBlock object.
 * @return Allocation strategy.
 */
AllocStrategy alloc_getStrat(const AllocBlock *b);

/**
 * @brief Used memory of the AllocBlock. Zero if NULL block.
 * 
 * @param b AllocBlock object.
 * @return Used memory (bytes).
 */
size_t alloc_getUsed(const AllocBlock *b);

/**
 * @brief Index into AllocBlock.
 * 
 * @param b AllocBlock object.
 * @param byteIdx Index (byte position) to index at.
 * @return Pointer to item at given index.
 */
void *alloc_index(AllocBlock *b, size_t byteIdx);

/**
 * @brief Insert data into the AllocBlock at the given index.
 * 
 * @param b AllocBlock object.
 * @param data Pointer to data (will be copied).
 * @param byteIdx Index (byte position) to insert at.
 * @param size Size (bytes) of data.
 * @return true if insert succeeded, false otherwise.
 */
bool alloc_insert(AllocBlock *b, const void *data, size_t byteIdx, size_t size);

/**
 * @brief Check if the AllocBlock is empty.
 * 
 * @param b AllocBlock object.
 * @return true if there no used memory, false otherwise.
 */
bool alloc_isEmpty(const AllocBlock *b);

/**
 * @brief Create a new AllocBlock.
 * 
 * @param size Initial size of block (bytes).
 * @param strat Allocation strategy.
 * @return AllocBlock object (or NULL if failure).
 */
AllocBlock *alloc_new(size_t size, AllocStrategy strat);

/**
 * @brief Removes data from an AllocBlock.
 * 
 * @param b AllocBlock object.
 * @param byteIdx Index (byte position) to remove at.
 * @param size Size (bytes) of data.
 * @return true if remove succeeded, false otherwise.
 */
bool alloc_remove(AllocBlock *b, size_t byteIdx, size_t size);

/**
 * @brief Resize a memory block. 
 * 
 * @param b AllocBlock object.
 * @param size New size (bytes). This is the required size, but actual size could be larger 
 * depending on allocation strategy.
 * @return true if resize succeeded, false otherwise.
 */
bool alloc_resize(AllocBlock *b, size_t size);

/**
 * @brief Set data directly into the AllocBlock at a specified index (overwrites).
 * 
 * @param b AllocBlock object.
 * @param data Pointer to data (will be copied).
 * @param byteIdx Index (byte position) to set at.
 * @param size Size (bytes) of data.
 * @return true if set succeeded, false otherwise.
 */
bool alloc_setAt(AllocBlock *b, const void *data, size_t byteIdx, size_t size);

/**
 * @brief Set the Allocation strategy.
 * 
 * @param b AllocBlock object. 
 * @param strat New allocation strategy.
 */
void alloc_setStrat(AllocBlock *b, AllocStrategy strat);

/**
 * @brief Splits an AllocBlock into two separate blocks. Original block (b) is free'd.
 * 
 * @param b AllocBlock object that will be split. 
 * @param lb Address to store the left AllocBlock in.
 * @param rb Address to store the right AllocBlock in.
 * @param byteIdx Index (byte position) to split at.
 * @return true if split succeeded, false otherwise.
 */
bool alloc_split(AllocBlock *b, AllocBlock **lb, AllocBlock **rb, size_t byteIdx);

#endif // ALLOC_H_INCLUDED
