#include "scanner.h"
#include "parser.h"
#define STRLENGTH 200
#define APOSTROF_ASCII 39


// states defined in parser.h as static const integer variables (constants)

/*enum states {

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
	scCOMMA,
	scLESS,
	scGREATER,
	sHCOMMA,
	sWHITESPACE
};*/

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


	int* type_ptr;	// the type of lexem that is being send
	char att_ptr[STRLENGTH];	// value of the lexem
	type_ptr = malloc(sizeof(int));



	printf("*************************************************************************\n");	// Just a tool to differ between start of the log and new instance in console

	// Parser decides he wants lexem, only one for now, so he calls getToken function
	getToken(fd, type_ptr, att_ptr); // passes file decriptor of the file he opened, at this moment getToken write on stdout, therefore is void, in future it will be returning id & value of the lexem through pointers that will be passed to it


	printf("Token(%d, %s);\n", *type_ptr, att_ptr);

	// Parser decides he wants more lexems, so he gets it.
	getToken(fd, type_ptr, att_ptr);	// 2
	printf("Token(%d, %s);\n", *type_ptr, att_ptr);
		getToken(fd, type_ptr, att_ptr);	// 2
	printf("Token(%d, %s);\n", *type_ptr, att_ptr);
		getToken(fd, type_ptr, att_ptr);	// 2
	printf("Token(%d, %s);\n", *type_ptr, att_ptr);
		getToken(fd, type_ptr, att_ptr);	// 2
	printf("Token(%d, %s);\n", *type_ptr, att_ptr);
		getToken(fd, type_ptr, att_ptr);	// 2
	printf("Token(%d, %s);\n", *type_ptr, att_ptr);
		getToken(fd, type_ptr, att_ptr);	// 2
	printf("Token(%d, %s);\n", *type_ptr, att_ptr);
	

	fclose(fd); // what has been opened, needs to be closed again
	
	printf("*************************************************************************\n");	// Just a tool to differ between start of the log and new instance in console

	free(type_ptr);
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

void getToken(FILE* fd, int* type_ptr, char* att_ptr)
{
	printf("!--- Function getToken() started. ---!\n");
	

	/*att_ptr[0] = 'C';
	att_ptr[1] = 'a';
	att_ptr[2] = 'u';
	att_ptr[3] = '\0';

	*type_ptr = 10;*/

	// usage of att_ptr => att_ptr[index] = c;
	// usage of type_ptr => *type_ptr = 10; // and we're okay

	int c; // we need char represented as integer, because most of the functions requires it
	int cx; // the character after the c

	int actState = 0; // starting state for final state automat
	int fcv = -1; // flow control variable; initialized on -1, because we increase it on the start of the loop

	bool inComment = false;
	bool terminator = false;

	while ( (c = fgetc(fd)) != EOF)
	{

		if(isspace(c) && actState != sSTRING)
		{
			continue; // when we hit the white space and we are not in the middle of the string, we skip the rest of the loop. For greater good.
		}

		if(c == '{' && actState != sSTRING)	// here we find out that commentary has started
		{
			inComment = true;
			continue;
		}

		if(inComment)	// if commentary started, we also have to check for its ending
		{
			if(c == '}')
			{
				inComment = false;	// if we reach the end of commentary, we set flow control variable inComment back to false
				printf("*Commentary skipped!\n");	// also in debug mode we print out a message about skipping a commentary
			}
			continue;
		}

		fcv++; // increasing the flow control variable (it is set to -1 by default, so it is 0 for the first loop after this line)

		cx = getNextChar(fd);	// getting the next character and testing it for end of file -> if it would be end of file, we would have to force send token, because there will be no next loop walkthrough
		

		switch(actState)
		{
			case sSTART:
				printf("*Case: sSTART entered!\n");

				if( isalpha(c) || c == '_')
				{
					actState = sIDENT;
					att_ptr[fcv] = c;
					break; // we breake it here, because we identified what's next state;
				}

				if( c == APOSTROF_ASCII )
				{
					actState = sSTRING;
					att_ptr[fcv] = c;
				}

				if ( c == '0')
				{
					actState = sZERO;
					att_ptr[fcv] = c; // this part is questionable
				}

				if( c >= '1' && c <= '9' )
				{
					actState = sNUMBER;
					att_ptr[fcv] = c;
				}

				if(c == '+' || c == '-' || c == '^')
				{
					actState = sOPERATOR;
					att_ptr[fcv] = c;
				}

				if(c == ':')
				{
					actState = scCOMMA;
					att_ptr[fcv] = c;
				}

				if(c == '<')
				{
					actState = scLESS;
					att_ptr[fcv] = c;
				}

				if(c == '>')
				{
					actState = scGREATER;
					att_ptr[fcv] = c;
				}

				if( c == ';')
				{
					actState = sHCOMMA;
					att_ptr[fcv] = c;
				} 
			break;

			case scCOMMA:

				*type_ptr = t_colon;

			break;

			case sIDENT:
				
					*type_ptr = IDENTIFIER;	// we know the token type is IDENTIFIER (defined in scanner.h, tType type)

				if(isalpha(c) || c == '_' || isdigit(c))
				{
					att_ptr[fcv] = c;	// if the loaded char respond to the mask, we extend outcoming value of the token
				}
				else	// if character does not respond to the mask, we set terminator on true causing while interuption (and function end)
				{
					terminator = true;
					att_ptr[fcv] = '\0'; // making sure that string is null terminated... you know that joke ... :)
					
				}

			break;

			default:
				printf("You ended in default case.\n");
			break;

		}



		if(terminator)
		{
			fcv = 0; // reset flow control variable
			fseek(fd, -1, SEEK_CUR);	// fseeking back about 1 character because we will need to read it again
			break;
		}


	}
	

	printf("\n!--- Function getToken() ended.   ---!\n");
}

