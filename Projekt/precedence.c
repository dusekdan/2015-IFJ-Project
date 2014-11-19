/***********************precedence.c*****************************/
/* Soubor: precednce.c - Syntaktická analýza výrazů 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/*					Matúš Bútora (xbutor01)						*/
/****************************************************************/

#include "precedence.h"


int main() {

	return 0;

}

void stackInit(tStack *stack) {

	stack->top = NULL;
}

bool stackEmpty(tStack *stack) {

	if(stack->top == NULL)
		return true;

	else
		return false;
}