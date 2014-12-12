
#include <time.h>

char *randstring(int length) {   

    srand(time(NULL) * length * (rand() % 100000000));

    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    size_t stringLen = 26*2+10+7;        
    char *randomString;

    randomString = malloc(sizeof(char) * (length));
    InsertLastMarius(& Smetisko, randomString);

    if (!randomString) {
        return (char*)0;
    }

    unsigned int key = 0;

    for (int n = 0;n < length;n++) {          
        key = rand() % stringLen;          
        randomString[n] = string[key];
    }

    //randomString[length] = '\0';

    return randomString;
}