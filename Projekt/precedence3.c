/***********************precedence3.c****************************/
/* Soubor: precedenc3e.c - Syntaktická analýza výrazů 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/*					Matúš Bútora (xbutor01)						*/
/*					Roman Jaška	 (xjaska00)						*/
/****************************************************************/

#include "precedence3.h"
#include <unistd.h>
//#include "scanner2.c"


token gibtok() {
	getNextToken(fd, tok);
/*
	token tok = malloc(sizeof(struct token));

	char x = ' ';

	while(x == ' ') 
		scanf("%c", &x);

	switch(x) {

		case ';':
			tok->type = t_semicolon;
			break;

		case '(':
			tok->type = t_l_parrent;
			break;

		case ')':
			tok->type = t_r_parrent;
			break;

		case 'i':
			tok->type = t_expr_int;
			break;

		case 'd':
			tok->type = t_expr_dou;
			break;

		case 's':
			tok->type = t_expr_str;
			break;
		
		case 'v':
			tok->type = t_var_id;
			break;

		case '+':
			tok->type = t_plus;
			break;

		case '-':
			tok->type = t_minus;
			break;

		case '*':
			tok->type = t_mul;
			break;

		case '/':
			tok->type = t_div;
			break;

		case '<':
			tok->type = t_less;
			break;
	
		case '>':
			tok->type = t_more;
			break;

		case '=':
			tok->type = t_equal;
			break;

		case 'n':
			tok->type = t_nequal;
			break;

		case 'm':
			tok->type = t_moreeq;
			break;

		case 'l':
			tok->type = t_lesseq;
			break;

		default:
			printf("kokot\n");
			exit(1);
	}	
*/
	return tok;
}
/*
	Blok funkcí pro prácí se zásobníkem
*/

	/* Inicializace zásobníku */
void stackInit(tStack *stack) {

	stack->top = NULL;
}
	/* Funkce vrácí hodnotu true, pokud je zásobník prázdný. False v opačném případě. */
bool stackEmpty(tStack *stack) {

	if(stack->top == NULL)
		return true;

	else
		return false;
}
	
	/* Funkce vloží na zásobník novou položku i s daty. Pokud se vložení nepovede, vrací false. */
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
	/* Funkce smaže vrchní položku zásobníku a přes ukazatel tOpData *data vrací její data. */
void stackPop(tStack *stack, tOpData *data) {

	if(stack->top != NULL) {

		tElement temp;

		temp = stack->top;
		*data = stack->top->data;
		stack->top = stack->top->pointer;

		free(temp);
	}
}

	/* Funkce přečte vrchní položku a vrátí její data. Nic ze zásobníku nemaže! */
tOpData stackTop(tStack *stack) {

	return (stack->top->data);
}

	/* Funkce pro uvolnění celého zásobníku. Postupně prochází zásobníkem a maže každou položku zásobníku. */
