#include "main.h"

// func to count the length of the string
int countStrLength (char * str){
    int length = 0;
    while (str != NULL && str[length]!='\0'){
        length++;
    }
    return length;
}
// same as sprintf but implemented it my own since it is not mentioned to use it
char* convertIntToStr(int num, bool isSigned){
    char numInStrReverse [12];
    int currentStrIndex = 0;
    bool isNeg = false;
    if (isSigned && num <0){
        isNeg = true;
        num = 0-num;
    }
    if (num == 0){
        numInStrReverse[currentStrIndex]='0';
        currentStrIndex++;
    }

    while (num>0){
        int lastDigit = num%10;
        numInStrReverse[currentStrIndex] = (char)(lastDigit+'0');
        currentStrIndex++;
        num/=10;
    }
    if (isSigned && isNeg){
        numInStrReverse[currentStrIndex] = '-';
        currentStrIndex++;
    }
    char *numInStr = (char *)malloc(currentStrIndex + 1 * sizeof(char));

    for (int i = 0 ;i<currentStrIndex; i++){
        numInStr[currentStrIndex - i - 1] = numInStrReverse[i];
    }
    numInStr[currentStrIndex] = '\0';
    return numInStr;

}
char hexComplement(int num){
    switch (num){
        case 10: return 'A';
        case 11: return 'B';
        case 12: return 'C';
        case 13: return 'D';
        case 14: return 'E';
        case 15: return 'F';
        default: return (char)(num + '0');
    }

}
// func to handle decimal to binary conversion
char* numericConversion(int num, char conversionType){
    int conversionNumLimit = 20;
    char numReverse [conversionNumLimit];
    int currentLength= 0;
    if (num == 0){
        numReverse[currentLength] = '0';
        currentLength++;
    }
    int conversionDivider;
    if (conversionType == 'b') conversionDivider = 2;
    else if (conversionType == 'x' || conversionType == 'X') conversionDivider = 16;
    else if (conversionType == 'o') conversionDivider = 8;

    while(num >0){
        if (conversionType == 'x' || conversionType == 'X')
            numReverse[currentLength] = hexComplement(num % conversionDivider) ;
        else
            numReverse[currentLength] = (char)((num % conversionDivider)+'0') ;
        num = num/conversionDivider;
        currentLength++;
    }
    char *numInSTR = (char *)malloc(currentLength * sizeof(char));
    for (int i = 0 ;i<currentLength; i++){
        numInSTR[currentLength - i - 1] = numReverse[i];
    }
    numInSTR[currentLength] = '\0';
    return numInSTR;
}

