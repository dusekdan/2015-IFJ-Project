#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#define APOSTROF_ASCII 39

/************************ VYTRŽENO Z PARSER.H **********************-*/

static const int t_var       =  1;
static const int t_colon     =  2;
static const int t_semicolon =  3;
static const int t_l_parrent =  4;
static const int t_r_parrent =  5;
static const int t_function  =  6;
static const int t_forward   =  7;
static const int t_begin     =  8;
static const int t_end       =  9;
static const int t_period    = 10;
static const int t_comma     = 11;
static const int t_assign    = 12;
static const int t_if        = 13;
static const int t_then      = 14;
static const int t_else      = 15;
static const int t_while     = 16;
static const int t_do        = 17;
static const int t_readln    = 18;
static const int t_write     = 19;
static const int t_var_id    = 20;
static const int t_expr      = 21;
static const int t_fun_id    = 22;
static const int t_term      = 23;
//static const int t_param     = 24;
//static const int t_read_id   = 25;  //////////////////////////////read id nakoniec nebude treba
static const int t_integer   = 26;
static const int t_real      = 27;
static const int t_string    = 28;
static const int t_boolean   = 29;
static const int t_dollar    = 30;

static const int t_plus      = 31;// +
static const int t_minus     = 32;// -
static const int t_mul       = 33;// *
static const int t_div       = 34;// /
/// zatvorky su t_l_parrent a rparrent cize 4 a 5
static const int t_less      = 35;//<
static const int t_more      = 36;//>
static const int t_lesseq    = 37;//<=
static const int t_moreeq    = 38;//>=
static const int t_equal     = 39;//=
static const int t_nequal    = 40;//<>
static const int t_expr_val  = 41;// tento terminal ak prislo nieco konecne ako cislo alebo string, vtomto pripade bude v odpovedajucej casti struktury token obsah
                                  // ale moze sa tam vyskytnut aj premenna cize, var_id a v tom pripade je v *val_str bude obsahovat nazov premennej a po vyhladani
                                  // v tabulke symbolov zistis typ, (hledam->data->type), tam su typy int 1 real 2 string 3 bool 4, iny typ nieje platna premenna
static const int t_striska = 42; //^


typedef struct token
{
    int type;      //Typ podla zoznamu terminalov
    char *val_str; //Názov v prípade že ide o id, tak sem pojde jeho nazov, ak ide o string sem ide obsah stringu
    int   val_int; //Hodnota int ak ide o integer alebo 0/1 ak ide o bool
    double val_flo; //Hodnota na float (real)
    //////////String netreba nato pozijeme val_str
    //////////Bool netreba naten pouzijeme val_int
}*token;

/************************ VYTRŽENO Z PARSER.H **********************-*/


/************************ ENUM PRO CASE ****************************-*/

enum states {
	sSTART,
	sIDENT,
	sSTRING,
	sZERO,
	sNUMBER,
	sREAL,
	sSINGLEOPER,
	sDOUBLEOPER
};	

int maxStringLength = 200; // maximal string length that we've been able to reach (string allocation based on it)

/************************ ENUM PRO CASE ****************************-*/

//void getNextToken();
int getNextChar(FILE* fd);
void getNextToken(FILE* fd, token TToken);


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


