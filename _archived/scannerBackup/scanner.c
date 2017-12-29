#include "scanner.h"
#define STRLENGTH 200
#define APOSTROF_ASCII 39

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
	sOPERATOR,
	scDCOMMA,
	scLESS,
	scGREATER,
	sHCOMMA
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
			printf("Unable to open the file!\n");
			return 1; // every time you dont manage to open file, error
		}

	printf("***********************************************************************\n");
	
	printf("\n1.\n");
	getToken(fd);
	printf("\n2.\n");
	getToken(fd);
	printf("\n3.\n");
	getToken(fd);
	printf("\n4.\n");
	getToken(fd);
	printf("\n5.\n");
	getToken(fd);
	printf("\n6.\n");
	getToken(fd);
	printf("\n7.\n");
	getToken(fd);
	printf("\n8.\n");
	getToken(fd);

	fclose(fd);

	return 0;

}

void getToken(FILE* fd)
{
	printf("----- Funkce getToken()...started. -----\n");
	char testToken[STRLENGTH]; // každých dalších 200 bude třeba reallocovat. Běžně by k tomu nemělo dojít.

	int c; // this is actually char, but we need its numerical representation
	int fcv = -1; // flow control variable; initialized on -1, because we increase it on the start of the loop
	int actState = 0; // počáteční stav
	bool terminator = false; // v moment kdy narazíme na :=<> atd. tak nastavujeme na true -> while končí
	bool fseekD = true; // kvůli potřebě potlačit nutnost posouvat se o jeden znak (když nenačítáme jeden napřed), umožňuji rozhodovat zda se bude nebo nebude fseekovat
	bool inComment = false; // in case we are in comment, we skipt everything (and we need this variable to determine that)


	while ( ((c = fgetc(fd) ) != EOF) && terminator == false )
	{


		if(isspace(c) && actState != sSTRING)
		{
			continue;
		}


		// this is the part where I test if next char is  eof
		int nx = fgetc(fd);
		bool forceSend = false;
		if(nx == EOF)
		{
			printf("END OF FILE NEXT CHARACTER NIGGA!\n");	
			forceSend = true;
		}
		fseek(fd, -1, SEEK_CUR);



		if(c == '{' && actState != sSTRING)
		{
			inComment = true;
			continue;
		}

		if(inComment)
		{
			if(c == '}')
			{
				inComment = false;
				printf("*Commentary skipped!\n");
			}
			continue;
		}

		fcv++; // zvýšení flow control variable je na začátku cyklu, neboť je inicializovaná na -1
		
		printf("Entering the #%d with char: '%c'\n", fcv, c);

		switch(actState)
		{

			case sSTART:	

				printf("*Case: sSTART entered!\n");

				if( isalpha(c) || c == '_')
				{
					actState = sIDENT;
					testToken[fcv] = c;
					break; // we breake it here, because we identified what's next state;
				}

				if( c == APOSTROF_ASCII )
				{
					actState = sSTRING;
					testToken[fcv] = c;
				}

				if ( c == '0')
				{
					actState = sZERO;
					testToken[fcv] = c; // this part is questionable
				}

				if( c >= '1' && c <= '9' )
				{
					actState = sNUMBER;
					testToken[fcv] = c;
				}

				if(c == '+' || c == '-' || c == '^')
				{
					actState = sOPERATOR;
					testToken[fcv] = c;
				}

				if(c == ':')
				{
					actState = scDCOMMA;
					testToken[fcv] = c;
				}

				if(c == '<')
				{
					actState = scLESS;
					testToken[fcv] = c;
				}

				if(c == '>')
				{
					actState = scGREATER;
					testToken[fcv] = c;
				}

				if( c == ';')
				{
					actState = sHCOMMA;
					testToken[fcv] = c;
				} 
				break;
				// když neskončíme v první podmínce, dále zjišťujeme co je příští status

			case sIDENT:
				
				printf("*Case: sIDENT entered!\n");

				if(isalpha(c) || c == '_' || isdigit(c))
				{
					testToken[fcv] = c;	// odpovídá-li znak povoleným znakům v identifikátoru, rozšíříme stávající token name/value o tuto hodnotu a cyklus jede odznovu
				}
				else	// nalezneme znak, který není platným identifikátorem => nastavujeme terminator na true
				{
					terminator = true;
					testToken[fcv] = '\0'; // making sure that string is null terminated... you know that joke ... :)
					printf("-> Sending a Token: %s\n", testToken);			
					printf("%s", testToken);
				}

			break;

			case sSTRING:
				printf("Jsem v sString\n");
			break;

			case sZERO:
				printf("Jsem v sZERO\n");
			break;

			case sNUMBER:
				printf("Jsem v sNUMBER\n");
			break;

			case sOPERATOR:
				printf("Jsem v sOPERATOR\n");
			break;

			case scDCOMMA:
				
				// sem dorazím až v moment, kdy jsem prokazatelně načetl dvojtečku
				printf("*Case scDCOMMA entered\n");
					
					// po dvojtečce může následovat buď "cokoliv jiného" nebo "rovná se"
					if(c != '=') // nenásleduje-li rovnítko, víme, že odesíláme dvojtečku jako TOKEN.
					{
						terminator = true;
						printf("->Sending a Token: %s\n", testToken);
						printf("%s", testToken);
					}
					else
					{
						terminator = true;
						fseekD = false;
						testToken[fcv] = c;
						testToken[(fcv+1)] = '\0'; // null terminated string je dobry string
						printf("->Sending a Token: %s\n", testToken);
						printf("%s", testToken);
					}


			break;

			case scLESS:
				printf("Jsem v scLESS\n");
			break;

			case scGREATER:
				printf("Jsem v scGREATER\n");
			break;
			case sHCOMMA:
				printf("*Case: sHCOMMA entered\n");
				
				terminator = true;
				printf("->Sending a Token: %s\n", testToken);
				printf("%s", testToken);

			break;

		}

		if(forceSend)
		{
			printf("->Sending a forced Token: %s\n", testToken);
			
		}

		if(terminator)
		{

			int i;
			for(i = 0; i<=sizeof(testToken); i++)
			{
				testToken[i] = '\0';
			}

			fcv = 0;	// so much to the reseting tokenVariable & flow control variable
			
			if(fseekD)
			{
				fseek(fd, -1, SEEK_CUR);	// protože znak budeme potřebovat načíst při přístím průchodu cyklem, posouváme se v souboru o jedno zpět
			}



			// pojistný odeslání tokenu

			break;

		}



		

	}
	//#printf("----- Funkce getToken() ...ended -----\n");
}