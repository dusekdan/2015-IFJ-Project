
#include <time.h>

char *randstring(int length) {   

	int mySeed = 27054685;
    srand(time(NULL) * length * ++mySeed);

    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    size_t stringLen = 26*2+10+7;        
    char *randomString;

    randomString = malloc(sizeof(char) * (length));

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