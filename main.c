#include "main.h"
#include <unistd.h>
#include <stdarg.h>

static void handle_unsigned(unsigned int num, int base, int uppercase, int *count)
{
    char buffer[32];
    int i = 0;
    int j = 0;
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    if (num == 0)
    {
        write(1, "0", 1);
        (*count)++;
        return;
    }

    while (num > 0)
    {
        buffer[i++] = digits[num % base];
        num /= base;
    }

    // Write digits in reverse order (MSB to LSB)
    for (j = i - 1; j >= 0; j--)
    {
        write(1, &buffer[j], 1);
        (*count)++;
    }
}

static void handle_binary(unsigned int num, int *count)
{
    char buffer[32]; // Holds binary digits (up to 32 bits)
    int i = 0;
    int j;

    if (num == 0)
    {
        write(1, "0", 1);
        (*count)++;
        return;
    }

    // Extract binary digits (LSB to MSB)
    while (num > 0)
    {
        buffer[i++] = '0' + (num % 2);
        num /= 2;
    }

    // Write digits in reverse (MSB to LSB)
    for (j = i - 1; j >= 0; j--)
    {
        write(1, &buffer[j], 1);
        (*count)++;
    }
}

static void handle_int(int num, int *count)
{
    char buffer[12];
    int is_negative = 0;
    int i = 0;
    int j = 0;

    if (num == 0)
    {
        write(1, "0", 1);
        (*count)++;
        return;
    }

    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }

    while (num > 0)
    {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    if (is_negative)
    {
        buffer[i++] = '-';
    }

    // Reverse the digits
    for (j = 0; j < i / 2; j++)
    {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    // Write each character
    for (j = 0; j < i; j++)
    {
        write(1, &buffer[j], 1);
        (*count)++;
    }
}

int _printf(const char *format, ...)
{
    va_list args;
    int count = 0;
    char *s;
    char c;
    int num;
    unsigned int num_unsigned;

    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
                c = va_arg(args, int);
                write(1, &c, 1);
                count++;
                break;
            case 's':
                s = va_arg(args, char *);
                if (!s)
                    s = "(null)";
                for (; *s; s++)
                {
                    write(1, s, 1);
                    count++;
                }
                break;
            case 'd':
            case 'i':
                num = va_arg(args, int);
                handle_int(num, &count);
                break;
            case 'b':
                num_unsigned = va_arg(args, unsigned int);
                handle_binary(num_unsigned, &count);
                break;
            case 'u':
                num_unsigned = va_arg(args, unsigned int);
                handle_unsigned(num_unsigned, 10, 0, &count);
                break;
            case 'o':
                num_unsigned = va_arg(args, unsigned int);
                handle_unsigned(num_unsigned, 8, 0, &count);
                break;
            case 'x':
                num_unsigned = va_arg(args, unsigned int);
                handle_unsigned(num_unsigned, 16, 0, &count);
                break;
            case 'X':
                num_unsigned = va_arg(args, unsigned int);
                handle_unsigned(num_unsigned, 16, 1, &count);
                break;
            case '%':
                write(1, "%", 1);
                count++;
                break;
            default:
                write(1, "%", 1);
                write(1, format, 1);
                count += 2;
                break;
            }
        }
        else
        {
            write(1, format, 1);
            count++;
        }
        format++;
    }

    va_end(args);
    return count;
}

// Temporary main for testing
int main(void)
{
    _printf("Chars: %c %c\n", 'X', 'Y');
    _printf("Chars: %c %c are awesome chars\n", 'X', 'Y');
    _printf("String: %s\n", "Task 1");
    _printf("Percent: %%\n");
    _printf("Decimal: %d\n", 123);
    _printf("Negative: %i\n", -456);
    _printf("Zero: %d\n", 0);
    _printf("98 in binary: %b\n", 98);
    _printf("Zero: %b\n", 0);
    _printf("One: %b\n", 1);
    _printf("Unsigned: %u\n", 123456);
    _printf("Octal: %o\n", 64);
    _printf("Hex (lower): %x\n", 255);
    _printf("Hex (upper): %X\n", 255);
    _printf("Zero (hex): %x\n", 0);
    return 0;
}
