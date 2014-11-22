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
		printf("jsem v pici, nepovedl se malloc\n");
		return false;
	}

	newElem->data = element;
	newElem->pointer = stack->top;
	stack->top = newElem;

	return true;
}

void stackPop(tStack *stack) {

	if(stack->top != NULL) {

		tElement temp;

		temp = stack->top;
		stack->top = stack->top->pointer;

		free(temp);
	}
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

int zpracuj(tToken token, tOpData *column) {		// zjisteni typu tokenu, nastaveni inxdexu

	/*switch(token.type) {

		case "+":
			column->element = PLUS;
			break;
		
		case "-":
			column->element = MINUS;
			break;

		case "*":
			column->element = MUL;
			break;

		case "/":
			column->element = DIV;
			break;

		case "(":
			column->element = LEFT;
			break;

		case ")":
			column->element = RIGHT;
			break;

		case "<":
			column->element = LESS;
			break;

		case ">":
			column->element = MORE;
			break;

		case "<=":
			column->element = LESSEQUAL;
			break;

		case ">=":
			column->element = MOREEQUAL;
			break;

		case "=":
			column->element = EQUAL;
			break;

		case "<>":
			column->element = NONEQUAL;
			break;*/
											// cisla a retezce budu rovnou ukladat do tabulky symbolu
		/*case INTEGER:
		case REAL:

			column->element = ID;
			break;

		case STRING:

			column->element = ID;
			break;

						// atd......
	}*/

	return 0;
}


void precedenceParser() {				// hlavni funkce precedencni analyzy

	printf("Jsem v parseru.\n");

	tStack stack;
	stackInit(&stack);

	tOpData help;		// pro ulozeni dolaru na zasobnik
	tOpData column;	// indexy do precedencni tabulky 
	tOpData row;
	int error;
	tToken token;


	help.element = DOLAR;
	stackPush(&stack, help);		// pushnuti dolaru na vrchol zasobniku

	do {

		if((error = zpracuj(token, &column)) == 0) {

			printf("jsem po funkci zpracuj %d\n", error);

		}

	}

	while(stack.top->data.element != DOLAR);

	stackPop(&stack);

}


