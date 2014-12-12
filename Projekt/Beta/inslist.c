//#include "inslist.h"
//#include "ial.c"

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
	/*tListItem *tmp;
	
	while(L->first != NULL)
	{
		tmp = L->first;
		L->first = L->first->next;
		//free(tmp);
	}
	*/
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
		//printf("Vlozil jsem instrukci %d\n__________________________________\n", new->instruction.instype);

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
	//printf("%d INLAST\n",IN->last);
	//tListItem * revert = IN->last;
	//printf("inactive\n");
	Select(IN, start->next);
	for (int i = 0; i < howMuch; i++)
	{
		printf("replikujem ty kokot\n");
		insertInst(OUT, IN->active->instruction.instype, IN->active->instruction.adr1, IN->active->instruction.adr2, IN->active->instruction.result);
		Succ(IN);
	}

	IN->active=NULL;

}

void Select(tInsList *L, tListItem *destination)
{
	//printf("aktivny bol %u\n",L->active );
	L->active = destination;
	//printf("aktivny je teraz %u\n",L->active );
}

void Succ(tInsList *L)
//posune aktivitu na dalsi prvok v zozname
{
	/*if(L->active != NULL)
	{
		L->active = L->active->next;
	}*/
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
		printf("nie je aktivna instrukcia\n");		
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

/*int main()
{
	return 0;
}*/