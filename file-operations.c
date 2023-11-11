// This library will manage all file operations, including:
// open_file, read_file, print_content, close_file, get_file_size,
// create_new_file, write_to_file, move_file, and rename_file.

#include "file-operations.h"

// Open a file with specified path and mode
FILE* open_file(const char* file_path, const char* mode) {
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

// Read an open file
char* read_file(FILE* file) {
    // Check if file is valid
    if (!file) {
        fprintf(stderr, "File not opened\n");
        return NULL;
    }

    // Get file size using get_file_size
    fseek(file, 0, SEEK_SET); // Set file pointer at beginning of file
    long length = get_file_size(file);
    if (length == -1) {
        
    }

}

// Close an opened file
void close_file(FILE* file) {
    if(file) {
        // if file pointer isn't null, close file
        fclose(file);
    }
}

// Get size of a file
size_t get_file_size(const char* file_path) {
    // Open file in read mode
    FILE *file = open_file(file_path, 'r');
    // Handle errors in file opening
    if (!file) {
        // Return 0 if file doesn't exist
        return 0;
    }

    // Move file pointer to end of file
    fseek(file, 0, SEEK_END);
    // Tell position of file pointer (now at end), giving file size
    size_t size = ftell(file);

    // Close file
    close_file(file);
    // Return file size
    return size;
}

// Writes specified content to a provided file object
void write_to_file(FILE* file, const char* content) {
    if (file && content) {
        // Writes to file if both file and content exist
        fputs(content, file);
    }
}

void move_file() {

}