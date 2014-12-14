#include "main.h"

int main()
{
    InitMarius(&Smetisko);
    InitList(&IL);
    fd = fopen(argv[1], "r");
    if(fd == NULL)
    {
        fprintf(stderr,"Encountered an error while opening the file!\n");
        errorHandler(errInt);
    }
    startTable();
    tok = malloc (sizeof(struct token));
    InsertLastMarius (& Smetisko, tok);
    if (tok!=NULL)
    {
        getNextToken (fd,tok);
        nt_program (tok);
        interpret(&rootTS, &IL);
    }
    else
    {
        errorHandler(errInt);
        return 1;
    }
    mariuspedersen(&Smetisko);
    return 0;
}