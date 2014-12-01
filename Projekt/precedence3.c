/***********************precedence3.c****************************/
/* Soubor: precedence.c - Syntaktická analýza výrazů 			*/
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
/*
int main() {

	precedenceParser();

	return 0;

}*/

token gibtok() {
	gib_tok(tok);
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

int zpracuj(token tok, tOpData *column) {		// zjisteni typu tokenu, nastaveni inxdexu

	//printf("zpracovavam\n");
	char *key;

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

			if((key = malloc(sizeof(char)*(strlen(tok->val_str) + 1))) == NULL) {

				errorHandler(errInt);
				return -1;
			}

			strcat(key, "V");
			strcat(key, tok->val_str);
		
		
			if((searchSymbol(&localTS, key)) != 0)
				printf("Nasel jsem v lokalni tabulce symbolu.\n");

			else {

				if((searchSymbol(&rootTS, key)) != 0)
					printf("Nasel jsem v globalni tabulce symbolu.\n");

				else {
					printf("Promenna nebyla nalezena.\n");
					return -1;
				}
			}

		case 41:							// integer, bool
		case 42:
		case 43:
		case 44:

			column->element = ID;

			if((column->symbol = malloc(sizeof(struct tData))) != NULL) {

				if(tok->type == t_expr_int) {
					column->symbol->type = tok->type;
					column->symbol->content.integer = tok->val_int;
				}

				if(tok->type == t_expr_dou) {
					column->symbol->type = tok->type;
					column->symbol->content.real = tok->val_flo;
				}

				if(tok->type == t_expr_str) {
					column->symbol->type = tok->type;
					column->symbol->content.string = tok->val_str;
				}

				if(tok->type == t_expr_boo) {
					column->symbol->type = tok->type;
					column->symbol->content.boolean = tok->val_int;
				}
			}
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

	tOpData temp;

	infix2post(&stack1, &stack2);		// prevedeni vyrazu na postfixovou notaci

	while(stackEmpty(&stack2) != true) {		// prechozeni na druhy zasobnik + kontrolni vypsani

		stackPop(&stack2, &temp);
		stackPush(&stack1, temp);
	}

	int x = reduction(&stack1, &stack2);		// provedeni redukce
	printf("Navratovy typ vyrazu: %d\n", x);

	stackDispose(&stack1);
	stackDispose(&stack2);


	return x;
}

int reduction(tStack *stack1, tStack *stack2) {

	tOpData temp;
	tOpData change;
	tOpData help;
	int checkRule;
	int control;
	int endCheck = 0;
	int returnType = - 1;
	int concat;
	int boolean = 0;

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

				switch(temp.element) {

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

					if(change.symbol->type == help.symbol->type) {

						if(change.symbol->type == t_expr_str)

							if(checkRule == PLUS)
								concat = 1;
							else {

								printf("S retezci se da provest jen konkatenace.\n");
								return -1;
							}
						
						else 
							concat = 0;


						if(boolean == 1)
							returnType = t_expr_boo;
						else
							returnType = change.symbol->type;

						returnType = change.symbol->type;
					}
					// pretypovani na real z intu - musim dodelat, abych se dostal k hodnotam
					else if((change.symbol->type == t_expr_int && help.symbol->type == t_expr_dou) || (help.symbol->type == t_expr_int && help.symbol->type == t_expr_int)) {
						
						if(change.symbol->type == t_expr_int)		// bud prvni je int a pretypujeme
							change.symbol->type = t_expr_dou;

						if(help.symbol->type == t_expr_int)			// nebo druhy
							help.symbol->type = t_expr_dou;

						if(boolean == 1)							// pokud mame logickou operaci, musime vracet boolean hodnotu
							returnType = t_expr_boo;
						else
							returnType = change.symbol->type;

						returnType = change.symbol->type;
					}

					else {
						printf("Ve vyrazu nejsou stejne typy.\n");
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



int priority(int x) {

	if(x == LEFT)
		return 0;

	if(x == LESS || x == MORE || x == LESSEQUAL || x == MOREEQUAL || x == EQUAL || x == NONEQUAL)
		return 1;

	if(x == PLUS || x == MINUS)
		return 2;

	if(x == MUL || x == DIV)
		return 3;

	return -1;
}

void infix2post(tStack *stack1, tStack *stack2) {

	tOpData temp;		// token
	tOpData help;		// pomocne
	bool skipGib=true;

	do {
		
		//Doplnene osetrenie prveho tokenu
		if (skipGib==false)
			tok=gibtok();
		skipGib=false;
printf("--token je %d a skipGib je %d\n",tok->type, skipGib);

		if((zpracuj(tok, &temp)) == 0) {

			if(temp.element == ID) {
				stackPush(stack2, temp);
				//printf("%d\n", temp.element);
			}
			else {
			
				if(temp.element == LEFT)
					stackPush(stack1, temp);

				else {

					if(temp.element == RIGHT) {

						stackPop(stack1, &help);
						while(help.element != LEFT) {
							stackPush(stack2, help);
							stackPop(stack1, &help);
							//printf("%d\n", help.element);
						}
					}

					else {

						while(stackEmpty(stack1) != true && (priority(temp.element) <= priority(stackTop(stack1).element))) {

							stackPop(stack1, &help);
							stackPush(stack2, help);
							//printf("%d\n", help.element);
						}

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