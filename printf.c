#include "main.h"


char *buffer;
int buff_indx = 0;

int write_to_buffer(char c){
    if (buff_indx >= 1024) {
        write(1, buffer, buff_indx);
        buff_indx = 0;
    }
    buffer[buff_indx++] = c;
    return 1;
}

int	print_char(int c)
{
	//return write(1, &c, 1);
	return write_to_buffer((char)c);
}


int	print_digit(long n, int base)
{
	int		count;
	char	*symbols;

	symbols = "0123456789abcdef";
	if (n < 0)
	{
		//write(1, "-", 1);
		write_to_buffer('-');
		return print_digit(-n, base) + 1;
	}
	else if (n < base)
		return print_char(symbols[n]);
	else
	{
		count = print_digit(n / base, base);
		return count + print_digit(n % base, base);
	}
}

int	print_digit_X(long n, int base)
{
	int		count;
	char	*symbols;

	symbols = "0123456789ABCDEF";
	if (n < 0)
	{
		//write(1, "-", 1);
		write_to_buffer('-');
		return print_digit_X(-n, base) + 1;
	}
	else if (n < base)
		return print_char(symbols[n]);
	else
	{
		count = print_digit_X(n / base, base);
		return count + print_digit_X(n % base, base);
	}
}

int	print_str(char *s, int precision)
{
	int	count;
	count = 0;
	int i=0;
	int len = strlen(s);

	if(len<=precision){
	while (*s){
		//count += write(1, s++, 1);
		if( (*s>0&&*s<32) || *s >= 127){
            count+=write_to_buffer('\\');
            count+=write_to_buffer('x');
            if((long)*s < 16){
                count+=write_to_buffer('0');
                print_digit_X((long)*s,16);
            }
            else
                print_digit_X((long)*s,16);
            s++;
		}
		else
            count += write_to_buffer(*s++);
    }
	}
	else{
        while (*s){
        if(i>=precision)break;
		//count += write(1, s++, 1);
		if( (*s>0&&*s<32) || *s >= 127){
            count+=write_to_buffer('\\');
            count+=write_to_buffer('x');
            if((long)*s < 16){
                count+=write_to_buffer('0');
                print_digit_X((long)*s,16);
            }
            else
                print_digit_X((long)*s,16);
            s++;


		}
		else
            count += write_to_buffer(*s++);
        i++;
    }
	}
	return count;
}

int print_format(char specifier, va_list ap, char length_modifier, int width, int precision, int left_justify) {
    int count = 0;
    char *str;
    char pad_char = ' ';

    if (specifier == 'c') {
        char c = (char)va_arg(ap, int);
        int len = 1; // Because the length of the character is always 1
        if (width > len && !left_justify) {
            for (int i = 0; i < width - len; i++) {
                count += write_to_buffer(pad_char);
            }
        }
        count += write_to_buffer(c);
        if (width > len && left_justify) {
            for (int i = 0; i < width - len; i++) {
                count += write_to_buffer(pad_char);
            }
        }
    }
    else if (specifier == 's') {
        str = va_arg(ap, char *);
        int len = strlen(str);
        if (precision < len) {
            len = precision;
        }
        if (width > len && !left_justify) {
            for (int i = 0; i < width - len; i++) {
                count += write_to_buffer(pad_char);
            }
        }
        count += print_str(str, precision);
        if (width > len && left_justify) {
            for (int i = 0; i < width - len; i++) {
                count += write_to_buffer(pad_char);
            }
        }
    } else if (specifier == 'd' || specifier == 'i') {
	//Note that d and i are the same but the only difference when using scanf -- When I looked up information online, I found that it said this.
        long num;
        if (length_modifier == 'l') {
            num = va_arg(ap, int);
        } else if (length_modifier == 'h') {
            num = (short)va_arg(ap, int);
        } else {
            num = va_arg(ap, int);
        }
        char num_str[32];
        int len = sprintf(num_str, "%ld", num);
        if (precision > len) {
            for (int i = 0; i < precision - len; i++) {
                count += write_to_buffer('0');
            }
        }
        if (width > len && !left_justify) {
            for (int i = 0; i < width - len; i++) {
                count += write_to_buffer(pad_char);
            }
        }
        count += print_digit(num, 10);
        if (width > len && left_justify) {
            for (int i = 0; i < width - len; i++) {
                count += write_to_buffer(pad_char);
            }
        }
    } else if (specifier == 'u' || specifier == 'o' || specifier == 'x' || specifier == 'X') {
        unsigned long num;
        if (length_modifier == 'l') {
            num = va_arg(ap, unsigned int);
        } else if (length_modifier == 'h') {
            num = (unsigned short)va_arg(ap, unsigned int);
        } else {
            num = va_arg(ap, unsigned int);
        }
        char num_str[32];
        int len = sprintf(num_str, "%lu", num);
        if (precision > len) {
            for (int i = 0; i < precision - len; i++) {
                count += write_to_buffer('0');
            }
        }
        if (width > len && !left_justify) {
            for (int i = 0; i < width - len; i++) {
                count += write_to_buffer(pad_char);
            }
        }
        if (specifier == 'u') {
            count += print_digit(num, 10);
        } else if (specifier == 'o') {
            count += print_digit(num, 8);
        } else if (specifier == 'x') {
            count += print_digit(num, 16);
        } else if (specifier == 'X') {
            count += print_digit_X(num, 16);
        }
        if (width > len && left_justify) {
            for (int i = 0; i < width - len; i++) {
                count += write_to_buffer(pad_char);
            }
        }
    } else {
	//count += write(1, &specifier, 1);
        count += write_to_buffer(specifier);
    }
    return count;
}

int _printf(const char *format, ...) {
    va_list ap;
    int count = 0;
    buffer = (char *)malloc(1024 * sizeof(char));

    va_start(ap, format);
    while (*format) {
        if (*format == '%') {
            format++;
            int left_justify = 0;
            if (*format == '-') {
                left_justify = 1;
                format++;
            }
            int width = 0;
            int precision = -1;


            while (*format >= '0' && *format <= '9') {
                width = width * 10 + (*format - '0');
                format++;
            }


            if (*format == '.') {
                format++;
                precision = 0;
                while (*format >= '0' && *format <= '9') {
                    precision = precision * 10 + (*format - '0');
                    format++;
                }
            }

            char length_modifier = '\0';
            if (*format == 'l' || *format == 'h') {
                length_modifier = *format;
                format++;
            }
            count += print_format(*format, ap, length_modifier, width, precision, left_justify);
        } else {
            count += write_to_buffer(*format);
        }
        format++;
    }
    va_end(ap);

    if (buff_indx > 0) {
        write(1, buffer, buff_indx);
        buff_indx = 0;
    }


    return count;
}
