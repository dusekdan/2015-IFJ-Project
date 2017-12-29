/***********************ial.c************************************/
/* Soubor: ial.c - Obsahujicí funkce do předmětu IAL 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03) xdusek21 Daniel Dušek						*/
/****************************************************************/

#include "funkcie.h"

int length(char *string)
{
	return strlen(string);
}

char*  copy(char *string, int i, int n) {		// danova prace
	
	// vzdálenost mezi i a n je n
	
	char *tmpString = malloc(n);	// char is one by its definition 
	
		if(tmpString == NULL)
		{
			printf("Allocation error!\n");
			exit(99);
		}
	
	for(int c = 0; c < 7; c++)
	{
		tmpString[c] = string[(i+c)];
	}

	return tmpString;
}


int main() {

	char *string = "'x'#10'z'";

	copy(string, 1, 4);

	return 0;

}
