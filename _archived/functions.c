
#include <time.h>

char *randstring(int length) {   

    srand(time(NULL) * length * (rand() % 100000000));

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

char * funCopy(char *string, int i, int n) {       // danova prace
    
    // vzdálenost mezi i a n je n
   if(i > strlen(string))
   {
        errorHandler(errRunRest);
   }
    int poss = strlen(string) - (i - 1);

    n = (n > poss) ? poss : n;


    char *tmpString = malloc(n*(sizeof(char)));    // char is one by its definition    
        if(tmpString == NULL)
        {
            printf("Allocation error!\n");
            exit(99);
        }
        else
            InsertLastMarius(& Smetisko, tmpString);
    
    for(int c = 0; c < n; c++)
    {
        tmpString[c] = string[(i-1+c)];
    }

    return tmpString;
}
