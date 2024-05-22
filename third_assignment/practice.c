#include "20182745.h"

int main() {
    // Open file for writing
    FILE *file = fopen("hello.txt", "w");

    // Write to file
    const char *text = "SSU";
    int written = fwrite(text, 1, strlen(text), file);
    if (written != strlen(text)) {
        fclose(file);
        return 1;
    }
    
    // Flush and close the file
    fflush(file);
    fclose(file);

    // Open file for reading
    file = fopen("hello.txt", "r");

    // Read from file
    char buffer[BUFSIZE];
    int read = fread(buffer, 1, BUFSIZE - 1, file);
    buffer[read] = '\0';  // Null-terminate the string

    // Print the read text
    printf("Read from file: %s\n", buffer);

    // Close the file
    fclose(file);

    return 0;
}
