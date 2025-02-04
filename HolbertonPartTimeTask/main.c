#include "main.h"

int main()
{
    // testing task 1
    int numOfCharsPrinted = _printf("Name: %s\nFirst Letter: %c\n%%\nTask 1 done!\n----------\n","Yasmeen",'Y');
    // testing task 2
    _printf("Number of chars Printed by s: %i\nNumber of chars Printed by i in negative: %i\nTask 2 done!\n----------\n",numOfCharsPrinted,-numOfCharsPrinted);
    // testing task 4
    _printf("Binary Num of 98 is : %b\nTask 3 done!\n----------\n",98);
    // testing task 5
    _printf("Unsigned int of 98: %u\nOctal representation of 98: %o\nHexdecimal representation of 98 using x: %x\nHexdecimal representation of 42267 using X: %X\nBinary representation of 98 is : %b\nTask 4 done!\n----------\n",98,98,98,42267,98);
    // testing task 7
    _printf("Testing S: %S\nTask 7 done!\n----------\n","Best \nSchool");
    // testing task 8
    _printf("Testing length modifier h with number -131411542: %hi\nTesting length modifier l with number -131411542: %li\n",-131411542, -131411542);
    _printf("Testing length modifier h with number 131411542: %hu\nTesting length modifier l with number 131411542: %lu\n",131411542, 131411542);
    _printf("Testing length modifier l with number octal: %lo\nTesting length modifier l with hexa number 42267: %lx\nTask 8 done!\n----------\n",42267, 42267);

    // testing task 9
    _printf("Name: %13s\n%%\nTask 9 done!\n----------\n","Yasmeen");
    // testing task 10
    _printf("Number of chars Printed: %.5u\nNumber of chars Printed by d in negative: %.10d\nTask 10 done!\n----------\n",numOfCharsPrinted,-numOfCharsPrinted);
    // testing task 11
    _printf("Left aligned: %-7iTask 11 done!\n----------\n",1312);

    return 0;
}
