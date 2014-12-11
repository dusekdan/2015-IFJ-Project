/***********************precedence.c*****************************/
/* Soubor: precednce.c - Syntaktická analýza výrazů 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/*					Matúš Bútora (xbutor01)						*/
/****************************************************************/

#include "precedence.h"

token gib_tok() {
	
	getNextToken(fd, tok);
	return tok;
}

void stackInit(tStack *stack) {

	stack->top = NULL;
}

bool stackEmpty(tStack *stack) {

	if(stack->top == NULL)
		return true;

	else
		return false;
}

bool stackPush(tStack *stack, tOpData element) {

	tElement newElem;


	if((newElem = malloc(sizeof(struct tElement))) == NULL) {
		errorHandler(errInt);
		return false;
	}

	newElem->data = element;
	newElem->pointer = stack->top;
	stack->top = newElem;

	return true;
}

void stackPop(tStack *stack, tOpData *data) {

	if(stack->top != NULL) {

		tElement temp;

		temp = stack->top;
		*data = stack->top->data;
		stack->top = stack->top->pointer;

		free(temp);
	}
}

tOpData stackTop(tStack *stack) {

	return (stack->top->data);
}

void stackDispose(tStack *stack) {

	tOpData temp;

	while(stackEmpty(stack) != true) {

		stackPop(stack, &temp);
	}

	stack->top = NULL;
}


tPriority precedenceTable [14][14] = {

	{pMORE, pMORE, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pMORE, pMORE, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pMORE, pMORE, pMORE, pMORE, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pMORE, pMORE, pMORE, pMORE, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pEQUAL, pLESS, pLESS, pLESS, pLESS, pLESS, pLESS, pLESS, pEMPTY},
	{pMORE, pMORE, pMORE, pMORE, pEMPTY,pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pEMPTY, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pLESS, pMORE},
	{pMORE, pMORE, pMORE, pMORE, pEMPTY, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pMORE, pEMPTY, pMORE},
	{pLESS, pLESS, pLESS, pLESS, pLESS, pEMPTY, pLESS, pLESS, pLESS, pLESS, pLESS, pLESS, pLESS, pEMPTY},
};

int zpracuj(token tok, tOpData *column) {

	char *key;
	tNodePtr node;

	switch(tok->type) {

		case 31:
			column->element = PLUS;
			break;
		
		case 32:
			column->element = MINUS;
			break;

		case 33:
			column->element = MUL;
			break;

		case 34:
			column->element = DIV;
			break;

		case 4:
			column->element = LEFT;
			break;

		case 5:
			column->element = RIGHT;
			break;

		case 35:
			column->element = LESS;
			break;

		case 36:
			column->element = MORE;
			break;

		case 37:
			column->element = LESSEQUAL;
			break;

		case 38:
			column->element = MOREEQUAL;
			break;

		case 39:
			column->element = EQUAL;
			break;

		case 40:
			column->element = NONEQUAL;
			break;

		case 3:								// strednik
		case 14:							// then
		case 17:							// do
		case 9:								// end

			column->element = DOLAR;
			break;

		case 20:

			column->element = ID;

			if((node = malloc(sizeof(struct tNodePtr))) == NULL) {

				errorHandler(errInt);
				return -1;
			}

			if((key = malloc(sizeof(char)*(strlen(tok->val_str) + 1))) == NULL) {

				errorHandler(errInt);
				return -1;
			}

			memset (key, 0, strlen(key)); //Treba key pred prvým strcatom vynulovať, inak ak je tam bordel, pripája sa až zaň.
			strcat(key, "V");
			strcat(key, tok->val_str);
		
			if((node = searchSymbol(&localTS, key)) != 0);
				//printf("Nasel jsem %s v lokalni tabulce symbolu.\n",key);

			else {

				if((node = searchSymbol(&rootTS, key)) != 0);
					//printf("Nasel jsem %s v globalni tabulce symbolu.\n",key);

				else {
					free(key);
					free(node);
					printf("Promenna nebyla nalezena.\n");
					return -1;
				}
			}

			column->key = key;

			//free(key);

			if((column->symbol = malloc(sizeof(struct tData))) != NULL) {

				if(node->data->type == sym_var_rea)
					column->symbol->type = t_expr_dou;
			
				if(node->data->type == sym_var_int)
					column->symbol->type = t_expr_int;
			
				if(node->data->type == sym_var_str)
					column->symbol->type = t_expr_str;
			
				if(node->data->type == sym_var_boo)
					column->symbol->type = t_expr_boo;
			}
			break;

		case 41:	// int, real, bool, string
		case 42:
		case 43:						
		case 44:	

			if((column->symbol = malloc(sizeof(struct tData))) != NULL) {
				
				column->element = ID;
				
				if(tok->type == t_expr_int)
					column->symbol->content.integer = tok->val_int;
				if(tok->type == t_expr_boo)
					column->symbol->content.boolean = tok->val_int;
				if(tok->type == t_expr_str)
					column->symbol->content.string = tok->val_str;
				if(tok->type == t_expr_dou)
					column->symbol->content.real = tok->val_flo;


				column->symbol->type = tok->type;
			}

			if((key = malloc(sizeof(char)*(strlen(randstring(20))))) == NULL) {

				errorHandler(errInt);
				return -1;
			}

			if((node = malloc(sizeof(struct tNodePtr))) == NULL) {

				errorHandler(errInt);
				return -1;
			}

			node = insertSymbol(&rootTS, key, column->symbol);
			column->key = key;

			//free(key);
			//free(column->symbol);	
			break;

		default:

			errorHandler(errSyn);
			return -1;	
	}

	return 0;
}


