
/*
    File Type   : C Header
    Description : String Operations
*/

#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <string.h>

#include "mem.h"

/**
 * @brief Join two strings together. Allocates space for a string that the user must free. 
 * 
 * @param l Left string.
 * @param r Right String.
 * @return Joined string.
 */
char *str_join(char *l, char *r);

#endif
