
/*
    File        : mem.h
    Description : Dynamic memory management operations.
*/

#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Allocates a block of memory of the given size.
 *
 * @param size The number of bytes to allocate. If 0, NULL is returned.
 * @return A pointer to the allocated memory block, or NULL if `size` is 0.
 *         Exits the program if allocation fails.
 */
void *mem_alloc(size_t size);

/**
 * @brief Allocates memory for an array of elements and initialises them to zero.
 *
 * @param num The number of elements to allocate. If 0, NULL is returned.
 * @param size The size of each element in bytes. If 0, NULL is returned.
 * @return A pointer to the allocated memory block, or NULL if `num` or `size` is 0.
 *         Exits the program if allocation fails.
 */
void *mem_calloc(size_t num, size_t size);

/**
 * @brief Resizes a previously allocated memory block.
 *
 * @param p A pointer to the previously allocated memory block, or NULL.
 * @param size The new size in bytes. If 0, the memory is freed.
 * @return A pointer to the reallocated memory block, or NULL if size is 0.
 *         Exits the program if reallocation fails (when size is non-zero).
 */
void *mem_realloc(void *p, size_t size);

#endif // MEM_H_INCLUDED
