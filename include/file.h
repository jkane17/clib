
/*
    File Type   : C Header
    Description : File Operations
*/

#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "alloc.h"
#include "mem.h"

#define BUFF_SIZE 1024

/**
 * @brief Open a file. Exit program on failure.
 * 
 * @param path  : Path to file
 * @param modes : File access mode(s)
 * @return FILE* : File pointer
 */
FILE *file_open(const char *path, const char *modes);

/**
 * @brief Read a file
 * 
 * @param path : Path to file
 * @return char* : File text
 */
char *file_read(char *path);

#endif
