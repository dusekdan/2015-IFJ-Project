#include "scanner.h"

enum states {

	sSTART,		// pocatecni stav
	sIDENT,		// identifikator
	sSTRING,	// retezec
	sESC, 		// escape sekvence
	sCOMENT, 	// komentar
	sOPERATORS,	// dvouznakove operatory
	sNUMBER,
	sZERO,
	sREAL,
	sPREEXP,
	sEXP,	
	sSIGN,
};

char* keywords[] = {
	"begin", "boolean", "do", "else", "end", "false", "find", "forward", "function",
	"if", "integer", "readln", "real", "sort", "string", "then", "true", "var", "while",
	"write"
};


int main()
{

	FILE* fd = fopen("testfile1", "r");
		if(fd == NULL)
		{
			printf("Nepodarilo se otvorit subor. Madarsko ftw!\n");
			return 1; // every time you dont manage to open file, error
		}

	getToken(fd);

	fclose(fd);

	return 0;

}


void getToken(FILE* fd)
{


	char c; // co asi
	int actState = 0; // nastavujeme na nulu, neb 0 je start, a to je počáteční stav.
	tToken token;

	while( (c = fgetc(fd) ) != EOF )
	{
	
		switch(actState)
		{
			case sSTART:

				if( isalpha(c) || c == '_')
				{
					actState = sIDENT;

					token.attribute = c;	// uložit znak do tokenu (asi jako jeho value), breaknout switch a cyklem najet do nového stavu
					
				}
				break;
			
			case sIDENT:
			token.attribute = strcat(token.attribute, c);
			break;

		}

		printf("%s", token.attribute);
	


	}

	printf("Ahoj, jsem na konci getTokenu.\n");

}