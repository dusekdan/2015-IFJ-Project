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

int length(char *string)
{
	return strlen(string);
}

void copy(char *s, int beginStr, int count) {		// filipova prace

	char c;
	int i = beginStr;

	while(count != 0) {

		c = s[i];

		// printf("%c", c);

		count--;
		i++;

	}

	printf("\n");

}


int main() {

	char *string = "'x'#10'z'";

	copy(string, 1, 4);

	return 0;

}
