//#include "inslist.c"


tInsList LL;
tInsList LR;
bool lastbool;
bool vypocet = false;
int lastint = 0;
double lastdouble = 0;
char *laststring;


char *concate(char *s1, char *s2)
{
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);

	char *result = malloc(len1+len2 + 1);

	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);
	return result;
}

int interpret(tNodePtr *TS, tInsList *currIL)	//precitaj si zadanie real %g, atd
{
	printf("zacal interpret a som v insliste %u\n",currIL);

	tContent conOld[100];
	tNodePtr currentTerm; 
	int l;
	int r;
	int cmp;
	First(currIL);
	tInstruction *new;
	tNodePtr temp;
	tNodePtr temp2;



	while(currIL->active != NULL) {

		printf("INSTUKCIE: %d\n", currIL->active->instruction.instype);
		Succ(currIL);

	}

	First(currIL);

	//if (currIL->active->instruction.instype!=0)
	//	printf("NULLKURAV\n");else
	
	do
	{

		new = Copy(currIL);
		
		switch(new->instype)
		{
							//ARITMETICKE OPERACIE//
			case I_ADDI:
				//printf("adik zacal a kokot je %d\n",kokot);
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr)new->adr2);
					printf("%d\n", temp->data->content.integer);

					lastint += temp->data->content.integer + temp2->data->content.integer;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastint += temp2->data->content.integer;

				//(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer + ((tData)new->adr2)->content.integer);
				}
				printf("ADDI: %d\n", lastint);

				
				//printf("%d\n", ((tData)new->result)->content.integer);
				//printf("adik skoncil\n");
				break;
			
			case I_ADDR:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					lastdouble += temp->data->content.real + temp2->data->content.real;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastdouble += temp2->data->content.real;
				}
				printf("ADDR: %g\n", lastdouble);
				break;
			
			case I_CONCATE:		
				temp = ((tNodePtr) new->adr1);
				printf("%u %u\n", new->adr1,new->adr2);
				temp2 = ((tNodePtr) new->adr2);
				printf("temp2 %s\n",temp2->data->content.string);

				laststring =  concate(temp->data->content.string, temp2->data->content.string);			
				printf("CONCATE: %s\n", laststring);
				break;

			case I_SUBI:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					lastint = temp->data->content.integer - temp2->data->content.integer;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr)new->adr2);
					lastint -= temp2->data->content.integer;
				}
				printf("SUBI: %d\n", lastint);
				break;

			case I_SUBR:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					lastdouble = temp->data->content.real - temp2->data->content.real;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastdouble -= temp2->data->content.real;
				}
				printf("SUBR: %g\n", lastdouble);
				break;

			case I_MULI:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					lastint = temp->data->content.integer * temp2->data->content.integer;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastint *= temp2->data->content.integer;
				}
				printf("MULI: %d\n", lastint);
				break;		

			case I_MULR:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					lastdouble = temp->data->content.real * temp2->data->content.real;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastdouble *= temp2->data->content.real;
				}
				printf("MULR: %g\n", lastint);
				break;
			
			case I_DIVI:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					lastint = temp->data->content.integer / temp2->data->content.integer;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastint = lastint / temp2->data->content.integer;
				}
				printf("DIVI: %d\n", lastint);
				break;

			case I_DIVR:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					lastdouble = temp->data->content.real / temp2->data->content.real;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastdouble = lastdouble / temp2->data->content.real;
				}
				printf("DIVR: %g\n", lastdouble);
				break;		
			
			case I_ASGNI:						
				((tData) new->result)->content.integer = lastint;
				printf("vysledok ASGNI: %d\n", (((tData) new->result)->content.integer));	
				lastint = 0;
				vypocet = false;
				break;		

			case I_ASGNR:
				((tData) new->result)->content.real = lastdouble;
				printf("vysledok ASGNR: %g\n", (((tData) new->result)->content.real));
				lastdouble = 0;
				vypocet = false;
				break;

			case I_ASGNS:
				(((tData) new->result)->content.string) = malloc(sizeof(char) * strlen(laststring));
				strcpy((((tData) new->result)->content.string), laststring);
				laststring;// = NULL;
				
				if (debug==true)
					printf("vysledok ASGNS: %s\n", (((tData) new->result)->content.string));		
				break;

			case I_ASGNB:
				(((tData) new->result)->content.boolean) = lastbool;

				if (debug==true)
				printf("vysledok ASGNB: %d\n", (((tData) new->result)->content.boolean));
				break;		
								//LOGICKE OPERACIE//
			case I_MORE:
				
				temp = ((tNodePtr) new->adr1);
				temp2 = ((tNodePtr) new->adr2);

				if(temp->data->type == t_expr_int)
				{
					if(temp->data->content.integer > temp2->data->content.integer)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_dou)
				{
					if(temp->data->content.real > temp2->data->content.real)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp > 0)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else 
				{
					if(temp->data->content.boolean > temp2->data->content.boolean)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				}
				break;
			
			case I_LESS:
				//printf("som v lese\n");

				temp = ((tNodePtr) new->adr1);
				//printf("prva kokotina ok\n");
				temp2 = ((tNodePtr) new->adr2);

				//printf("dataypz je %d\n",temp->data->type );
				if(temp->data->type == t_expr_int)
				{//printf("je to int\n");
					//if((((tData) new->adr1)->content.integer) < (((tData) new->adr2)->content.integer))
					if(temp->data->content.integer < temp2->data->content.integer)
					{//printf("chcem nastavit\n");
						lastbool = true;//printf("nastavil som\n");
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{//printf("chcem nastavit fa\n");
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_dou)
				{
					if(temp->data->content.real < temp2->data->content.real)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp < 0)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else 
				{
					if(temp->data->content.boolean < temp2->data->content.boolean)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				}printf("koniec lesa\n");
				break;

			case I_EMORE:
				
				temp = ((tNodePtr) new->adr1);
				temp2 = ((tNodePtr) new->adr2);

				if(temp->data->type == t_expr_int)
				{
					if(temp->data->content.integer >= temp2->data->content.integer)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_dou)
				{
					if(temp->data->content.real >= temp2->data->content.real)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp >= 0)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else 
				{
					if(temp->data->content.boolean >= temp2->data->content.boolean)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				}
				break;

			case I_ELESS:
				
				temp = ((tNodePtr) new->adr1);
				temp2 = ((tNodePtr) new->adr2);
				
				if(temp->data->type == t_expr_int)
				{
					if(temp->data->content.integer <= temp2->data->content.integer)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_dou)
				{
					if(temp->data->content.real <= temp2->data->content.real)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp <= 0)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else 
				{
					if(temp->data->content.boolean <= temp2->data->content.boolean)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				}
				break;

			case I_EQUAL:
				
				temp = ((tNodePtr) new->adr1);
				temp2 = ((tNodePtr) new->adr2);

				if(temp->data->type == t_expr_int)
				{
					if(temp->data->content.integer == temp2->data->content.integer)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_dou)
				{
					if(temp->data->content.real == temp2->data->content.real)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean)= true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp == 0)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else 
				{
					if(temp->data->content.boolean == temp2->data->content.boolean)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				}
				break;
			
			case I_NEQUAL:
				
				temp = ((tNodePtr) new->adr1);
				temp2 = ((tNodePtr) new->adr2);

				if(temp->data->type == t_expr_int)
				{
					if(temp->data->content.integer != temp2->data->content.integer)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_dou)
				{
					if(temp->data->content.real != temp2->data->content.real)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else if(temp->data->type == t_expr_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp != 0)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				} else 
				{
					if(temp->data->content.boolean != temp2->data->content.boolean)
					{
						lastbool = true;
						//(((tData) new->result)->content.boolean) = true;
						//printf("%d\n", (((tData) new->result)->content.boolean));
				
					} else 
					{
						lastbool = false;
						//(((tData) new->result)->content.boolean) = false;
						//printf("%d\n", (((tData) new->result)->content.boolean));
					}
				}
				break;
							//FUNKCIE//
			
			case I_READI:
				if(scanf("%d", &(((tData) new->result)->content.integer)) != 1)
					errorHandler(errRunLoad);
				
				if (debug==true)
					printf("%d\n", (((tData) new->result)->content.integer));
				break;

			case I_READR:	
				if(scanf("%lg", &(((tData) new->result)->content.real)) != 1)
					errorHandler(errRunLoad);
				
				if (debug==true)
					printf("%lg\n", (((tData) new->result)->content.real));
				break;	

			case I_READS:	
				if(scanf("%s", (((tData) new->result)->content.string)) != 1)
					errorHandler(errRunLoad);
				
				if (debug==true)
					printf("%s\n", (((tData) new->result)->content.string));
				break;

			case I_WRITEI:
				printf("%i\n", (((tData) new->result)->content.integer));
				break;

			case I_WRITER:
				printf("%g\n", (((tData) new->result)->content.real));
				break;

			case I_WRITES:
				printf("sdsdd%s\n", (((tData) new->result)->content.string));
				break;
				
			case I_LENGTH:
				printf("nie je funkcia nie je interpret\n");
				break;

			case I_COPY:
				printf("nie je funkcia nie je interpret\n");
				break;

			case I_FIND:
				(((tData) new->result)->content.integer) = BMASearch((((tData) new->adr1)->content.string), (((tData) new->adr2)->content.string));
				printf("jythy%d\n", (((tData) new->result)->content.integer));
				break;
		
			case I_SORT:
				l = 0;
				r = strlen((((tData) new->adr1)->content.string));
				r--;
				(((tData) new->result)->content.string) = allocQuickSort((((tData) new->adr1)->content.string), l, r);
				printf("ergerg%s\n", (((tData) new->result)->content.string));
				break;
			
			case I_IF:printf("lastboo je %d\n",lastbool );
				if(lastbool == true)	
				{

					printf("then vetva\n");//lastbool = 0;

					interpret(&rootTS, ((tInsList *) new->adr1));
					printf("then vetva skoncila\n");	
								
					break;
				} else 
				{
					printf("volam picu z else\n");	//lastbool = 0;	
					interpret(&rootTS, ((tInsList *) new->adr2));
					
					break;
				}
			case I_VAR:break;
			
			case I_WHILE:
				printf("som vo vhwiweled\n");

				while(lastbool == true)
				{
					interpret(&rootTS, ((tInsList *) new->adr1));
					getchar();
				}
				break;

			case I_FCE:
				//tContent *conPtrs[100];
				

				currentTerm = (((tData) new->adr1)->nextArg);

				for(int i = 0; i < ((tData) new->adr1)->argCount; i++)
				{
					conOld[i] = currentTerm->data->content; 
					currentTerm->data->content = ((tContent*) new->adr1)[i];
					currentTerm = currentTerm->data->nextArg;
				}

				interpret(&(((tData) new->adr1)->localTSadr), &*(((tData) new->adr1)->localILadr));

				for(int i = 0; i < ((tData) new->adr1)->argCount; i++)
				{
					currentTerm->data->content = conOld[i]; 
					currentTerm = currentTerm->data->nextArg;
				}	
			break;
			default: printf("kokot dostal som %d\n",new->instype);
				break;
		}	
		Succ(currIL);

	} while(currIL->active != NULL);
 return 0; 
}


