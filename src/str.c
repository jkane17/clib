
/*
    File Type   : C Source
    Description : String Operations
*/

#include "str.h"

char *str_join(char *s1, char *s2) {
    size_t len = strlen(s1) + strlen(s2);
    char *s = (char *) mem_alloc(len * sizeof(char));
    strcpy(s, s1);
    strcat(s, s2);
    return s;
}
