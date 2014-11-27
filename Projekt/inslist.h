/*ARITEMETICKE OPERACIE*/
#define I_ADD 		0
#define I_SUB 		1
#define I_MUL 		2
#define I_DIV 		3
#define I_ASGN 		4
/***LOGICKE OPERACIE***/
#define	I_MORE 		5
#define I_LESS 		6
#define I_EMORE 	7
#define I_ELESS 	8
#define I_EQUAL 	9
#define I_NEQUAL 	10
/********FUNKCIE********/
#define I_READ 		11
#define I_WRITE 	12
#define I_LENGTH 	13
#define I_COPY 		14
#define I_FIND 		15
#define I_SORT 		16


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

