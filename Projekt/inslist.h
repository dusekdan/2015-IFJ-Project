/*ARITEMETICKE OPERACIE*/
#define I_ADDI 		0	//adr1 + adr2 = result
#define I_ADDR 		1   //adr1 + adr2 = result
#define I_CONCATE	2   //adr1 + adr2 = result
#define I_SUBI 		3   //adr1 - adr2 = result
#define I_SUBR 		4   //adr1 - adr2 = result 
#define I_MULI 		5   //adr1 * adr2 = result
#define I_MULR 		6   //adr1 * adr2 = result
#define I_DIVI 		7   //adr1 / adr2 = result
#define I_DIVR 		8   //adr1 / adr2 = result
#define I_ASGNI 	9	//result = adr1
#define I_ASGNR		10  //result = adr1 
#define I_ASGNS		11  //result = adr1
/***LOGICKE OPERACIE***/
#define	I_MORE 		12  //adr1 > adr2   result = true
#define I_LESS 		13  //adr1 < adr2   result = true
#define I_EMORE 	14  //adr1 >= adr2  result = true
#define I_ELESS 	15  //adr1 <= adr2  result = true
#define I_EQUAL 	16  //adr1 == adr2  result = true
#define I_NEQUAL 	17  //adr1 != adr2  result = true
/********FUNKCIE********/
#define I_READI 	18  //scanf result
#define I_READR		19  //scanf result
#define I_READS		20  //scanf result
#define I_WRITEI 	21  //printf result
#define I_WRITER	22  //printf result
#define I_WRITES	23  //printf result
#define I_LENGTH 	24  //nie je funkcia
#define I_COPY 		25  //nie je funkcia
#define I_FIND 		26  //result = BMASearch(adr1, adr2)  vrati v resulte poziciu podretazca adr2 v retazci adr1; adr1,adr2 su stringy result je integer
#define I_SORT 		27  //result =  allocQuickSort(adr1)
#define I_IF		28  //result == true spravi cast instrukcii od then, inak robi else
#define I_WHILE		29	//result == true
#define I_FOR		30  //result == true

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "errorHandler.c"
#include "ial.c"

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
bool insertInst (tInsList *list, int operace, void*adr1, void*adr2, void*result); //vlozi instrukciu do listu

