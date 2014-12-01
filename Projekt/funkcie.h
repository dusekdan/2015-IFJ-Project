/***********************funkcie.c************************************/
/* Soubor: ial.c - Obsahujicí funkce do předmětu IAL 				*/
/* Předmět: Formalní jazyky a překladače (IFJ) 						*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  	*/
/* Varianta zadání: b/1/I 											*/
/* Datum: prosinec 2014												*/
/* Kódování: UTF-8													*/
/* Autoři:			Filip Kalous (xkalou03)							*/
/*					Matúš Bútora (xbutor01)							*/
/********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void copy(char *s, int beginStr, int count);