#include "main.h"
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

static void flush_buffer(BufferState *state)
{
    if (state->index > 0)
    {
        write(1, state->buffer, state->index);
        state->total += state->index;
        state->index = 0;
    }
}

static void add_to_buffer(char c, BufferState *state)
{
    state->buffer[state->index++] = c;
    if (state->index == 1024)
    {
        flush_buffer(state);
    }
}

static void add_padding(int width, int data_len, BufferState *state)
{
    int i = 0;
    int padding = width - data_len;
    if (padding > 0)
    {
        for (i = 0; i < padding; i++)
            add_to_buffer(' ', state);
    }
}

static void handle_char(char c, int width, BufferState *state)
{
    add_padding(width, 1, state);
    add_to_buffer(c, state);
}

static void add_string(const char *s, int width, BufferState *state)
{
    int len = 0;

    if (!s)
    {
        s = "(null)";
    }

    len = strlen(s);

    add_padding(width, len, state);
    for (; *s; s++)
    {
        add_to_buffer(*s, state);
    }
}

static void handle_string(char *s, int width, BufferState *state)
{
    int len = 0;
    char *p;

    if (!s)
    {
        s = "(null)";
    }
    len = 0;

    // Calculate effective length
    for (p = s; *p; p++)
    {
        if (*p < 32 || *p >= 127)
            len += 4;
        else
            len += 1;
    }

    add_padding(width, len, state);

    for (; *s; s++)
    {
        unsigned char c = *s;

        if (c >= 32 && c < 127)
        {
            handle_char(c, 1, state);
        }
        else
        {
            handle_char('\\', 1, state);
            handle_char('x', 1, state);
            handle_char("0123456789ABCDEF"[c >> 4], 1, state);   // High nibble
            handle_char("0123456789ABCDEF"[c & 0x0F], 1, state); // Low nibble
        }
    }
}

static void handle_int(int num, int width, BufferState *state)
{
    char num_str[20];
    int is_neg = num < 0;
    int i = 0;
    int j = 0;
    int num_len = 0;
    int total_len = 0;

    if (num == 0)
    {
        num_str[i++] = '0';
    }
    else
    {
        num = is_neg ? -num : num;
        while (num > 0)
        {
            num_str[i++] = '0' + (num % 10);
            num /= 10;
        }
    }

    num_len = i;
    total_len = num_len + (is_neg ? 1 : 0);
    add_padding(width, total_len, state);

    if (is_neg)
        num_str[i++] = '-';

    for (j = i - 1; j >= 0; j--)
    {
        add_to_buffer(num_str[j], state);
    }
}

static void handle_unsigned(unsigned int num, int base, int uppercase, int width, BufferState *state)
{
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    char buffer[32];
    int i = 0;
    int j = 0;

    if (num == 0)
    {
        buffer[i++] = '0';
    }
    else
    {
        while (num > 0)
        {
            buffer[i++] = digits[num % base];
            num /= base;
        }
    }

    add_padding(width, i, state);

    for (j = i - 1; j >= 0; j--)
    {
        add_to_buffer(buffer[j], state);
    }
}

int _printf(const char *format, ...)
{
    char c;
    char *s;
    char *S;
    unsigned unsigned_num;
    char length = 0;
    int width = 0;
    BufferState state = {.index = 0, .total = 0};
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;

            // Parse width
            while (isdigit(*format))
            {
                width = width * 10 + (*format - '0');
                format++;
            }

            if (*format == 'h' || *format == 'l')
            {
                length = *format;
                format++;
            }

            switch (*format)
            {
            case 'c':
            {
                c = va_arg(args, int);
                handle_char(c, width, &state);
                break;
            }
            case 's':
            {
                s = va_arg(args, char *);
                add_string(s, width, &state);
                break;
            }
            case 'S':
                S = va_arg(args, char *);
                handle_string(S, width, &state);
                break;
            case 'd':
            case 'i':
            {
                if (length == 'l')
                {
                    long num = va_arg(args, long);
                    handle_int(num, width, &state);
                }
                else if (length == 'h')
                {
                    short num = (short)va_arg(args, int);
                    handle_int(num, width, &state);
                }
                else
                {
                    int num = va_arg(args, int);
                    handle_int(num, width, &state);
                }
                break;
            }
            case 'b':
            {
                unsigned_num = va_arg(args, unsigned int);
                handle_unsigned(unsigned_num, 2, 0, width, &state);
                break;
            }
            case 'u':
            {
                if (length == 'l')
                {
                    unsigned long num = va_arg(args, unsigned long);
                    handle_unsigned(num, 10, 0, width, &state);
                }
                else if (length == 'h')
                {
                    unsigned short num = (unsigned short)va_arg(args, unsigned int);
                    handle_unsigned(num, 10, 0, width, &state);
                }
                else
                {
                    unsigned int num = va_arg(args, unsigned int);
                    handle_unsigned(num, 10, 0, width, &state);
                }
                break;

                break;
            }
            case 'o':
            {
                if (length == 'l')
                {
                    unsigned long num = va_arg(args, unsigned long);
                    handle_unsigned(num, 8, 0, width, &state);
                }
                else if (length == 'h')
                {
                    unsigned short num = (unsigned short)va_arg(args, unsigned int);
                    handle_unsigned(num, 8, 0, width, &state);
                }
                else
                {
                    unsigned int num = va_arg(args, unsigned int);
                    handle_unsigned(num, 8, 0, width, &state);
                }
                break;
            }
            case 'x':
            {
                if (length == 'l')
                {
                    unsigned long num = va_arg(args, unsigned long);
                    handle_unsigned(num, 16, 0, width, &state);
                }
                else if (length == 'h')
                {
                    unsigned short num = (unsigned short)va_arg(args, unsigned int);
                    handle_unsigned(num, 16, 0, width, &state);
                }
                else
                {
                    unsigned int num = va_arg(args, unsigned int);
                    handle_unsigned(num, 16, 0, width, &state);
                }
                break;
            }
            case 'X':
            {
                if (length == 'l')
                {
                    unsigned long num = va_arg(args, unsigned long);
                    handle_unsigned(num, 16, 1, width, &state);
                }
                else if (length == 'h')
                {
                    unsigned short num = (unsigned short)va_arg(args, unsigned int);
                    handle_unsigned(num, 16, 1, width, &state);
                }
                else
                {
                    unsigned int num = va_arg(args, unsigned int);
                    handle_unsigned(num, 16, 1, width, &state);
                }
                break;
            }
            case '%':
            {
                add_to_buffer('%', &state);
                break;
            }
            default:
            {
                add_to_buffer('%', &state);
                add_to_buffer(*format, &state);
                break;
            }
            }
        }
        else
        {
            add_to_buffer(*format, &state);
        }
        format++;
    }

    flush_buffer(&state);
    va_end(args);
    return state.total;
}

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
    _printf("%S\n", "Best\nSchool");
    _printf("%S\n", "Hello\tthere");
    _printf("Long: %ld\n", 123456789L);                      // Output: 123456789
    _printf("Short: %hd\n", (short)-123);                    // Output: -123
    _printf("Unsigned long: %lu\n", 4294967295UL);           // Output: 4294967295
    _printf("Unsigned short: %hu\n", (unsigned short)65535); // Output: 65535
    _printf("Width 5: |%5d|\n", 123);                        // |  123|
    _printf("Width 10: |%10s|\n", "hello");                  // |     hello|
    _printf("Width 6: |%6S|\n", "Hi\t");                     // |Hi\x09|
    return 0;
}
