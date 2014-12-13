//#include "inslist.c"

int kanter=0;
bool lastbool;
bool vypocet = false;
int lastint = 0;
double lastdouble = 0;
char *laststring = NULL;
char *tmpstring = NULL;
void * lastAdr = NULL;
void * lastAdr1 = NULL;



char *concate(char *s1, char *s2)
{
	int len1 = (int)strlen(s1);
	int len2 = (int)strlen(s2);

	char * result = malloc(sizeof(char)*(len1+len2));
	if(result == NULL)	errorHandler(errInt);
	
	InsertLastMarius(&Smetisko, result);
	printf("concate dal mariosi %u\n",result);
	memset (result, 0, len1+len2);
	strcpy (result, s1);
	strcat (result, s2);
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


	if(debug == true)
	{
		while(currIL->active != NULL) {

		printf("INSTUKCIE LISTU %d: %d\n", currIL, currIL->active->instruction.instype);
		Succ(currIL);
		}
	}

	First(currIL);

	//if (currIL->active->instruction.instype!=0)
	//	printf("NULLKURAV\n");else
	
	do
	{

		new = Copy(currIL);
		//printf("%sAKTUALNA INSTRUKCIA LISTU %d: %d%s\n",KYEL,currIL,new->instype,KNRM);
		
		switch(new->instype)
		{
            case I_NOP: break;
							//ARITMETICKE OPERACIE//
			case I_ADDI:
				
				if(((tNodePtr) new->adr2) == NULL)
				{
					temp = ((tNodePtr) new->adr1);
					//printf("TEMP: %d\n", temp->data->content.integer);


					if(debug == true)printf("\n\n Horne %d + %d | LI:%d A1:%d A2:%d\n\n", lastint,temp->data->content.integer,lastint,temp->data->content.integer,((tNodePtr)new->adr2)->data->content.integer);


					lastint += temp->data->content.integer;
					if(debug == true)printf("lastint: \n", lastint);
				}
				else
				{
					if(vypocet == false)
					{
						temp = ((tNodePtr) new->adr1);//	printf("TEMP v else: %d\n", temp->data->content.integer);
						temp2 = ((tNodePtr)new->adr2);//	printf("TEMP2 v else: %d\n", temp2->data->content.integer);
						lastAdr = temp2;
						if(debug == true)printf("LASTADR je %p\n",lastAdr );

						if(debug == true)printf("\n\n Stredne %d + %d + %d | LI:%d A1:%d A2:%d\n\n", lastint, temp->data->content.integer,temp2->data->content.integer,lastint,temp->data->content.integer,temp2->data->content.integer);


						lastint += temp->data->content.integer + temp2->data->content.integer;	//printf("lastint  v else %d\n", lastint);
						vypocet = true;
					} else
					{
						//temp = ((tNodePtr) new->adr1);	printf("TEMP posledny: %d\n", temp->data->content.integer);
						temp2 = ((tNodePtr) new->adr2);	if(debug == true)printf("TEMP2 posledny: %d\n", temp2->data->content.integer);
						lastAdr = temp2;
						if(debug == true)printf("LASTADR je %p\n",lastAdr );

						if ( lastAdr==lastAdr1)
						{
							temp2 = ((tNodePtr) new->adr1);
							if(debug == true)printf("narval som adresu jedna do adresy 2\n");
						}
						if(debug == true)printf("\n\n Dolne %d + %d | LI:%d A1:%d A2:%d\n\n", lastint,temp2->data->content.integer,lastint,((tNodePtr)new->adr1)->data->content.integer,temp2->data->content.integer);

						
						lastint += temp2->data->content.integer;	if(debug == true)printf("LASTINT: %d\n", lastint);
					}		
				}
				
				if(debug == true)
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
				
				if(debug == true)
					printf("ADDR: %g\n", lastdouble);
				break;
			
			case I_CONCATE:		
				if(((tNodePtr) new->adr2) == NULL)
				{
					temp = ((tNodePtr) new->adr1);

					laststring = malloc(sizeof (char)* (int)strlen(temp->data->content.string));
					if(laststring == NULL)	errorHandler(errInt);
					InsertLastMarius(&Smetisko, laststring);

					//printf("Mariuskovi som dal %u\n",laststring);

					memset(laststring, 0, strlen(laststring));
					strcat(laststring, temp->data->content.string);
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
						laststring = concate(laststring, temp2->data->content.string);
					}
				}
				
				if(debug == true)
					printf("CONCATE: %s\n", laststring);
				break;

			case I_SUBI:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);
					lastAdr = temp2;
						if(debug == true)printf("LASTADR je %p\n",lastAdr );
					if(debug == true)printf("\n\n%d - %d | LI:%d A1:%d A2:%d\n\n", temp->data->content.integer,temp2->data->content.integer,lastint,temp->data->content.integer,temp2->data->content.integer);
					lastint = temp->data->content.integer - temp2->data->content.integer;
					vypocet = true;
				} else
				{
					temp2 = ((tNodePtr)new->adr2);
					lastAdr = temp2;
						if(debug == true)printf("LASTADR je %p\n",lastAdr );
					if(debug == true)printf("\n\n%d - %d | LI:%d A1:%d A2:%d\n\n", lastint,temp2->data->content.integer,lastint,((tNodePtr)new->adr1)->data->content.integer,temp2->data->content.integer);
					
	//megakontrolasveta9000
					if ( lastAdr==lastAdr1)
						{
							temp2 = ((tNodePtr) new->adr1);
							if(debug == true)printf("narval som adresu jedna do adresy 2\n");
						}

					lastint -= temp2->data->content.integer;
				}
				
				if(debug == true)
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
				
				if(debug == true)
					printf("SUBR: %g\n", lastdouble);
				break;

			case I_MULI:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);
					lastAdr1 = temp;
						printf("LASTADR1 je %p\n",lastAdr1 );
					printf("\n\n%d * %d | LI:%d A1:%d A2:%d\n\n", temp->data->content.integer,temp2->data->content.integer,lastint,temp->data->content.integer,temp2->data->content.integer);
					lastint = temp->data->content.integer * temp2->data->content.integer;
					vypocet = true;
				} 
				else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastAdr1 = temp;
						printf("LASTADR1 je %p\n",lastAdr1 );
					printf("\n\n%d * %d | LI:%d A1:%d A2:%d\n\n", lastint,temp2->data->content.integer,lastint,((tNodePtr)new->adr1)->data->content.integer,temp2->data->content.integer);
					lastint *= temp2->data->content.integer;
				}
				
				if(debug == true)
					printf("MULI: %d\n", lastint);
				break;		

			case I_MULR:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					lastdouble = temp->data->content.real * temp2->data->content.real;
					vypocet = true;
				} 
				else
				{
					temp2 = ((tNodePtr) new->adr2);
					lastdouble *= temp2->data->content.real;
				}
				
				if(debug == true)
					printf("MULR: %g\n", lastint);
				break;
			
			case I_DIVI:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);
					lastAdr1 = temp;
						if(debug == true)printf("LASTADR1 je %p\n",lastAdr1 );
					if(temp2->data->content.integer == 0)
					{
						errorHandler(errRunZdiv);
					} 
					else
					{
						if(debug == true)printf("\n\n%d / %d | LI:%d A1:%d A2:%d\n\n", temp->data->content.integer,temp2->data->content.integer,lastint,temp->data->content.integer,temp2->data->content.integer);
						lastint = temp->data->content.integer / temp2->data->content.integer;
						vypocet = true;
					}
				} 
				else
				{
					temp2 = ((tNodePtr) new->adr2);
					
					if(temp2->data->content.integer == 0)
					{
						errorHandler(errRunZdiv);
					} 
					else 
						{	if(debug == true)printf("\n\n%d / %d | LI:%d A1:%d A2:%d\n\n", lastint,temp2->data->content.integer,lastint,((tNodePtr) new->adr1)->data->content.integer,temp2->data->content.integer);
							lastint = lastint / temp2->data->content.integer;
						}
				}
				
				if(debug ==  true)
					printf("DIVI: %d\n", lastint);
				break;

			case I_DIVR:
				if(vypocet == false)
				{
					temp = ((tNodePtr) new->adr1);
					temp2 = ((tNodePtr) new->adr2);

					if(temp2->data->content.real == 0)
					{
						errorHandler(errRunZdiv);
					}
					else
					{
						lastdouble = temp->data->content.real / temp2->data->content.real;
						vypocet = true;
					}
				} 
				else
				{
					temp2 = ((tNodePtr) new->adr2);
					
					if(temp2->data->content.real == 0)
					{
						errorHandler(errRunZdiv);
					}
					else
					{
						lastdouble = lastdouble / temp2->data->content.real;
					}
				}
				
				if(debug == true)
					printf("DIVR: %g\n", lastdouble);
				break;		
			
			case I_ASGNI:						
				if(lastint < 0) errorHandler(errRunRest);	

				((tData) new->result)->content.integer = lastint;		
				lastint = 0;
				vypocet = false;

				if(debug == true)	
					printf("%svysledok ASGNI: %d%s\n", KGRN, (((tData) new->result)->content.integer),KNRM);
				break;		

			case I_ASGNR:
				if(lastdouble < 0) errorHandler(errRunRest);	

				((tData) new->result)->content.real = lastdouble;			
				lastdouble = 0;
				vypocet = false;
				
				if(debug == true)
					printf("vysledok ASGNR: %g\n", (((tData) new->result)->content.real));
				break;

			case I_ASGNS:			
				(((tData) new->result)->content.string) = malloc(sizeof(char) * strlen(laststring));
				if(((tData) new->result)->content.string == NULL)	errorHandler(errInt);
				InsertLastMarius(&Smetisko, (((tData) new->result)->content.string));
				
				strcpy((((tData) new->result)->content.string), laststring);
				
				laststring = NULL;
				vypocet =  false;
				
				if(debug == true)
					printf("vysledok ASGNS: %s\n", (((tData) new->result)->content.string));		
				break;

			case I_ASGNB:
				(((tData) new->result)->content.boolean) = lastbool;

				if(debug == true)
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
						
					} else 
					{
						
						lastbool = false;
						
					}
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
				{
					if(temp->data->content.real > temp2->data->content.real)
					{
						
						lastbool = true;
						
					} else 
					{
						
						lastbool = false;
						
					}
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp > 0)
					{
						
						lastbool = true;
						
				
					} else 
					{
						
						lastbool = false;
					
					}
				} else 
				{
					if(temp->data->content.boolean > temp2->data->content.boolean)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
						
					}
				}
				break;
			
			case I_LESS:
				
				temp = ((tNodePtr) new->adr1);			
				temp2 = ((tNodePtr) new->adr2);

				printf("PRVE: %d\n", temp->data->content.integer);
				printf("DRUHE: %d\n", temp2->data->content.integer);
				if(temp->data->type == t_expr_int || temp->data->type == sym_var_int)
				{
					
					if(temp->data->content.integer < temp2->data->content.integer)
					{
						
						lastbool = true;
						
				
					} else 
					{
						
						lastbool = false;
					
					}
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
				{
					if(temp->data->content.real < temp2->data->content.real)
					{
						
						lastbool = true;
						
				
					} else 
					{
						
						lastbool = false;
						
					}
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp < 0)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
						
					}
				} else 
				{
					if(temp->data->content.boolean < temp2->data->content.boolean)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
					
					}
				}
				break;

			case I_EMORE:
				
				temp = ((tNodePtr) new->adr1);
				temp2 = ((tNodePtr) new->adr2);

				if(temp->data->type == t_expr_int || temp->data->type == sym_var_int)
				{
					if(temp->data->content.integer >= temp2->data->content.integer)
					{
						
						lastbool = true;
			
				
					} else 
					{
						
						lastbool = false;
					
					}
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
				{
					if(temp->data->content.real >= temp2->data->content.real)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
					
					}
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp >= 0)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
				
					}
				} else 
				{
					if(temp->data->content.boolean >= temp2->data->content.boolean)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
					
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
				
				
					} else 
					{
						
						lastbool = false;
						
					}
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
				{
					if(temp->data->content.real <= temp2->data->content.real)
					{
						
						lastbool = true;
				
				
					} else 
					{
						
						lastbool = false;
					
					}
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp <= 0)
					{
						
						lastbool = true;
				
				
					} else 
					{
						
						lastbool = false;
						
					}
				} else 
				{
					if(temp->data->content.boolean <= temp2->data->content.boolean)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
					
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
						
				
					} else 
					{
						
						lastbool = false;
					
					}
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
				{
					if(temp->data->content.real == temp2->data->content.real)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
						
					}
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp == 0)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
				
					}
				} else 
				{
					if(temp->data->content.boolean == temp2->data->content.boolean)
					{
						
						lastbool = true;
				
				
					} else 
					{
						
						lastbool = false;
				
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
					
				
					} else 
					{
						
						lastbool = false;
				
				
					}
				} else if(temp->data->type == t_expr_dou || temp->data->type == sym_var_rea)
				{
					if(temp->data->content.real != temp2->data->content.real)
					{
						
						lastbool = true;
					
					} else 
					{
						
						lastbool = false;
						
					}
				} else if(temp->data->type == t_expr_str || temp->data->type == sym_var_str)
				{
					cmp = strcmp(temp->data->content.string, temp2->data->content.string);
					if(cmp != 0)
					{
						
						lastbool = true;
						
				
					} else 
					{
						
						lastbool = false;
						
					}
				} else 
				{
					if(temp->data->content.boolean != temp2->data->content.boolean)
					{
						
						lastbool = true;
					
				
					} else 
					{
						
						lastbool = false;
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

			case I_WRITE:
				
				for(int i = 0; i < *((int*) new->adr1); i++)
				{
					
					switch((*((tData**) new->adr2)[i])->type)
					{
						case sym_var_int:
						case t_expr_int:
							printf("%d", (*((tData**) new->adr2)[i])->content.integer);
							break;
					
						case sym_var_rea:
						case t_expr_dou:
							printf("%g", (*((tData**) new->adr2)[i])->content.real);
							break;
						
						case sym_var_str:
						case t_expr_str:
							printf("%s", (*((tData**) new->adr2)[i])->content.string);
							break;

						case sym_var_boo:
						case t_expr_boo:
							printf("%d", (*((tData**) new->adr2)[i])->content.boolean);		
						break;
					
						default: printf("KOOOORVA\n");
					}
				}
				break;

			case I_IF:
				printf("lastboo je %d\n",lastbool );
				
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
					//getchar();
				}
				break;

			case I_FCE:
					


				printf("FUNKCIA ZACINA DRZTE SI KLOBUKY\n");
				currentTerm = (((tData) new->adr1)->nextArg);
				printf("%s\n", currentTerm->data->name);
				//odpamatanie kokotka
				if(new->result!=NULL)
				conVarOld = *((tContent *) new->result);//else printf("je nal kokot\n");
				//printf("odpamatal som si kokotka %d\n",conVarOld.integer);

				for(int i = 0; i < ((tData) new->adr1)->argCount; i++)
				{
					//printf("1 FORIK zaciatok\n");
					conOld[i] = currentTerm->data->content;
					//printf("integer conoldu je %d\n",conOld[i].integer );
					printf("integer currentu je %d\n",(*((tContent **)new->adr2)[i]).integer );
                    //printf("idem tam byt\n");
                    currentTerm->data->content = (*((tContent**) new->adr2)[i]);
                    //printf("som tu\n");
					//printf("po ulozeni je v currentTerme %d\n",currentTerm->data->content.integer );
					currentTerm = currentTerm->data->nextArg;
					//printf("1 FORIK koniec\n");
				}

				tNodePtr hledam = searchSymbol  (&rootTS,"Flength");
				//printf("nasel \n");
				char *nazovfunkcie = ((tData) new->adr1)->name;
				//if (nazovfunkcie==NULL)printf("je nal kurva\n");
				//printf("funkcia je %s\n",nazovfunkcie);

				if(strcmp(nazovfunkcie, "length") == 0)
				{
					lastint = strlen(((tData) new->adr1)->nextArg->data->content.string);
					//printf("%d\n", lastint);
				}
				else
					if(strcmp(nazovfunkcie, "copy") == 0)
					{
						laststring = funCopy(((tData) new->adr1)->nextArg->data->content.string,
											 ((tData) new->adr1)->nextArg->data->nextArg->data->content.integer,
											 ((tData) new->adr1)->nextArg->data->nextArg->data->nextArg->data->content.integer);
					}
					else
						if(strcmp(nazovfunkcie, "find") == 0)
						{
							lastint = BMASearch(((tData) new->adr1)->nextArg->data->content.string,
											 ((tData) new->adr1)->nextArg->data->nextArg->data->content.string);
						}
						else
							if(strcmp(nazovfunkcie, "sort") == 0)
							{
								laststring = allocQuickSort(((tData) new->adr1)->nextArg->data->content.string,0,strlen(((tData) new->adr1)->nextArg->data->content.string)-1);
							}
							else
							{
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
               				}

                

                printf("%sFUNKCIA SKONCILA ODLOZTE SI KLOBUCIKY%s\n",KRED,KNRM);	
			break;
			
			default: printf("kokot dostal som %d\n",new->instype);
			break;
		}	
		Succ(currIL);


	} while(currIL->active != NULL);
 return 0; 
}
