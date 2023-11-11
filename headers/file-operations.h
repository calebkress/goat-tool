#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdio.h> // Include necessary standard library header

// Function prototypes
FILE* open_file(const char* file_path, const char* mode);
char* read_file(FILE* file);
void close_file(FILE* file);
size_t get_file_size(const char* file_path);
void write_to_file(FILE* file, const char* content);
void move_file();

#endif
