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



typedef struct Item {		// struktura polozky stromu

	int id;
	struct Item *rptr;
	struct Item *lptr;
} *ItemPtr;


void quickSort(int pole[], int l, int r);
void partition(int pole[], int *i, int *j)