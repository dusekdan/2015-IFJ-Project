/***********************ial.c************************************/
/* Soubor: ial.c - Obsahujicí funkce do předmětu IAL 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/****************************************************************/

#include "funkcie.h"

int length(char *s) {
	
	printf("%s\n", s);
	return 0;

}


void copy(char *s) {

	printf("%s\n", s);

}


int main() {

	char *string = "'x'#10'z'";

	length(string);
	copy(string);

	return 0;

}