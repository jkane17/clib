
/*
    File Type   : C Header
    Description : Memory Operations
*/

#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "math.h"

/**
 * @brief Allocate [size] bytes of memory.
 * 
 * @param size Number of bytes to allocate.
 * @return Pointer to allocated memory address (if successful).
 */
void *mem_alloc(size_t size);

/**
 * @brief Re-allocate the previously allocated block in [p], making the new block [size] bytes long.
 * 
 * @param ptr Pointer to memory address to be reallocated.
 * @param size Number of bytes to allocate.
 * @return Pointer to reallocated memory address (if successful).
 */
void *mem_realloc(void *p, size_t size);

/**
 * @brief Shift items of an array n spaces to the left (-ve n) or right (+ve n).
 * 
 * @param arr Pointer to array
 * @param len Total number of items in array
 * @param itemSize Size (in bytes) of each individual item in array
 * @param idx Index at which the shift operation occurs
 * @param n Number of places to shift left (-ve n) or right (+ve n)
*/
void mem_shift(void *arr, size_t len, size_t itemSize, size_t idx, int n);

#endif
