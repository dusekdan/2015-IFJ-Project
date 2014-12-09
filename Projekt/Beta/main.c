/*#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#include "errorHandler.c"
#include "inslist.c"
#include "ial.c"*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#include "errorHandler.c"
#include "inslist.h"
tInsList *localIL;
#include "ial.c"
#include "inslist.c"

FILE*fd=NULL;
tInsList IL;

//#include "inslist.c"
//#include "precedence3.c"
#include "parser.c"
#include "scanner2.c"
#include "interpret.c"


/*
FILE*fd=NULL;
tInsList IL;
//#include "inslist.c"
//#include "precedence3.c"
#include "parser.c"
#include "scanner2.c"*/

int main(int argc, char const *argv[])
{
    //printf("%d\n",argc );
    if (argc==3)
    if (strcmp(argv[2],"-d")==0)
        debug=true;

    InitList(&IL);

    fd = fopen(argv[1], "r");
    if(fd == NULL)
    {
        printf("Encountered an error while opening the file!\n");
        errorHandler(errInt);
    }



    startTable();
    tok = malloc(sizeof(struct token));
    if (tok!=NULL)
    {
        getNextToken (fd,tok);
        nt_program (tok);
        free(tok);
    }
    else
    {
        errorHandler(errInt);
        return 1;
    }
/*
    printf("%sred\n", KRED);
    printf("%sgreen\n", KGRN);
    printf("%syellow\n", KYEL);
    printf("%sblue\n", KBLU);
    printf("%smagenta\n", KMAG);
    printf("%scyan\n", KCYN);
    printf("%swhite\n", KWHT);
    printf("%snormal\n", KNRM);*/

    //printf("ELEGEBLEGE %d\n",globalArr[0]->integer);
    interpret(&rootTS, &IL);
    disposeTable(&rootTS);
    if (localTS!=NULL) disposeTable(&localTS);
    fclose(fd);
    DisposeList(&IL);
    return 0;
}