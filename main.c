#include "headers/file-operations.h"
#include "headers/string-operations.h"
#include "headers/utilities.h"
#include <stdio.h>
#include <string.h>

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
        if (argc != 4) {
            printf("Error: Incorrect number of arguments for search operation\n");
            return 1;
        }
        search_string_in_file(argv[2], argv[3]);  // Search for string in file
    } else if (strcmp(switch_arg, "-s") == 0) {
        // Print size of file(s)
        for (int i = 2; i < argc; i++) {
            size_t size = get_file_size(argv[i]);
            printf("Size of %s: %zu bytes\n", argv[i], size);
        }
    } else if (strcmp(switch_arg, "-q") == 0) {
        // Print permissions of file(s)
        for (int i = 2; i < argc; i++) {
            int permissions = get_file_permissions(argv[i]);
            printf("Permissions of %s: %03o\n", argv[i], permissions);
        }
    } else if (strcmp(switch_arg, "-m") == 0) {
        // Merge multiple files into one
        if (argc < 4) {
            printf("Error: Insufficient arguments for merge operation\n");
            return 1;
        }
        merge_files((const char* const*)(argv + 2), argc - 3, argv[argc - 1]);
    } else if (strcmp(switch_arg, "-c") == 0) {
        if (argc != 4) {
            printf("Error: Incorrect number of arguments for compression operation\n");
            return 1;
        }
        // Compress file contents
        compress_content(argv[2], argv[3]);
    } else if (strcmp(switch_arg, "-d") == 0) {
        if (argc != 4) {
            printf("Error: Incorrect number of arguments for decompression operation\n");
            return 1;
        }
        // Decompress file contents
        decompress_content(argv[2], argv[3]);
    } else if (strcmp(switch_arg, "-n") == 0) {
        // Move a file
        if (argc != 4) {
            printf("Error: Incorrect number of arguments for move operation\n");
            return 1;
        }
        move_file(argv[2], argv[3]);
    } else if (strcmp(switch_arg, "-h") == 0) {
        // Display help page
        display_help_page();
    } else {
        printf("Invalid switch: %s\n", switch_arg);
        display_help_page();
    }

    return 0;
}
