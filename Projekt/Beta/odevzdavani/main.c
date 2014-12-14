#include "main.h"

int main(int argc, char * argv [])
{
    InitMarius(&Smetisko);
    InitList(&IL);
    if (argc != 2)
        exit(errInt);
    fd = fopen(argv[1], "r");
    if(fd == NULL)
    {
        fprintf(stderr,"Encountered an error while opening the file!\n");
        exit(errInt);
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