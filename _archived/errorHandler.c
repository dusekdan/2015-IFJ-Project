#include "errorHandler.h"

int errorHandler (int errorType)
{
    /*switch (errorType)
    {
        case 1:     break;
        default:    printf("Unspecified error occured.\n");
                    break;
    }*/
    printf("chyba %d\n",errorType );
    return(errorType);
}