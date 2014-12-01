/***********************ial.c************************************/
/* Soubor: ial.h - Hlavičkový soubor pro ial.c 			        */
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)                     */
/*                  Roman Jaška  (xjaska00)						*/
/****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include "parser.h"


#ifndef max
	#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

typedef struct {			// data promenne v symbolu

	int integer;
	double real;
	char *string;
	bool boolean;

} tContent;
/*  LEX ČÁST */

typedef struct tData {		// symbol

	char *name;				// nazev symbolu
	int type; 			    // datovy typ symbolu
	int argCount;			// pocet argumentu funkce, pro promennou hodnota NULL
	struct tNodePtr *localTSadr; //Adresa lokalnej tabulky
	//void *nextArg;			// ukazatel na dalsi argument funkce
	tContent content;		// obsah promenne		
} *tData;


typedef struct tNodePtr {		// uzel tabulky symbolu

	char *key;
	tData data;

	struct tNodePtr *rptr;
	struct tNodePtr *lptr;
} *tNodePtr;

extern tNodePtr rootTS;		// ukazatel na zacatek tabulky symbolu


void quickSort(char *text, int l, int r);		// quicksort, nejspise jen cast
void partition(char *text, int *i, int *j);		// rozdeleni pro quickSort

void init(tNodePtr *rootTS);
void initTable(tNodePtr *rootTS);
void disposeTable(tNodePtr *rootTS);
tNodePtr createNode(char *key, tData data);
tNodePtr insertSymbol(tNodePtr *rootTS, char *key, tData data);
tNodePtr searchSymbol(tNodePtr *rootTS, char* key);

int BMASearch();
void BMACountBadChar();
void BMACountGoodSuffix();
void BMASuffixes();

