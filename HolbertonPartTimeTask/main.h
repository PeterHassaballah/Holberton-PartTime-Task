#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

// Function prototypes
int countStrLength(char *str);
char* convertIntToStr(int num, bool isSigned);
char hexComplement(int num);
char* numericConversion(int num, char conversionType);
char* writeWithBuffer(char *currentStr, int currentStrLength, int bufferLimit);
char* isAsciiValid(char c);
int _printf(const char *format, ...);

#endif /* MAIN_H */

