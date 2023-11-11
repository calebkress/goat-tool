// This library will manage all file operations, including:
// open_file, read_file, print_content, close_file, get_file_size,
// create_new_file, write_to_file, move_file, and rename_file.

#include "../headers/file-operations.h"
#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>

// Open a file with specified path and mode
FILE* open_file(const char* file_path, const char* mode) {
    // Try to open file with specified path and mode
    FILE *file = fopen(file_path, mode);
    // Return error if file doesn't exist
    if (!file) {
        perror("Error opening file.");
        return NULL;
    }
    // Return opened file pointer
    return file;
}

// Close an opened file
void close_file(FILE* file) {
    if(file) {
        // If file pointer isn't null, close file
        fclose(file);
    }
}

// Read an open file's contents
char* read_file(const char* file_path) {
    // Get file size using file path
    long length = get_file_size(file_path);  
    if (length < 0) {
        return NULL;  // Return NULL if file size couldn't be determined
    }

    // Open file for reading
    FILE* file = open_file(file_path, "r");  
    if (!file) {
        return NULL;  // Return NULL if file couldn't be opened
    } 

    // Allocate memory for file content
    char* content = (char*) malloc(length + 1);  
    if (content) {
        fread(content, 1, length, file);  // Read file into buffer
        content[length] = '\0';  // Null-terminate string
    }

    close_file(file);  // Close file
    return content;  // Return file content
}

// Writes specified content to a provided file object
void write_to_file(FILE* file, const char* content) {
    if (file && content) {
        // Writes to file if both file and content exist
        fputs(content, file);
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

// Retrieve permissions of given file
int get_file_permissions(const char* file_path) {
    // Define struct of type stat to get file info
    struct stat st;
    if (stat(file_path, &st) == 0) {
        return st.st_mode & 0777;  // Return file permissions, isolating permission bits
    } else {
        perror("Error getting file permissions");
        return -1;  // Return -1 on error
    }
}

// Multithreading setup
// Struct for thread args
typedef struct {
    const char* file_path;
    char* content;
} ThreadArgs;

// Thread fn to read file content
void* read_file_thread(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;
    thread_args->content = read_file(thread_args->file_path);
    return NULL;
}

// Merge multiple files into a single file using multithreading
void merge_files(const char* const* input_paths, int num_files, const char* output_path) {
    pthread_t threads[num_files];
    ThreadArgs thread_args[num_files];

    // Create threads for each file
    for (int i = 0; i < num_files; ++i) {
        thread_args[i].file_path = input_paths[i];
        pthread_create(&threads[i], NULL, read_file_thread, &thread_args[i]);
    }

    // Join threads
    for (int i = 0; i < num_files; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Merge content from all threads into output file
    FILE* output_file = open_file(output_path, "w");
    if (output_file) {
        for (int i = 0; i < num_files; ++i) {
            if (thread_args[i].content) {
                write_to_file(output_file, thread_args[i].content);
                free(thread_args[i].content);  // Free memory allocated in thread
            }
        }
        close_file(output_file);
    }
}

// Move a file from one path to another
void move_file(const char* source_path, const char* destination_path) {
    if (rename(source_path, destination_path) != 0) {
        perror("Error moving file");  // Print error if rename fails
    }
}