#include "../headers/string-operations.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Function to check if a substring exists in a string
int contains(const char *line, const char *substring) {
    return strstr(line, substring) != NULL ? 1 : 0;
}

// Searches for a string in a given line
int search_string_in_line(const char *line, const char *string) {
    return contains(line, string);
}

// Compresses content using the specified scheme
void compress_content(const char *input, char *output) {
    int count = 0;
    char last_char = input[0];

    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == last_char) {
            count++;
        } else {
            if (last_char == ' ') {
                sprintf(output, "%s1 ", output);
            } else {
                sprintf(output, "%s%c%d", output, last_char, count);
            }
            count = 1;
            last_char = input[i];
        }
    }

    // Handle the last sequence
    if (last_char == ' ') {
        sprintf(output, "%s1 ", output);
    } else {
        sprintf(output, "%s%c%d", output, last_char, count);
    }
}

// Decompresses content from the specified scheme
void decompress_content(const char *input, char *output) {
    int i = 0, j = 0, num;
    char ch;

    while (input[i] != '\0') {
        if (isdigit(input[i])) {
            // Read the number (count of characters)
            num = atoi(&input[i]);

            while (isdigit(input[i])) i++;

            // Expand the character sequence
            for (int k = 0; k < num; k++, j++) {
                output[j] = ch;
            }
        } else {
            ch = input[i++];
            // Special handling for space character
            if (ch == '1' && input[i] == ' ') {
                output[j++] = ' ';
                i++; // Skip the space after '1'
            }
        }
    }

    output[j] = '\0'; // Null-terminate the output string
}

int main() {
    // Test the contains function
    const char* line = "Hello, world!";
    const char* substring = "world";
    if (contains(line, substring)) {
        printf("'%s' contains '%s'\n", line, substring);
    } else {
        printf("'%s' does not contain '%s'\n", line, substring);
    }

    // Test the search_string_in_line function
    const char* search_line = "This is a test line.";
    const char* search_string = "test";
    if (search_string_in_line(search_line, search_string)) {
        printf("'%s' contains '%s'\n", search_line, search_string);
    } else {
        printf("'%s' does not contain '%s'\n", search_line, search_string);
    }

    // Test the compress_content function
    const char* input = "aaabbbccddddd";
    char output[100];
    compress_content(input, output);
    printf("Compressed: %s\n", output);

    // Test the decompress_content function
    const char* compressed_input = "a3b3c2d5";
    char decompressed_output[100];
    decompress_content(compressed_input, decompressed_output);
    printf("Decompressed: %s\n", decompressed_output);

    return 0;
}