void stackDispose(tStack *stack) {

	tOpData temp;

	while(stackEmpty(stack) != true) {

		stackPop(stack, &temp);
	}

	stack->top = NULL;
}

	/* Dvourozměrné pole pro uchování precedenční tabulky. */
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

	/* Funkce příjímá přečtěný token a ukazatel na data. Zjístí typ operace nebo hodnoty v tokenu (např. int nebo minus) a do dat uloží odpovídající hodnotu, 
	 * s kterou se dále pracuje při práci s precedenční tabulkou. */
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

			free(key);

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

		case 41:							// integer, bool
		case 44:	

			if((column->symbol = malloc(sizeof(struct tData))) != NULL) {
				column->element = ID;
				column->symbol->content.integer = tok->val_int;
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

			free(key);
			//free(column->symbol);
			break;

		case 42: 						// string

			if((column->symbol = malloc(sizeof(struct tData))) != NULL) {
				column->element = ID;
				column->symbol->content.string = tok->val_str;
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
			free(key);
			//free(column->symbol);
			break;

		case 43:						// realna hodnota

			if((column->symbol = malloc(sizeof(struct tData))) != NULL) {
				column->element = ID;
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
			free(key);
			//free(column->symbol);
			break;

		default:

			errorHandler(errSyn);
			return -1;	
	}

	return 0;
}

/* Řídící funkce precedenční syntaktické analýzy. Při správném průchodu celou analýzou výrazu vrací datový typ výsledku. 
 * Celý výraz je před zpracováním převeden z infixové reprezentace výrazu na postfixovou. */
int precedenceParser() {

	tStack stack1;
	stackInit(&stack1);

	tStack stack2;
	stackInit(&stack2);							// inicializace zásobníků

	tOpData temp;

	infix2post(&stack1, &stack2);				// prevedeni vyrazu na postfixovou notaci

	while(stackEmpty(&stack2) != true) {		// prechozeni na druhy zasobnik

		stackPop(&stack2, &temp);
		stackPush(&stack1, temp);
	}

	int x = reduction(&stack1, &stack2);		// provedeni redukce
	//printf("Navratovy typ vyrazu: %d\n", x);

	stackDispose(&stack1);						// zrušení a uvolnění paměti obou zásobníků
	stackDispose(&stack2);


	return x;
}

/* Funkce zajišťující zkontrolování výrazu podle pravidel a precedenční tabulky. */
int reduction(tStack *stack1, tStack *stack2) {

	tOpData temp;
	tOpData change;
	tOpData help;				// pomocné proměnné se strukturou tOpData pro práci se zásobníkem
	int checkRule;
	int control;
	int endCheck = 0;
	int returnType = - 1;		// proměnná uchovávající návrátový typ výrazu
	int concat;					// rozlišení zda budeme provádět matematickou operaci sčítání nebo konkatenaci řetězců
	int boolean = 0;
	int matusOp;


	while(stackEmpty(stack1) != true) {

		temp = stackTop(stack1);
		
		if(temp.element == ID) {		// pouzijeme pravidlo E -> i, pokud ne, tk jen pushneme da druhy zasobnik

			stackPop(stack1, &change);
			temp.element = NETERM;
			stackPush(stack2, temp);
			temp = stackTop(stack1);

			if(stackTop(stack1).element == DOLAR)
				return stackTop(stack1).symbol->type;
		}

		else {

			endCheck++;
			stackPop(stack1, &change);
			stackPush(stack2, change);
		}
	}

	while(stackEmpty(stack2) != true) {		// prehozeni si vyrazu na druhy zasobnik

		stackPop(stack2, &temp);
		stackPush(stack1, temp);
	}

	endCheck--;		// kvuli Dolaru

	while(endCheck != 0) {

		endCheck--;

		change = stackTop(stack1);			// zatim si ho ponecham, podle dalsiho bud pushnu nebo ne

		if(change.element == NETERM) {		// pravidlo zatim funguje

			stackPop(stack1, &change);
			help = stackTop(stack1);
			returnType = change.symbol->type;

			if(help.element == NETERM) {		// 2. E

				stackPop(stack1, &help);

				if((stackTop(stack1).element) == NETERM) {	// pokud 3. E, tk popneme a psuhneme na 2. zasobnik

					while((stackTop(stack1).element ) == NETERM) {	// dokud dostavam E, tk pushuju na druhy zasobnik

						stackPop(stack1, &temp);
						stackPush(stack2, temp);		// pushneme prvni E na druhy zasobnik
					}
				}

				stackPop(stack1, &temp);	// nacteme 3. znak

				switch(temp.element) {		// zjisteni operatoru, nyni uz vime, kterou operaci budeme s neterminaly provadet

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
						boolean = 1;
						break;
					case MORE:
						checkRule = MORE;
						control = 1;
						boolean = 1;
						break;
					case MOREEQUAL:
						checkRule = MOREEQUAL;
						control = 1;
						boolean = 1;
						break;
					case LESSEQUAL:
						checkRule = LESSEQUAL;
						control = 1;
						boolean = 1;
						break;
					case EQUAL:
						checkRule = EQUAL;
						control = 1;
						boolean = 1;
						break;
					case NONEQUAL:
						checkRule = NONEQUAL;
						control = 1;
						boolean = 1;
						break;

					default:
						control = 0;
						boolean = 0;
						break;
				}

				if(control == 1) {
					/* Pokud jsou oba neterminály stejného typu */
					if(change.symbol->type == help.symbol->type) {

						if(change.symbol->type == t_expr_str) {
							
							if(checkRule == PLUS)
								concat = 1;
							
							else if(checkRule == LESS || checkRule == MORE || checkRule == MOREEQUAL || checkRule == LESSEQUAL || checkRule == EQUAL || checkRule == NONEQUAL)
								concat = 0;
							else {

								printf("S retezci se tato operace neda provest.\n");
								return -1;
							}
						}
						else 
							concat = 0;


						if(boolean == 1)						// pokud budeme provádět logickou operaci, výsledný datový typ musí být boolean
							returnType = t_expr_boo;
						else
							returnType = change.symbol->type;

						if((matusOp = myOp2matousOp(checkRule, change.symbol->type)) != -1) {		// zapsani instrukce do instrukcniho listu

							insertInst(NULL, matusOp, &change.symbol, &help.symbol, NULL);
							printf("Vlozil jsem instrukci %d s ukazateli %u a %u\n", matusOp, &change.symbol, &help.symbol);
						}
					}
					/* Pokud je jeden z neterminálů typu integer a druhý je reálného typu */
					else if((change.symbol->type == t_expr_int && help.symbol->type == t_expr_dou) || (help.symbol->type == t_expr_int && help.symbol->type == t_expr_int)) {
						
						if(change.symbol->type == t_expr_int) {		// bud prvni je int a pretypujeme
							
							change.symbol->content.real = (double) change.symbol->content.integer;
							change.symbol->type = t_expr_dou;
						}

						if(help.symbol->type == t_expr_int)	{		// nebo druhy
							
							help.symbol->content.real = (double) help.symbol->content.integer;
							help.symbol->type = t_expr_dou;
						}


						if(boolean == 1)							// pokud mame logickou operaci, musime vracet boolean hodnotu
							returnType = t_expr_boo;
						else
							returnType = change.symbol->type;

						if((matusOp = myOp2matousOp(checkRule, change.symbol->type)) != -1) {		// zapsani instrukce do instrukcniho listu

							insertInst(&IL, matusOp, &change.symbol, &help.symbol, NULL);
							printf("Vlozil jsem instrukci %d s ukazateli %u a %u\n", matusOp, &change.symbol, &help.symbol);
						}
					}
					/* Chybné výrazy */
					else {
						//printf("Ve vyrazu nejsou stejne typy.\n");
						errorHandler(errSemTypArg);
					}

					temp.element = NETERM;		// misto pravidla pushneme jen E
					stackPush(stack1, temp);

					if(stackEmpty(stack2) != true) {
						
						stackPop(stack2, &temp);
						stackPush(stack1, temp);
					}
				}
			}

			else {

				printf("Pro toto neexistuje zadne pravidlo111.\n");
				errorHandler(errSyn);
			}
		}

		else {
			printf("Pro toto neexistuje zadne pravidlo222.\n");
			errorHandler(errSyn);
		}
	}
	
	/*printf("------VYSTUP------\n");		//vypis vystupu
	while(stackEmpty(stack1) != true) {

		stackPop(stack1, &temp);
		printf("%d\n", temp.element);
	}*/

	return returnType;
}


	/* Pomocná funkce pro převod z infixu na postfix, určuje prioritu mezi operátory pomocí precedenční tabulky */
int priority(int x, int y) {

	switch(precedenceTable[x][y]) {

		case pEQUAL:

			return pEQUAL;
			break;

		case pEMPTY:

			return pEMPTY;
			break;

		case pLESS:

			return pLESS;
			break;

		case pMORE:

			return pMORE;
			break;

		default:

			return -1;
			break;
	}
/*
	if(x == LEFT)
		return 0;

	if(x == LESS || x == MORE || x == LESSEQUAL || x == MOREEQUAL || x == EQUAL || x == NONEQUAL)
		return 1;

	if(x == PLUS || x == MINUS)
		return 2;

	if(x == MUL || x == DIV)
		return 3;

	return -1;
	*/
}

	/* Funkce zajišťující převod z infixové reprezentace na postfixovou reprezentaci výrazu */
void infix2post(tStack *stack1, tStack *stack2) {

	tOpData temp;		// token
	tOpData help;		// pomocne
	bool skipGib=true;

	do {
		
		//Doplnene osetrenie prveho tokenu
		if (skipGib==false)
			tok=gibtok();
		skipGib=false;

		if((zpracuj(tok, &temp)) == 0) {

			if(temp.element == ID)
				stackPush(stack2, temp);

			else {
			
				if(temp.element == LEFT)
					stackPush(stack1, temp);

				else {

					if(temp.element == RIGHT) {

						stackPop(stack1, &help);

						while(help.element != LEFT) {
							stackPush(stack2, help);
							stackPop(stack1, &help);

						}
					}

					else {

						while(stackEmpty(stack1) != true && (priority(temp.element, stackTop(stack1).element) == pMORE)) {
						//while(stackEmpty(stack1) != true && (priority(temp.element) <= priority(stackTop(stack1).element))) {

							stackPop(stack1, &help);
							stackPush(stack2, help);
						}

						if(temp.element == DOLAR && stackEmpty(stack1) != true) {

							stackPush(stack2, (stackTop(stack1)));
						}
						else 
							stackPush(stack1, temp);
					}	
				}
			}	
		}
	}
	while(temp.element != DOLAR);

	/*while(stackEmpty(stack2) != true) {

		stackPop(stack2, &temp);
		printf("%d\n", temp.element);
	}*/
}

	/* Změnění operací na odpovídající v interpretu */
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

char *randstring(int length) {   

    srand(getpid());

    char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    size_t stringLen = 26*2+10+7;        
    char *randomString;

    randomString = malloc(sizeof(char) * (length +1));

    if (!randomString) {
        return (char*)0;
    }

    unsigned int key = 0;

    for (int n = 0;n < length;n++) {          
        key = rand() % stringLen;          
        randomString[n] = string[key];
    }

    randomString[length] = '\0';

    return randomString;
}