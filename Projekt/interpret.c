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

int interpret(tNodePtr *TS, tInsList *IL)	//precitaj si zadanie real %g, atd
{

	int l;
	int r;

	First(IL);
	tInstruction *new;
	//tData vysl;
	do
	{

		new = Copy(IL);
		
		switch(new->instype)
		{
							/*ARITMETICKE OPERACIE*/
			case I_ADDI:
				(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer + ((tData)new->adr2)->content.integer);
				printf("%d\n", ((tData)new->result)->content.integer);
				break;

			case I_ADDR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real + ((tData)new->adr2)->content.real);
				printf("%g\n", ((tData)new->result)->content.real);
				break;
			
			case I_CONCATE:		
				(((tData) new->result)->content.string) =  concate(((tData) new->adr1)->content.string, ((tData) new->adr2)->content.string);			
				printf("%s\n", ((tData)new->result)->content.string);
				break;

			case I_SUBI:
				(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer - ((tData)new->adr2)->content.integer);
				printf("%d\n", ((tData)new->result)->content.integer);
				break;

			case I_SUBR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real - ((tData)new->adr2)->content.real);
				printf("%g\n", ((tData)new->result)->content.real);
				break;

			case I_MULI:
				(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer * ((tData)new->adr2)->content.integer);
				printf("%d\n", ((tData)new->result)->content.integer);
				break;		

			case I_MULR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real * ((tData)new->adr2)->content.real);
				printf("%g\n", ((tData)new->result)->content.real);
				break;
			
			case I_DIVI:
				(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer / ((tData)new->adr2)->content.integer);
				printf("%d\n", ((tData)new->result)->content.integer);
				break;

			case I_DIVR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real / ((tData)new->adr2)->content.real);
				printf("%g\n", ((tData)new->result)->content.real);
				break;		
			
			case I_ASGNI:
				(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer);
				printf("vysledok ASGNI: %i\n", (((tData) new->result)->content.integer));
				break;		

			case I_ASGNR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real);
				printf("vysledok ASGNR: %g\n", (((tData) new->result)->content.real));

			case I_ASGNS:
				(((tData) new->result)->content.string) = (((tData) new->adr1)->content.string);
				printf("vysledok ASGNS: %s\n", (((tData) new->result)->content.string));		

							/*LOGICKE OPERACIE*/
			case I_MORE:
				if((((tData) new->adr1)->content.integer) > (((tData) new->adr2)->content.integer))
				{
					(((tData) new->result)->content.boolean)= true;
					printf("%d\n", (((tData) new->result)->content.boolean));
				
				} else 
				{
					(((tData) new->result)->content.boolean) = false;
					printf("%d\n", (((tData) new->result)->content.boolean));
				}
				break;
			
			case I_LESS:
				if((((tData) new->adr1)->content.integer) < (((tData) new->adr2)->content.integer))
				{
					(((tData) new->result)->content.boolean) = true;
					printf("%d\n", (((tData) new->result)->content.boolean));
				
				} else 
				{
					(((tData) new->result)->content.boolean) = false;
					printf("%d\n", (((tData) new->result)->content.boolean));
				}
				break;

			case I_EMORE:
				if((((tData) new->adr1)->content.integer) >= (((tData) new->adr2)->content.integer))
				{
					(((tData) new->result)->content.boolean) = true;
					printf("%d\n", (((tData) new->result)->content.boolean));
				
				} else 
				{
					(((tData) new->result)->content.boolean) = false;
					printf("%d\n", (((tData) new->result)->content.boolean));
				}
				break;

			case I_ELESS:
				if((((tData) new->adr1)->content.integer) <= (((tData) new->adr1)->content.integer))
				{
					(((tData) new->result)->content.boolean)  = true;
					printf("%d\n", (((tData) new->result)->content.boolean));
				
				} else 
				{
					(((tData) new->result)->content.boolean) = false;
					printf("%d\n", (((tData) new->result)->content.boolean));
				}
				break;
		
			case I_EQUAL:
				if((((tData) new->adr1)->content.integer) == (((tData) new->adr2)->content.integer))
				{
					(((tData) new->result)->content.boolean) = true;
					printf("%d\n", (((tData) new->result)->content.boolean));
				
				} else 
				{
					(((tData) new->result)->content.boolean) = false;
					printf("%d\n", (((tData) new->result)->content.boolean));
				}
				break;
			
			case I_NEQUAL:
				if((((tData) new->adr1)->content.integer) != (((tData) new->adr2)->content.integer))
				{
					(((tData) new->result)->content.boolean) = true;
					printf("%d\n", (((tData) new->result)->content.boolean));
				
				} else 
				{
					(((tData) new->result)->content.boolean) = false;
					printf("%d\n", (((tData) new->result)->content.boolean));
				}
				break;
							/*FUNKCIE*/
			
			case I_READI:
				scanf("%i", &(((tData) new->result)->content.integer));
				printf("%i\n", (((tData) new->result)->content.integer));		
				break;

			case I_READR:
				scanf("%lg", &(((tData) new->result)->content.real));
				printf("%lg\n", (((tData) new->result)->content.real));
				break;	

			case I_READS:
				scanf("%s", (((tData) new->result)->content.string));
				printf("%s\n", (((tData) new->result)->content.string));
				break;

			case I_WRITEI:
				printf("%i\n", (((tData) new->result)->content.integer));
				break;

			case I_WRITER:
				printf("%g\n", (((tData) new->result)->content.real));
				break;

			case I_WRITES:
				printf("%s\n", (((tData) new->result)->content.string));
				break;
				
			case I_LENGTH:
				printf("nie je funkcia nie je interpret\n");
				break;

			case I_COPY:
				printf("nie je funkcia nie je interpret\n");
				break;

			case I_FIND:
				(((tData) new->result)->content.integer) = BMASearch((((tData) new->adr1)->content.string), (((tData) new->adr2)->content.string));
				printf("%d\n", (((tData) new->result)->content.integer));
				break;
		
			case I_SORT:
				l = 0;
				r = strlen((((tData) new->adr1)->content.string));
				r--;
				(((tData) new->result)->content.string) = allocQuickSort((((tData) new->adr1)->content.string), l, r);
				printf("%s\n", (((tData) new->result)->content.string));
				break;
			
			/*case I_IF:
				if(node3->data->content.boolean == true)	
				{
					//interpret(&rootTS, IL);
					printf("robim\n");
					break;
				} else 
				{
					//interpret(&rootTS, &IL);
					printf("robim else\n");
				break;
				}
			case I_WHILE:
				while(node->data->content.boolean == true)
				{
					
					break;
				}*/
		}	
		Succ(IL);

	} while(IL->active != NULL);
 return 0; 
}


