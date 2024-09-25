
/*
    File Type   : C Header
    Description : File Operations
*/

#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"

#define _ALLOC_STRAT ALLOC_STRAT_BUDDY
#define _BUFF_SIZE 1024

/**
 * @brief Create a file and write content to it if it does not already exist.
 * 
 * @param path Path to the file to be created.
 * @param content Content to write to the file.
 * @return true if the file was created and written to successfully, false otherwise.
 */
bool file_create(const char *path, const char *content);

/**
 * @brief Delete a file.
 * 
 * @param path Path to the file to be deleted.
 * @return true if the file was deleted successfully or did not exist, false otherwise.
 */
bool file_delete(const char *path);

/**
 * @brief Open a file with the specified mode. Exits the program if it fails to open a file.
 * 
 * @param path Path to the file to be opened. Must be a valid path string.
 * @param modes File access mode(s) in the form of a string (e.g., "r", "w", "rb").
 * @return Pointer to the opened file.
 */
FILE *file_open(const char *path, const char *modes);

/**
 * @brief Read the entire content of a file into a dynamically allocated buffer.
 * 
 * @param path Path to the file to be read. Must be a valid path string.
 * @return Pointer to the file content as a dynamically allocated string. NULL if an error occurs.
 * 
 * @note The caller is responsible for freeing the returned string. If memory allocation fails or 
 * the file cannot be opened, NULL will be returned. The file content is concatenated into a 
 * single dynamically allocated buffer.
 */
char *file_read(const char *path);

#endif
