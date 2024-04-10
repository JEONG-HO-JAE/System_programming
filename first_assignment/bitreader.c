#include "bitreader.h"


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

float bitPatternToFloat(const char *bitPattern) {
    // 부호 비트 추출
    int sign = (bitPattern[0] == '1') ? -1 : 1;

    // 지수 추출 및 계산
    int exponent = 0;
    for (int i = 1; i <= 8; ++i) {
        exponent = exponent * 2 + (bitPattern[i] - '0');
    }
    exponent -= 127; // Bias를 제거하여 실제 지수 계산

    // 가수 추출 및 계산
    float mantissa = 1.0; // 정규화된 수는 암시적으로 1을 앞에 가집니다.
    for (int i = 9; i < 32; ++i) {
        if (bitPattern[i] == '1') {
            mantissa += pow(2, -1 * (i - 8));
        }
    }

    // 최종 float 값 계산
    float value = sign * mantissa * pow(2, exponent);
    return value;
}

double bitPatternToDouble(const char *bitPattern) {
    int sign = (bitPattern[0] == '1') ? -1 : 1; // 부호 결정

    // 지수 부분 해석
    int exponent = 0;
    for (int i = 1; i <= 11; ++i) {
        exponent = exponent * 2 + (bitPattern[i] - '0');
    }
    exponent -= 1023; // Bias 제거

    // 가수 부분 해석
    double mantissa = 1.0; // 암시적으로 맨 앞에 1이 있음
    for (int i = 12; i < 64; ++i) {
        if (bitPattern[i] == '1') {
            mantissa += pow(2, -1 * (i - 11));
        }
    }

    // 최종 double 값 계산
    double value = sign * mantissa * pow(2, exponent);
    return value;
}

void printAsDataTypes(unsigned char *bytes, size_t size) {

    // printf("address: ");
    // for(int i = 0; i < size; i++){
    //     printf("%p ", (void *)(bytes + i));
    // }
    // printf("\n");
    printf("\n");
    printf("================Result==============\n");

    printf("Signed char: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", *(signed char *)(bytes + i * sizeof(signed char)));
    }
    printf("\n");

    // Print each byte as an ASCII character if printable, otherwise print a dot
    printf("ASCII character: ");
    signed char *schar_val = (signed char *)bytes;
    for (int i = 0; i < size; i++) {
        if (schar_val[i] >= 0 && schar_val[i] <= 127) {
            printf("%c ", schar_val[i]);
        } else {
            printf(". ");
        }
    }
    printf("\n");

    printf("Unsigned char : ");
    for (int i = 0; i < size; i++) {
        printf("%u ", *(unsigned char *)(bytes + i * sizeof(unsigned char)));
    }
    printf("\n");

    printf("Signed int : ");
    for (int i = 0; i < size / sizeof(signed int); i++) {
        printf("%d ", *(signed int *)(bytes + i * sizeof(signed int)));
    }
    printf("\n");

    printf("Unsigned int : ");
    for (int i = 0; i < size/sizeof(unsigned int); i++) {
        printf("%u ", *(unsigned int *)(bytes + i * sizeof(unsigned int)));
    }
    printf("\n");


    printf("Float : ");
    for (size_t i = 0; i < size / 4; ++i) {
        const unsigned char *block = &bytes[i * 4];
        
        // (32bit + null)
        char bitPattern[33];
        int bitIndex = 0;
        
        // byte -> bit (format: char*)
        for (int j = 0; j < 4; ++j) {
            for (int bit = 7; bit >= 0; --bit) {
                bitPattern[bitIndex++] = (block[j] & (1 << bit)) ? '1' : '0';
            }
        }   
        // End of bit = Null
        bitPattern[bitIndex] = '\0';
        float floatValue = bitPatternToFloat(bitPattern);
        printf("%.4e ", floatValue);
    }
    printf("\n");

    printf("Double : ");
    for (size_t i = 0; i < size / 8; ++i) {
        const unsigned char *block = &bytes[i * 8];
        
        // (64bit + null)
        char bitPattern[65];
        int bitIndex = 0;
        
        // byte -> bit (format: char*)
        for (int j = 0; j < 4; ++j) {
            for (int bit = 7; bit >= 0; --bit) {
                bitPattern[bitIndex++] = (block[j] & (1 << bit)) ? '1' : '0';
            }
        }   
        // End of bit = Null
        bitPattern[bitIndex] = '\0';
        double doubleValue = bitPatternToDouble(bitPattern);
        printf("%.4e ", doubleValue);
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