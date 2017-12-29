/***********************main.h***********************************/
/* Soubor:  main.h - hlavni funkce programu                     */
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "inslist.h"
#include "ial.h"
#include "errorHandler.h"
#include "mariuspedersen.h"
bool stmtMustntBeEmpty=false;
tSmetisko Smetisko;
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
#include "parser.c"
#include "scanner2.c"
#include "interpret.c"
#include "errorHandler.c"