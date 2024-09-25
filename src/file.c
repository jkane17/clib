
/*
    File Type   : C Source
    Description : File Operations
*/

#include "file.h"

bool file_create(const char *path, const char *content) {
    // Check if the file already exists
    FILE *checkFile = fopen(path, "r");
    if (checkFile != NULL) { fclose(checkFile); return false; }

    FILE *file = fopen(path, "w");
    if (file == NULL) return false;

    if (fprintf(file, "%s", content) < 0) { fclose(file); return false; }

    fclose(file);
    return true;
}

bool file_delete(const char *path) {
    if (!(remove(path) == 0 || errno == ENOENT)) {
        fprintf(stderr, "Error: Failed to delete file '%s': %s\n", path, strerror(errno));
        return false;
    }
    return true;
}

FILE *file_open(const char *path, const char *modes) {
    FILE *f = fopen(path, modes);
    if (f == NULL)
        fprintf(stderr, "Error: Failed to open file '%s': %s\n", path, strerror(errno));
    return f;
}

char *file_read(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) return NULL; 

    AllocBlock *block = alloc_new(_BUFF_SIZE, _ALLOC_STRAT);
    if (block == NULL) { fclose(f); return NULL; }

    size_t buffSize = 0;
    ssize_t lineLen = 0;
    char *line = NULL;

    while ((lineLen = getline(&line, &buffSize, f)) != -1) 
        alloc_append(block, line, lineLen * sizeof(char));

    free(line);
    fclose(f);
    
    char *text = (char*)alloc_getBlock(block);
    free(block);

    return text;
}