int precedenceParser() {				// hlavni funkce precedencni analyzy

	tStack stack1;
	stackInit(&stack1);

	tStack stack2;
	stackInit(&stack2);

	tOpData column;	// indexy do precedencni tabulky 
	tOpData row;
	tOpData temp;
	tOpData change;
	tOpData theEnd;

	numberOfExprInsts = 0;
	int error;
	int count = 0;
	int conStep = 1;
	int returnType = -1;
	bool skipGib=true;


	temp.element = DOLAR;
	stackPush(&stack1, temp);		// pushnuti dolaru na vrchol zasobniku

	do {

		if(conStep == 1) {
		
				if (skipGib==false)
					tok = gib_tok();
			
				skipGib=false;
				printf("TOKEN: %d\n", tok->type);
		
				if((error = zpracuj(tok, &column)) != 0)		// pokud skoncime s chybou, breakujeme
					break;

		}

		row = stackTop(&stack1);		// precteni tokenu na vrcholu zasobniku

		while(row.element == NETERM || row.element == SHIFT) { // preskakujeme na zasobniku neterminaly
			
			stackPop(&stack1, &change);
			stackPush(&stack2, change);
			row = stackTop(&stack1);
		}

		while(stackEmpty(&stack2) != true) {			// navraceni neterminalu na hlavni zasobnik

			stackPop(&stack2, &change);
			stackPush(&stack1, change);
		}

		printf("ROW: %d COLUMN: %d\n", row.element, column.element);

		switch(precedenceTable[row.element][column.element]) {

			case pEQUAL:				// equal
				
				conStep = 1;
				stackPush(&stack1, column);
				break;

			case pLESS:					// shift

				conStep = 1;
				while(stackTop(&stack1).element == NETERM || stackTop(&stack1).element == SHIFT) {	// preskakujeme na zasobniku neterminaly
			
					stackPop(&stack1, &change);
					stackPush(&stack2, change);
				}

				temp.element = SHIFT;						// pushnuti SHIFTu pred neterminal
				stackPush(&stack1, temp);

				while(stackEmpty(&stack2) != true) {		// vraceni neterminalu na zasobnik

					stackPop(&stack2, &change);
					stackPush(&stack1, change);
				}

				stackPush(&stack1, column);

				break;

			case pMORE:					// redukce

				conStep = 0;

				if((returnType = reduction(&stack1, &stack2)) < 0)
					return -1;	

				break;

			case pEMPTY:							// empty, syntax error

				stackDispose(&stack1);
				stackDispose(&stack2);
				errorHandler(errSyn);
				return -1;
				break;
		}

		while(stackTop(&stack1).element == NETERM || stackTop(&stack1).element == SHIFT) {

			stackPop(&stack1, &change);
			stackPush(&stack2, change);
		}

		theEnd = stackTop(&stack1);

		while(stackEmpty(&stack2) != true) {

			stackPop(&stack2, &change);
			stackPush(&stack1, change);
		}

	} while((column.element != DOLAR) || (theEnd.element != DOLAR));
	
	stackDispose(&stack1);
	stackDispose(&stack2);


	return returnType;	// jeste nedokonceno, zatim mi to funguje jen pro pravidlo E -> i, protoze se nedokazu vickrat zacyklit v te redukci
}


