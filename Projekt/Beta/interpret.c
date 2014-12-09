//#include "inslist.c"


tInsList LL;
tInsList LR;
bool lastbool;
bool vypocet = false;
int kokot = 0;
int pojebsa;


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

	/*while(currIL->active != NULL) {

		printf("%d\n", currIL->active->instruction.instype);
		Succ(currIL);
	}

	First(currIL);
*/
	//if (currIL->active->instruction.instype!=0)
	//	printf("NULLKURAV\n");else
	
	do
	{

		new = Copy(currIL);
		
		switch(new->instype)
		{
							//ARITMETICKE OPERACIE//
			case I_ADDI:
				printf("adik zacal a kokot je %d\n",kokot);
				if(vypocet == false)
				{
					temp = searchSymbol(&rootTS, ((char *)new->adr1));
					temp2 = searchSymbol(&rootTS, ((char *)new->adr2));

					kokot += temp->data->content.integer + temp2->data->content.integer;
					vypocet = true;
				} else
				{
					temp2 = searchSymbol(&rootTS, ((char *)new->adr2));
					kokot += temp2->data->content.integer;

				//(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer + ((tData)new->adr2)->content.integer);
				}
				printf("nn %d\n", kokot);
				
				//printf("%d\n", ((tData)new->result)->content.integer);
				printf("adik skoncil\n");
				break;
			
			case I_ADDR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real + ((tData)new->adr2)->content.real);
				printf("sdcs%g\n", ((tData)new->result)->content.real);
				break;
			
			case I_CONCATE:		
				(((tData) new->result)->content.string) =  concate(((tData) new->adr1)->content.string, ((tData) new->adr2)->content.string);			
				printf("sdcsdc%s\n", ((tData)new->result)->content.string);
				break;

			case I_SUBI:
				printf("sub9k, ahoj!\n");
				(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer - ((tData)new->adr2)->content.integer);
				printf("subikokotkurva%d\n", ((tData)new->result)->content.integer);
				break;

			case I_SUBR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real - ((tData)new->adr2)->content.real);
				printf("sdcsdc%g\n", ((tData)new->result)->content.real);
				break;

			case I_MULI:
				(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer * ((tData)new->adr2)->content.integer);
				printf("htyhth%d\n", ((tData)new->result)->content.integer);
				break;		

			case I_MULR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real * ((tData)new->adr2)->content.real);
				printf("trfeferg%g\n", ((tData)new->result)->content.real);
				break;
			
			case I_DIVI:
				(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer / ((tData)new->adr2)->content.integer);
				printf("mtynt%d\n", ((tData)new->result)->content.integer);
				break;

			case I_DIVR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real / ((tData)new->adr2)->content.real);
				printf("brtbr%g\n", ((tData)new->result)->content.real);
				break;		
			
			case I_ASGNI:				
				//printf("%d\n", kokot);
				//printf("result je %u\n",new->result );
				printf("idem asignovat\n");
				((tData) &new->result)->content.integer = kokot;
				//printf("vysledok ASGNI: %d\n", (((tData) &new->result)->content.integer));	
				kokot = 0;
				vypocet = false;
				break;		

			case I_ASGNR:
				(((tData) new->result)->content.real) = (((tData) new->adr1)->content.real);
				printf("vysledok ASGNR: %g\n", (((tData) new->result)->content.real));
				break;

			case I_ASGNS:
				(((tData) new->result)->content.string) = (((tData) new->adr1)->content.string);
				printf("vysledok ASGNS: %s\n", (((tData) new->result)->content.string));		
				break;

			case I_ASGNB:
				(((tData) new->result)->content.boolean) = (((tData) new->adr1)->content.boolean);
				break;		
								//LOGICKE OPERACIE//
			case I_MORE:
				if(((tData) new->adr1)->type == t_expr_int)
				{
					if((((tData) new->adr1)->content.integer) > (((tData) new->adr2)->content.integer))
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
				} else if(((tData) new->adr1)->type == t_expr_dou)
				{
					if((((tData) new->adr1)->content.real) > (((tData) new->adr2)->content.real))
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
				} else if(((tData) new->adr1)->type == t_expr_str)
				{
					cmp = strcmp((((tData) new->adr1)->content.string), (((tData) new->adr2)->content.string));
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
					if((((tData) new->adr1)->content.boolean) > (((tData) new->adr2)->content.boolean))
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
				printf("som v lese\n");

				temp = searchSymbol(&rootTS, ((char *)new->adr1));
				temp2 = searchSymbol(&rootTS, ((char *)new->adr2));

				if(temp->data->type == t_expr_int)
				{
					//if((((tData) new->adr1)->content.integer) < (((tData) new->adr2)->content.integer))
					if(temp->data->content.integer < temp2->data->content.integer)
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
				} else if(((tData) new->adr1)->type == t_expr_dou)
				{
					if((((tData) new->adr1)->content.real) < (((tData) new->adr2)->content.real))
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
				} else if(((tData) new->adr1)->type == t_expr_str)
				{
					cmp = strcmp((((tData) new->adr1)->content.string), (((tData) new->adr2)->content.string));
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
					if((((tData) new->adr1)->content.boolean) < (((tData) new->adr2)->content.boolean))
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

			case I_EMORE:
				if(((tData) new->adr1)->type == t_expr_int)
				{
					if((((tData) new->adr1)->content.integer) >= (((tData) new->adr2)->content.integer))
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
				} else if(((tData) new->adr1)->type == t_expr_dou)
				{
					if((((tData) new->adr1)->content.real) >= (((tData) new->adr2)->content.real))
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
				} else if(((tData) new->adr1)->type == t_expr_str)
				{
					cmp = strcmp((((tData) new->adr1)->content.string), (((tData) new->adr2)->content.string));
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
					if((((tData) new->adr1)->content.boolean) >= (((tData) new->adr2)->content.boolean))
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
				if(((tData) new->adr1)->type == t_expr_int)
				{
					if((((tData) new->adr1)->content.integer) <= (((tData) new->adr2)->content.integer))
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
				} else if(((tData) new->adr1)->type == t_expr_dou)
				{
					if((((tData) new->adr1)->content.real) <= (((tData) new->adr2)->content.real))
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
				} else if(((tData) new->adr1)->type == t_expr_str)
				{
					cmp = strcmp((((tData) new->adr1)->content.string), (((tData) new->adr2)->content.string));
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
					if((((tData) new->adr1)->content.boolean) <= (((tData) new->adr2)->content.boolean))
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
				if(((tData) new->adr1)->type == t_expr_int)
				{
					if((((tData) new->adr1)->content.integer) == (((tData) new->adr2)->content.integer))
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
				} else if(((tData) new->adr1)->type == t_expr_dou)
				{
					if((((tData) new->adr1)->content.real) == (((tData) new->adr2)->content.real))
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
				} else if(((tData) new->adr1)->type == t_expr_str)
				{
					cmp = strcmp((((tData) new->adr1)->content.string), (((tData) new->adr2)->content.string));
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
					if((((tData) new->adr1)->content.boolean) == (((tData) new->adr2)->content.boolean))
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
				if(((tData) new->adr1)->type == t_expr_int)
				{
					if((((tData) new->adr1)->content.integer) != (((tData) new->adr2)->content.integer))
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
				} else if(((tData) new->adr1)->type == t_expr_dou)
				{
					if((((tData) new->adr1)->content.real) != (((tData) new->adr2)->content.real))
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
				} else if(((tData) new->adr1)->type == t_expr_str)
				{
					cmp = strcmp((((tData) new->adr1)->content.string), (((tData) new->adr2)->content.string));
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
					if((((tData) new->adr1)->content.boolean) != (((tData) new->adr2)->content.boolean))
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
				scanf("%i", &(((tData) new->result)->content.integer));
				printf("rgrbvr%i\n", (((tData) new->result)->content.integer));		
				break;

			case I_READR:
				scanf("%lg", &(((tData) new->result)->content.real));
				printf("4t4t4g%lg\n", (((tData) new->result)->content.real));
				break;	

			case I_READS:
				scanf("%s", (((tData) new->result)->content.string));
				printf("t4t4%s\n", (((tData) new->result)->content.string));
				break;

			case I_WRITEI:
				printf("gfgfgf%i\n", (((tData) new->result)->content.integer));
				break;

			case I_WRITER:
				printf("sdsdsd%g\n", (((tData) new->result)->content.real));
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
			case 32:break;
			case I_WHILE:
				while(lastbool == true)
				{
					interpret(&rootTS, ((tInsList *) new->adr1));	
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