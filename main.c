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
        flush_buffer(state);
}

static void add_padding(int width, int data_len, int minus_flag, BufferState *state)
{
    int padding = width - data_len;
    int i = 0;
    if (padding > 0)
    {
        if (!minus_flag)
        {
            for (i = 0; i < padding; i++)
                add_to_buffer(' ', state);
        }
    }
}

static void handle_char(char c, int width, int minus_flag, BufferState *state)
{
    int i = 0;
    int data_len = 1;
    int padding = width - data_len;

    if (!minus_flag)
    {

        add_padding(width, data_len, 0, state);
    }
    add_to_buffer(c, state);
    if (minus_flag && padding > 0)
    {
        for (i = 0; i < padding; i++)
        {

            add_to_buffer(' ', state);
        }
    }
}

static void add_string(const char *s, int width, int precision, int minus_flag, BufferState *state)
{
    int len = 0;
    int padding = 0;
    int i = 0;

    if (!s)
    {

        s = "(null)";
    }

    len = strlen(s);

    if (precision != -1 && precision < len)
    {

        len = precision;
    }

    padding = width - len;

    if (!minus_flag)
    {

        add_padding(width, len, 0, state);
    }

    for (i = 0; i < len; i++)
    {

        add_to_buffer(s[i], state);
    }

    if (minus_flag && padding > 0)
    {
        for (i = 0; i < padding; i++)
        {

            add_to_buffer(' ', state);
        }
    }
}

static void handle_string(char *s, int width, int precision, BufferState *state)
{
    int input_len = 0;
    int output_len = 0;
    char *p;
    int max_chars;

    if (!s)
    {

        s = "(null)";
    }

    input_len = strlen(s);
    max_chars = (precision != -1) ? precision : input_len;

    // Calculate output length for non-printables
    for (p = s; *p && max_chars > 0; p++, max_chars--)
    {
        if (*p < 32 || *p >= 127)
            output_len += 4;
        else
            output_len += 1;
    }

    add_padding(width, output_len, 0, state);

    max_chars = (precision != -1) ? precision : input_len;
    for (p = s; *p && max_chars > 0; p++, max_chars--)
    {
        unsigned char c = *p;
        if (c >= 32 && c < 127)
        {
            add_to_buffer(c, state);
        }
        else
        {
            add_to_buffer('\\', state);
            add_to_buffer('x', state);
            add_to_buffer("0123456789ABCDEF"[c >> 4], state);
            add_to_buffer("0123456789ABCDEF"[c & 0x0F], state);
        }
    }
}

static void handle_int(int num, int width, int precision, int minus_flag, BufferState *state)
{
    char num_str[20];
    int is_neg = num < 0;
    int i = 0, num_len = 0, leading_zeros = 0, total_len, padding, j = 0;

    if (num == 0)
    {
        if (precision == 0)
        {
            num_len = 0;
        }
        else
        {
            num_str[i++] = '0';
            num_len = 1;
        }
    }
    else
    {
        num = is_neg ? -num : num;
        while (num > 0)
        {
            num_str[i++] = '0' + (num % 10);
            num /= 10;
        }
        num_len = i;
    }

    if (precision != -1)
    {
        leading_zeros = precision - num_len;
        if (leading_zeros < 0)
            leading_zeros = 0;
        num_len += leading_zeros;
    }

    total_len = num_len + (is_neg ? 1 : 0);
    padding = width - total_len;

    if (!minus_flag && padding > 0)
    {
        for (j = 0; j < padding; j++)
            add_to_buffer(' ', state);
    }

    if (is_neg)
        add_to_buffer('-', state);

    for (j = 0; j < leading_zeros; j++)
        add_to_buffer('0', state);

    for (j = i - 1; j >= 0; j--)
        add_to_buffer(num_str[j], state);

    if (minus_flag && padding > 0)
    {
        for (j = 0; j < padding; j++)
            add_to_buffer(' ', state);
    }
}

