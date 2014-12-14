/***********************precedence.h*****************************/
/* Soubor: precedence.h - Syntaktická analýza výrazů 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/*					Matúš Bútora (xbutor01)						*/
/****************************************************************/

#include <time.h>



#define STACKSIZE 50;


typedef enum {

	pMORE,		// > redukce
	pLESS,		// < shift
	pEQUAL,		// = equal
	pEMPTY		// syntakticka chyba - prazdne misto v precedencni tabulce

} tPriority;

typedef enum {

	PLUS,		//0
	MINUS,		//1
	MUL,		//2
	DIV,		//3
	LEFT,		//4
	RIGHT,		//5
	LESS,		//6
	MORE,		//7
	LESSEQUAL,	//8
	MOREEQUAL,	//9
	EQUAL,		//10
	NONEQUAL,	//11
	ID,			//12
	DOLAR,		//13
	NETERM,		//14
	SHIFT		//15

} tOperators;

// struktura pro použití v precedenci, obsahuje tData z tabulky symbolu
typedef struct {

	tData symbol;
	tOperators element;
	char *key;

} tOpData;

/*  */
typedef struct tElement {

	tOpData data;
	struct tElement *pointer;		// nepouzivane naokonec

} *tElement;
/* ukazatel na vrchol zásobníku */
typedef struct {

	tElement top;

} tStack;


void stackInit(tStack *stack);		// inicializace zasobniku
//bool stackFull();					// kontrola plneho zasobniku
bool stackEmpty(tStack *stack);		// kontrola prazdneho zasobniku
tOpData stackTop(tStack *stack);
void stackPop(tStack *stack, tOpData *data);
bool stackPush(tStack *stack, tOpData element);
void stackDispose(tStack *stack);

int precedenceParser();
int zpracuj(token tok, tOpData *column);
int reduction(tStack *stack1, tStack *stack2);
char *randstring(int length);
int myOp2matousOp(int myOp, int type1, int type2);
tNodePtr searchData(char *key);