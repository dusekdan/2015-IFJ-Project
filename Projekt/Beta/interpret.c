//#include "inslist.c"

int kanter=0;
bool lastbool;
bool vypocet = false;
int readyInt = 0;
double readyDou = 0;
int resArrIntIndex = -1;
int resArrDouIndex = -1;
double lastdouble = 0;
char *laststring = NULL;
char *tmpstring = NULL;
void * lastAdr = NULL;
void * lastAdr1 = NULL;
bool recycleAdr = false;
int * currentInstType;
bool realOnly = false;



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

void * preklopenie (tInsList *currIL)
{
	
	struct ListItem *origActiv = currIL -> active;
	First(currIL);
	while (currIL -> active != NULL)
			{
				currentInstType = & (currIL -> active -> instruction . instype);

				if ( * currentInstType >= I_ADDI   &&
					 * currentInstType <= I_NEQUAL &&
					 * currentInstType != I_ASGNI  &&
					 * currentInstType != I_ASGNS  &&
					 * currentInstType != I_ASGNR  &&
					 * currentInstType != I_ASGNB   )
				{
					if (currIL -> active -> instruction . adr1 != NULL)
						((tNodePtr)currIL -> active -> instruction . adr1)->data->used = false;
					if (currIL -> active -> instruction . adr2 != NULL)
						((tNodePtr)currIL -> active -> instruction . adr2)->data->used = false;
				
				}

			Succ(currIL);
			}
	currIL->active=origActiv;
}

//Funkcia na prehodenie hodnot z integroveho pola do pola double
void intArr2douArr (int * intArr, double * douArr, int count)
{
	for (int i = 0; i < count; ++i)
		(douArr [i]) = ((double) (intArr [i]));
	resArrDouIndex = resArrIntIndex;
	readyDou = (double) readyInt;

	/*for (int j = 0; j < count; ++j)
		printf("GGGGG %g\n",douArr[j] );*/
}

