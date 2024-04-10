#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Function Prototypes
void printAsDataTypes(unsigned char *bytes, size_t size);
void readBitsSaveAsByte(FILE *file);
float bitPatternToFloat(const char *bitPattern);
double bitPatternToDouble(const char *bitPattern);