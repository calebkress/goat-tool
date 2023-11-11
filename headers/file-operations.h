#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdlib.h>
#include <stdio.h>

// Function prototypes:
// Function to open a file with given mode (r, w, etc.)
FILE* open_file(const char* file_path, const char* mode);

// Function to close a file
void close_file(FILE* file);

// Function to read entire content of a file
char* read_file(const char* file_path);

// Function to write content to a file
void write_to_file(FILE* file, const char* content);

// Function to get size of a file
size_t get_file_size(const char* file_path);

// Function to get file permissions as an integer
int get_file_permissions(const char* file_path);

// Function to merge multiple files into a single file
void merge_files(const char* const* input_paths, int num_files, const char* output_path);

// Function to move a file from one path to another
void move_file(const char* source_path, const char* destination_path);

#endif
