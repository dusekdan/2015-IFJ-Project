/*     ________________   ___/-\___     ___/-\___     ___/-\___
     / /             ||  |---------|   |---------|   |---------|
    / /              ||   |       |     | | | | |     |   |   |
   / /             __||   |       |     | | | | |     | | | | |
  / /   \\        I  ||   |       |     | | | | |     | | | | |
 (-------------------||   | | | | |     | | | | |     | | | | |
 ||               == ||   |_______|     |_______|     |_______|
 ||  Marius Pedersen | =============================================
 ||          ____    |                                ____      |
( | o      / ____ \                                 / ____ \    |)
 ||      / / . . \ \                              / / . . \ \   |
[ |_____| | .   . | |____________________________| | .   . | |__]
          | .   . |                                | .   . |
           \_____/                  -cfbd-          \_____/
*/

//#include <mcheck.h>


void InitMarius (tSmetisko * Smetisko)
//inicializuje zoznam
{
    if (supertruck==true)printf("Pan Pedersen se snazi inicializovat list.\n");
    Smetisko -> first  = NULL;
    Smetisko -> last   = NULL;
    Smetisko -> active = NULL;
    if (supertruck==true)printf("Pan Pedersen uspesne inicializoval list.\n");
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
    printf("idem dispose\n");
    while (Smetisko -> first != NULL)
    {
        tmp = Smetisko -> first;
        //printf("I'm about to free pointer %u\n",tmp );
        free(tmp);
        Smetisko -> first = Smetisko -> first->next;
        //printf("Novy first je %p\n",(void*)Smetisko->first );
        
    }
    printf("skoncil som\n");
    Smetisko -> active = NULL;
    Smetisko -> last   = NULL;
}
//int totalodpad=0;
void InsertLastMarius (tSmetisko * Smetisko, void * Odpad)
//vlozi polozku na koniec zoznamu
{//totalodpad++;
    //printf("vlozil jsem odpad %d\n",totalodpad );
    //if (&(Smetisko -> active -> Odpad) != &Odpad)
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
            printf("nemohol alokovat\n");
            exit(1);
        }
    }//else printf("byli stejne\n");
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
    if (supertruck==true)
    {
    printf("Dobry den, jmenuji sa Marius Pedersen a prisel jsem vycistit Vas odpad.\n");
    fclose(fd);
    
    
{
    printf("       ________________   ___/-\\___     ___/-\\___     ___/-\\___\n");
    printf("     / /             ||  |---------|   |---------|   |---------|\n");
    printf("    / /              ||   |       |     | | | | |     |   |   |\n");
    printf("   / /             __||   |       |     | | | | |     | | | | |\n");
    printf("  / /    \\        I  ||   |       |     | | | | |     | | | | |\n");
    printf(" (-------------------||   | | | | |     | | | | |     | | | | |\n");
    printf(" ||               == ||   |_______|     |_______|     |_______|\n");
    printf(" || Marius Pedersen  | =============================================\n");
    printf(" ||          ____    |                                ____      |\n");
    printf("( | o      / ____ \\                                 / ____ \\    |)\n");
    printf(" ||      / / . . \\ \\                              / / . . \\ \\   |\n");
    printf("[ |_____| | .   . | |____________________________| | .   . | |__]\n");
    printf("          | .   . |                                | .   . |\n");
    printf("           \\_____/                  -cfbd-          \\_____/\n");
}

    FirstMarius (SmetiskoPTR);
    //printf("nastavil sem prvni smetisko\n");
    //tNodePtr MariusTree;
    //init (&MariusTree);

    
    while ((SmetiskoPTR->active) != NULL)
    {   //printf("som zas dnu\n");
       // 
       // insetSymbol (&MariusTree, )

        //printf("%d\n",Smetisko->active->Odpad );
        
        //printf("I'm about to free pointer %u\n", SmetiskoPTR->active->Odpad );
        //printf("Odpad je %d, should be %d\n",mprobe(Smetisko -> active -> Odpad),MCHECK_OK);
        //if (Smetisko -> first != Smetisko ->first->Odpad)
        free (SmetiskoPTR -> active -> Odpad);
        //printf("vycistil odpad\n");
        //tKontajner * tmp = SmetiskoPTR->first;
        //SmetiskoPTR->active =( SmetiskoPTR -> active -> next);
        //printf("Smetisko first je %u\n", (SmetiskoPTR -> active));
        //printf("tmp je %u\n", tmp);

        //free (tmp);
        //printf("Odpad je %d, should be %d\n",mprobe(Smetisko -> active -> Odpad),MCHECK_FREE);

        trashCounter++;
        if (supertruck==true)printf("Freenul jsem odpad %d\n",trashCounter);
        
        SuccMarius (SmetiskoPTR);
        //printf("smetisko active je %p\n",(void * )Smetisko->active );
        /*printf("SMETISKO JE               %u\n",SmetiskoPTR);
        printf("SMETISKO FIRST JE         %u\n",SmetiskoPTR->first);
        printf("SMETISKO FIRST ODPAD JE   %u\n",SmetiskoPTR->first->Odpad);*/
    }
    printf("Uspesne jsem provedl %d operaci free.\n",trashCounter);
    
    DisposeMarius(SmetiskoPTR);

    /*tNodePtr currentNode = rootTS;
    while (currentNode->rptr!=NULL)
    {
        currentNode=currentNode->rptr;
    }
    printf("som v nnajhlbsom pravom a je to %s a jeho localts je %u\n",currentNode->key,currentNode->data->localTSadr );
    *///DisposeList(&IL);
    }
    disposeTable(&rootTS);
    //exit(666);

    return 0;
    

}