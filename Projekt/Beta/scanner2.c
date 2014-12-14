/***********************scanner.c********************************/
/* Soubor: scanner.c - Obsahuje lexikální analyzátor 		*/
/* Předmět: Formalní jazyky a překladače (IFJ) 			*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 					*/
/* Datum: prosinec 2014						*/
/* Kódování: UTF-8						*/
/* Autoři:			Daniel Dušek (xdusek21)		*/
/* Poznámka: Na česko-anglických komentářích je poznat jak 	*/
/* náročnost programu s časem rostla				*/
/****************************************************************/
#define APOSTROF_ASCII 39
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
	A_JSI_V_PCI,
	UNEXPECTED_CHAR, 
	WRONG_OPERATOR
};

int maxStringLength = 200; // maximal string length that we've been able to reach (string allocation based on it)

int baseStringLength = 200; // base is 200, everytime we reach this limit, we double it (expecatation that if string level is reached, next even longer string may be present)
int newAllocationSpace;

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
			fprintf(stderr, "Allocation error! (strBuffer)\n");
			errorHandler(errInt);
		}

	char *tmpBuffer; // reallocation purposes only 


	char *entityBuffer;
	entityBuffer = malloc(baseStringLength);	// same as few lines up
		if(entityBuffer == NULL)
		{
			fprintf(stderr, "Alocation error! (entityBuffer)\n");
			errorHandler(errInt);
		}

	bool forceTokenSend = false;

	bool inComment = false;
	bool terminateLoop = false;
	bool fseeker = true;
	bool numberIntCase = false;
	bool numberDoubleCase = false;

	bool notAllowedChar = false;

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

	while( 1 )
	{


		c = fgetc(fd);
		cx = getNextChar(fd);

	// spaces

	if(isspace(c) && actState == sSTART)
	{
		continue;
	}

	// commentaries

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
			//printf("Commentary skipped!\n");
		}
		if(c == EOF)
		{
			fprintf(stderr, "Unexpected end of FILE YOU NIGGER!\n");
			errorHandler(errLex);
		}
		continue;
	}

	if(cx == EOF || cx == -1)
	{
		fseeker = false;
	}

	switch(actState)
	{

		case 999:
		break;

		case sSTART:

			if(isalpha(c) || c == '_')
			{
				actState = sIDENT;
				strBuffer[fcv] = c;
				break;
			}

			if(c == '+' || c == '.' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ',' || c == '=' ||  c== ';' || /* tyto znaky mohou vést na double op*/ c == ':' || c == '<' || c == '>')
			{
				actState = sSINGLEOPER;
				strBuffer[fcv] = c;
				break;
			}

			if(c == APOSTROF_ASCII)
			{
				actState = sSTRING;
				apostrof = true;
				strBuffer[fcv] = c;
				fcv--; // snížení indexu ve stringu
				break;
			}	

			if( c >= '0' && c <= '9')
			{
				actState = sNUMBER;
				strBuffer[fcv] = c;
				break;
			}	

			actState = UNEXPECTED_CHAR;	// dostaneme-li se sem, víme, že znak není povolen, tedy vyhodíme hlášku a skončíme.

		break;


		case sIDENT:
			tokType = t_var_id;
			
			if(isalpha(c) || c == '_' || isdigit(c))
			{
			

				strBuffer[fcv] = c;	// if the loaded char respond to the mask, we extend outcoming value of the token
			

			}
			else // if character does not respond to the mask, we set terminator on true causing while interuption (and function end)
			{

					strBuffer[fcv] = '\0'; // making sure that string is null terminated... you know that joke ... :)
					terminateLoop = true;

					if(c == '(')
					{
						tokType = t_fun_id;
					}
					else
					{
						while(isspace(c))
						{
							c = fgetc(fd);
						}

						if(c == '(')
						{
							tokType = t_fun_id;
						}

					}


					fseek(fd, -1, SEEK_CUR);
					fseeker = false;

			}
		break;


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

				// else case -> we get here and we don't end up in any case

				actState = WRONG_OPERATOR;
		break;

		case sSTRING:


			if(c == EOF || c == -1)
			{
				fprintf(stderr, "Unexpected EOF!\n");
				errorHandler(errLex);
			}

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
					strBuffer[fcv] = c;	// pouhé přidání znaku do stringu (no big deal)
					break;
				}
				else if(c == APOSTROF_ASCII)
				{
					// pokud narazíme na apostrof, dve moznosti - konec retezce, nebo entita/dvojitej
					if(cx != APOSTROF_ASCII && cx != '#')
					{
						apostrof = false; // string terminated
						strBuffer[fcv] = '\0';	// NULL TERMINATION FOR THE WIN!!!
						break;
					}

					if(cx == APOSTROF_ASCII)
					{
						skipChar = 1;
						strBuffer[fcv] = APOSTROF_ASCII;
						break;
					}

					if(cx == '#')
					{
						fcv--;
						entity = true;
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
						exit(1);
					}

					if(c == '#' && !isdigit(cx))
					{
						fprintf(stderr, "Incorrect entity entry!\n");
						errorHandler(errLex);
					}

						// at this we should be sure we get at least #number (letter may occur later though)

					if(c == '#')
					{
						fcv--;
						break;
					}

					if(!isdigit(c) && c == APOSTROF_ASCII)	// v tento moment je konec entity
					{

						long tentityID = strtol(entityBuffer, &strtolErrPtr, 10); // base of 10
								if(entityBuffer == strtolErrPtr)
								{
									fprintf(stderr, "Entity not convertable!\n");
									errorHandler(errLex);
								}
						entityID = tentityID;
						
								if(entityID < 1 || entityID > 255)
								{
									fprintf(stderr, "Entity out of its range. Or you possibly run the script too many times in a short time -> buffer was not empty on time. %d\n", entityID);
									errorHandler(errLex);
								}

						strBuffer[fcv] = entityID;
						resetString(entityBuffer); // reset entityBufferu
						
						entityID = 0;
						efcv = -1; // reset efcv
						entity = false;
						break;
					}

					if(isdigit(c))
					{
						fcv--;
						efcv++;
						entityBuffer[efcv] =  c;
						break;
					}
				}
				else
				{
					tokType = t_expr_str;
					terminateLoop = true;
					inString = true;
					break;
				}
			}
		break;


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
						fprintf(stderr, "Trial to convert a string that is not convertable to long. Integer branch.\n");
						errorHandler(errLex);
					} 

				tokInt = tmpTokInt;	// now we have value of the integer stored in an integer variable, which is what we wanted, right.

				//printf("%s | %d\n", strBuffer, tokInt); // printing out original string & tokInt, debug mode only


				terminateLoop = true;
				numberIntCase = true;		// we let the program know that we need to terminate the loop and as we end in integercase, we need to fill the variable
			}
		break;

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
									fprintf(stderr, "Trial to convert non-convertable string to double! Real branch.\n");
									errorHandler(errLex);
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
									fprintf(stderr, "Wrong format for REAL!\n");
									errorHandler(errLex);
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
							fprintf(stderr, "Wrong format for REAL!\n");
							errorHandler(errLex);
						}

					}

					if(eContained && signContained) // v tento moment už přijímáme jen čísla, cokoliv jiného vede na odeslání tokenu
					{

						if(strBuffer[fcv-1] == '+' || strBuffer[fcv-1] == '-') // pokud předchozí byl +-, je třeba přijmout číslo
						{

							if(!isdigit(c))
							{
								fprintf(stderr, "Wrong format for REAL!\n");
								errorHandler(errLex);
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
									fprintf(stderr, "Trial to convert non-convertable string to double! Real breanch!\n");
									errorHandler(errLex);
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
									fprintf(stderr, "Wrong format for REAL!\n");
									errorHandler(errLex);
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
									fprintf(stderr, "Trial to convert non-convertable string to double!\n");
									errorHandler(errLex);
								}
							terminateLoop = true;
							numberDoubleCase = true;
							break;
						}
						break;
					}
				}
		break;

		case WRONG_OPERATOR:
			fprintf(stderr, "Operator does not exist!\n");
			free(strBuffer);
			free(entityBuffer);
			errorHandler(errLex);
		break;

		case UNEXPECTED_CHAR:
			if(c == EOF || c == -1)
			{
				//printf("End of file occured!\n");
				return -1;
			}
			
			fseek(fd, -2, SEEK_CUR);	// Swiggity swooty, I’m comin’ for that booty
			c = fgetc(fd);
			fprintf(stderr, "Unexpected character! ('%c')\n", c);
			free(strBuffer);
			free(entityBuffer);
			errorHandler(errLex);
		break;

		default: 
			fprintf(stderr, "Case that should never occur just occured. Mayday, mayday, we're going down!\n");
			errorHandler(errLex);
		break;
	}

	//reallocation works JUST FINE ;) (proud of this one)

	if((fcv+1) == baseStringLength)
		{
			// time for reallocation
			//printf("Additional memory required!\n");

			newAllocationSpace = 2*baseStringLength;


			tmpBuffer = realloc(strBuffer, newAllocationSpace);
				if(!tmpBuffer)
				{
					free(strBuffer);	// in case reallocation failed, it is important to free strBuffer
					fprintf(stderr, "Realloc failed!\n");
					errorHandler(errInt);
				}
				else
				{
					strBuffer = tmpBuffer;
				}

				if(strBuffer == NULL)
				{
					fprintf(stderr, "Failed to allocate additional space!\n");
					errorHandler(errInt);
				}
				else
				{
					baseStringLength = newAllocationSpace;
				//	printf("Reallocation successfull!\n");
				//	printf("Allocated %d bytes of memory.\n", baseStringLength);
				}


		}


	fcv++;

	if(terminateLoop)
	{
		TToken->type = tokType;
		TToken->val_int = -1;
		TToken->val_flo = -1.0;

		if(tokType == t_fun_id)
		{
			TToken->val_str = malloc(baseStringLength);	// once again, sizeof(char) is one by its definition, so there's no need to use it
				if(TToken->val_str == NULL)
				{
					fprintf(stderr, "Token allocation for string failed!\n");
					errorHandler(errInt);
				}
			strcpy(TToken->val_str, strBuffer);
		}

		if(inString)
		{
			TToken->val_str = malloc(baseStringLength);
					if(TToken->val_str == NULL)
					{
						fprintf(stderr, "Token allocation for string failed!\n");
						errorHandler(errInt);
					}
					strcpy(TToken->val_str, strBuffer);			
		}



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
							fprintf(stderr, "Token allocation for string failed!\n");
							free(strBuffer);
							free(entityBuffer);
							errorHandler(errInt);
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
							fprintf(stderr, "Token allocation for string failed!\n");
							free(strBuffer);
							free(entityBuffer);
							errorHandler(errInt);
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

		if(numberIntCase)
		{
			TToken->val_int = tokInt;
			TToken->val_flo = -1.0;
		}

		if(numberDoubleCase)
		{
			TToken->val_flo = tokDouble;
			TToken->val_int = -1;
		}




		if(TToken->type == t_var_id) // Pokud je token type stále t_var_id, uložím do stringové složky v tokenové struktuře jeho LOWERCASE název
		{
			TToken->val_str = malloc(baseStringLength);
				if(TToken->val_str == NULL)
				{
					fprintf(stderr, "Token allocation for string failed!\n");
					errorHandler(errInt);
				}
			makeStringLowerCase(strBuffer);
			strcpy(TToken->val_str,strBuffer);			
		}

		if(fseeker)	// pokud fseekback není vypnutý, posuneme se ukazatelem do souboru o jednu zpět, abychom vykompenzovali (a připravili pro další průchod) znak, co nám ukončil tento průchod.
		{
			fseek(fd, -1, SEEK_CUR);
		}

		//printf("Odeslán token s typem: %d (values: %s|%d|%f)\n", TToken->type, TToken->val_str, TToken->val_int, TToken->val_flo);
		free(strBuffer);
		free(entityBuffer);
		
		
			return 1;
		

	break;
	}


}



}
