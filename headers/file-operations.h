#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
FILE* open_file(const char* file_path, const char* mode);
char* read_file(const char* file_path);
void close_file(FILE* file);
size_t get_file_size(const char* file_path);
void write_to_file(FILE* file, const char* content);
void move_file();

#endif
