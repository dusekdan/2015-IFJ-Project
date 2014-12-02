#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
static const int t_expr_int  = 41;
static const int t_expr_str  = 42;
static const int t_expr_dou  = 43;                                    // tento terminal ak prislo nieco konecne ako cislo alebo string, vtomto pripade bude v odpovedajucej casti struktury token obsah
static const int t_expr_boo  = 44; 
                                  // ale moze sa tam vyskytnut aj premenna cize, var_id a v tom pripade je v *val_str bude obsahovat nazov premennej a po vyhladani
                                  // v tabulke symbolov zistis typ, (hledam->data->type), tam su typy int 1 real 2 string 3 bool 4, iny typ nieje platna premenna



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
	sDOUBLEOPER,
	sENTITY,
	sFUNCTIONID,
	A_JSI_V_PCI
};	

int maxStringLength = 200; // maximal string length that we've been able to reach (string allocation based on it)

int baseStringLength = 200; // base is 200, everytime we reach this limit, we double it (expecatation that if string level is reached, next even longer string may be present)
int newAllocationSpace;

/************************ ENUM PRO CASE ****************************-*/

/************************ keywords ***********************************/

char* keywords[] = {
	"begin", "boolean", "do", "else", "end", "false", "find", "forward", "function",
	"if", "integer", "readln", "real", "sort", "string", "then", "true", "var", "while",
	"write"
};






//void getNextToken();
int getNextChar(FILE* fd);
int getNextToken(FILE* fd, token TToken);
void makeStringLowerCase(char *string);
void resetString(char *string);


void makeStringLowerCase(char *string)
{
	int i;
	int stringLength;
	stringLength = strlen(string);
	for(i = 0; i < stringLength; i++)
	{
		string[i] = tolower(string[i]);
	}
}

