#include "errorHandler.h"

int errorHandler (int errorType)
{
    switch (errorType)
    {
        case 1:     break;
        default:    printf("Unspecified error occured.\n");
                    break;
    }
    printf("%schyba %d%s\n",KRED,errorType,KNRM );
    exit(errorType);
}