static void handle_unsigned(unsigned int num, int base, int uppercase, int width, int precision, int minus_flag, BufferState *state)
{
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    char buffer[32];
    int i = 0, leading_zeros = 0, padding;
    int num_len = 0;
    int j = 0;

    if (num == 0)
    {
        if (precision == 0)
            i = 0;
        else
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

    num_len = i;

    if (precision != -1)
    {
        leading_zeros = precision - num_len;
        if (leading_zeros < 0)
            leading_zeros = 0;
        num_len += leading_zeros;
    }

    padding = width - num_len;

    if (!minus_flag && padding > 0)
    {
        for (j = 0; j < padding; j++)
            add_to_buffer(' ', state);
    }

    for (j = 0; j < leading_zeros; j++)
        add_to_buffer('0', state);

    for (j = i - 1; j >= 0; j--)
        add_to_buffer(buffer[j], state);

    if (minus_flag && padding > 0)
    {
        for (j = 0; j < padding; j++)
            add_to_buffer(' ', state);
    }
}

int _printf(const char *format, ...)
{
    int minus_flag = 0;
    int width = 0;
    int precision = -1;
    char length = 0;
    BufferState state = {.index = 0, .total = 0};
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;

            if (*format == '-')
            {
                minus_flag = 1;
                format++;
            }

            while (isdigit(*format))
            {
                width = width * 10 + (*format - '0');
                format++;
            }

            if (*format == '.')
            {
                format++;
                precision = 0;
                while (isdigit(*format))
                {
                    precision = precision * 10 + (*format - '0');
                    format++;
                }
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
                char c = va_arg(args, int);
                handle_char(c, width, minus_flag, &state);
                break;
            }
            case 's':
            {
                char *s = va_arg(args, char *);
                add_string(s, width, precision, minus_flag, &state);
                break;
            }
            case 'S':
            {
                char *S = va_arg(args, char *);
                handle_string(S, width, precision, &state);
                break;
            }
            case 'd':
            case 'i':
            {
                if (length == 'l')
                {
                    long num = va_arg(args, long);
                    handle_int(num, width, precision, minus_flag, &state);
                }
                else if (length == 'h')
                {
                    short num = (short)va_arg(args, int);
                    handle_int(num, width, precision, minus_flag, &state);
                }
                else
                {
                    int num = va_arg(args, int);
                    handle_int(num, width, precision, minus_flag, &state);
                }
                break;
            }
            case 'b':
            {
                unsigned int num = va_arg(args, unsigned int);
                handle_unsigned(num, 2, 0, width, -1, 0, &state);
                break;
            }
            case 'u':
            case 'o':
            case 'x':
            case 'X':
            {
                int base = (*format == 'u') ? 10 : (*format == 'o') ? 8
                                                                    : 16;
                int uppercase = (*format == 'X');
                if (length == 'l')
                {
                    unsigned long num = va_arg(args, unsigned long);
                    handle_unsigned(num, base, uppercase, width, precision, minus_flag, &state);
                }
                else if (length == 'h')
                {
                    unsigned short num = (unsigned short)va_arg(args, unsigned int);
                    handle_unsigned(num, base, uppercase, width, precision, minus_flag, &state);
                }
                else
                {
                    unsigned int num = va_arg(args, unsigned int);
                    handle_unsigned(num, base, uppercase, width, precision, minus_flag, &state);
                }
                break;
            }
            case '%':
                add_to_buffer('%', &state);
                break;
            default:
                add_to_buffer('%', &state);
                add_to_buffer(*format, &state);
                break;
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
    _printf("Width 5: |%5d|\n", 123);                        // | 123|
    _printf("Width 10: |%10s|\n", "hello");                  // | hello|
    _printf("Width 6: |%6S|\n", "Hi\t");
    _printf("Precision (d): %.5d\n", 42);               // 00042
    _printf("Left-aligned string: |%-10s|\n", "hello"); // |hello     |
    _printf("Left-aligned integer: |%-10d|\n", 123);    // |123       |
    _printf("Left-aligned hex: |%-5x|\n", 255);         // |ff   |
    _printf("With precision: |%-10.5d|\n", 42);         // |00042     |
    _printf("Right-aligned: |%10s|\n", "hello");        // |     hello|
    return 0;
}
