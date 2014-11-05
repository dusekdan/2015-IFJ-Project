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
	scGREATER
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
	getToken(fd);
	getToken(fd);

	fclose(fd);

	return 0;

}

void getToken(FILE* fd)
{
	printf("Start.\n");
	char testToken[STRLENGTH]; // každých dalších 200 bude třeba reallocovat. Běžně by k tomu nemělo dojít.

	int c; // this is actually char, but we need its numerical representation
	int fcv = -1; // flow control variable; initialized on -1, because we increase it on the start of the loop
	int actState = 0; // počáteční stav
	bool terminator = false; // v moment kdy narazíme na :=<> atd. tak nastavujeme na true -> while končí
	bool inComment = false; // in case we are in comment, we skipt everything (and we need this variable to determine that)


	while ( ((c = fgetc(fd) ) != EOF) && terminator == false )
	{
		

		if(isspace(c) && actState != sSTRING)
		{
			continue;
		}

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
				printf("I skipped comment by the way...\n");
			}
			continue;
		}

		fcv++; // zvýšení flow control variable je na začátku cyklu, neboť je inicializovaná na -1
		

		switch(actState)
		{

			case sSTART:	

				printf("jsem tu\n");

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

				if( c >= '1' || c <= '9' )
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
				break;
				// když neskončíme v první podmínce, dále zjišťujeme co je příští status

			case sIDENT:
				
				if(isalpha(c) || c == '_' || isdigit(c))
				{
					testToken[fcv] = c;	// odpovídá-li znak povoleným znakům v identifikátoru, rozšíříme stávající token name/value o tuto hodnotu a cyklus jede odznovu
				}
				else	// nalezneme znak, který není platným identifikátorem => nastavujeme terminator na true
				{
					terminator = true;
					printf("Tady posilam TOKEN!\n");
					printf("Token name: %s\n",testToken);					
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
				
				printf("Jsem v scDCOMMA\n");
				
					if(c == '=')
					{
						testToken[fcv] = c;
					}

					if(strcmp(testToken, ":=") == 0)
					{
						printf("Token hotovy, posilam: %s\n", testToken);
						terminator = true;
					}

			break;

			case scLESS:
				printf("Jsem v scLESS\n");
			break;

			case scGREATER:
				printf("Jsem v scGREATER\n");
			break;

		}


		if(terminator)
		{

			printf("Before reset: %s\n", testToken);

			int i;
					for(i = 0; i<=sizeof(testToken); i++)
					{
						testToken[i] = '\0';
					}

					fcv = 0;

			fseek(fd, -1, SEEK_CUR);
			printf("The znak: %c\n", c); // after fseek
			printf("After reset: %s\n", testToken);
			break;

		}

	}
	printf("Konec.\n");
}