int interpret(tNodePtr *TS, tInsList *currIL)	//precitaj si zadanie real %g, atd
{
	if(debug == true) printf("zacal interpret a som v insliste %u\n",currIL);

	tContent conOld[100];
	tContent conVarOld;
	tNodePtr currentTerm; 
	int l;
	int r;
	int cmp;
	
	tInstruction *new;
	tNodePtr temp;
	tNodePtr temp2;

	tNodePtr A1 = NULL;
	tNodePtr A2 = NULL;

	int longestExpressionLength = 0; //dlzka najdlhsieho vyrazu
	int currentExpressionLength = 0; //dlzka aktualneho vyrazu
	First(currIL);
	//if(debug == true)
	//{
		//Prechadzanie vsetkych instrukcii z listu pre anál
		while (currIL -> active != NULL)
		{
			currentInstType = & (currIL -> active -> instruction . instype);
			
			// Nájdenie najdlhšieho výrazu (počet operátorov)

			if ( * currentInstType >= I_ADDI   &&
				 * currentInstType <= I_NEQUAL &&
				 * currentInstType != I_ASGNI  &&
				 * currentInstType != I_ASGNS  &&
				 * currentInstType != I_ASGNR  &&
				 * currentInstType != I_ASGNB   )
				
				{
					currentExpressionLength++;
					if (recycleAdr == true)
					{
						if (currIL -> active -> instruction . adr1 != NULL)
							//printf("ADR1 je %p\n", (tNodePtr)new->adr1);
						{
							printf("idem dat false na %s\n",((tNodePtr)currIL -> active -> instruction . adr1)->key);
							printf("jeho iteger je %d\n", ((tNodePtr)currIL -> active -> instruction . adr1)->data->content.integer );
							((tNodePtr)currIL -> active -> instruction . adr1)->data->used = false;
						}
						if (currIL -> active -> instruction . adr2 != NULL)
						{
							printf("idem dat false na %s\n",((tNodePtr)currIL -> active -> instruction . adr2)->key);
							printf("jeho iteger je %d\n", ((tNodePtr)currIL -> active -> instruction . adr2)->data->content.integer );
							((tNodePtr)currIL -> active -> instruction . adr2)->data->used = false;
						}
					}
				}
			else
			{
				//if (debug == true) printf ("currentExpressionLength: %d\n", currentExpressionLength);
				if ( currentExpressionLength > longestExpressionLength)
					longestExpressionLength = currentExpressionLength;
				currentExpressionLength = 0;
			}

			// Koniec hladania najdlhšieho výrazu (počet operátorov)

		/*	new = Copy(currIL);
		
		if (currIL->active->instruction.instype !=32)
		{	
		*/printf("INSTUKCIE LISTU %d: %d \n", currIL, currIL->active->instruction.instype);/*
		if (((tNodePtr)new->adr1) != NULL)
		printf("A1 = %p:  %d | ", (void*)((tNodePtr)new->adr1), ((tNodePtr)new->adr1)->data->content.integer);
		if (((tNodePtr)new->adr2) != NULL)
		printf("A2 = %p:  %d", (void*)((tNodePtr)new->adr2),  ((tNodePtr)new->adr2)->data->content.integer);
		printf("\n");
		//if (((tNodePtr)new->result) != NULL)
		//printf("RE:  %d | \n",  ((tNodePtr)new->result)->data->content.integer);*/
		
		//}
		Succ(currIL);
		}
		if (recycleAdr==true) recycleAdr=false;
		printf("longestExpressionLength: %d\n",longestExpressionLength);
	//}
	new = NULL;



	int resArrInt [longestExpressionLength];
	double resArrDou [longestExpressionLength];

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
				A1 = (tNodePtr) new -> adr1;
				A2 = (tNodePtr) new -> adr2;

				// Prisla nam len jedna adresa => priradenie

				if (A2 == NULL)
				{
					if (debug == true) printf("len priradenie\n");

					if (A1 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

						readyInt = A1 -> data -> content . integer;

					if (debug == true) printf("ADDI: d:%d g:\n", readyInt, readyDou);
					break;
				}

				// Prisli nam dve nove adresy => zaciatok noveho medzivypoctu

				if (A1 -> data -> used == false && A2 -> data -> used == false)
				{
					if (debug == true) printf("dve nove\n");

					if (A1 -> data -> content . initialized == false ||
						A2 -> data -> content . initialized == false  ) 
						errorHandler (errRunUnin);
					if (realOnly == false)
					{
						resArrIntIndex++;
						resArrInt [resArrIntIndex] = A1 -> data -> content . integer + A2 -> data -> content . integer;
						A1 -> data -> used = true;
						A2 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDouIndex++;
						printf("%g + %g \n",(double) (A1 -> data -> content . integer),(double) (A2 -> data -> content . integer));
						resArrDou [resArrDouIndex] = (double) (A1 -> data -> content . integer) + (double) (A2 -> data -> content . integer);
						A1 -> data -> used = true;
						A2 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}
					
					if (debug == true) printf("ADDI: d:%d g:%g\n", readyInt, readyDou);
					break;
				}

				// Prva adresa nebola pouzita ale druha ano => pouzijem prvu

				if (A1 -> data -> used == false && A2 -> data -> used == true)
				{	
					if (debug == true) printf("prva\n");

					if (A1 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					if (realOnly == false)
					{ 
						resArrInt [resArrIntIndex] += A1 -> data -> content . integer;
						A1 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDou [resArrDouIndex] += (double) (A1 -> data -> content . integer);
						A1 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}

					if (debug == true) printf("ADDI: d:%d g:%g\n", readyInt, readyDou);
					break;
				}

				// Prva adresa uz bola druha nie  => pouzijem druhu

				if (A1 -> data -> used == true && A2 -> data -> used == false)
				{
					if (debug == true) printf("druha\n");

					if (A2 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);
					if (realOnly == false)
					{
						resArrInt [resArrIntIndex] += A2 -> data -> content . integer;
						A2 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDou [resArrDouIndex] += (double) (A2 -> data -> content . integer);
						A2 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}
					

					if (debug == true) printf("ADDI: d:%d g:%g\n", readyInt, readyDou);
					break;
				}

				// Obe adresy boli pouzite => zratam medzivysledky

				if (A1 -> data -> used == true && A2 -> data -> used == true)
				{
					if (debug == true) printf("aniani\n");

					if (realOnly == false)
					{
						if (resArrIntIndex-1 != -1)
						{
							resArrInt [resArrIntIndex-1] += resArrInt [resArrIntIndex];
							resArrIntIndex--;
							readyInt = resArrInt [resArrIntIndex];
						}
						else
							readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						if (resArrDouIndex-1 != -1)
						{
							resArrDou [resArrDouIndex-1] += resArrDou [resArrDouIndex];
							resArrDouIndex--;
							readyDou = resArrDou [resArrDouIndex];
						}
						else
							readyDou = resArrDou [resArrDouIndex];
					}

					if (debug == true) printf("ADDI: d:%d g:%g\n", readyInt, readyDou);
					break;
				}

				errorHandler (errRunRest);

				break;
			
			case I_ADDR:
			if (realOnly==false)
			{
				realOnly = true;
				intArr2douArr(&(resArrInt[0]),&(resArrDou[0]),longestExpressionLength);
			}
				
				A1 = (tNodePtr) new -> adr1;
				A2 = (tNodePtr) new -> adr2;

				if (A1 -> data -> type == t_expr_int || A1 -> data -> type == sym_var_int )
					A1 -> data -> content . real = (double) A1 -> data -> content . integer;
				if (A2 -> data -> type == t_expr_int || A2 -> data -> type == sym_var_int )
					A2 -> data -> content . real = (double) A2 -> data -> content . integer;

				// Prisla nam len jedna adresa => priradenie

				if (A2 == NULL)
				{
					if (debug == true) printf("len priradenie\n");

					if (A1 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					readyDou = A1 -> data -> content . real;

					if(debug == true) printf("ADDR: %g\n", readyDou);
					break;
				}

				// Prisli nam dve nove adresy => zaciatok noveho medzivypoctu

				if (A1 -> data -> used == false && A2 -> data -> used == false)
				{
					if (debug == true) printf("dve nove\n");

					if (A1 -> data -> content . initialized == false ||
						A2 -> data -> content . initialized == false  ) 
						errorHandler (errRunUnin);

					resArrDouIndex++;
					resArrDou [resArrDouIndex] = A1 -> data -> content . real + A2 -> data -> content . real;
					A1 -> data -> used = true;
					A2 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("ADDR: %g\n", readyDou);
					break;
				}

				// Prva adresa nebola pouzita ale druha ano => pouzijem prvu

				if (A1 -> data -> used == false && A2 -> data -> used == true)
				{	
					if (debug == true) printf("prva\n");

					if (A1 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					resArrDou [resArrDouIndex] += A1 -> data -> content . real;
					A1 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("ADDR: %g\n", readyDou);
					break;
				}

				// Prva adresa uz bola druha nie  => pouzijem druhu

				if (A1 -> data -> used == true && A2 -> data -> used == false)
				{
					if (debug == true) printf("druha\n");

					if (A2 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					resArrDou [resArrDouIndex] += A2 -> data -> content . real;
					A2 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("ADDR %g\n", readyDou);
					break;
				}

				// Obe adresy boli pouzite => zratam medzivysledky

				if (A1 -> data -> used == true && A2 -> data -> used == true)
				{
					if (debug == true) printf("aniani\n");

					if (resArrDouIndex-1 != -1)
					{
						resArrDou [resArrDouIndex-1] += resArrDou [resArrDouIndex];
						resArrDouIndex--;
						readyDou = resArrDou [resArrDouIndex];
					}
					else
						readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("ADDR: %g\n", readyDou);
					break;
				}

				errorHandler (errRunRest);

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
				A1 = (tNodePtr) new -> adr1;
				A2 = (tNodePtr) new -> adr2;

				// Prisli nam dve nove adresy => zaciatok noveho medzivypoctu

				if (A1 -> data -> used == false && A2 -> data -> used == false)
				{
					if (A1 -> data -> content . initialized == false ||
						A2 -> data -> content . initialized == false  )
					{
						fprintf(stderr, "Variable '%s' uninitialized.\n", A1 -> data -> name);
						errorHandler (errRunUnin);
					}
						

					if (debug == true) printf("dve nove\n");

					if (realOnly == false)
					{
						resArrIntIndex++;
						resArrInt [resArrIntIndex] = A1 -> data -> content . integer - A2 -> data -> content . integer;
						A1 -> data -> used = true;
						A2 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDouIndex++;
						resArrDou [resArrDouIndex] = (double) (A1 -> data -> content . integer) - (double) (A2 -> data -> content . integer);
						A1 -> data -> used = true;
						A2 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}
					if(debug == true) printf("SUBI: d%d g%g\n", readyInt,readyDou);
					break;
				}

				// Prva adresa nebola pouzita ale druha ano => pouzijem prvu

				if (A1 -> data -> used == false && A2 -> data -> used == true)
				{	
					if (A1 -> data -> content . initialized == false) 
					{
						fprintf(stderr, "Variable '%s' uninitialized.\n", A1 -> data -> name);
						errorHandler (errRunUnin);
					}

					if (debug == true) printf("prva lebo druha menom %s mala used\n",A2 -> key);

					if (realOnly == false)
					{
						resArrInt [resArrIntIndex] -= A1 -> data -> content . integer;
						A1 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDou [resArrDouIndex] -= (double) (A1 -> data -> content . integer);
						A1 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}

					if(debug == true) printf("SUBI: d%d g%g\n", readyInt,readyDou);
					break;
				}

				// Prva adresa uz bola druha nie  => pouzijem druhu

				if (A1 -> data -> used == true && A2 -> data -> used == false)
				{
					if (A2 -> data -> content . initialized == false) 
					{
						fprintf(stderr, "Variable '%s' uninitialized.\n", A1 -> data -> name);
						errorHandler (errRunUnin);
					}

					if (debug == true) printf("druha\n");

					if (realOnly == false)
					{
						resArrInt [resArrIntIndex] -= A2 -> data -> content . integer;
						A2 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDou [resArrDouIndex] -= (double) (A2 -> data -> content . integer);
						A2 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}
					if(debug == true) printf("SUBI: d%d g%g\n", readyInt, readyDou);
					break;
				}

				// Obe adresy boli pouzite => zratam medzivysledky

				if (A1 -> data -> used == true && A2 -> data -> used == true)
				{
					if (debug == true) printf("aniani\n");

					if (realOnly == false)
					{
						if (resArrIntIndex-1 != -1)
						{
							resArrInt [resArrIntIndex-1] -= resArrInt [resArrIntIndex];
							resArrIntIndex--;
							readyInt = resArrInt [resArrIntIndex];
						}
						else
							readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						if (resArrDouIndex-1 != -1)
						{
							resArrDou [resArrDouIndex-1] -= resArrDou [resArrDouIndex];
							resArrDouIndex--;
							readyDou = resArrDou [resArrDouIndex];
						}
						else
							readyDou = resArrDou [resArrDouIndex];
					}

					if(debug == true) printf("SUBI: d%d g%g\n", readyInt, readyDou);
					break;
				}

				errorHandler (errRunRest);

				break;

			case I_SUBR:
				if (realOnly==false)
				{
					realOnly = true;
					intArr2douArr(&(resArrInt[0]),&(resArrDou[0]),longestExpressionLength);
				}

				if (A1 -> data -> type == t_expr_int || A1 -> data -> type == sym_var_int )
					A1 -> data -> content . real = (double) A1 -> data -> content . integer;
				if (A2 -> data -> type == t_expr_int || A2 -> data -> type == sym_var_int )
					A2 -> data -> content . real = (double) A2 -> data -> content . integer;

				A1 = (tNodePtr) new -> adr1;
				A2 = (tNodePtr) new -> adr2;

				// Prisli nam dve nove adresy => zaciatok noveho medzivypoctu

				if (A1 -> data -> used == false && A2 -> data -> used == false)
				{
					if (debug == true) printf("dve nove\n");

					if (A1 -> data -> content . initialized == false ||
						A2 -> data -> content . initialized == false  ) 
						errorHandler (errRunUnin);

					resArrDouIndex++;
					resArrDou [resArrDouIndex] = A1 -> data -> content . real - A2 -> data -> content . real;
					A1 -> data -> used = true;
					A2 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("SUBR: %g\n", readyDou);
					break;
				}

				// Prva adresa nebola pouzita ale druha ano => pouzijem prvu

				if (A1 -> data -> used == false && A2 -> data -> used == true)
				{	
					if (debug == true) printf("prva\n");

					if (A1 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					resArrDou [resArrDouIndex] -= A1 -> data -> content . real;
					A1 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("SUBR: %g\n", readyDou);
					break;
				}

				// Prva adresa uz bola druha nie  => pouzijem druhu

				if (A1 -> data -> used == true && A2 -> data -> used == false)
				{
					if (debug == true) printf("druha\n");

					if (A2 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					resArrDou [resArrDouIndex] -= A2 -> data -> content . real;
					A2 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("SUBR %g\n", readyDou);
					break;
				}

				// Obe adresy boli pouzite => zratam medzivysledky

				if (A1 -> data -> used == true && A2 -> data -> used == true)
				{
					if (debug == true) printf("aniani\n");
					printf("resArrDouIndex je %d\n",resArrDouIndex);
					if (resArrDouIndex-1 != -1)
					{
						resArrDou [resArrDouIndex-1] -= resArrDou [resArrDouIndex];
						resArrDouIndex--;
						readyDou = resArrDou [resArrDouIndex];
					}
					else
						readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("SUBR: %g\n", readyDou);
					break;
				}

				errorHandler (errRunRest);

				break;

			case I_MULI:
				A1 = (tNodePtr) new -> adr1;
				A2 = (tNodePtr) new -> adr2;

				// Prisli nam dve nove adresy => zaciatok noveho medzivypoctu

				if (A1 -> data -> used == false && A2 -> data -> used == false)
				{
					if (debug == true) printf("dve nove\n");

					if (A1 -> data -> content . initialized == false ||
						A2 -> data -> content . initialized == false  ) 
						errorHandler (errRunUnin);
					if (realOnly == false)
					{
						resArrIntIndex++;
						resArrInt [resArrIntIndex] = A1 -> data -> content . integer * A2 -> data -> content . integer;
						A1 -> data -> used = true;
						A2 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDouIndex++;
						resArrDou [resArrDouIndex] = (double) (A1 -> data -> content . integer) * (double) (A2 -> data -> content . integer);
						A1 -> data -> used = true;
						A2 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}
					if(debug == true) printf("MULI: d%d g%g\n", readyInt, readyDou);
					break;
				}

				// Prva adresa nebola pouzita ale druha ano => pouzijem prvu

				if (A1 -> data -> used == false && A2 -> data -> used == true)
				{	
					if (debug == true) printf("prva\n");

					if (A1 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					if ( realOnly == false)
					{
						resArrInt [resArrIntIndex] *= A1 -> data -> content . integer;
						A1 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDou [resArrDouIndex] *= (double) (A1 -> data -> content . integer);
						A1 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}

					if(debug == true) printf("MULI: d%d g%g\n", readyInt, readyDou);
					break;
				}

				// Prva adresa uz bola druha nie  => pouzijem druhu

				if (A1 -> data -> used == true && A2 -> data -> used == false)
				{
					if (debug == true) printf("druha\n");

					if (A2 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);
					if (realOnly == false)
					{
						resArrInt [resArrIntIndex] *= A2 -> data -> content . integer;
						A2 -> data -> used = true;
						readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						resArrDou [resArrDouIndex] *= (double) (A2 -> data -> content . integer);
						A2 -> data -> used = true;
						readyDou = resArrDou [resArrDouIndex];
					}

					if(debug == true) printf("MULI: d%d g%g\n", readyInt, readyDou);
					break;
				}

				// Obe adresy boli pouzite => zratam medzivysledky

				if (A1 -> data -> used == true && A2 -> data -> used == true)
				{
					if (debug == true) printf("aniani\n");

					if (realOnly == false)
					{
						if (resArrIntIndex-1 != -1)
						{
							resArrInt [resArrIntIndex-1] *= resArrInt [resArrIntIndex];
							resArrIntIndex--;
							readyInt = resArrInt [resArrIntIndex];
						}
						else
							readyInt = resArrInt [resArrIntIndex];
					}
					else
					{
						if (resArrDouIndex-1 != -1)
						{
							resArrDou [resArrDouIndex-1] *= resArrDou [resArrDouIndex];
							resArrDouIndex--;
							readyDou = resArrDou [resArrDouIndex];
						}
						else
							readyDou = resArrDou [resArrDouIndex];
					}

					if(debug == true) printf("MULI: d%d g%g\n", readyInt, readyDou);
					break;
				}

				errorHandler (errRunRest);

				break;
			
			case I_MULR:
				if (realOnly==false)
				{
					realOnly = true;
					intArr2douArr(&(resArrInt[0]),&(resArrDou[0]),longestExpressionLength);
				}

				if (A1 -> data -> type == t_expr_int || A1 -> data -> type == sym_var_int )
					A1 -> data -> content . real = (double) A1 -> data -> content . integer;
				if (A2 -> data -> type == t_expr_int || A2 -> data -> type == sym_var_int )
					A2 -> data -> content . real = (double) A2 -> data -> content . integer;

				realOnly = true;
				A1 = (tNodePtr) new -> adr1;
				A2 = (tNodePtr) new -> adr2;

				// Prisli nam dve nove adresy => zaciatok noveho medzivypoctu

				if (A1 -> data -> used == false && A2 -> data -> used == false)
				{
					if (debug == true) printf("dve nove\n");

					if (A1 -> data -> content . initialized == false ||
						A2 -> data -> content . initialized == false  ) 
						errorHandler (errRunUnin);

					resArrDouIndex++;
					resArrDou [resArrDouIndex] = A1 -> data -> content . real * A2 -> data -> content . real;
					A1 -> data -> used = true;
					A2 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("MULR: %g\n", readyDou);
					break;
				}

				// Prva adresa nebola pouzita ale druha ano => pouzijem prvu

				if (A1 -> data -> used == false && A2 -> data -> used == true)
				{	
					if (debug == true) printf("prva\n");

					if (A1 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					resArrDou [resArrDouIndex] *= A1 -> data -> content . real;
					A1 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("MULR: %g\n", readyDou);
					break;
				}

				// Prva adresa uz bola druha nie  => pouzijem druhu

				if (A1 -> data -> used == true && A2 -> data -> used == false)
				{
					if (debug == true) printf("druha\n");

					if (A2 -> data -> content . initialized == false) 
						errorHandler (errRunUnin);

					resArrDou [resArrDouIndex] *= A2 -> data -> content . real;
					A2 -> data -> used = true;
					readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("MULR: %g\n", readyDou);
					break;
				}

				// Obe adresy boli pouzite => zratam medzivysledky

				if (A1 -> data -> used == true && A2 -> data -> used == true)
				{
					if (debug == true) printf("aniani\n");

					if (resArrDouIndex-1 != -1)
					{
						resArrDou [resArrDouIndex-1] *= resArrDou [resArrDouIndex];
						resArrDouIndex--;
						readyDou = resArrDou [resArrDouIndex];
					}
					else
						readyDou = resArrDou [resArrDouIndex];

					if(debug == true) printf("MULR: %g\n", readyDou);
					break;
				}

				errorHandler (errRunRest);

				break;

			case I_DIVR:

				temp = ((tNodePtr) new->adr1);
				temp2 = ((tNodePtr) new->adr2);

				if(temp->data->type == sym_var_int || temp->data->type == t_expr_int)
					temp->data->content.real = (double) temp->data->content.integer;
	
					
				if(temp2->data->type == sym_var_int || temp2->data->type == t_expr_int)
					temp2->data->content.real = (double) temp2->data->content.integer;
			
				if(vypocet == false)
				{

					
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
					//temp2 = ((tNodePtr) new->adr2);
					
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
				if (readyInt < 0) errorHandler(errRunRest);	

				((tData) new->result)->content.integer = readyInt;
				((tData) new->result)->content.initialized = true;		
				readyInt = 0;
				vypocet = false;
				resArrIntIndex = -1;
				preklopenie(currIL);


				if(debug == true)	
					printf("%svysledok ASGNI: %d%s\n", KGRN, (((tData) new->result)->content.integer),KNRM);
				break;		

			case I_ASGNR:
				if(readyDou < 0) errorHandler(errRunRest);	

				((tData) new->result)->content.real = readyDou;
				((tData) new->result)->content.initialized = true;				
				readyDou = 0;
				vypocet = false;
				resArrIntIndex = -1;
				preklopenie(currIL);

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
				((tData) new->result)->content.initialized = true;
				
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
					recycleAdr = true;
					interpret(&rootTS, ((tInsList *) new->adr1));
					preklopenie(currIL);

					currIL->active = revert;
					//((tInsList *) new->adr1)->active=revert->next;
					printf("then vetva skoncila\n");	
								
					break;
				} else 
				{
					printf("volam picu z else\n");	//lastbool = 0;
					tListItem * revert = currIL->active;
					//tListItem * revert = ((tInsList *) new->adr1)->active;
					recycleAdr = true;
					interpret(&rootTS, ((tInsList *) new->adr2));
					preklopenie(currIL);

					//((tInsList *) new->adr1)->active=revert->next;
					currIL->active = revert;
					break;
				}
			case I_VAR:break;
			
			case I_WHILE:
				printf("som vo vhwiweled\n");

				while(lastbool == true)
				{
					recycleAdr = true;
					interpret(&rootTS, ((tInsList *) new->adr1));
					preklopenie(currIL);
				}
				break;

			case I_FCE:
					


				printf("FUNKCIA ZACINA DRZTE SI KLOBUKY\n");
				currentTerm = (((tData) new->adr1)->nextArg);
				//printf("%s\n", currentTerm->data->name);
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
					readyInt = strlen(((tData) new->adr1)->nextArg->data->content.string);
					//printf("%d\n", readyInt);
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
							readyInt = BMASearch(((tData) new->adr1)->nextArg->data->content.string,
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
								
								recycleAdr = true;
								interpret(/*&(((tData) new->adr1)->localTSadr)*/NULL, (((tData) new->adr1)->localILadr));
								preklopenie(currIL);
								
								printf("__________________________________END_%d\n",kanter);kanter--;
								
								
								currIL->active = revert;

                
            	
				                switch((((tData) new->adr1)->type))
								{
									case sym_fun_int:
									case sym_fok_int:	
										readyInt = (*((tContent *) new->result)).integer;
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
								//printf("readyInt je %d\n",readyInt );

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