int reduction(tStack *stack1, tStack *stack2) {

	tOpData help = stackTop(stack1);
	tOpData change;
	tOpData temp1;
	tOpData temp2;
	tOpData temp3;
	int concat = 0;
	bool boolean = false;
	int returnType = -1;
	int matusOp;
	int control = 0;
	int checkRule;
	printf("reduction\n");

	//printf("HELP: %d\n", help.element);

	while(help.element != SHIFT/* && stackEmpty(stack1) != true*/) {			//	dostaneme se az na SHIFT, vse na druhem zasobniku pouzijeme pro jedno z pravidel

		stackPop(stack1, &change);
		stackPush(stack2, change);
		help = stackTop(stack1);
	}


	//help = stackTop(stack1);
	if(help.element == SHIFT) {

		stackPop(stack2, &temp1);	// nacteme si dalsi oper z druheho zasobniku
		printf("%d\n", temp1.element);
		
		if(temp1.element == ID && stackEmpty(stack2) == true) {		// zacneme od nejjednodusiho - E->ID
			

			/*checkRule = PLUS;
			if((matusOp = myOp2matousOp(checkRule, temp1.symbol->type)) != -1) {

				numberOfExprInsts++;
				if(localIL == NULL) {
							
					insertInst(&IL, matusOp, searchData(temp1.key), NULL, NULL);
					printf("Vlozil jsem instrukci %d s ukazatelem %u do listu %u\n", matusOp, &temp1.symbol, &IL);
				}
				else {

					insertInst(localIL, matusOp, searchData(temp1.key), NULL, NULL);
					printf("Vlozil jsem instrukci %d s ukazatelem %u do listu %u\n", matusOp, &temp1.symbol, localIL);
				}
					
				returnType = temp1.symbol->type;
			}*/

			returnType = temp1.symbol->type;
			stackPop(stack1, &change);		// popneme SHIFT, ze zasobniku, uz neni potreba
			change = temp1;
			change.element = NETERM;
			stackPush(stack1, change);		// pushneme dle pravidla - E (neterminal)
		}

		else if(temp1.element == NETERM) {	// nyni vsechna pravidla pro neterminaly

			stackPop(stack2, &temp2);
			printf("%d\n", temp2.element);

			switch(temp2.element) {
				//printf("%d\n", temp2.symbol->type);

				case PLUS:
					checkRule = PLUS;
					control = 1;
					break;
				case MINUS:
				
					checkRule = MINUS;
					control = 1;
					break;
				case MUL:
					checkRule = MUL;
					control = 1;
					break;
				case DIV:
					checkRule = DIV;
					control = 1;
					break;
				case LESS:
					checkRule = LESS;
					control = 1;
					boolean = true;
					break;
				case MORE:
					checkRule = MORE;
					boolean = true;
					control = 1;
					break;
				case MOREEQUAL:
					checkRule = MOREEQUAL;
					boolean = true;
					control = 1;
					break;
				case LESSEQUAL:
					checkRule = LESSEQUAL;
					boolean = true;
					control = 1;
					break;
				case EQUAL:
					checkRule = EQUAL;
					boolean = true;
					control = 1;
					break;
				case NONEQUAL:
					checkRule = NONEQUAL;
					boolean = true;
					control = 1;
					break;

				default:
					control = 0;
					errorHandler(errSyn);
					break;
			}

			if(control == 0)
				return -1;

			else {

				stackPop(stack2, &temp3);
				printf("%d\n", temp3.element);

				if(temp3.element == NETERM) {

					if(temp1.symbol->type == temp3.symbol->type) {

						if(temp1.symbol->type == t_expr_str) {

							if(checkRule == PLUS)
								concat = 1;

							else if(checkRule == LESS || checkRule == MORE || checkRule == MOREEQUAL || checkRule == LESSEQUAL || checkRule == EQUAL || checkRule == NONEQUAL)
								concat = 0;
							
							else {

								fprintf(stderr, "S retezci se tato operace neda provest.\n");
								return -1;
							} 
						}

						if(boolean == true)
							returnType = t_expr_boo;
						else
							returnType = temp1.symbol->type;

						numberOfExprInsts++;

						if((matusOp = myOp2matousOp(checkRule, temp1.symbol->type)) != -1) {
							
							if(localIL == NULL) {
								
								insertInst(&IL, matusOp, searchData(temp1.key), searchData(temp3.key), NULL);
								printf("Vlozil jsem instrukci %d s ukazateli %d a %d do listu %u\n", matusOp, temp1.symbol->content.integer, temp3.symbol->content.integer, &IL);
							}
							else {

								insertInst(localIL, matusOp, searchData(temp1.key), searchData(temp3.key), NULL);
								printf("Vlozil jsem instrukci %d s ukazateli %u a %u do listu %u\n", matusOp, temp1.symbol->content.integer, temp3.symbol->content.integer, localIL);
							}
						}
					}
					else if((temp1.symbol->type == t_expr_int && temp3.symbol->type == t_expr_dou) || (temp3.symbol->type == t_expr_int && temp1.symbol->type == t_expr_int)) {

						if(temp1.symbol->type == t_expr_int) {		// bud prvni je int a pretypujeme
							
							temp1.symbol->content.real = (double) temp1.symbol->content.integer;
							temp1.symbol->type = t_expr_dou;
						}

						if(temp3.symbol->type == t_expr_int)	{		// nebo druhy
							
							temp3.symbol->content.real = (double) temp3.symbol->content.integer;
							temp3.symbol->type = t_expr_dou;
						}

						if(boolean == 1)							// pokud mame logickou operaci, musime vracet boolean hodnotu
							returnType = t_expr_boo;
						else
							returnType = temp1.symbol->type;

						numberOfExprInsts++;

						if((matusOp = myOp2matousOp(checkRule, temp1.symbol->type)) != -1) {
							
							if(localIL == NULL) {
								
								insertInst(&IL, matusOp, searchData(temp1.key), searchData(temp3.key), NULL);
								printf("Vlozil jsem instrukci %d s ukazateli %u a %u do listu %u\n", matusOp, &temp1.symbol, &temp3.symbol, &IL);
							}
							else {

								insertInst(localIL, matusOp, searchData(temp1.key), searchData(temp3.key), NULL);
								printf("Vlozil jsem instrukci %d s ukazateli %u a %u do listu %u\n", matusOp, &temp1.symbol, &temp3.symbol, localIL);
							}
						}
					}

					else {
						//printf("Ve vyrazu nejsou stejne typy.\n");
						errorHandler(errSemTypArg);
					}


					if(stackEmpty(stack2) == true) {
						printf("ahoj\n");
						stackPop(stack1, &change);	// odstraneni <
						change = temp1;
						change.element = NETERM;
						stackPush(stack1, change);
					}
				}

				else {

					errorHandler(errSyn);
					return -1;
				} 
			}
		}

		else if(temp1.element == LEFT) {

			stackPop(stack2, &temp2);

			if(temp2.element == NETERM) {

				stackPop(stack2, &temp3);

				if(temp3.element == RIGHT && stackEmpty(stack2) == true) {

					stackPop(stack1, &change);
					change.element = NETERM;
					change = temp2;
					stackPush(stack1, change);
				}
			}
		}

		else {

			errorHandler(errSyn);
			return -1;
		}
	}

	return returnType;

}

