/***********************mariuspedersen.c*************************/
/* Soubor: mariuspedersen.c - garbage collector                 */
/* Předmět: Formalní jazyky a překladače (IFJ)                  */
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I                                       */
/* Datum: prosinec 2014                                         */
/* Kódování: UTF-8                                              */
/* Autoři:          Roman Jaška  (xjaska00)                     */
/*                  Matúš Bútora (xbutor01)                     */
/****************************************************************/

typedef struct Kontajner
{
    void * Odpad;
    struct Kontajner *next;
} *tKontajner;

typedef struct 
{
    struct Kontajner *first;
    struct Kontajner *active;
    struct Kontajner *last;
} tSmetisko;
