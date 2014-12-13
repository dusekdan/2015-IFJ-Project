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
#include "inslist.h"
#include "ial.h"
#include "errorHandler.h"
#include "mariuspedersen.h"
tSmetisko Smetisko;
bool supertruck=false;
FILE*fd=NULL;
tInsList IL;
tInsList *localIL;
int numberOfExprInsts=0;
int trashCounter = 0;
#include "mariuspedersen.c"

#include "functions.c"



#include "ial.c"
#include "inslist.c"
#include "parser.h"



//#include "inslist.c"
//#include "precedence3.c"
#include "parser.c"
#include "scanner2.c"
#include "interpret.c"
#include "errorHandler.c"


/*
FILE*fd=NULL;
tInsList IL;
//#include "inslist.c"
//#include "precedence3.c"
#include "parser.c"
#include "scanner2.c"*/

int main(int argc, char const *argv[])
{
    InitMarius(&Smetisko);
    //printf("%d\n",argc );
    if (argc>=3)
    if (strcmp(argv[2],"-d")==0)
        debug=true;
    if (argc==4)
    if (strcmp(argv[3],"-mariuspedersen")==0)
        supertruck=true;

    InitList(&IL);

    fd = fopen(argv[1], "r");
    if(fd == NULL)
    {
        printf("Encountered an error while opening the file!\n");
        errorHandler(errInt);
    }



    startTable();
    tok = malloc (sizeof(struct token));
    InsertLastMarius (& Smetisko, tok);
    if (tok!=NULL)
    {
        getNextToken (fd,tok);
        nt_program (tok);
        /*printf("Run Interpret? (Y/<any>)\n");
        if(getchar()=='Y')*/
            interpret(&rootTS, &IL);
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
    
    mariuspedersen(&Smetisko);
    //DisposeMarius(&Smetisko);
    return 0;
}