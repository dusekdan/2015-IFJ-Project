/***********************inslist.c******************************/
/* Soubor: inslist.c - interpretacia jazyka IFJ14  			    */
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Matúš Bútora (xbutor01)						*/
/*					Roman Jaška  (xjaska00)						*/
/*					Filip Kalous (xkalou03)						*/
/****************************************************************/


void InitList(tInsList *L)
//inicializuje zoznam
{
	L->first = NULL;
	L->last = NULL;
	L->active = NULL;
}

void DisposeList(tInsList *L)
//zrusenie prvok zoznamu, zoznam sa nachadza v stave, v akom bol po inicializaci
{

	L->active = NULL;
	L->last = NULL;
}

void InsertLast(tInsList *L, tInstruction I)
//vlozi polozku na koniec zoznamu
{
	tListItem *new;
	if((new = malloc(sizeof(tListItem))) != NULL)
	{
		InsertLastMarius(& Smetisko, new);
		new->instruction = I;
		new->next = NULL;

		if(L->first == NULL)
		{
			L->first = new;
		
		} else
		{
			L->last->next = new;
		}	
		
		L->last = new;

	} else 
	{
		errorHandler(errInt);
	}
}

void InsertFirst(tInsList *L, tInstruction I)
//vlozi polozku na zaciatok zoznamu
{
	tListItem *new;
	if((new = malloc(sizeof(tListItem))) != NULL)
	{
		InsertLastMarius(& Smetisko, new);
		new->instruction = I;
		new->next = L->first;

		if(L->first == NULL)
		{
			L->last = new;
		
		} else 
		{
			L->first = new;
		}
		
	} else
	{
		errorHandler(errInt);
	}
}

void First(tInsList *L)
//nastavi aktivitu na prvy prvok
{
	L->active = L->first;
}

void Replicator(tInsList *IN, tInsList *OUT, tListItem *start, int howMuch)
{

	Select(IN, start->next);
	for (int i = 0; i < howMuch; i++)
	{
		insertInst(OUT, IN->active->instruction.instype, IN->active->instruction.adr1, IN->active->instruction.adr2, IN->active->instruction.result);
		Succ(IN);
	}

	IN->active=NULL;

}

void Select(tInsList *L, tListItem *destination)
{

	L->active = destination;
}

void Succ(tInsList *L)
//posune aktivitu na dalsi prvok v zozname
{

	if(L->active != NULL)
	{
		if(L->active->next != NULL)
		{
			 L->active = L->active->next;

		} else L->active = NULL;	
	}
}

tInstruction *Copy(tInsList *L)
//vrati hodnotu aktivneho prvku v zozname
{
	if(L->active == NULL)
	{	
		return NULL;
	
	} else return &(L->active->instruction);  	
}

bool insertInst (tInsList *list, int operace, void*adr1, void*adr2, void*result)
{

	if(list == NULL) return 0;
    bool retval = false;
    tInstruction *new = malloc(sizeof(tInstruction));
    InsertLastMarius(& Smetisko, new);
    new->instype = operace;
    new->adr1=adr1;
    new->adr2=adr2;
    new->result=result;
    
    InsertLast(list, *new);

    retval=true;
    return retval;

}