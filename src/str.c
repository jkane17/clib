
/*
    File Type   : C Source
    Description : String Operations
*/

#include "str.h"

char *str_join(char *l, char *r) {
    size_t len = strlen(l) + strlen(r);
    char *s = (char *) mem_alloc(len * sizeof(char));
    strcpy(s, l);
    strcat(s, r);
    return s;
}
