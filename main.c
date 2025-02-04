#include "main.h"
#include "printf.c"

int	main()
{
    int count;

    //Test the code for task 1,2
    count = _printf("Hello %s\n","Ahmed");
    printf("%d\n",count);
    count = _printf("Your grade is %c\n",'B');
    printf("%d\n",count);
    count = _printf("%d %i\n",10,10);
    printf("%d\n",count);

    //Test the code for Task 4
    count = _printf("%b\n",98);
    printf("%d\n",count);

    //Test the code for Task 5
    count = _printf("%u\n",43);
    printf("%d\n",count);
    count = _printf("%o\n",43);
    printf("%d\n",count);
    count = _printf("%x\n",43);
    printf("%d\n",count);
    count = _printf("%X\n",42);
    printf("%d\n",count);

    //Test the code for Task 7
    count = _printf("%s\n","Hello\nSchool");
    printf("%d\n",count);

    //Test the code for Task 8
    count = _printf("%hi\n",33000); //Exceeds The Range
    printf("%d\n",count);
    count = _printf("%hi\n",30000); //In Range
    printf("%d\n",count);

    //Test the code for Task 9,10,11
    count = _printf("%5.2s\n","Ahmed");
    printf("%d\n",count);
    count = _printf("%7u\n",55211);
    printf("%d\n",count);

    free(buffer);
    return 0;
}
