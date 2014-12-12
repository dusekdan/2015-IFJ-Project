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
    printf("Pan Pedersen se snazi inicializovat list.\n");
    Smetisko -> first  = NULL;
    Smetisko -> last   = NULL;
    Smetisko -> active = NULL;
    printf("Pan Pedersen uspesne inicializoval list.\n");
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
    int trashCounter = 1;
    
    printf("Dobry den, jmenuji sa Marius Pedersen a prisel jsem vycistit Vas odpad.\n");
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
    while (Smetisko -> active != NULL)
    {
        //printf("%d\n",Smetisko->active->Odpad );
        free (Smetisko -> active -> Odpad);
        if (supertruck==true)printf("Freenul jsem odpad %d\n",trashCounter++);
        SuccMarius (Smetisko);
    }
    printf("Uspesne jsem vycistil veskerou alokovanou pamet.\n");
    //disposeTable(&localTS);
    fclose(fd);
    DisposeList(&IL);/*disposeTable(&rootTS);*/
    DisposeMarius( Smetisko);
    

}