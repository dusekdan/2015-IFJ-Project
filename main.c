/***********************main.c***********************************/
/* Soubor:  main.c - hlavni funkce programu                     */
/* Předmět: Formalní jazyky a překladače (IFJ)                  */
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I                                       */
/* Datum: prosinec 2014                                         */
/* Kódování: UTF-8                                              */
/* Autoři:          Roman Jaška (xjaska00)                      */
/*                  Matúš Bútora (xbutor01)                     */
/*                  Filip Kalous (xkalou03)                     */
/*                  Daniel Dušek (xdusek21)                     */
/****************************************************************/

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