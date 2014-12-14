/***********************mariuspedersen.c*************************/
/* Soubor: mariuspedersen.c - garbage collector                 */
/* Předmět: Formalní jazyky a překladače (IFJ)                  */
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I                                       */
/* Datum: prosinec 2014                                         */
/* Kódování: UTF-8                                              */
/* Autoři:          Roman Jaška  (xjaska00)                     */
/*                  Matúš Bútora (xbutor01)                     */
/****************************************************************/

void InitMarius (tSmetisko * Smetisko)
//inicializuje zoznam
{
    Smetisko -> first  = NULL;
    Smetisko -> last   = NULL;
    Smetisko -> active = NULL;
}

void FirstMarius (tSmetisko * Smetisko)
//nastavi aktivitu na prvy prvok
{
    Smetisko -> active = Smetisko -> first;
}

void DisposeMarius (tSmetisko * Smetisko)
//zrusenie prvok zoznamu, zoznam sa nachadza v stave, v akom bol po inicializaci
{
    tKontajner tmp;
    FirstMarius (Smetisko);
    while (Smetisko -> first != NULL)
    {
        tmp = Smetisko -> first;
        free(tmp);
        Smetisko -> first = Smetisko -> first->next;
        
    }
    Smetisko -> active = NULL;
    Smetisko -> last   = NULL;
}

void InsertLastMarius (tSmetisko * Smetisko, void * Odpad)
//vlozi polozku na koniec zoznamu
{
    {    
        tKontajner new;
        if ((new = malloc (sizeof (tKontajner))) != NULL)
        {
            new -> Odpad = Odpad;
            new -> next  =  NULL;

            if ( Smetisko -> first == NULL)
            {
                Smetisko -> first = new;
            
            }
            else
            {
                Smetisko -> last -> next = new;
            }   
            
            Smetisko -> last = new;
        }
        else 
        {
            fprintf(stderr,"nemohol alokovat\n");
            exit(99);
        }
    }
}



void SuccMarius (tSmetisko * Smetisko)
//posune aktivitu na dalsi prvok v zozname
{
    if (Smetisko -> active != NULL)
        if (Smetisko -> active -> next != NULL)
        {
            Smetisko -> active = Smetisko -> active -> next;
        }
        else 
            Smetisko -> active = NULL;
}



int mariuspedersen (tSmetisko * SmetiskoPTR)
{
    fclose(fd);


    FirstMarius (SmetiskoPTR);
    
    while ((SmetiskoPTR->active) != NULL)
    {
        free (SmetiskoPTR -> active -> Odpad);
        
        trashCounter++;
        
        SuccMarius (SmetiskoPTR);
    }
    
    DisposeMarius(SmetiskoPTR);

    

    disposeTable(&rootTS);

    return 0;
    

}