char * writeWithBuffer (char * currentStr, int currentStrLength, int bufferLimit){
    write(1, currentStr, currentStrLength);
    free(currentStr);
    return (char *)malloc(bufferLimit * sizeof(char));
}
char* isAsciiValid(char c){
    int asciiValue = (int) c;
    if ((asciiValue > 0 && asciiValue < 32) || asciiValue>=127){
            char * tempConversion = numericConversion(asciiValue,'x');
            int count =countStrLength(tempConversion);
            char *conversion = (char *)malloc(5 * sizeof(char));
            conversion[0] = '\\'; conversion[1] = 'x';
            if (count<2){
                conversion[2] = '0'; conversion[3]=tempConversion[0];
            }
            else {conversion[2] = tempConversion[0]; conversion[3]=tempConversion[1];}
            conversion[4] = '\0';
        return conversion;
    }
    else {
      char *valid =(char *)malloc(2 * sizeof(char));
      valid[0] =c;
      valid[1]='\0';
      return valid;
    }
}
int _printf(const char *format, ...){
    // variable to hold number of characters limit
    int bufferLimit = 1024;
    // variable to hold the number of characters printed
    int numOfCharsPrinted = 0;
    // holding the arguments
    va_list listPtr;
    va_start(listPtr, format);

    char *currentStr = (char *)malloc(bufferLimit * sizeof(char));
    int currentStrLength = 0;
    int index = 0;

    while (format[index]!= '\0'){
        // if current char is % this means a specifier is next
        if(format[index] =='%' ){
            // hold the specfier
            char specifier = format[index+1];
            char lengthModfier = ' ';
            int widthField = 0;
            int fieldPrecision = 0;
            bool leftAligned = false;

            if (specifier == '-'){
                leftAligned = true;
                index++;
                specifier = format[index+1];
            }

            if (specifier == 'l' || specifier=='h'){
                lengthModfier = specifier;
                index++;
                specifier = format[index+1];
            }
            if ((int)specifier >= 48 && (int)specifier <=57){
                widthField = specifier-'0';
                index++;
                specifier = format[index+1];
                while((int)specifier >= 48 && (int)specifier <=57){
                    widthField *=10;
                    widthField+=specifier-'0';
                    index++;
                    specifier = format[index+1];
                }
                if (!leftAligned){
                    if (currentStrLength + widthField >= 1024)
                        currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                    for (int i=0;i<widthField;i++){
                    currentStr[currentStrLength++] = ' ';
                    numOfCharsPrinted++;
                    }
                }
            }
            if (specifier == '.'){
                fieldPrecision = format[index+2] -'0';
                index+=2;
                specifier = format[index+1];
                while((int)specifier >= 48 && (int)specifier <=57){
                    fieldPrecision *=10;
                    fieldPrecision+=specifier-'0';
                    index++;
                    specifier = format[index+1];
                }
            }

            switch(specifier){
            case '%':
                if (currentStrLength + 1 >= 1024)
                    currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                currentStr[currentStrLength++] = '%';
                numOfCharsPrinted++;
                break;
            case 'c':{
                char argument = va_arg(listPtr,int);
                if (currentStrLength + 1 >= 1024)
                    currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                currentStr[currentStrLength++] = argument;
                numOfCharsPrinted++;
                break;
            }
            case 's':{
                char * strArgument = va_arg(listPtr,char*);
                int strArgumentLength = countStrLength(strArgument);
                if (currentStrLength + strArgumentLength >= 1024)
                    currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                for (int i=0; i<strArgumentLength;i++)
                    currentStr[currentStrLength++] = strArgument[i];
                numOfCharsPrinted+= strArgumentLength;
                break;
            }
            case 'd':
            case 'i':{
                char * argumentAsStr;
                if (lengthModfier == 'h'){
                    short argument = va_arg(listPtr,signed int);
                    argumentAsStr = convertIntToStr (argument, true);
                }
                else if (lengthModfier == 'l'){
                    long argument = va_arg(listPtr,long);
                    argumentAsStr = convertIntToStr (argument, true);
                }
                else{
                    int argument = va_arg(listPtr,signed int);
                    argumentAsStr = convertIntToStr (argument, true);
                }
                int argumentLength  = countStrLength(argumentAsStr);
                int fieldDifference = 0;
                if (fieldPrecision > 0)
                    fieldDifference= abs(fieldPrecision - argumentLength);
                if (currentStrLength + fieldDifference + argumentLength >= 1024)
                    currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                while (!leftAligned && fieldDifference > 0)
                {
                    currentStr[currentStrLength++] = '0';
                    fieldDifference--;
                    numOfCharsPrinted++;
                }
                for (int i=0; i<argumentLength;i++)
                    currentStr[currentStrLength++] = argumentAsStr[i];
                while (leftAligned && fieldDifference > 0)
                {
                    currentStr[currentStrLength++] = '0';
                    fieldDifference--;
                    numOfCharsPrinted++;
                }
                numOfCharsPrinted+=argumentLength;
                break;
            }
            case 'b':
            case 'x':
            case 'X':
            case 'o':{
                char * argumentAsStr;
                if (lengthModfier == 'h'){
                    short argument = va_arg(listPtr, int);
                    argumentAsStr = numericConversion (argument, specifier);
                }
                else if (lengthModfier == 'l'){
                    long argument = va_arg(listPtr,long);
                    argumentAsStr = numericConversion (argument, specifier);
                }
                else{
                    int argument = va_arg(listPtr,int);
                    argumentAsStr = numericConversion (argument, specifier);
                }
                int argumentLength = countStrLength(argumentAsStr);
                 int fieldDifference = 0;
                if (fieldPrecision > 0)
                    fieldDifference= abs(fieldPrecision - argumentLength);
                if (currentStrLength + fieldDifference + argumentLength >= 1024)
                    currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                while (!leftAligned && fieldDifference > 0)
                {
                    currentStr[currentStrLength++] = '0';
                    fieldDifference--;
                    numOfCharsPrinted++;
                }
                for (int i=0; i<argumentLength;i++)
                    currentStr[currentStrLength++] = argumentAsStr[i];
                while (leftAligned && fieldDifference > 0)
                {
                    currentStr[currentStrLength++] = '0';
                    fieldDifference--;
                    numOfCharsPrinted++;
                }
                numOfCharsPrinted+=argumentLength;
                break;
            }
            case 'u':{
                char * argumentAsStr;
                if (lengthModfier == 'h'){
                    short argument = va_arg(listPtr,unsigned int);
                    argumentAsStr = convertIntToStr (argument, false);
                }
                else if (lengthModfier == 'l'){
                    long argument = va_arg(listPtr,long);
                    argumentAsStr = convertIntToStr (argument, false);
                }
                else{
                    int argument = va_arg(listPtr,unsigned int);
                    argumentAsStr = convertIntToStr (argument, false);
                }
                int argumentLength = countStrLength(argumentAsStr);
                 int fieldDifference = 0;
                if (fieldPrecision > 0)
                    fieldDifference= abs(fieldPrecision - argumentLength);
                if (currentStrLength + fieldDifference + argumentLength >= 1024)
                    currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                while (!leftAligned && fieldDifference > 0)
                {
                    currentStr[currentStrLength++] = '0';
                    fieldDifference--;
                    numOfCharsPrinted++;
                }
                for (int i=0; i<argumentLength;i++)
                    currentStr[currentStrLength++] = argumentAsStr[i];
                while (leftAligned && fieldDifference > 0)
                {
                    currentStr[currentStrLength++] = '0';
                    fieldDifference--;
                    numOfCharsPrinted++;
                }
                numOfCharsPrinted+=argumentLength;
                break;
            }
            case 'S':{
                char * strArgument = va_arg(listPtr,char*);
                int strArgumentLength = countStrLength(strArgument);
                if (currentStrLength + strArgumentLength >= 1024)
                    currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                char c;
                for (int i=0; i<strArgumentLength;i++){
                    if (strArgument[i] == '\\'){
                        char c = '\\'+strArgument[i+1];
                        i++;
                    }
                    else c = strArgument[i];
                    char* validAscii = isAsciiValid(c);
                    for (int j = 0; j < countStrLength(validAscii);j++){
                        currentStr[currentStrLength++] = validAscii[j];
                        numOfCharsPrinted++;
                    }
                }
                //numOfCharsPrinted+= strArgumentLength;
                break;
            }



        }
        index+=2;
        if (leftAligned && widthField>0){
                if (currentStrLength + widthField >= 1024)
                        currentStr = writeWithBuffer(currentStr, currentStrLength, bufferLimit);
                    for (int i=0;i<widthField;i++){
                    currentStr[currentStrLength++] = ' ';
                    numOfCharsPrinted++;
                    }
            }
        }
        else{

            // if not a specifier append the string to the array
            currentStr[currentStrLength] = format[index];
            currentStrLength++;
            numOfCharsPrinted++;
            index++;
        }

    }

// if format string ended and there is still chars in the buffer write them
if (currentStrLength!=0)
    writeWithBuffer(currentStr, currentStrLength, bufferLimit);

return numOfCharsPrinted;
}
