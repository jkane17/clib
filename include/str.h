
/*
    File Type   : C Header
    Description : String Operations
*/

#ifndef STR_H_INCLUDED
#define STR_H_INCLUDED

#include <string.h>

#include "mem.h"

/**
 * @brief Join two strings together. Allocates space for a string that the user 
 * must free. 
 * 
 * @param s1 : Left string
 * @param s2 : Right String
 * @return char* : Joined string
 */
char *str_join(char *s1, char *s2);

#endif
