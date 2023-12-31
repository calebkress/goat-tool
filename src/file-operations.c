#include "../headers/file-operations.h"
#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>

// Open a file with specified path and mode
FILE* open_file(const char* file_path, const char* mode) {
    // Try to open file with specified path and mode
    FILE* file = fopen(file_path, mode);
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
    // Check for supported format first (ex: .doc files not supported)
    const char *extension = strrchr(file_path, '.');
    if (extension != NULL && strcmp(extension, ".doc") == 0) {
        return (size_t)-2; // Special value for unsupported format
    }

    // Open file in read mode
    FILE *file = open_file(file_path, "r");
    // Handle errors in file opening
    if (!file) {
        // Return special value if file doesn't exist
        return (size_t)-1;
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

// Helper function to extract basename from file path
char* custom_basename(char* path) {
    char* base = strrchr(path, '/');
    return base ? base + 1 : path;
}

// Retrieve permissions of given file
int get_file_permissions(const char* file_path) {
    struct stat st;

    // Check if the file exists
    if (access(file_path, F_OK) != 0) {
        printf("ERROR: File does not exist.\n");
        return -1;
    }

    // Check if it's a directory
    if (stat(file_path, &st) == 0 && S_ISDIR(st.st_mode)) {
        printf("ERROR: Path is a directory, not a file.\n");
        return -1;
    }

    // Retrieve and return file permissions
    if (stat(file_path, &st) == 0) {
        return st.st_mode & 0777;  // Return file permissions
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
    // Check if output directory exists
    char* output_dir = strdup(output_path);
    char* dir = dirname(output_dir);
    if (access(dir, F_OK) != 0) {
        printf("ERROR: Destination directory does not exist.\n");
        free(output_dir);
        return;
    }
    free(output_dir);

    // Create threads and args for each file
    pthread_t threads[num_files];
    ThreadArgs thread_args[num_files];

    // Check if each input file exists
    for (int i = 0; i < num_files; ++i) {
        if (access(input_paths[i], F_OK) != 0) {
            printf("ERROR: %s does not exist.\n", input_paths[i]);
            return; // Early return if any file doesn't exist
        }
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
                free(thread_args[i].content); // Free memory allocated in thread
            }
        }
        close_file(output_file);
        printf("Files merged successfully.\n");
    }
}

// Move a file from one path to another
void move_file(const char* source_path, const char* destination_path) {
    if (rename(source_path, destination_path) != 0) {
        perror("Error moving file");
    } else {
        printf("File moved successfully.\n");
    }
}

char* change_extension_to_goat(const char *file_path) {
    if (file_path == NULL) {
        return NULL;
    }

    // Find the length of the file path
    int path_len = strlen(file_path);

    // Calculate the new length: length of path + length of ".goat" - length of old extension
    int new_path_len = path_len;
    for (int i = path_len - 1; i >= 0; i--) {
        if (file_path[i] == '.') {
            new_path_len = i + 5; // 4 for "goat" and 1 for '\0'
            break;
        }
    }

    // Allocate memory for the new file path
    char *new_path = (char *)malloc(new_path_len * sizeof(char));
    if (new_path == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Copy the file path up to the last '.'
    strncpy(new_path, file_path, new_path_len - 5);

    // Add the new extension
    strcpy(new_path + new_path_len - 5, ".goat");

    return new_path;
}
char* change_extension_to_txt(const char *file_path) {
    if (file_path == NULL) {
        return NULL;
    }

    // Find the length of the file path
    int path_len = strlen(file_path);

    // Calculate the new length: length of path + length of ".txt" - length of old extension
    int new_path_len = path_len;
    for (int i = path_len - 1; i >= 0; i--) {
        if (file_path[i] == '.') {
            new_path_len = i + 4; // 3 for "txt" and 1 for '\0'
            break;
        }
    }

    // Allocate memory for the new file path
    char *new_path = (char *)malloc(new_path_len * sizeof(char));
    if (new_path == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Copy the file path up to the last '.'
    strncpy(new_path, file_path, new_path_len - 4);

    // Add the new extension
    strcpy(new_path + new_path_len - 4, ".txt");

    return new_path;
}