char *randstring(int length) {   

	int mySeed = 27054685;
    srand(time(NULL) * length * ++mySeed);

    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    size_t stringLen = 26*2+10+7;        
    char *randomString;

    randomString = malloc(sizeof(char) * (length));

    if (!randomString) {
        return (char*)0;
    }

    unsigned int key = 0;

    for (int n = 0;n < length;n++) {          
        key = rand() % stringLen;          
        randomString[n] = string[key];
    }

    //randomString[length] = '\0';

    return randomString;
}

int myOp2matousOp(int myOp, int type) {

	int matusOp;

	switch(myOp) {

		case PLUS:

			if(type == t_expr_int)
				matusOp = I_ADDI;

			else if(type == t_expr_dou) 
				matusOp = I_ADDR;

			else if(type == t_expr_str)
				matusOp = I_CONCATE;

			else
				return -1;

			break;

		case MINUS:

			if(type == t_expr_int)
				matusOp = I_SUBI;

			else if(type == t_expr_dou) 
				matusOp = I_SUBR;

			else
				return -1;

			break;
		case MUL:

			if(type == t_expr_int)
				matusOp = I_MULI;

			else if(type == t_expr_dou) 
				matusOp = I_MULR;

			else
				return -1;

			break;
		case DIV:

			if(type == t_expr_int)
				matusOp = I_DIVI;

			else if(type == t_expr_dou) 
				matusOp = I_DIVR;

			else
				return -1;

			break;
		case LESS:
			matusOp = I_LESS;
			break;
		case MORE:
			matusOp = I_MORE;
			break;
		case MOREEQUAL:
			matusOp = I_EMORE;
			break;
		case LESSEQUAL:
			matusOp = I_ELESS;
			break;
		case EQUAL:
			matusOp = I_EQUAL;
			break;
		case NONEQUAL:
			matusOp = I_NEQUAL;
			break;
	}

	return matusOp;
}

tNodePtr searchData(char *key) {

	tNodePtr node;

	if((node = searchSymbol(&localTS, key)) != 0);
				//printf("Nasel jsem %s v lokalni tabulce symbolu.\n",key);
	else {

		if((node = searchSymbol(&rootTS, key)) != 0);
					//printf("Nasel jsem %s v globalni tabulce symbolu.\n",key);
		else {
			free(key);
			free(node);
			printf("Promenna nebyla nalezena.\n");
			return NULL;
		}
	}

	return node;
}
