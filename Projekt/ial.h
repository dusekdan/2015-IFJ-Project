/***********************ial.c************************************/
/* Soubor: ial.h - Hlavičkový soubor pro ial.c 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {			// typ ulozene promenne nebo funkce

	bool integer;
	bool real;
	bool boolean;
	bool string;
	bool empty;
} tType;

typedef struct {			// data promenne v symbolu

	int integer;
	double real;
	char *string;
	bool boolean;

} tContent;
/*  LEX ČÁST */
typedef struct {		// symbol

	char *name;				// nazev symbolu
	tType type; 			// datovy typ symbolu
	bool varType;			// true pokud je promenna, false pro funkci
	int argCount;			// pocet argumentu funkce, pro promennou hodnota NULL
	void *nextArg;			// ukazatel na dalsi argument funkce
	tContent content;		// obsah promenne		
} tData;


typedef struct tUzel {		// uzel tabulky symbolu

	int id;
	tData data;
	struct tUzel *rptr;
	struct tUzel *lptr;
} *tNodePtr;

extern tNodePtr *rootTS;		// ukazatel na zacatek tabulky symbolu


void quickSort(int pole[], int l, int r);		// quicksort, nejspise jen cast
void partition(int pole[], int *i, int *j);		// rozdeleni pro quickSort

void initTable(void);
void disposeTable();
void insertSymbol();
void searchSymbol();
void selectSymbol();

