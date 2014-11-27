#include "interpret.h"
#include "inslist.c"
#include "ial.c"




int interpret()
{

	//tInsList IL;
	tInsList IL;
	InitList(&IL);


	
	
	tInstruction *new = malloc(sizeof(tInstruction));

	new->instype = I_NEQUAL;

	InsertFirst(&IL, *new);
	//printf("%d\n",IL.first->instruction.instype);


	init(&rootTS);
	
	tNodePtr node;
	tNodePtr node2;
	tNodePtr node3;

	tData data = malloc(sizeof(struct tData));
	tData data2 = malloc(sizeof(struct tData));
	tData data3 = malloc(sizeof(struct tData));

	data->name = "adr1";
	data->content.integer = 50;

	data2->name = "adr2";
	data2->content.integer = 500;

	data3->name = "result";
	data3->content.integer = 0;

	node = insertSymbol(&rootTS, "adr1", data);
	node2 = insertSymbol(&rootTS, "adr2", data2);
	node3 = insertSymbol(&rootTS, "result", data3);


	printf("%i je hodnota adr1\n", node->data->content.integer);
	printf("%i je hodnota adr2\n", node2->data->content.integer);

	//printf("%s\n", (*rootTS).key);
	//printf("%s\n", (*rootTS).lptr->key);
	//printf("%s\n", (*rootTS).rptr->key);
	//printf("%s\n", (*rootTS).rptr->lptr->key);

	/*tNodePtr hledam = searchSymbol(&rootTS, "adr1");

		printf("%s\n", hledam->key);*/

	//while(1)
	//{
		//First(IL);
		switch(new->instype)
		{
							/*ARITMETICKE OPERACIE*/
			case I_ADD:
				(node3->data->content.integer) = (node->data->content.integer) + (node2->data->content.integer);
				printf("vysledok ADD: %i\n", node3->data->content.integer);
				break;
			
			case I_SUB:
				(node3->data->content.integer) = (node->data->content.integer) - (node2->data->content.integer);
				printf("vysledok SUB: %i\n", node3->data->content.integer);
				break;

			case I_MUL:
				(node3->data->content.integer) = (node->data->content.integer) * (node2->data->content.integer);
				printf("vysledok MUL: %i\n", node3->data->content.integer);
				break;		

			case I_DIV:
				(node3->data->content.integer) = (node->data->content.integer) / (node2->data->content.integer);
				printf("vysledok DIV: %i\n", node3->data->content.integer);
				break;		
			
			case I_ASGN:
				(node3->data->content.integer) = (node->data->content.integer);
				printf("vysledok ASGN: %i\n", node3->data->content.integer);
				break;		

							/*LOGICKE OPERACIE*/
			case I_MORE:
				if((node->data->content.integer) > (node2->data->content.integer))
				{
					node3->data->content.boolean = true;
					printf("%d\n", node3->data->content.boolean);
				
				} else 
				{
					node3->data->content.boolean = false;
					printf("%d\n", node3->data->content.boolean);
				}
				break;
			
			case I_LESS:
				if((node->data->content.integer) < (node2->data->content.integer))
				{
					node3->data->content.boolean = true;
					printf("%d\n", node3->data->content.boolean);
				
				} else 
				{
					node3->data->content.boolean = false;
					printf("%d\n", node3->data->content.boolean);
				}
				break;

			case I_EMORE:
				if((node->data->content.integer) >= (node2->data->content.integer))
				{
					node3->data->content.boolean = true;
					printf("%d\n", node3->data->content.boolean);
				
				} else 
				{
					node3->data->content.boolean = false;
					printf("%d\n", node3->data->content.boolean);
				}
				break;

			case I_ELESS:
				if((node->data->content.integer) <= (node2->data->content.integer))
				{
					node3->data->content.boolean = true;
					printf("%d\n", node3->data->content.boolean);
				
				} else 
				{
					node3->data->content.boolean = false;
					printf("%d\n", node3->data->content.boolean);
				}
				break;
		
			case I_EQUAL:
				if((node->data->content.integer) == (node2->data->content.integer))
				{
					node3->data->content.boolean = true;
					printf("%d\n", node3->data->content.boolean);
				
				} else 
				{
					node3->data->content.boolean = false;
					printf("%d\n", node3->data->content.boolean);
				}
				break;
			
			case I_NEQUAL:
				if((node->data->content.integer) != (node2->data->content.integer))
				{
					node3->data->content.boolean = true;
					printf("%d\n", node3->data->content.boolean);
				
				} else 
				{
					node3->data->content.boolean = false;
					printf("%d\n", node3->data->content.boolean);
				}
				break;
							/*FUNKCIE*/
		
				
		}	
	

 return 0; 
}


int main()
{
	
	//tInsList IL;
	//tNodePtr *rootTS;
	interpret();
	/*printf("%d\n", I_ADD);
	tInsList IL;
	InitList(&IL);
	
	tInstruction *new = malloc(sizeof(tInstruction));

	new->instype = I_ADD;


	InsertFirst(&IL, *new);
	printf("%d\n",IL.first->instruction.instype);

	tInstruction I;
	
	I->instype = 2;*/
	
	return 0;
}