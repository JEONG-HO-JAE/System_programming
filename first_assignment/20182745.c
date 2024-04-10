#include "20182745.h"


int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    FILE* file = fopen("input", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }
    
    readBitsSaveAsByte(file);
    fclose(file);
    return 0;
}


void printAsDataTypes(unsigned char *bytes, size_t size) {
    printf("================Result==============\n");

    printf("Signed char: ");
    for (int i = size-1; i >= 0; i--) {
        printf("%d ", *(signed char *)(bytes + i * sizeof(signed char)));
    }
    printf("\n");

    // Print each byte as an ASCII character if printable, otherwise print a dot
    printf("ASCII character: ");
    signed char *schar_val = (signed char *)bytes;
    for (int i = size-1; i >= 0; i--) {
        if (schar_val[i] >= 0 && schar_val[i] <= 127) {
            printf("%c ", schar_val[i]);
        } else {
            printf(". ");
        }
    }
    printf("\n");

    printf("Unsigned char : ");
    for (int i = size-1; i >= 0; i--) {
        printf("%u ", *(unsigned char *)(bytes + i * sizeof(unsigned char)));
    }
    printf("\n");


    printf("Signed int : ");
    for (int i = size - sizeof(int); i >= 0; i -= sizeof(int)) {
        int val;
        memcpy(&val, bytes + i, sizeof(int));
        printf("%d ", val);
    }
    printf("\n");

    printf("Unsigned int : ");
    for (int i = size - sizeof(int); i >= 0; i -= sizeof(int)) {
        unsigned int val;
        memcpy(&val, bytes + i, sizeof(int));
        printf("%u ", val);
    }
    printf("\n");

    printf("Float: ");
    for (int i = size - sizeof(float); i >= 0; i -= sizeof(float)) {
        float val;
        memcpy(&val, bytes + i, sizeof(float));
        printf("%.4f ", val);
    }
    printf("\n");

    printf("Double : ");
    for (int i = size - sizeof(double); i >= 0; i -= sizeof(double)) {
        double val;
        memcpy(&val, bytes + i, sizeof(double));
        printf("%.4f", val);
    }
    printf("\n");

}

void readBitsSaveAsByte(FILE *file) {
    int bit;
    unsigned char bitCount = 0;
    unsigned char byte = 0;

    unsigned char *bytes = NULL;
    size_t size = 0;
    printf("================Info==============\n");
    printf("Input data(bit data): ");
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
        printf("Size: %lu Byte(s)\n", size);
        printAsDataTypes(bytes, size); // Consider checking size before interpreting the data
    }
    free(bytes); // Move free outside the loop
}