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

int main() {

	precedenceParser();

	return 0;

}

token gibTok(int count) {

	token tok = malloc(sizeof(struct token));

	if(count == 0) {

		tok->type = t_expr_int;
		tok->val_int = 10;
	}
	if(count == 1) {
		
		tok->type = t_mul;
	}
	if(count == 2) {

		tok->type = t_expr_int;
		tok->val_int = 20;
	}
	if(count == 3) {
	
		tok->type = t_plus;
	}

	if(count == 4) {
	
		tok->type = t_expr_int;
		tok->val_int = 30;
	}
	if(count == 5) {

		tok->type = t_semicolon;
	}

	count++;
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

	printf("zpracovavam\n");
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

			column->element = DOLAR;
			break;

		case 20:

			if((key = malloc(sizeof(char)*(strlen(tok->val_str) + 1))) == NULL) {

				errorHandler(errInt);
				return -1;
			}

			strcat(key, "V");
			strcat(key, tok->val_str);


			if((searchSymbol(&rootTS, key)) == 0)
				return -1;
			if((searchSymbol(&localTS, key)) == 0)
				return -1;


		case 41:							// integer, bool
		case 42:
		case 43:

			column->element = ID;

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

	tOpData temp;		// pro ulozeni dolaru na zasobnik
	tOpData column;	// indexy do precedencni tabulky 
	tOpData row;
	tOpData change;
	int error;
	int count = 0;


	temp.element = DOLAR;
	stackPush(&stack1, temp);		// pushnuti dolaru na vrchol zasobniku

	do {

		token tok = gibTok(count);
		count++;
		
		if((error = zpracuj(tok, &column)) != 0)		// pokud skoncime s chybou, breakujeme
			break;
		
		row = stackTop(&stack1);		// precteni tokenu na vrcholu zasobniku

		
		while((row.element == NETERM || row.element == SHIFT) && row.element != DOLAR) { // preskakujeme na zasobniku neterminaly
			
			stackPop(&stack1, &change);
			stackPush(&stack2, change);
			row = stackTop(&stack1);
		}

		while(stackEmpty(&stack2) != true) {			// navraceni neterminalu na hlavni zasobnik

			stackPop(&stack2, &change);
			stackPush(&stack1, change);
		}

		printf("row = %d  column = %d\n", row.element, column.element);

		switch(precedenceTable[row.element][column.element]) {

			case pEQUAL:				// equal

				printf("equal\n");
				stackPush(&stack1, column);
				break;

			case pLESS:					// shift
				printf("shift\n");

				while(row.element == NETERM || row.element == SHIFT) {	// preskakujeme na zasobniku neterminaly
			
					stackPop(&stack1, &change);
					stackPush(&stack2, change);
					row = stackTop(&stack1);
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

				printf("redukce\n");

				if(reduction(&stack1, &stack2) < 0)
					return -1;

				if(column.element < DOLAR) {		// pokud mame jeste neco na vstupu potrebujeme pred E pushnout SHIFT
					
					stackPop(&stack1, &change);
					temp.element = SHIFT;
					stackPush(&stack1, temp);
					stackPush(&stack1, change);
				}
				
				stackPush(&stack1, column);			// pushujeme az po redukci, zbytecne by se nam tam operator pletl

				break;

			case pEMPTY:							// empty, syntax error

				printf("empty\n");
				printf("Syntaktická chyba.\n");
				errorHandler(errSyn);
				return -1;
				break;
		}
	}

	while(stack1.top->data.element != DOLAR);

	stackPop(&stack1, &temp);			// odstraneni dolaru

	while(stackEmpty(&stack1) != true) {		// vypis zbytku na zasobniku

		stackPop(&stack1, &temp);
		printf("%d\n", temp.element);

	}


	return 0;	// jeste nedokonceno, zatim mi to funguje jen pro pravidlo E -> i, protoze se nedokazu vickrat zacyklit v te redukci
}


int reduction(tStack *stack1, tStack *stack2) {

	tOpData help = stackTop(stack1);
	tOpData change;
	tOpData temp;

	printf("jsem v redukci\n");

	while(help.element != SHIFT && stackEmpty(stack1) != true) {			//	dostaneme se az na SHIFT, vse na druhem zasobniku pouzijeme pro jedno z pravidel
		
		stackPop(stack1, &change);
		stackPush(stack2, change);
		help = stackTop(stack1);
	}

	if(help.element == SHIFT) {

		stackPop(stack2, &temp);	// nacteme si dalsi oper z druheho zasobniku

		if(temp.element == ID) {		// zacneme od nejjednodusiho - E->ID

			stackPop(stack2, &temp);

			if(stackEmpty(stack2) == true) {	// prvni pravidlo splneno

				stackPop(stack1, &temp);		// popneme SHIFT, ze zasobniku, uz neni potreba
				temp.element = NETERM;
				stackPush(stack1, temp);		// pushneme dle pravidla - E (neterminal)
				
				printf("Bylo provedeno 12. pravidlo.\n");
			}
		}

		else if(temp.element == NETERM) {	// nyni vsechna pravidla pro neterminaly

			int control = 0;
			int checkRule;

			stackPop(stack2, &temp);

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
					break;
				case MORE:
					checkRule = MORE;
					control = 1;
					break;
				case MOREEQUAL:
					checkRule = MOREEQUAL;
					control = 1;
					break;
				case LESSEQUAL:
					checkRule = LESSEQUAL;
					control = 1;
					break;
				case EQUAL:
					checkRule = EQUAL;
					control = 1;
					break;
				case NONEQUAL:
					checkRule = NONEQUAL;
					control = 1;
					break;

				default:
					fprintf(stderr, "Nevyhovuje pravidlum1.\n");
					control = 0;
					errorHandler(errSyn);
					break;
			}

			if(control == 0)
				return -1;

			else {

				stackPop(stack2, &temp);

				if(temp.element == NETERM) {

					if(stackEmpty(stack2) == true) {

						stackPop(stack1, &temp);
						temp.element = NETERM;
						stackPush(stack2, temp);

						printf("Vyhovuje pravidlu %d.\n", checkRule + 1);
					}
				}

				else {

					fprintf(stderr, "Nevyhovuje pravidlum2.\n");
					errorHandler(errSyn);
					return -1;
				} 
			}
		}

		else if(temp.element == LEFT) {

			stackPop(stack2, &temp);

			if(temp.element == NETERM) {

				stackPop(stack2, &temp);

				if(temp.element == RIGHT && stackEmpty(stack2) == true) {

					stackPop(stack1, &temp);
					temp.element = NETERM;
					stackPush(stack1, temp);
				}
			}
		}

		else {

			fprintf(stderr, "Nevyhovuje pravidlum\n");
			errorHandler(errSyn);
			return -1;
		}
	}

	printf("redukce skoncila\n");
	return 0;

}


