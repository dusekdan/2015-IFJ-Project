#include "inslist.c"






char *concate(char *s1, char *s2)
{
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);

	char *result = malloc(len1+len2 + 1);

	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);
	return result;
}

int interpret(tNodePtr *TS, tInsList *IL)
{

	int l;
	int r;
	/*tInsList IL;
	InitList(&IL);


	
	
	tInstruction *new = malloc(sizeof(tInstruction));

	new->instype = I_SORT;

	InsertFirst(&IL, *new);
	//printf("%d\n",IL.first->instruction.instype);


	init(&rootTS);*/
	
	tNodePtr node;
	tNodePtr node2;
	tNodePtr node3;

	tData data = malloc(sizeof(struct tData));
	tData data2 = malloc(sizeof(struct tData));
	tData data3 = malloc(sizeof(struct tData));

	data->name = "adr1";
	data->content.string = "alibaba";
	data->content.real = 2;
	
	data2->name = "adr2";
	data2->content.string = "li";
	data2->content.real = 10;
	
	data3->name = "result";
	data3->content.string = "macka";
	//data3->content.integer = 0;
	
	node = insertSymbol(&rootTS, "adr1", data);
	node2 = insertSymbol(&rootTS, "adr2", data2);
	node3 = insertSymbol(&rootTS, "result", data3);

	

	
	//printf("%s je hodnota adr1\n", node->data->content.string);
	//printf("%s je hodnota adr2\n", node2->data->content.string);*/

	//printf("%s\n", (*rootTS).key);
	//printf("%s\n", (*rootTS).lptr->key);
	//printf("%s\n", (*rootTS).rptr->key);
	//printf("%s\n", (*rootTS).rptr->lptr->key);

	/*tNodePtr hledam = searchSymbol(&rootTS, "adr1");

		printf("%s\n", hledam->key);*/
	First(IL);
	tInstruction *new;
	
	do
	{

		new = Copy(IL);
		
		switch(new->instype)
		{
							/*ARITMETICKE OPERACIE*/
			case I_ADDI:
				(node3->data->content.integer) = (node->data->content.integer) + (node2->data->content.integer);
				printf("vysledok ADDI: %i\n", node3->data->content.integer);
				break;

			case I_ADDR:
				(node3->data->content.real) = (node->data->content.real) + (node2->data->content.real);
				printf("vysledok ADDR: %f\n", node3->data->content.real);
				break;	
			
			case I_CONCATE:
				node3->data->content.string = concate(node->data->content.string, node2->data->content.string);
				printf("%s\n", node3->data->content.string);
				free(node3->data->content.string);
				break;

			case I_SUBI:
				(node3->data->content.integer) = (node->data->content.integer) - (node2->data->content.integer);
				printf("vysledok SUBI: %i\n", node3->data->content.integer);
				break;

			case I_SUBR:
				(node3->data->content.real) = (node->data->content.real) - (node2->data->content.real);
				printf("vysledok SUBR: %f\n", node3->data->content.real);
				break;

			case I_MULI:
				(node3->data->content.integer) = (node->data->content.integer) * (node2->data->content.integer);
				printf("vysledok MULI: %i\n", node3->data->content.integer);
				break;		

			case I_MULR:
				(node3->data->content.real) = (node->data->content.real) * (node2->data->content.real);
				printf("vysledok MULR: %f\n", node3->data->content.real);
				break;
			
			case I_DIVI:
				(node3->data->content.integer) = (node->data->content.integer) / (node2->data->content.integer);
				printf("vysledok DIV: %i\n", node3->data->content.integer);
				break;

			case I_DIVR:
				(node3->data->content.real) = (node->data->content.real) / (node2->data->content.real);
				printf("vysledok DIVR: %f\n", node3->data->content.real);
				break;		
			
			case I_ASGNI:
				(node3->data->content.integer) = (node->data->content.integer);
				printf("vysledok ASGNI: %i\n", node3->data->content.integer);
				break;		

			case I_ASGNR:
				(node3->data->content.real) = (node->data->content.real);
				printf("vysledok ASGNR: %f\n", node3->data->content.real);

			case I_ASGNS:
				(node3->data->content.string) = (node->data->content.string);
				printf("vysledok ASGNS: %s\n", node3->data->content.string);		

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
			
			case I_READI:
				scanf("%i", &(node3->data->content.integer));
				printf("%i\n", (node3->data->content.integer));		
				break;

			case I_READR:
				scanf("%lf", &(node3->data->content.real));
				printf("%lf\n", (node3->data->content.real));
				break;	

			case I_READS:
				scanf("%s", (node3->data->content.string));
				printf("%s\n", (node3->data->content.string));
				break;

			case I_WRITEI:
				printf("%i\n", (node3->data->content.integer));
				break;

			case I_WRITER:
				printf("%f\n", (node3->data->content.real));
				break;

			case I_WRITES:
				printf("%s\n", (node3->data->content.string));
				break;
				
			case I_LENGTH:
				printf("nie je funkcia nie je interpret\n");
				break;

			case I_COPY:
				printf("nie je funkcia nie je interpret\n");
				break;

			case I_FIND:
				(node3->data->content.integer) = BMASearch((node->data->content.string), (node2->data->content.string));
				printf("%d\n", node3->data->content.integer);
				break;
		
			case I_SORT:
				l = 0;
				r = strlen((node->data->content.string));
				r--;
				node3->data->content.string = allocQuickSort((node->data->content.string), l, r);
				printf("%s\n", node3->data->content.string);
				break;
			
			//case I_IF:
				//if(node3->data->content.boolean == true)	
		
		}	
		Succ(IL);

	} while(IL->active != NULL);

 return 0; 
}


int main()
{
	
	

	tInsList IL;
	InitList(&IL);


	insertInst (&IL,I_CONCATE, NULL, NULL, NULL);
	
	/*tInstruction *new = malloc(sizeof(tInstruction));

	new->instype = I_CONCATE;

	InsertLast(&IL, *new);*/

	insertInst (&IL,I_ADDR, NULL, NULL, NULL);

	//new->instype = I_ADDR;
	//InsertLast(&IL, *new);
	//printf("%d\n",IL.first->instruction.instype);


	init(&rootTS);
	
	/*tNodePtr node;
	tNodePtr node2;
	tNodePtr node3;

	tData data = malloc(sizeof(struct tData));
	tData data2 = malloc(sizeof(struct tData));
	tData data3 = malloc(sizeof(struct tData));

	data->name = "adr1";
	data->content.string = "alibaba";

	data2->name = "adr2";
	data2->content.string = "li";

	data3->name = "result";
	data3->content.string = "macka";

	node = insertSymbol(&rootTS, "adr1", data);
	node2 = insertSymbol(&rootTS, "adr2", data2);
	node3 = insertSymbol(&rootTS, "result", data3);



	
	//printf("%s je hodnota adr1\n", node->data->content.string);
	//printf("%s je hodnota adr2\n", node2->data->content.string);*/
	
	interpret(&rootTS, &IL);
	
	return 0;
}