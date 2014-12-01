/********FUNKCIE********/
#define I_READ 		0
#define I_WRITE 	1
#define I_LENGTH 	2
#define I_COPY 		3
#define I_FIND 		4
#define I_SORT 		5
/*ARITEMETICKE OPERACIE*/
#define I_ADD 		6
#define I_SUB 		7
#define I_MUL 		8
#define I_DIV 		9
#define I_ASGN 		10
/***LOGICKE OPERACIE***/
#define	I_MORE 		11
#define I_LESS 		12
#define I_EMORE 	13
#define I_ELESS 	14
#define I_EQUAL 	15
#define I_NEQUAL 	16

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

