// This library will manage all utility functions, including:
// get_file_permissions and display_help_page.

#include "../headers/utilities.h"
#include "../headers/file-operations.h"
#include <stdio.h>

// Prints contents of a file to console
void print_content(const char* file_path) {
    char* content = read_file(file_path);
    if (content) {
        printf("%s\\n", content);
        free(content);
    }
}

// Displays the help page for GoatTool
void display_help_page() {
    printf("GoatTool Help Page:\n");
    printf(" -p <file_paths>       Print the contents of one or more files.\n");
    printf(" -g <file_path> <search_string> Search a file for a particular string and print matching lines.\n");
    printf(" -s <file_paths>       Print the size of one or more files.\n");
    printf(" -m <file_paths> <output_path> Merge multiple files into a single file.\n");
    printf(" -c <input_file_path> <output_file_path> Compress a text file into the .goat format.\n");
    printf(" -d <input_file_path> <output_file_path> Decompress a .goat file to retrieve the original text.\n");
    printf(" -q <file_paths>       Print the permissions of one or more files as an integer (000 to 777).\n");
    printf(" -n <source_path> <destination_path> <new_name> Move a file to a new location with a new name.\n");
    printf(" -h                   Print this help page.\n");
}

