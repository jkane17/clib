
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
    
    size_t buffSize = 0, textLen = 0, textSize = BUFF_SIZE;
    int lineLen = 0;
    char *line = NULL, *text = mem_alloc(textSize);
    strcpy(text, "\0");

    while ((lineLen = getline(&line, &buffSize, f)) != -1) {
        textLen += lineLen;
        // TODO : Use alloc lib here
        if (textLen >= textSize) {
            textSize += BUFF_SIZE * (int)(1 + (textLen - textSize) / BUFF_SIZE); 
            text = mem_realloc(text, textSize);
        }
        strcat(text, line);
        free(line);
        line = NULL;
        buffSize = 0;
    }

    fclose(f);

    return text;
}
