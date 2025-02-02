#ifndef MAIN_H
#define MAIN_H

#include <stdarg.h>

typedef struct
{
    char buffer[1024];
    int index;
    int total;
} BufferState;

int _printf(const char *format, ...);

#endif
