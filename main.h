#ifndef MAIN_H

#define MAIN_H

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int print_char(int c);
int print_digit(long n, int base);
int	print_str(char *s, int precision);
int print_format(char specifier, va_list ap, char length_modifier, int width, int precision, int left_justify);
int _printf(const char *format, ...);
int print_digit_X(long n, int base);
int write_to_buffer(char c);

#endif
