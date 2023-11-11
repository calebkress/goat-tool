#ifndef STRING_OPERATIONS_H
#define STRING_OPERATIONS_H

// Function prototypes
int contains(const char *line, const char *substring);
int search_string_in_line(const char *line, const char *string);
void search_string_in_file(const char* file_path, const char* search_string);
void compress_content(const char *input, char *output);
void decompress_content(const char *input, char *output);

#endif
