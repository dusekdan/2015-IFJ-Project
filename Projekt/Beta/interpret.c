//#include "inslist.c"


tInsList LL;
tInsList LR;
bool lastbool;
int kokot;


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
	tContent conOld[100];
	tNodePtr currentTerm; 
	int l;
	int r;
	int cmp;
	First(IL);
	tInstruction *new;

	while(IL->active != NULL)
	{

		new = Copy(IL);
		
		switch(new->instype)
		{
							/*ARITMETICKE OPERACIE*/
			case I_ADDI:
				kokot =  (((tData) new->adr1)->content.integer + ((tData)new->adr2)->content.integer);
				//(((tData) new->result)->content.integer) = (((tData) new->adr1)->content.integer + ((tData)new->adr2)->content.integer);
				printf("%d\n", kokot);
				//printf("%d\n", ((tData)new->result)->content.integer);
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

			case I_ASGNB:
				(((tData) new->result)->content.boolean) = (((tData) new->adr1)->content.boolean);
							/*LOGICKE OPERACIE*/
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
				if(((tData) new->adr1)->type == t_expr_int)
				{
					if((((tData) new->adr1)->content.integer) < (((tData) new->adr2)->content.integer))
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
			
			case I_IF:
				if(lastbool == true)	
				{
					
					interpret(&rootTS, ((tInsList *) new->adr1));	
					lastbool = 0;				
					break;
				} else 
				{
					
					interpret(&rootTS, ((tInsList *) new->adr2));
					lastbool = 0;
					break;
				}
			
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
		}	
		Succ(IL);

	} //while(IL->active != NULL);
 return 0; 
}

/*
int main()
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