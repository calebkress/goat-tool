#include "headers/file-operations.h"
#include "headers/string-operations.h"
#include "headers/utilities.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h> 
#include <limits.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // If no arguments are provided, display the help page
        display_help_page();
        return 0;
    }

    // Process the switch (command-line argument)
    char *switch_arg = argv[1];
    if (strcmp(switch_arg, "-p") == 0) {
        // Print contents of file(s)
        for (int i = 2; i < argc; i++) {
            print_content(argv[i]);  // Directly pass the file path
        }
    } else if (strcmp(switch_arg, "-g") == 0) {
        // Handle -g switch for searching strings in a file
        if (argc == 2) {
            // No file or search string specified
            printf("ERROR: File or search string not specified.\n");
        } else if (argc == 3) {
            // File specified but no search string
            printf("ERROR: Search string not specified.\n");
        } else if (argc > 4) {
            // Too many arguments
            printf("ERROR: Too many arguments for search operation.\n");
        } else {
            // Correct number of arguments, proceed with search
            search_string_in_file(argv[2], argv[3]);
        }
    } else if (strcmp(switch_arg, "-s") == 0) {
        if (argc == 2) {
            printf("ERROR: No file specified.\n");
        } else {
            for (int i = 2; i < argc; i++) {
                size_t size = get_file_size(argv[i]);
                if (size == (size_t)-1) {
                    printf("ERROR: File does not exist.\n");
                } else if (size == (size_t)-2) {
                    printf("ERROR: Incorrect file format.\n");
                } else {
                    // Extract just the file name from the path
                    const char* file_name = strrchr(argv[i], '/');
                    file_name = (file_name != NULL) ? file_name + 1 : argv[i];
                    printf("Size of %s: %zu bytes\n", file_name, size);
                }
            }
        }
    } else if (strcmp(switch_arg, "-q") == 0) {
        if (argc == 2) {
            printf("ERROR: No file specified.\n");
        } else {
            for (int i = 2; i < argc; i++) {
                int permissions = get_file_permissions(argv[i]);
                if (permissions != -1) {
                    printf("Permissions of %s: %03o\n", custom_basename(argv[i]), permissions);
                }
            }
        }
    } else if (strcmp(switch_arg, "-m") == 0) {
        if (argc < 4) {
            printf("ERROR: Files to merge not specified.\n");
        } else if (argc == 4) {
            // When only two arguments are provided, destination file is not specified
            printf("ERROR: Destination file not specified.\n");
        } else {
            // When there are more than two arguments, proceed with merge
            merge_files((const char* const*)(argv + 2), argc - 3, argv[argc - 1]);
        }
    } else if (strcmp(switch_arg, "-c") == 0) {
        // Compress file into .goat format
        if (argc != 4) {
            printf("Error: Incorrect number of arguments for compression operation\n");
            return 1;
        }

        // Read content of input file
        char* input_content = read_file(argv[2]);
        if (!input_content) {
            printf("Error: Unable to read file %s\n", argv[2]);
            return 1;
        }

        // Prepare buffer for compressed content
        char* compressed_content = malloc(strlen(input_content) * 2); // Allocate enough memory
        if (!compressed_content) {
            printf("Error: Memory allocation failed\n");
            free(input_content);
            return 1;
        }

        // Compress content
        compress_content(input_content, compressed_content);
        //change file format
        int old_name_len = strlen(argv[3]);
        char* new_name = malloc(old_name_len * sizeof(char));
        strcpy(new_name,argv[3]);
        
        // Write compressed content to output file
        FILE* output_file = open_file(argv[3], "w");
        if (!output_file) {
            printf("Error: Unable to open file %p for writing\n", argv[3]);
            free(input_content);
            free(compressed_content);
            return 1;
        }
        write_to_file(output_file, compressed_content);

        // Clean up
        close_file(output_file);
        free(input_content);
        free(compressed_content);
    } else if (strcmp(switch_arg, "-d") == 0) {
        // Decompress a .goat file
        if (argc != 4) {
            printf("Error: Incorrect number of arguments for decompression operation\n");
            return 1;
        }

        // Step 1: Read the content of the compressed input file
        char* compressed_content = read_file(argv[2]);
        if (!compressed_content) {
            printf("Error: Unable to read file %s\n", argv[2]);
            return 1;
        }

        // Prepare buffer for decompressed content
        // Assuming decompressed content might be larger than the compressed data
        char* decompressed_content = malloc(strlen(compressed_content) * 2); // Allocate enough memory
        if (!decompressed_content) {
            printf("Error: Memory allocation failed\n");
            free(compressed_content);
            return 1;
        }

        // Decompress content
        decompress_content(compressed_content, decompressed_content);

        // Write decompressed content to output file
        FILE* output_file = open_file(argv[3], "w");
        if (!output_file) {
            printf("Error: Unable to open file %s for writing\n", argv[3]);
            free(compressed_content);
            free(decompressed_content);
            return 1;
        }
        write_to_file(output_file, decompressed_content);

        // Clean up
        close_file(output_file);
        free(compressed_content);
        free(decompressed_content);
    } else if (strcmp(switch_arg, "-n") == 0) {
        // Check if required arguments are provided
        if (argc < 4) {
            // Determine specific error based on number of arguments
            if (argc == 2) {
                printf("ERROR: Source file not specified.\n");
            } else if (argc == 3) {
                printf("ERROR: Destination path or new name not specified.\n");
            } else {
                printf("Error: Incorrect number of arguments for move operation\n");
            }
        } else {
            // Before attempting to move, check if source file exists
            if (access(argv[2], F_OK) != 0) {
                printf("ERROR: File does not exist.\n");
            } else {
                // Extract directory from destination path and check if it exists
                char dest_dir[PATH_MAX];
                strcpy(dest_dir, argv[3]);
                dirname(dest_dir);
                if (access(dest_dir, F_OK) != 0) {
                    printf("ERROR: Destination directory does not exist.\n");
                } else {
                    // Perform the move operation
                    move_file(argv[2], argv[3]);
                    printf("File moved and renamed successfully.\n");
                }
            }
        }
    } else if (strcmp(switch_arg, "-h") == 0) {
        // Display help page
        display_help_page();
    } else {
        printf("Invalid switch: %s\n", switch_arg);
        display_help_page();
    }

    return 0;
}