/*int main()
{
	
	

	tInsList IL;
	InitList(&IL);

	init(&rootTS);

	tData koks = malloc(sizeof(struct tData));

	koks->name = "yolo";
	koks->type = t_expr_int;
	koks->content.integer = 50;
	koks->content.string = "abb";
	//koks->content.real = 4;

	tData koks2 = malloc(sizeof(struct tData));

	koks2->name = "swag";
	koks2->type = t_expr_int;
	koks2->content.integer = 5;
	koks2->content.string = "ba";
	//koks2->content.real = 6;

	tData koks3 = malloc(sizeof(struct tData));

	koks3->name ="nub";
	koks3->type = t_expr_int;
	koks3->content.integer = 2;


	tNodePtr node1 = insertSymbol(&rootTS, koks->name, koks);
	tNodePtr node2 = insertSymbol(&rootTS, koks2->name, koks2);
	tNodePtr node3 = insertSymbol(&rootTS, koks3->name, koks3);


	
	//insertInst(&IL, I_ADDI, (node1->data), (node2->data), (node3->data));
	//insertInst(&IL, I_ADDR, (node1->data), (node2->data), (node3->data));
	//insertInst(&IL, I_CONCATE, (node1->data), (node2->data), (node3->data));
	//insertInst(&IL, I_FIND, (node1->data), (node2->data), (node3->data));
	
	insertInst(&IL, I_ADDI, (node1->data), (node2->data), (node3->data));
	insertInst(&IL, I_SUBI, (node1->data), (node2->data), (node3->data));	
	insertInst(&IL, I_CONCATE, (node1->data), (node2->data), (node3->data));	
	insertInst(&IL, I_SORT, (node1->data), (node2->data), (node3->data));		
	//insertInst(&IL, I_ADDI, (node1->data), (node2->data), (node3->data));
	//insertInst(&IL, I_WHILE, (node1->data), (node2->data), (node3->data));
	
	
	
	InitList(&LL);

	init(&rootTS);

	tData pes = malloc(sizeof(struct tData));

	pes->name = "alik";
	pes->type = t_expr_str;
	pes->content.integer = 500;
	pes->content.string = "abb";
	pes->content.real = 4;

	tData pes2 = malloc(sizeof(struct tData));

	pes2->name = "trezorek";
	pes2->type = t_expr_str;
	pes2->content.integer = 50;
	pes2->content.string = "ba";
	pes2->content.real = 6;

	tData pes3 = malloc(sizeof(struct tData));

	pes3->name ="kusaj";
	pes3->type = t_expr_int;
	pes3->content.integer = 2;


	tNodePtr node4 = insertSymbol(&rootTS, pes->name, pes);
	tNodePtr node5 = insertSymbol(&rootTS, pes2->name, pes2);
	tNodePtr node6 = insertSymbol(&rootTS, pes3->name, pes3);

	insertInst(&LL, I_ADDI, (node4->data), (node5->data), (node6->data));



	
	InitList(&LR);

	init(&rootTS);

	tData macka = malloc(sizeof(struct tData));

	macka->name = "jebnuty";
	macka->type = t_expr_int;
	macka->content.integer = 100;

	tData macka2 =  malloc(sizeof(struct tData));

	macka2->name = "GEGED";
	macka2->type = t_expr_int;
	macka2->content.integer = 1;

	tData macka3 = malloc(sizeof(struct tData));

	macka3->name = "DOLAN";
	macka3->type =  t_expr_int;
	macka3->content.integer = 5;

	tNodePtr node7 = insertSymbol(&rootTS, macka->name, macka);
	tNodePtr node8 =  insertSymbol(&rootTS, macka2->name, macka2);
	tNodePtr node9 = insertSymbol(&rootTS, macka3->name, macka3);


	insertInst(&LR, I_SUBI, (node7->data), (node8->data), (node9->data));

	interpret(&rootTS, &IL);

	//disposeTable(&rootTS);
	//DisposeList(&IL);
	//free(koks);
	//free(koks2);
	
	return 0;
}*/