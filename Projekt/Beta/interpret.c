//#include "inslist.c"

int kanter=0;
bool lastbool;
bool vypocet = false;
int lastint = 0;
double lastdouble = 0;
char *laststring = NULL;
char *tmpstring = NULL;


char *concate(char *s1, char *s2)
{
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);

	char *result = malloc(len1+len2 + 1);
	InsertLastMarius(&Smetisko, result);
	
	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);
	return result;
}

int interpret(tNodePtr *TS, tInsList *currIL)	//precitaj si zadanie real %g, atd
{
	printf("zacal interpret a som v insliste %u\n",currIL);

	tContent conOld[100];
	tContent conVarOld;
	tNodePtr currentTerm; 
	int l;
	int r;
	int cmp;
	First(currIL);
	tInstruction *new;
	tNodePtr temp;
	tNodePtr temp2;



	while(currIL->active != NULL) {

		printf("INSTUKCIE LISTU %d: %d\n", currIL, currIL->active->instruction.instype);
		Succ(currIL);

	}

	First(currIL);

	//if (currIL->active->instruction.instype!=0)
	//	printf("NULLKURAV\n");else
	
	do
	{

		new = Copy(currIL);
		printf("%sAKTUALNA INSTRUKCIA LISTU %d: %d%s\n",KYEL,currIL,new->instype,KNRM);
		switch(new->instype)
		{
            case I_NOP: break;
							//ARITMETICKE OPERACIE//
			case I_ADDI:
				if(((tNodePtr) new->adr2) == NULL)
				{
					temp = ((tNodePtr) new->adr1);
					lastint += temp->data->content.integer;
				}
				else
				{
					if(vypocet == false)
					{
						temp = ((tNodePtr) new->adr1);
						temp2 = ((tNodePtr)new->adr2);
						//printf("%d\n", temp->data->content.integer);

						lastint += temp->data->content.integer + temp2->data->content.integer;
						vypocet = true;
					} else
					{
						temp2 = ((tNodePtr) new->adr2);
						lastint += temp2->data->content.integer;
					}		
				}
				printf("ADDI: %d\n", lastint);
				break;
			
			case I_ADDR:
				if(((tNodePtr) new->adr2) == NULL)
				{
					temp = ((tNodePtr) new->adr1);
					lastdouble += temp->data->content.real;
				}
				else
				{
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
				}
				printf("ADDR: %g\n", lastdouble);
				break;
			
			case I_CONCATE:		
				if(((tNodePtr) new->adr2) == NULL)
				{
					temp = ((tNodePtr) new->adr1);

					laststring = malloc(sizeof strlen(temp->data->content.string));
					InsertLastMarius(&Smetisko, laststring);
					strcpy(laststring, temp->data->content.string);
				}
				else
				{
					if(vypocet == false)
					{
						temp = ((tNodePtr) new->adr1);
						temp2 = ((tNodePtr) new->adr2);
			
						laststring =  concate(temp->data->content.string, temp2->data->content.string);			
						vypocet = true;
					} else
					{
						temp2 = ((tNodePtr) new->adr2);
						
						InsertLastMarius(&Smetisko, tmpstring);
						laststring = concate(laststring, temp2->data->content.string);
					}
				}
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
				printf("%d\n", lastint);
				printf("%svysledok ASGNI: %d%s\n", KGRN, (((tData) new->result)->content.integer),KNRM);	
				lastint = 0;
				vypocet = false;
				printf("vynuloval som lastint\n");
				break;		

			case I_ASGNR:
				((tData) new->result)->content.real = lastdouble;
				printf("vysledok ASGNR: %g\n", (((tData) new->result)->content.real));
				lastdouble = 0;
				vypocet = false;
				break;

			case I_ASGNS:
				(((tData) new->result)->content.string) = malloc(sizeof(char) * strlen(laststring));
				InsertLastMarius(&Smetisko, (((tData) new->result)->content.string));
				strcpy((((tData) new->result)->content.string), laststring);
				laststring = NULL;
				
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

				if(temp->data->type == t_expr_int || temp->data->type == sym_var_int)
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
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
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
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
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
				//printf("prva kokotina ok %d\n", temp->data->content.integer);
				temp2 = ((tNodePtr) new->adr2);
				//printf("druha kokotina ok %d\n", temp2->data->content.integer);
				//printf("dataypz je %d\n",temp->data->type );
				if(temp->data->type == t_expr_int || temp->data->type == sym_var_int)
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
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
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
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
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
				}printf("koniec lesa a lastgul je %d\n",lastbool);
				break;

			case I_EMORE:
				
				temp = ((tNodePtr) new->adr1);
				temp2 = ((tNodePtr) new->adr2);

				if(temp->data->type == t_expr_int || temp->data->type == sym_var_int)
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
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
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
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
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
				
				if(temp->data->type == t_expr_int || temp->data->type == sym_var_int)
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
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
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
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
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

				if(temp->data->type == t_expr_int || temp->data->type == sym_var_int)
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
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
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
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
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

				if(temp->data->type == t_expr_int || temp->data->type == sym_var_int)
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
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
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
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
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
					tListItem * revert = currIL->active;
					//printf("pamatam si %d\n",((tInsList *) new->adr1)->active->instruction.instype);
					interpret(&rootTS, ((tInsList *) new->adr1));

					currIL->active = revert;
					//((tInsList *) new->adr1)->active=revert->next;
					printf("then vetva skoncila\n");	
								
					break;
				} else 
				{
					printf("volam picu z else\n");	//lastbool = 0;
					tListItem * revert = currIL->active;
					//tListItem * revert = ((tInsList *) new->adr1)->active;
					interpret(&rootTS, ((tInsList *) new->adr2));
					//((tInsList *) new->adr1)->active=revert->next;
					currIL->active = revert;
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
				
				printf("FUNKCIA ZACINA DRZTE SI KLOBUKY\n");
				currentTerm = (((tData) new->adr1)->nextArg);
				
				//odpamatanie kokotka
				conVarOld = *((tContent *) new->result);
				printf("odpamatal som si kokotka %d\n",conVarOld.integer);

				for(int i = 0; i < ((tData) new->adr1)->argCount; i++)
				{
					//printf("1 FORIK zaciatok\n");
					conOld[i] = currentTerm->data->content;
					//printf("integer conoldu je %d\n",conOld[i].integer );
					printf("integer currentu je %d\n",(*((tContent **)new->adr2)[i]).integer );
                    currentTerm->data->content = (*((tContent**) new->adr2)[i]);
					//printf("po ulozeni je v currentTerme %d\n",currentTerm->data->content.integer );
					currentTerm = currentTerm->data->nextArg;
					//printf("1 FORIK koniec\n");
				}


				tListItem * revert = currIL->active;
				
				printf("%srevert si pamata %u v %u %s\n",KCYN,revert->instruction.instype, revert,KNRM );
				kanter++;
				printf("__________________________________LEVEL_VNORENI_%d\n",kanter);
				
				interpret(/*&(((tData) new->adr1)->localTSadr)*/NULL, (((tData) new->adr1)->localILadr));
				
				printf("__________________________________END_%d\n",kanter);kanter--;
				
				
				currIL->active = revert;


				
                
            	
                switch((((tData) new->adr1)->type))
				{
					case sym_fun_int:
					case sym_fok_int:	
						lastint = (*((tContent *) new->result)).integer;
						break;

					case sym_fun_rea:
					case sym_fok_rea:
						lastdouble = (*((tContent *) new->result)).real;
						break;	

					case sym_fun_str:
					case sym_fok_str:
						laststring = (*((tContent *) new->result)).string;
						break;

					case sym_fun_boo:
					case sym_fok_boo:
						lastbool =(*((tContent *) new->result)).boolean;
						break;
					default: printf("KORVOOOOOOOOOOOOOOOOOOOOO\n");exit(563415616);
				}
				//printf("vratil som %d\n", (*((tContent *) new->result)).integer);
				//printf("lastint je %d\n",lastint );

                *((tContent *) new->result) = conVarOld;
                //printf("obnovil som si kokotka %d\n",(*((tContent *) new->result)).integer);

                currentTerm = (((tData) new->adr1)->nextArg);

				for(int i = 0; i < ((tData) new->adr1)->argCount; i++)
				{
					currentTerm->data->content = conOld[i];
					(((tContent**) new->adr2)[i])=NULL;
					currentTerm = currentTerm->data->nextArg;
				}

                printf("%sFUNKCIA SKONCILA ODLOZTE SI KLOBUCIKY%s\n",KRED,KNRM);	
			break;
			
			default: printf("kokot dostal som %d\n",new->instype);
			break;
		}	
		Succ(currIL);
		/*if (currIL->active != NULL)
		printf("Aktiv instu listu::: %d\n", currIL->active->instruction.instype);
*/
	} while(currIL->active != NULL);
 return 0; 
}
