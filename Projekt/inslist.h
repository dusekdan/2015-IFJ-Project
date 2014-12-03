/*ARITEMETICKE OPERACIE*/
#define I_ADDI 		0
#define I_ADDR 		1
#define I_CONCATE	2
#define I_SUBI 		3
#define I_SUBR 		4
#define I_MULI 		5
#define I_MULR 		6
#define I_DIVI 		7
#define I_DIVR 		8
#define I_ASGNI 	9	
#define I_ASGNR		10
#define I_ASGNS		11
/***LOGICKE OPERACIE***/
#define	I_MORE 		12
#define I_LESS 		13
#define I_EMORE 	14
#define I_ELESS 	15
#define I_EQUAL 	16
#define I_NEQUAL 	17
/********FUNKCIE********/
#define I_READI 	18
#define I_READR		19
#define I_READS		20
#define I_WRITEI 	21
#define I_WRITER	22
#define I_WRITES	23
#define I_LENGTH 	24
#define I_COPY 		25
#define I_FIND 		26
#define I_SORT 		27

/*#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "errorHandler.c"
#include "ial.c"*/

typedef struct 
{
	int instype;
	void *adr1;
	void *adr2;
	void *result;
} tInstruction;

typedef struct ListItem
{
	tInstruction instruction;
	struct ListItem *next;
}tListItem;

typedef struct 
{
	struct ListItem *first;
	struct ListItem *last;
	struct ListItem *active;
}tInsList;


void InitList(tInsList *L);						//inicializuje zoznam
void DisposeList(tInsList *L);					//zrusenie prvok zoznamu, zoznam sa nachadza v stave, v akom bol po inicializaci
void InsertLast(tInsList *L, tInstruction I);	//vlozi polozku na koniec zoznamu
void InsertFirst(tInsList *L, tInstruction I);  //vlozi polozku na zaciatok zoznamu
tInstruction *Copy(tInsList *L);				//vrati hodnotu aktivneho prvku v zozname
void Succ(tInsList *L);							//posune aktivitu na dalsi prvok v zozname
void First(tInsList *L); 						//nastavi aktivitu na prvy prvok

