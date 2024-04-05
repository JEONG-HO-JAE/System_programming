#include "bitreader.h"

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
       
    readBitsSaveAsByte(file);
    fclose(file);
    return 0;
}
void printAsDataTypes(unsigned char *bytes, size_t size) {

    printf("Signed char: ");
    for (int i = size/sizeof(signed char) - 1; i >=0; i--) {
        printf("%d ", *(signed char *)(bytes + i * sizeof(signed char)));
    }
    printf("\n");

    // Print each byte as an ASCII character if printable, otherwise print a dot
    printf("ASCII character: ");
    signed char *schar_val = (signed char *)bytes;
    for (int i = size - 1; i >= 0; i--) {
        if (schar_val[i] >= 32 && schar_val[i] <= 126) {
            printf("%c ", schar_val[i]);
        } else {
            printf(". ");
        }
    }
    printf("\n");

    printf("Unsigned char : ");
    for (int i = size/sizeof(unsigned char) - 1; i >= 0; i--) {
        printf("%u ", *(unsigned char *)(bytes + i * sizeof(unsigned char)));
    }
    printf("\n");

    printf("Signed int : ");
    for (int i = size/sizeof(signed int) - 1; i >= 0; i--) {
        printf("%d ", *(signed int *)(bytes + i * sizeof(signed int)));
    }
    printf("\n");

    printf("Unsigned int : ");
    for (int i = size/sizeof(unsigned int) - 1; i >= 0; i--) {
        printf("%u ", *(unsigned int *)(bytes + i * sizeof(unsigned int)));
    }
    printf("\n");

    printf("Float: ");
    if (size >= sizeof(float)) {
        for (int i = size / sizeof(float) - 1; i >= 0; i--) {
            //printf("%f, %d, %d", (float*)(bytes + i * sizeof(float)), *(float*)(bytes + i * sizeof(float)), bytes + i * sizeof(float));
            printf("%.4f ", *(float*)(bytes + i * sizeof(float)));
        }
    }
    printf("\n");

    printf("Double: ");
    if (size >= sizeof(double)) {
        for (int i = size / sizeof(double) - 1; i >= 0; i--) {
            printf("%.4f", *(double *)(bytes + i * sizeof(double)));
        } 
    }
    printf("\n");
}

void readBitsSaveAsByte(FILE *file) {
    int bit;
    unsigned char bitCount = 0;
    unsigned char byte = 0;

    unsigned char *bytes = NULL;
    size_t size = 0;

    // Read characters from the file
    while ((bit = fgetc(file)) != EOF) {
        printf("%c", bit);
        // Check if the character is '0' or '1'
        if (bit == '0' || bit == '1') {
            // Set the corresponding bit in the byte
            if (bit == '1') {
                byte |= (1 << (7 - bitCount)); // Fixed to use bitwise OR assignment
            }
            bitCount++;
            // If we have read 8 bits, save the byte
            if (bitCount == 8) {
                unsigned char *newBytes = realloc(bytes, size + 1);
                if (!newBytes) {
                    perror("Memory allocation failed");
                    free(bytes);
                    exit(EXIT_FAILURE);
                }
                bytes = newBytes;
                bytes[size] = byte;
                size++;
                bitCount = 0;
                byte = 0;
            }
        }
    }

    if (size > 0) {
        printAsDataTypes(bytes, size); // Consider checking size before interpreting the data
    }
    free(bytes); // Move free outside the loop
}