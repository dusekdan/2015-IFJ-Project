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

void InitMarius (tSmetisko * Smetisko)
//inicializuje zoznam
{
    if (supertruck==true)printf("Pan Pedersen se snazi inicializovat list.\n");
    Smetisko -> first  = NULL;
    Smetisko -> last   = NULL;
    Smetisko -> active = NULL;
    if (supertruck==true)printf("Pan Pedersen uspesne inicializoval list.\n");
}

void DisposeMarius (tSmetisko * Smetisko)
//zrusenie prvok zoznamu, zoznam sa nachadza v stave, v akom bol po inicializaci
{
    tKontajner * tmp;
    
    while (Smetisko -> first != NULL)
    {
        tmp = Smetisko -> first;
        Smetisko -> first = Smetisko -> first->next;
        free(tmp);
    }
    
    Smetisko -> active = NULL;
    Smetisko -> last   = NULL;
}

void InsertLastMarius (tSmetisko * Smetisko, void * Odpad)
//vlozi polozku na koniec zoznamu
{
    if (&(Smetisko -> active -> Odpad) != &Odpad)
    {    
        tKontajner * new;
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
            exit(1);
        }
    }else printf("byli stejne\n");
}

void FirstMarius (tSmetisko * Smetisko)
//nastavi aktivitu na prvy prvok
{
    Smetisko -> active = Smetisko -> first;
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

int mariuspedersen (tSmetisko * Smetisko)
{
    
    printf("Dobry den, jmenuji sa Marius Pedersen a prisel jsem vycistit Vas odpad.\n");
    fclose(fd);
    int trashCounter = 0;
    if (supertruck==true)
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

    FirstMarius (Smetisko);
    printf("nastavil sem prvni smetisko\n");
    while (Smetisko -> active != NULL)
    {
        //printf("%d\n",Smetisko->active->Odpad );
        if (Smetisko -> active -> Odpad != NULL)
        {    
            free (Smetisko -> active -> Odpad);
            Smetisko -> active -> Odpad = NULL;
        }
        trashCounter++;
        if (supertruck==true)printf("Freenul jsem odpad %d\n",trashCounter);
        
        SuccMarius (Smetisko);
    }
    printf("Uspesne jsem provedl %d operaci free.\n",trashCounter);
    //disposeTable(&localTS);
    
    //DisposeList(&IL);/*disposeTable(&rootTS);*/
    DisposeMarius(Smetisko);
    return 0;
    

}