
/*
    File Type   : C Source
    Description : File Operations
*/

#include "file.h"

FILE *file_open(const char *path, const char *modes) {
    FILE *f = fopen(path, modes);
    assert(f && "File open failed");
    return f;
}

char *file_read(char *path) {
    FILE *f = file_open(path, "r");

    AllocBlock *block = alloc_new(BUFF_SIZE, ALLOC_STRAT_BUDDY);
    size_t buffSize = 0;
    int lineLen = 0;
    char *line = NULL;

    while ((lineLen = getline(&line, &buffSize, f)) != -1) {
        alloc_append(block, line, lineLen * sizeof(char));
        free(line);
        line = NULL;
        buffSize = 0;
    }

    fclose(f);
    char *text = (char*) alloc_getBlock(block);
    free(block);

    return text;
}