void resetString(char *string)
{
	int i;
	int stringLength;
	stringLength = strlen(string);
	for(i = 0; i < stringLength; i++)
	{
		string[i] = '\0';
	}
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


int getNextToken(FILE* fd, token TToken)
{

	int c, cx; 
	int actState = sSTART; 
	int fcv = 0;


	int tokType;

	int tokInt;
	double tokDouble;

	char *strBuffer;
	strBuffer = malloc(baseStringLength); // we alloc only to "baseStringLength", because size of char is one by definition
		if(strBuffer == NULL)
		{
			printf("Allocation error! (strBuffer)\n");
			exit(99);
		}

	char *tmpBuffer; // reallocation purposes only 


	char *entityBuffer;
	entityBuffer = malloc(baseStringLength);	// same as few lines up
		if(entityBuffer == NULL)
		{
			printf("Alocation error! (entityBuffer)\n");
			exit(99);
		}

	bool forceTokenSend = false;

	bool inComment = false;
	bool terminateLoop = false;
	bool fseeker = true;
	bool numberIntCase = false;
	bool numberDoubleCase = false;

	bool inString = false;
	bool apostrof = false;
	bool entity = false;
	int skipChar = 0;
	int efcv = -1;
	int entityID;

	// I actually hate myself for doing this, but time is the time
	bool signContained = false;
	bool eContained = false;
	bool dotContained = false;


	char *strtolErrPtr; // error string from strtol() comes here
	char *strtodErrPtr; // same here for strtod() 



	/*if(TToken->val_str != NULL)
	{
		free(TToken->val_str);
	}*/

	/******* STARTING BUFFER RESET *******/
	//resetString(strBuffer);


	while ( 1 )
	{

		c = fgetc(fd);

		/****************************** DEALING WITH EXTRA SPACES (WHITE SPACES) ******************************/
		
		if((isspace(c) && actState == sSTART ) || (actState == sIDENT && isspace(c)))
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

		
		cx = getNextChar(fd);	// getting the next character and testing it for end of file -> if it would be end of file, we would have to force send token, because there will be no next loop walkthrough


		/*if(fcv == (maxStringLength-1))
		{
			printf("inhere!\n");
			strBuffer = realloc(strBuffer, (2*sizeof(strBuffer)));
			printf("%s", strBuffer);
			if(strBuffer == NULL)
			{
				printf("yup, we're fucked!\n");
				exit(99);
			}
		}*/


		/************************** TESTING A NEXT CHARACTER ON BEING EOF *************************/
		// nutno doplnit - nasázeno v každé místě, kde je možné hitnout konec 
		/*if(cx == EOF || isspace(cx)) && actState == sSTART)
		{
			forceTokenSend = true;
			forceTokenSend = (cx == EOF || isspace(cx)) && actState == sSTART)?true:false;
		}*/

		/************************** AND THE SWITCH FUN BEGINS **************************************/

		switch(actState)
		{

			case sSTART:

				if(isalpha(c) || c == '_')
				{
					actState = sIDENT;
					strBuffer[fcv] = c;
					break; // break comes here because we already identified what kind of state we're dealing with
				} else if( c == APOSTROF_ASCII )
				{
					//printf("vyhodnoceno kladne!\n");
					actState = sSTRING;
					apostrof= true;
					strBuffer[fcv] = c;
					fcv--; // decrease index in string
					break;
				}else if( c >= '0' && c <= '9')
				{
					actState = sNUMBER;
					strBuffer[fcv] = c;
					break;
				} else if(c == '+' || c == '.' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ',' || c == '^' || c == '=' ||  c== ';' || /* tyto znaky mohou vést na double op*/ c == ':' || c == '<' || c == '>')
				{
					actState = sSINGLEOPER;	 // je třeba provést kontrolu na doubleop
					strBuffer[fcv] = c;
					break;
				}
				else
				{
					actState = A_JSI_V_PCI;
				}
			
			break; // this is sSTART emergency break

			case sSTRING:
			//printf("Jsem ve stringu!\n");

			if(apostrof && !entity)	// jsme uvnitř stringu
			{


				if(skipChar == 1)
				{
					fcv--;
					skipChar--;
					break;
				}

				if(c > 31 && c <= 255 && c != APOSTROF_ASCII)
				{
					strBuffer[fcv] = c;
					break;
				}
				else if(c == APOSTROF_ASCII)
				{
					// pokud narazíme na apostrof, dve moznosti - konec retezce, nebo entita/dvojitej
					if(cx != APOSTROF_ASCII && cx != '#')
					{
						//printf("terminated_in_dual_condition!\n");
						apostrof = false; // string terminated
						strBuffer[fcv] = '\0';	// NULL TERMINATION FOR THE WIN!!!
						break;
					}

					if(cx == APOSTROF_ASCII)
					{
						skipChar = 1;
						strBuffer[fcv] = APOSTROF_ASCII;
						//fcv--; // decreasing the fcv variable to make sure we use the proper index in a string next time we store in it
						break;
					}

					if(cx == '#')
					{
						fcv--;
						entity = true;
						//printf("entity time!\n");
					}

				}

 
			}
			else 
			{
				if(entity)
				{

					if(c == '#' && cx == APOSTROF_ASCII)
					{
						printf("Incorrect entity entry!\n");
						exit(666);
					}

					if(c == '#' && !isdigit(cx))
					{
						printf("Incorrect entity entry!\n");
						exit(666);
					}

						// at this we should be sure we get at least #number (letter may occur later though)

					if(c == '#')
					{
						fcv--;
						break;
					}

					if(!isdigit(c) && c == APOSTROF_ASCII)	// v tento moment je konec entity
					{


						/*if(entityBuffer == NULL)
						{
							printf("Unable to proceed with entityBuffer set to NULL. \n");
							fclose(fd);
							exit(99);
						}*/

						long tentityID = strtol(entityBuffer, &strtolErrPtr, 10); // base of 10
								if(entityBuffer == strtolErrPtr)
								{
									printf("Non-convertable entity, fuck you.\n");
									exit(666);
								}
						entityID = tentityID;
						//printf("entityID: %d\n", entityID);
						
								if(entityID < 1 || entityID > 255)
								{
									printf("Entity out of its range. Or you possibly run the script too many times in a short time -> buffer was not empty on time. %d\n", entityID);
									exit(99);
								}

						//fcv--;
						strBuffer[fcv] = entityID;
						
						
							resetString(entityBuffer); // reset entityBufferu
						
						
						efcv = -1; // reset efcv
						entity = false;
						break;
					}

					if(isdigit(c))
					{
						//printf("efc: %d dostavam se sem: %c \n", efcv, c);
						fcv--;
						efcv++;
						entityBuffer[efcv] =  c;
						break;
					}
				}
				else
				{
					//strBuffer[fcv] = '\0';
					tokType = t_expr_str;
					terminateLoop = true;
					inString = true;
					break;
				}
			}


			break;

			case sIDENT:
				tokType = t_var_id; // (#!#) - nejsem si jistý, že sem dávám správný typ
				
				if(isalpha(c) || c == '_' || isdigit(c) || c == '(')
				{

					if(c == '(')
					{
						strBuffer[fcv] = '\0';
						terminateLoop = true;
						fseek(fd, -1, SEEK_CUR);
						fseeker = false;
						tokType = t_fun_id;
						break;
					}

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

				if(strBuffer[fcv-1] == '.')
				{
					strBuffer[fcv] = '\0';
					tokType = t_period;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == ',')
				{
					strBuffer[fcv] = '\0';
					tokType = t_comma;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == '(')
				{
					strBuffer[fcv] == '\0';
					tokType = t_l_parrent;
					terminateLoop = true;
					break;
				}

				if(strBuffer[fcv-1] == ')')
				{
					strBuffer[fcv] == '\0';
					tokType = t_r_parrent;
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
					tokType = t_expr_int;
					
					// situation: in strBuffer is stored nullterminated string containing INTEGER, with possible leading zeroes.
					// we need to strip this mothef*cker of this zeroes and make him play it our way.
					// That's the part where strtol kicks in

					int strtolBase = 10; // there's only decadic notation allowed in project, so base is 10
					long tmpTokInt;		 // temporary long variable, to be semantically correct
					

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

				// situation is we have practically two cases how we could end up here.
				// The first, we get "#." from sNUMBER where "#" represents variable number of digit in range 0-9
				// The second, we get "#e" or "#E", which we consider to be the same case 

				// in this case we can expect following formats
						// #.# [done]
						// #.#e#  or #.#E# [done]
						// #.#e-# or #.#E-#  [done]
						// #-#e+# or #.#E-# [done]
						// we have to deal with those formats one by one; branches for "e" and "E" will be shared though

				if(dotContained) // everything here will count on "#." as a an input
				{

					// multiple courses of action
					if(!eContained && !signContained)
					{
						// akceptujeme cisla a ecka, po kterých může následovat sign
						if(isdigit(c))
						{
							strBuffer[fcv] = c;
						}
						else 	// neni-li to pismeno, melo by to byt eE, jinak terminace a odeslani
						{
							if(c == 'e' || c == 'E')
							{
								strBuffer[fcv] = c;
								eContained = true;
								break;
							}
							else
							{
								strBuffer[fcv] = '\0';	// posíláme token, neboť jsme narazili na double tvaru "#.#"
								tokType = t_expr_dou;


								tokDouble = strtod(strBuffer, &strtodErrPtr);
									if(strBuffer == strtodErrPtr)
									{
										printf("Trial to convert non-convertable string to double! Real branch.\n");
										exit(666);
									}

								terminateLoop = true;
								numberDoubleCase = true;
								break;
							}
						}
					}

					if(eContained && !signContained)
					{

						if(strBuffer[fcv-1] == 'e' || strBuffer[fcv-1] == 'E')	 // pokud předchozí bylo eE, můžeme očekávat znak
						{

							if(c == '+' || c == '-')
							{
								strBuffer[fcv] = c;
								signContained = true;
								break;
							}
							else
							{
								if(isdigit(c)) // pokud tam nebude znaménko, ale číslo, stejně se budeme tvářit jako by znaménko bylo obsaženo (tzn. teď už nemůže být znovu)
								{
									strBuffer[fcv] = c;
									signContained = true;
									break;
								}
								else 	// jakýkoliv jiný znak není povolen == chyba literálu
								{
									printf("REAL ve spatnem tvaru!\n");
									exit(666);
								}
							}
						}

						if(isdigit(c))	// nebude-li předchozí přímo eE, přijímáme čísla a možnost zadávat znaménko padá
						{
							strBuffer[fcv] = c;
							signContained = true;
							break;
						}
						else
						{
							printf("REAL zase ve spatnem tvaru!\n");
							exit(666);
						}

					}

					if(eContained && signContained) // v tento moment už přijímáme jen čísla, cokoliv jiného vede na odeslání tokenu
					{

						if(strBuffer[fcv-1] == '+' || strBuffer[fcv-1] == '-') // pokud předchozí byl +-, je třeba přijmout číslo
						{

							if(!isdigit(c))
							{
								printf("REAL konci znamenkem == problem!\n");
								exit(666);
							}
						}

						if(isdigit(c))
						{
							strBuffer[fcv] = c;
							break;
						}
						else 	
						{
							strBuffer[fcv] = '\0';
							tokType = t_expr_dou;

							
							tokDouble = strtod(strBuffer, &strtodErrPtr);
								if(strBuffer == strtodErrPtr)
								{
									printf("Trial to convert non-convertable string to double! Real breanch!\n");
									exit(666);
								}

							terminateLoop = true;
							numberDoubleCase = true;
							break;
						}
					}
					break;
				}

				if(!dotContained) // není-li obsažena tečka, už ani nepřijde, předchozí znak musí být eE, neměla by nastat situace, kdy by nebyl.
				{
					if(eContained && !signContained)
					{
						// jeste neni obsazeno znamenko
						if(c == '+' || c == '-')
						{
								strBuffer[fcv] = c;
								signContained = true;
								break;
						}
						else
						{
								if(isdigit(c)) // pokud tam nebude znaménko, ale číslo, stejně se budeme tvářit jako by znaménko bylo obsaženo (tzn. teď už nemůže být znovu)
								{
									strBuffer[fcv] = c;
									signContained = true;
									break;
								}
								else 	// jakýkoliv jiný znak není povolen == chyba literálu
								{
									printf("REAL ve spatnem tvaru! (3)\n");
									exit(666);
								}
						}
						break;
					}


					if(eContained && signContained)
					{
						// obsazeno ecko i znamenko => prijimame pouze cisla, cokoliv jineho vede na odeslani tokenu
						if(isdigit(c))
						{
							strBuffer[fcv] = c;
							break;
						}
						else 	// pokud neni prijato cislo, je cas odeslat token
						{


							strBuffer[fcv] = '\0';
							tokType = t_expr_dou;

							tokDouble = strtod(strBuffer, &strtodErrPtr);
								if(strBuffer == strtodErrPtr)
								{
									printf("Trial to convert non-convertable string to double!\n");
									exit(666);
								}
							terminateLoop = true;
							numberDoubleCase = true;
							break;
						}
						break;
					}
				}

			break;

			case 998:
				printf("Error in single operator determination!\n");
			break; // same as one level up

			default:
				printf("Character not allowed!\n");
				exit(1);		
		}

		if((fcv+1) == baseStringLength)
		{
			// time for reallocation
			printf("Additional memory required!\n");

			newAllocationSpace = 2*baseStringLength;


			tmpBuffer = realloc(strBuffer, newAllocationSpace);
				if(!tmpBuffer)
				{
					free(strBuffer);	// in case reallocation failed, it is important to free strBuffer
					printf("Realloc failed!\n");
					fclose(fd);	// closing the file
					exit(99);
				}
				else
				{
					strBuffer = tmpBuffer;
				}

				if(strBuffer == NULL)
				{
					printf("Failed to allocate additional space!\n");
					fclose(fd);
					exit(99);
				}
				else
				{
					baseStringLength = newAllocationSpace;
					printf("Reallocation successfull!\n");
					printf("Allocated %d bytes of memory.\n", baseStringLength);
				}


		}

		fcv++; // increasing the flow control variable 


		/* end of switch fun */

		/* TOKEN SENDING AND LOOP TERMINATION */

		if(terminateLoop)
		{

			TToken->type = tokType; // anytime we get here, we set the tokType

				TToken->val_int = -1;
				TToken->val_flo = -1.0;		
		
			if(tokType == t_fun_id)
			{
				TToken->val_str = malloc(baseStringLength);
					if(TToken->val_str != NULL)
					{
						strcpy(TToken->val_str, strBuffer);
					}
			}

			if(inString)
			{
				TToken->val_str = malloc(baseStringLength);	// once again, sizeof(char) is one by its definition, so there's no need to use it
					if(TToken->val_str == NULL)
					{
						printf("Token allocation for string failed!\n");
						fclose(fd);
						exit(99);
					}
				strcpy(TToken->val_str, strBuffer);
			}



			// here I'm gonna test the string on being a key word and base on that I redecide what tokenType I send in the end
			if(!inString)
			{

				makeStringLowerCase(strBuffer); // makes content of strBuffer lowercase (required for successfull comparism)

				if(strcmp(strBuffer, "begin") == 0)
				{
					TToken->type = t_begin;
				}
				else if(strcmp(strBuffer, "boolean") == 0)
				{
					TToken->type = t_boolean;
				}
				else if(strcmp(strBuffer, "do") == 0)
				{
					TToken->type = t_do;
				}
				else if(strcmp(strBuffer, "else") == 0)
				{
					TToken->type = t_else;
				}
				else if(strcmp(strBuffer, "end") == 0)
				{
					TToken->type = t_end;
				}
				else if(strcmp(strBuffer, "false") == 0)
				{
					TToken->type = t_expr_boo;
					TToken->val_int = 0;	//(#!#)
				}
				else if(strcmp(strBuffer, "find") == 0)
				{
					TToken->type = t_fun_id;
					TToken->val_str = malloc(baseStringLength);
						if(TToken->val_str == NULL)
						{
							printf("Token allocation for string failed!\n");
							fclose(fd);
							exit(99);
						}
					strcpy(TToken->val_str,strBuffer);
				}
				else if(strcmp(strBuffer, "forward") == 0)
				{
					TToken->type = t_forward;
				}
				else if(strcmp(strBuffer, "function") == 0)
				{
					TToken->type = t_function;
				}
				else if(strcmp(strBuffer, "if") == 0)
				{
					TToken->type = t_if;
				}
				else if(strcmp(strBuffer, "integer") == 0)
				{
					TToken->type = t_integer;
				}
				else if(strcmp(strBuffer, "readln") == 0)
				{
					TToken->type = t_readln;
				}
				else if(strcmp(strBuffer, "real") == 0)
				{
					TToken->type = t_real;
				}
				else if(strcmp(strBuffer, "sort") == 0)
				{
					TToken->type = t_fun_id;
					TToken->val_str = malloc(baseStringLength);
						if(TToken->val_str == NULL)
						{
							printf("Token allocation for string failed!\n");
							fclose(fd);
							exit(99);
						}
					strcpy(TToken->val_str,strBuffer);
				}
				else if(strcmp(strBuffer, "string") == 0)
				{	
					TToken->type = t_string;
				}
				else if(strcmp(strBuffer, "then") == 0)
				{
					TToken->type = t_then;
				}
				else if(strcmp(strBuffer, "true") == 0)
				{
					TToken->type = t_expr_boo;
					TToken->val_int = 1; //(#!#)
				}
				else if(strcmp(strBuffer, "var") == 0)
				{
					TToken->type = t_var;
				}
				else if(strcmp(strBuffer, "while") == 0)
				{
					TToken->type = t_while;
				}
				else if(strcmp(strBuffer, "write") == 0)
				{
					TToken->type = t_write;
				}	
			}

			//printf("varID: %d\n", tokType);

			if(TToken->type == t_var_id)
			{
				TToken->val_str = malloc(baseStringLength);
					if(TToken->val_str == NULL)
					{
						printf("Token allocation for string failed!\n");
						fclose(fd);
						exit(99);
					}
				strcpy(TToken->val_str,strBuffer);
			}

			if(fseeker)
			{
				fseek(fd, -1, SEEK_CUR); // (#!#) posun o jeden znak zpět po odeslání tokenu
			}

			if(numberIntCase)
			{
				TToken->val_int = tokInt; // we ended in integer branch, so we fill "val_int" with fresh mea... I mean data.
				TToken->val_flo = -1.0;
				//resetString(strBuffer);
				//free(strBuffer);
			}
			else if(numberDoubleCase)
			{
				TToken->val_flo = tokDouble; // we ended in integer branch, so we fill "val_flo" with data.
				TToken->val_int = -1;
				//resetString(strBuffer);
				//free(strBuffer);
			}
			else
			{
				// do nothing atm
			}
			// Conclusion
			// When called on ínteger/real, in the responding data type is stored the value, in the other one, -1 is issued, in val_str is left the original string value
			// When called on string/identifier/whatever requires storage in strBuff, it's left in there (though memory is cleaned in the end)
			

				free(strBuffer);
				free(entityBuffer);


		if(c == EOF)
		{
			return -1;
		}
		else
		{
			return 1;
		}

			break;
		}
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

	FILE* fd = fopen("testFiles/t1string", "r");
		if(fd == NULL)
		{
			printf("Encountered an error while opening the file!\n");
			return 1;
		}

	printf("Starting a scanning process! \n");
			int pica = 0;
			while(1)
			{
				int td = getNextToken(fd, TToken);
				pica++;


		printf("Token #%d, structure string='%s', integer='%d', real='%f' (type=%d)\n", pica, TToken->val_str, TToken->val_int, TToken->val_flo, TToken->type);

	/*			if(TToken->type == t_expr_str || TToken->type == t_var_id)
				{
					free(TToken->val_str);
				}*/

				
				if(td == -1)
				{
					break;
				}				
			}

	printf("Ending a scanning process! \n");
	fclose(fd);
	free(TToken);
	return 0;

}