// This library will manage all file operations, including:
// open_file, read_file, print_content, close_file, get_file_size,
// create_new_file, write_to_file, move_file, and rename_file.

#include "file-operations.h"

// Open a file with specified path and mode
FILE *open_file(const char *file_path, const char *mode) {
    // Try to open file with specified path and mode
    FILE *file = fopen(file_path, mode);
    // Return error if file doesn't exist
    if (!file) {
        perror("Error opening file.");
        return NULL;
    }
    // Return opened file
    return file;
}

// Print the contents of a file to the console
void print_content(FILE *file) {
    // Define buffer to temporarily hold read content
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Read lines from file until EOF. fgets reads a line or a max of 1023
        // characters (one less than buffer size, leaving space for null terminator)
        printf("%s", buffer); // Print content to console
    }
}

