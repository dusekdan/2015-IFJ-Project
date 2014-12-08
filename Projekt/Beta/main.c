#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "errorHandler.c"
#include "inslist.h"
#include "ial.c"
#include "inslist.c"

FILE*fd=NULL;
tInsList IL;
//#include "inslist.c"
//#include "precedence3.c"
#include "parser.c"
#include "scanner2.c"
#include "interpret.c"

int main(int argc, char const *argv[])
{
    //if (argc!=1 && strcmp(argv[1],"-d")==0)debug=true;
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

    interpret(&rootTS, &IL);
   /* First(&IL);
    tInstruction *new;
    while(IL.active != NULL) {
        new = Copy(&IL);
        printf("%d\n", new->instype);
        Succ(&IL);
    }
*/
    disposeTable(&rootTS);
    if (localTS!=NULL) disposeTable(&localTS);
    fclose(fd);
    DisposeList(&IL);
    return 0;
}