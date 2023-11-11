// This library will manage all utility functions, including:
// get_file_permissions and display_help_page.

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
