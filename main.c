#include "main.h"
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

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

static void handle_char(char c, BufferState *state)
{
    add_to_buffer(c, state);
}

static void handle_string(char *s, BufferState *state)
{
    if (!s)
        s = "(null)";
    for (; *s; s++)
    {
        add_to_buffer(*s, state);
    }
}

static void handle_int(int num, BufferState *state)
{
    char num_str[12];
    int is_neg = num < 0;
    int i = 0;
    int j = 0;

    if (num == 0)
    {
        add_to_buffer('0', state);
        return;
    }

    if (is_neg)
        num = -num;

    while (num > 0)
    {
        num_str[i++] = '0' + (num % 10);
        num /= 10;
    }

    if (is_neg)
        num_str[i++] = '-';

    for (j = i - 1; j >= 0; j--)
    {
        add_to_buffer(num_str[j], state);
    }
}

static void handle_unsigned(unsigned int num, int base, int uppercase, BufferState *state)
{
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    char buffer[32];
    int i = 0;
    int j = 0;

    if (num == 0)
    {
        add_to_buffer('0', state);
        return;
    }

    while (num > 0)
    {
        buffer[i++] = digits[num % base];
        num /= base;
    }

    for (j = i - 1; j >= 0; j--)
    {
        add_to_buffer(buffer[j], state);
    }
}

int _printf(const char *format, ...)
{
    char c;
    char *s;
    int num;
    unsigned unsigned_num;
    BufferState state = {.index = 0, .total = 0};
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'c':
            {
                c = va_arg(args, int);
                handle_char(c, &state);
                break;
            }
            case 's':
            {
                s = va_arg(args, char *);
                handle_string(s, &state);
                break;
            }
            case 'd':
            case 'i':
            {
                num = va_arg(args, int);
                handle_int(num, &state);
                break;
            }
            case 'b':
            {
                unsigned_num = va_arg(args, unsigned int);
                handle_unsigned(unsigned_num, 2, 0, &state);
                break;
            }
            case 'u':
            {
                unsigned_num = va_arg(args, unsigned int);
                handle_unsigned(unsigned_num, 10, 0, &state);
                break;
            }
            case 'o':
            {
                unsigned_num = va_arg(args, unsigned int);
                handle_unsigned(unsigned_num, 8, 0, &state);
                break;
            }
            case 'x':
            {
                unsigned_num = va_arg(args, unsigned int);
                handle_unsigned(unsigned_num, 16, 0, &state);
                break;
            }
            case 'X':
            {
                unsigned_num = va_arg(args, unsigned int);
                handle_unsigned(unsigned_num, 16, 1, &state);
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
            // format++;
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
    return 0;
}
