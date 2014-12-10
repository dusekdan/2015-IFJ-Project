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

void copy(char *string, int i, int n) {		// danova prace
	
	int strLength = length(string);
	int newStringLength = strLength-i;
	
	char *tmpString = malloc(newStringLength);	// char is 1 by default so...
		if(tmpString == NULL)
		{
			printf("Failed to malloc!\n");
			exit(99);
		}
	
	int j;
	for(j = i-1; j <= newStringLength; ++j)
	{
		
	}
	// DOPÍČE POSRATÉ TO JE KURVA UŽ!

}


int main() {

	char *string = "'x'#10'z'";

	copy(string, 1, 4);

	return 0;

}
