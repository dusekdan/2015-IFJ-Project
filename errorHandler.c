/***********************errorHandler.c***************************/
/* Soubor: errorHandler.c - spracovani chyb                     */
/* Předmět: Formalní jazyky a překladače (IFJ)                  */
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I                                       */
/* Datum: prosinec 2014                                         */
/* Kódování: UTF-8                                              */
/* Autoři:          Roman Jaška (xjaska00)                      */
/****************************************************************/

int errorHandler (int errorType)
{
    fprintf(stderr,"Chyba %d\n",errorType);
    mariuspedersen(&Smetisko);
    DisposeMarius(&Smetisko);
    exit(errorType);
}