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
	sHCOMMA,
	sWHITESPACE
};

char* keywords[] = {
	"begin", "boolean", "do", "else", "end", "false", "find", "forward", "function",
	"if", "integer", "readln", "real", "sort", "string", "then", "true", "var", "while",
	"write"
};

/*
 *
 *	This main function only simulates requests from parser, which this subprogram will be receiving
 *
 */

int main()
{

	FILE* fd = fopen("testfile1", "r");
		
		if(fd == NULL)
		{
			printf("Unable to open the file!\n");
			return 1;
		}

	printf("*************************************************************************\n");	// Just a tool to differ between start of the log and new instance in console

	// Parser decides he wants lexem, only one for now, so he calls getToken function
	getToken(fd); // passes file decriptor of the file he opened, at this moment getToken write on stdout, therefore is void, in future it will be returning id & value of the lexem through pointers that will be passed to it

	// Parser decides he wants more lexems, so he gets it.
	getToken(fd);	// 2
	getToken(fd);	// 3
	getToken(fd);	// 4
	getToken(fd);	// 5
	getToken(fd);	// 6
	getToken(fd);	// 7
	getToken(fd);	// 8

	fclose(fd); // what has been opened, needs to be closed again
	
	printf("*************************************************************************\n");	// Just a tool to differ between start of the log and new instance in console

	return 0;	// when main runs through successfuly returns zero

}

int getNextChar(FILE* fd)
{
	// get the character
	int nextCharacter = fgetc(fd);

	// fseek it back, so you don't really move the file descriptor
	if(nextCharacter != EOF)
	{
		fseek(fd, -1, SEEK_CUR);
	}
	// returning the char (as int)
	return nextCharacter;
}

void getToken(FILE* fd)
{
	printf("!--- Function getToken() started. ---!\n");

	char testToken[STRLENGTH];

	int c; // we need char represented as integer, because most of the functions requires it
	int cx; // the character after the c

	int actState = 0; // starting state for final state automat
	int fcv = -1; // flow control variable; initialized on -1, because we increase it on the start of the loop

	while ( (c = fgetc(fd)) != EOF)
	{

		cx = getNextChar(fd);
		printf("(%c->%c)", c, cx);


	}
	

	printf("\n!--- Function getToken() ended.   ---!\n");
}

