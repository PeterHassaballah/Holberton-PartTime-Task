#include "main.h"
#include <unistd.h>
#include <stdarg.h>

int _printf(const char *format, ...)
{
    va_list args;
    int count = 0;
    char *s;
    char c;

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
    return 0;
}