int main()
{
	
	

	tInsList IL;
	InitList(&IL);

	init(&rootTS);

	tData koks = malloc(sizeof(struct tData));

	koks->name = "jmeno";
	koks->type = t_expr_int;
	koks->content.integer = 5;
	koks->content.string = "aliba";
	koks->content.real = 4;

	tData koks2 = malloc(sizeof(struct tData));

	koks2->name = "prijmenie";
	koks2->type = t_expr_int;
	koks2->content.integer = 50;
	koks2->content.string = "ba";
	koks2->content.real = 6;

	tData koks3 = malloc(sizeof(struct tData));

	koks3->name ="kokrspaniel";
	koks3->type = t_expr_int;
	koks3->content.integer = 2;


	tNodePtr node1 = insertSymbol(&rootTS, koks->name, koks);
	tNodePtr node2 = insertSymbol(&rootTS, koks2->name, koks2);
	tNodePtr node3 = insertSymbol(&rootTS, koks3->name, koks3);


	
	insertInst(&IL, I_ADDI, (node1->data), (node2->data), (node3->data));
	insertInst(&IL, I_ADDR, (node1->data), (node2->data), (node3->data));
	insertInst(&IL, I_CONCATE, (node1->data), (node2->data), (node3->data));
	insertInst(&IL, I_FIND, (node1->data), (node2->data), (node3->data));
	insertInst(&IL, I_SORT, (node1->data), (node2->data), (node3->data));	


	
	interpret(&rootTS, &IL);

	//disposeTable(&rootTS);
	//DisposeList(&IL);
	//free(koks);
	//free(koks2);
	
	return 0;
}