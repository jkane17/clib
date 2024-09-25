
/*
    File        : str.h
    Description : String operations.
*/

#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <stdlib.h> 
#include <string.h>

#include "mem.h"

/**
 * @brief Join two strings together into a new dynamically allocated string.
 * 
 * @param l Left string (must be non-NULL).
 * @param r Right string (must be non-NULL).
 * @return Pointer to the newly allocated string containing the concatenation of `l` and `r`.
 *         Returns NULL if memory allocation fails.
 */
char *str_join(const char *l, const char *r);

#endif // STR_H_INCLUDED
