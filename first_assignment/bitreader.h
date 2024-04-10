#ifndef BITREADER_H
#define BITREADER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function Prototypes
void printAsDataTypes(unsigned char *bytes, size_t size);
void readBitsSaveAsByte(FILE *file);
float bitPatternToFloat(const char *bitPattern);
double bitPatternToDouble(const char *bitPattern);

#endif // BITREADER_H