void getNextToken(FILE* fd, token TToken)
{

	int c, cx; 
	int actState = 0; 
	int fcv = -1;


	int tokType;

	int tokInt;
	double tokDouble;

	char *strBuffer;
	strBuffer = malloc(sizeof(char)*maxStringLength);
		if(strBuffer == NULL)
		{
			printf("Allocation error!\n");
			exit(666);
		}

	bool inComment = false;
	bool terminateLoop = false;
	bool fseeker = true;
	bool numberIntCase = false;
	bool numberDoubleCase = false;


	// I actually hate myself for doing this, but time is the time
	bool signContained = false;
	bool eContained = false;
	bool dotContained = false;


	/******* STARTING BUFFER RESET *******/
	int r;	
	int bufferLength = strlen(strBuffer);
	for(r = 0; r < bufferLength; r++)
	{
				strBuffer[r] = '\0'; // no better way to reset string than null terminate everything
	}


	while ( (c = fgetc(fd)) != EOF )
	{

		/****************************** DEALING WITH EXTRA SPACES (WHITE SPACES) ******************************/
		
		if(isspace(c) && actState != sSTRING)
		{
			continue; // when we hit the white space and we are not in the middle of the string, we skip the rest of the loop. For greater good.
		}

		/****************************** DEALING WITH COMMENTARIES (JUST SKIP THEM) ******************************/

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
				//printf("*Commentary skipped!\n");	// also in debug mode we print out a message about skipping a commentary
			}
			continue;
		}

		/*************************** SETTING UP FLOW-CONTROL VARIABLES AND STUFF ***************************/

		fcv++; // increasing the flow control variable (it is set to -1 by default, so it is 0 for the first loop after this line)
		cx = getNextChar(fd);	// getting the next character and testing it for end of file -> if it would be end of file, we would have to force send token, because there will be no next loop walkthrough


		/************************** TESTING A NEXT CHARACTER ON BEING EOF *************************/
		// nutno doplnit

		/************************** AND THE SWITCH FUN BEGINS **************************************/

		switch(actState)
		{

			case sSTART:

				if(isalpha(c) || c == '_')
				{
					actState = sIDENT;
					strBuffer[fcv] = c;
					break; // break comes here because we already identified what kind of state we're dealing with
				}

				if( c == APOSTROF_ASCII )
				{
					actState = sSTRING;
					strBuffer[fcv] = c;
					break;
				}

				/*if( c == '0') // so far appears to be useless condition
				{
					actState = sZERO;	// this case maybe won't be required
					// don't know what to put to buffer yet (#!#)
					break;
				}*/

				if( c >= '0' && c <= '9')
				{
					actState = sNUMBER;
					strBuffer[fcv] = c;
					break;
				}

				if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '=' ||  c== ';' || /* tyto znaky mohou vést na double op*/ c == ':' || c == '<' || c == '>')
				{
					actState = sSINGLEOPER;	 // je třeba provést kontrolu na doubleop
					strBuffer[fcv] = c;
					break;
				}
			
			break; // this is sSTART emergency break

			case sIDENT:
				tokType = t_var_id; // (#!#) - nejsem si jistý, že sem dávám správný typ
				
				if(isalpha(c) || c == '_' || isdigit(c))
				{
					strBuffer[fcv] = c;	// if the loaded char respond to the mask, we extend outcoming value of the token
				}
				else // if character does not respond to the mask, we set terminator on true causing while interuption (and function end)
				{
					strBuffer[fcv] = '\0'; // making sure that string is null terminated... you know that joke ... :)
					terminateLoop = true;
					fseek(fd, -1, SEEK_CUR);
					fseeker = false;
				}

			break; // this is sIDENT emergency break

			case sSINGLEOPER: // this case actually handles even doubleOperators
				
				// when we get here we know that it is truely single operator

				if(strBuffer[fcv-1] == ':')	// in this case we can expect these ":", ":="
				{
					if(c == '=')	// for ":=" case
					{
						strBuffer[fcv] = c;		// we add actual char to the string buffer
						strBuffer[fcv+1] = '\0';	// and null terminate that string
						tokType = t_assign;		// set up token type on t_assign
						terminateLoop = true;	// decide we will terminate the loop
						fseeker = false;		// but we won't fseek back (no need)
						break;
					}
					else 	// this is the case for  just":"
					{		// all of the contents here and bellow is analogical
						strBuffer[fcv] = '\0';
						tokType = t_colon;
						terminateLoop = true;
						break;
					}
				}

				if(strBuffer[fcv-1] == '<')	// expectable cases "<", "<=", "<>"
				{
					if(c == '=')	// for "<=" case
					{
						strBuffer[fcv] = c;
						strBuffer[fcv+1] = '\0';
						terminateLoop = true;
						tokType = t_lesseq;
						fseeker = false;
						break;
					}
					else
					{
						if(c == '>')	// for "<>" case
						{
							strBuffer[fcv] = c;
							strBuffer[fcv+1] = '\0';
							terminateLoop = true;
							tokType = t_nequal;
							fseeker = false;
							break;
						}
						else 		// and finally for just "<" case
						{							
							strBuffer[fcv] = '\0';
							tokType = t_less;
							terminateLoop = true;
							break;
						}
					}
				}

				if(strBuffer[fcv-1] == '>')	// expectable cases ">", ">="
				{
					if(c == '=')	// case for ">="
					{
						strBuffer[fcv] = c;
						strBuffer[fcv+1] = '\0';
						tokType = t_moreeq;
						terminateLoop = true;
						fseeker = false;
						break;
					}
					else 	// and case for simple ">"
					{
						strBuffer[fcv] = '\0';
						tokType = t_more;
						terminateLoop = true;
						break;
					}
				}

				// here we go only for one char operators

				if(strBuffer[fcv-1] == ';')		// semicolon case
				{
					strBuffer[fcv] = '\0';
					tokType = t_semicolon;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == '=')		// equal case
				{
					strBuffer[fcv] = '\0';
					tokType = t_equal;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == '^')		// "striska" case
				{
					strBuffer[fcv] = '\0';
					tokType = t_striska;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == '/')		// "slash" or "divide" case
				{
					strBuffer[fcv] = '\0';
					tokType = t_div;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == '*')		// multiplication case
				{
					strBuffer[fcv] = '\0';
					tokType = t_mul;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == '-') 	// minus case
				{
					strBuffer[fcv] = '\0';
					tokType = t_minus;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == '+')		// plus case
				{
					strBuffer[fcv] = '\0';
					tokType = t_plus;
					terminateLoop = true;
					//fseek(fd, -1, SEEK_CUR);
					break;
				}
				else
				{
					actState = 998;
					break;
				}

			break; // this is sSINGLEOPER emergency break

			case sNUMBER:
			
				if(isdigit(c) ||  c == '.' || c == 'e' || c == 'E')
				{

					if(c == '.' || c == 'e' || c == 'E')
					{
						actState = sREAL;

						if(c == '.')
						{
							dotContained = true;
						}

						if(c == 'e' || c == 'E')
						{
							eContained = true;
						}

					}

					strBuffer[fcv] = c;

				}
				else
				{
					// here we get when we get "terminal" for numeric type
						// so we got to save, fseekback and send token
					strBuffer[fcv] = '\0';
					tokType = t_expr_val;
					
					// situation: in strBuffer is stored nullterminated string containing INTEGER, with possible leading zeroes.
					// we need to strip this mothef*cker of this zeroes and make him play it our way.
					// That's the part where strtol kicks in

					int strtolBase = 10; // there's only decadic notation allowed in project, so base is 10
					long tmpTokInt;		 // temporary long variable, to be semantically correct
					char *strtolErrPtr;	 // error string from strtol() comes here

						tmpTokInt = strtol(strBuffer, &strtolErrPtr, strtolBase);
						if(strtolErrPtr == strBuffer)
						{
							printf("Trial to convert a string that is not convertable to long. Integer branch.\n");
							exit(666);
						} 

					tokInt = tmpTokInt;	// now we have value of the integer stored in an integer variable, which is what we wanted, right.

					//printf("%s | %d\n", strBuffer, tokInt); // printing out original string & tokInt, debug mode only


					terminateLoop = true;
					numberIntCase = true;		// we let the program know that we need to terminate the loop and as we end in integercase, we need to fill the variable
				}

			break; // this is sNUMBER emergency break 
			
			case sREAL:
				printf("we're in a sREAL case! with %c\n", c);

				// situation is we have practically two cases how we could end up here.
				// The first, we get "#." from sNUMBER where "#" represents variable number of digit in range 0-9
				// The second, we get "#e" or "#E", which we consider to be the same case 

				// in this case we can expect following formats
						// #.# [done]
						// #.#e#  or #.#E# [done]
						// #.#e-# or #.#E-#  [done]
						// #-#e+# or #.#E-# [done]
						// we have to deal with those formats one by one; branches for "e" and "E" will be shared though

				// FIRST CASE 

				/*if(strBuffer[fcv-1] == '.') // case for "#."
				{

					


					if(isdigit(c)) 
					{

					}

				}*/

				/*// first trial
				if(strBuffer[fcv-1] == 'e' || strBuffer[fcv-1] == 'E')	// case for "#e" or "#E"
				{
					// now we can only expect "+", "-" or nothing and then numbers
					if(isdigit(c) || c == '+' || c == '-')
					{
						strBuffer[fcv] = c;	// in case "+", "-" or digit comes, we extend the strBuffer
						// this of  course happens only once, in the moment we come from sNUMBER with "#e|#E" form
					}
					else // now comes something thta doesnt respond to the head, like terminal
					{
						printf("REAL format not valid! Something unexpected after \"e\" or \"E\"!(1)\n");
						exit(666);
					}
				}
				else // we dont get the first case format
				{
					// inside we test on first case format
					if(strBuffer[fcv-1] == '.')	// if positive, we accept only digits for this loop run
					{
						if(isdigit(c))
						{
							strBuffer[fcv] = c;
						}
						else 	// when we don't get a digit at this moment, that means REAL error
						{
							printf("REAL format not valid! (2)\n"); //(666)
						}
					}
					else 	// previous character was a number (nothing else can come so far)
					{
						if(c == 'e' || c == 'E' || isdigit(c))
						{
							strBuffer[fcv] = c;
						}
						else // other symbols are not allowed -> ending the calculations
						{
							tokDouble = 6.66;
							tokType = t_expr_val;
							strBuffer[fcv] = '\0';

							terminateLoop = true;
							numberDoubleCase = true;
							break; // it's break time 
						}
					}
				}*/

				// second trial
				/*if(strBuffer[fcv-1] == '.')
				{
					if(isdigit(c))
					{
						strBuffer[fcv] = c;
					}
					else
					{
						printf("REAL error, invalid character after 'dot'.\n");
						exit(666);
					}
				}
				else
				{
					if(strBuffer[fcv-1] == "e" || strBuffer == "E")
					{
						if(c == '+' || c == '-' || isdigit(c))
						{
							strBuffer[fcv] = c;
						}
						else
						{
							printf("REAL error, invalid character after 'e' or 'E'.\n");
							exit(666);
						}
					}

				}*/





				//numberDoubleCase = true;
				//tokDouble = 0.00;

			break;

			case 998:
				printf("Error in single operator determination!\n");
			break; // same as one level up

			default:
				printf("This should not occur! (case default)\n");		
		}


		/* end of switch fun */

		/* TOKEN SENDING AND LOOP TERMINATION */

		if(terminateLoop)
		{
			TToken->val_str = strBuffer;
			TToken->type = tokType;

			//printf("actually read char: %s\n", strBuffer);


			// buffer string reset
			/*int r;	// well... i cant do this here, because im pointing at this string and syntax a. wouldnt have a way to work with it. Reset needs to be initialized before entering the loop.
			for(r = 0; r < bufferLength; r++)
			{
				strBuffer[r] = '\0'; // no better way to reset string than null terminate everything
			}*/

			free(strBuffer);

			if(fseeker)
			{
				fseek(fd, -1, SEEK_CUR); // (#!#) posun o jeden znak zpět po odeslání tokenu
			}

			if(numberIntCase)
			{
				TToken->val_int = tokInt; // we ended in integer branch, so we fill "val_int" with fresh mea... I mean data.
			}

			if(numberDoubleCase)
			{
				TToken->val_flo = tokDouble; // we ended in integer branch, so we fill "val_flo" with data.
			}

			break;
		}

//(#!#) Místo posledního středníku (A jakékoliv další žádosti o další lexém) to pakuje dohromady středník a předchozí řetězec. Weird stuff
 	}


}



int main()
{

	token TToken = malloc(sizeof(struct token));
		if(TToken == NULL)
		{
			printf("Encountered allocation issue!\n");
			return 1;
		}

	FILE* fd = fopen("testfile1", "r");
		if(fd == NULL)
		{
			printf("Encountered an error while opening the file!\n");
			return 1;
		}

	printf("Starting a scanning process! \n");


			int pica;
			for(pica = 0; pica <= 19; pica++)
			{
				getNextToken(fd, TToken);
				printf("Token #%d, structure string='%s', integer='%d', real='%f' (type=%d)\n", pica, TToken->val_str, TToken->val_int, TToken->val_flo, TToken->type);
			}

	printf("Ending a scanning process! \n");
	return 0;

}