#include "../headers/string-operations.h"
#include "../headers/file-operations.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Function to check if a substring exists in a string
int contains(const char *line, const char *substring) {
    return strstr(line, substring) != NULL ? 1 : 0;
}

// Searches for a string in a given line
int search_string_in_line(const char *line, const char *string) {
    return contains(line, string);
}

void compress_content(const char *input, char *output) {
    int count = 1;  // Start count from 1
    char last_char = input[0];
    char buffer[128];  // Temporary buffer for sprintf
    output[0] = '\0';  // Initialize output as empty string

    for (int i = 1; input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            // Append last_char and count to output, then add space
            sprintf(buffer, "%c%d ", last_char, count);
            strcat(output, buffer);
            last_char = input[++i];  // Skip space and move to next character
            count = 1;
        } else if (input[i] == last_char) {
            count++;
        } else {
            // Append last_char and count to output
            sprintf(buffer, "%c%d", last_char, count);
            strcat(output, buffer);
            last_char = input[i];
            count = 1;
        }
    }

    // Handle last sequence
    sprintf(buffer, "%c%d", last_char, count);
    strcat(output, buffer);
}

// Decompresses content from the specified scheme
void decompress_content(const char *input, char *output) {
    int i = 0, j = 0, count;
    char ch;

    while (input[i] != '\0') {
        ch = input[i++];  // Read character

        if (ch == ' ') {
            output[j++] = ' ';  // Copy space as-is
        } else {
            // Read count of characters
            count = 0;
            while (isdigit(input[i])) {
                count = count * 10 + (input[i] - '0');
                i++;
            }

            // Expand character sequence
            for (int k = 0; k < count; k++) {
                output[j++] = ch;
            }
        }
    }

    output[j] = '\0'; // Null-terminate output string
}


void search_string_in_file(const char* file_path, const char* search_string) {
    // Use the open_file function to open the file for reading
    FILE *file = open_file(file_path, "r");

    // Check if file was successfully opened
    if (!file) {
        // If open_file returned NULL, file couldn't be opened
        printf("Error: Unable to open file %s\n", file_path);
        return;  // Exit function early if file opening fails
    }

    char *line = NULL;   // Pointer for storing current line
    size_t len = 0;      // Length of line
    ssize_t read;        // Number of characters read

    // Read file line by line
    while ((read = getline(&line, &len, file)) != -1) {
        // Check if current line contains search string
        if (search_string_in_line(line, search_string)) {
            // If line contains string, print it to console
            printf("%s", line);
        }
    }

    // After reading file, free memory allocated for 'line'
    free(line);

    // Use close_file function to close file
    close_file(file);
}