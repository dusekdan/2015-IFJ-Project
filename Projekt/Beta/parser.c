////////////////////Roman Jaška
////////////////////rozrobené
/*         ___
         ',_`""\        .---,
            \   :-""``/`    |
             `;'     //`\   /
             /   __     |   ('.
            |_ ./O)\     \  `) \
           _/-.    `      `"`  |`-.
       .-=; `                  /   `-.
      /o o \   ,_,           .        '.
      L._._;_.-'           .            `'-.
        `'-.`             '                 `'-.
            `.         '                        `-._
              '-._. -'                              '.
                 \                                    `\
                  |                                     \
                  |    |                                 ;   _.
                  \    |           |                     |-.((
                   ;.  \           /    /                |-.`\)
                   | '. ;         /    |                 |(_) )
                   |   \ \       /`    |                 ;'--'
                    \   '.\    /`      |                /
                     |   /`|  ;        \               /
                     |  |  |  |-._      '.           .'
                     /  |  |  |__.`'---"_;'-.     .-'
                    //__/  /  |    .-'``     _.-'`
                  jgs     //__/   //___.--''`
*/
#include "parser.h"

/* Ukazateľ na lokálnu tabuľku */
//tContent **globalArr;


//tContent *elegeblege[100];

/* Premenná rozhodujúca či mám hladať len v globálnej tabuľke */

bool searchGlobalOnly = true;

/* Aktuálny počet forward deklarácií čakajúcich na riadne telá */

int fwdDeclarations = 0;

/* Počítadlo počtu argumentov funkcie pri ich spracovaní v deklarácií **
** a pri následnom volaní                                             */

int pocetArg = 0;

/* Token pre lexikálny analyzátor */

token tok;

bool debug=false;

#include "precedence3.c"

/* Funkcia pre prichystanie key na prácu so symbolom */

char * createKey (char * prefix, char * suffix)
{
    /* Alokovanie pamäti pre výsledný kľúč = suffix + prefix */

    char * key = malloc (sizeof (char) * (strlen (suffix) + strlen (prefix)));

    /* Pri zlyhaní malloc volám internú chybu (99)         */
    /* Inak spojím prefix a suffix a vrátim výsledný string*/

    if (key == NULL)
    {
        printf ("jebol createKey\n");
        errorHandler (errInt);
    }

    else
    {
        memset (key, 0, strlen (key));
        strcpy (key, prefix);
        strcat (key, suffix);
    }
    return (key);
}

/* Funkcia ktorá vytvorí nový symbol podľa parametrov ak ešte neexistuje */

bool saveSymbol (tNodePtr * currTS, char * key, char * name, int type, int argCount, bool errOnRedef)
{
    /* Otestujem či už náhodou neexistuje daný symbol */

    if (searchSymbol (currTS, key) == 0)
    {

    /* Alokujem si miesto pre data pre nový symbol v tabulke */

    tData newsymbol = malloc (sizeof (struct tData));
    
    /* Ak alokácia zlyhala alebo predaný name či key je NULL volám chybu 99 */
    /* Ak alokácia prebehla bez problémov a názvy sú neprázdne, pokračujem  */

    if (newsymbol != NULL || key != NULL || name != NULL)
    {
        /*Do dátovej štruktúry uložím dáta predané funkcii*/

        newsymbol -> name     = name;
        newsymbol -> type     = type;
        newsymbol -> nextArg  = NULL;
        newsymbol -> argCount = argCount;
        newsymbol -> localTSadr = NULL;
        newsymbol -> localILadr = NULL;
        //newsymbol -> content.integer = rand() % 100;

        /*Uložím do aktuálnej tabulky nový symbol ktorý som si práve pripravil*/
        /*Ak vloženie zlyhá, vraciam internú chybu errInt(99)                 */

        if (insertSymbol (currTS, key, newsymbol) == 0)
        {
            printf ("jkebol saveSymbol\n");
            errorHandler (errInt);
        }

        return true; 
    }
        else
        {    
            printf ("jebol saveSymbol 2\n");
            errorHandler (errInt);
        }
    }
    else
        if (errOnRedef == true)
            errorHandler (errSemDef);
        return false;
}

/* Funkcia pre pridanie vstavaných funkcií do globálnej TS */ 

int buildemin ()
{
    saveSymbol (&rootTS, "Flength", "length", sym_fok_int, 1, true);
    saveSymbol (&rootTS, "Fcopy",   "copy",   sym_fok_str, 3, true);
    saveSymbol (&rootTS, "Ffind",   "find",   sym_fok_int, 2, true);
    saveSymbol (&rootTS, "Fsort",   "sort",   sym_fok_str, 1, true);
    return 0;
}

/* Funkcia vypisujúca názov terminálu z jeho typu */

void terminalis (int terminal, token tok)
{
    if (debug==true)
        printf("%s",KCYN);
    switch (terminal)
    {
        case t_var:       printf ("var\n");                  break;
        case t_colon:     printf (": ");                     break;
        case t_semicolon: printf (";\n");                    break;
        case t_l_parrent: printf ("( ");                     break;
        case t_r_parrent: printf (") ");                     break;
        case t_function:  printf ("function ");              break;
        case t_forward:   printf ("forward ");               break;
        case t_begin:     printf ("begin\n");                break;
        case t_end:       printf ("end\n");                  break;
        case t_period:    printf (".\n");                    break;
        case t_comma:     printf (", ");                     break;
        case t_assign:    printf (":= ");                    break;
        case t_if:        printf ("if ");                    break;
        case t_then:      printf ("then\n");                 break;
        case t_else:      printf ("else\n");                 break;
        case t_while:     printf ("while ");                 break;
        case t_do:        printf ("do ");                    break;
        case t_readln:    printf ("readln ");                break;
        case t_write:     printf ("write ");                 break;
        case t_var_id:    printf ("%s ", tok -> val_str);    break;
      //case 21: printf ("expr ");                           break;
        case t_fun_id:    printf ("%s ", tok -> val_str);    break;
      //case 23: printf ("term ");                           break;
      //case 24: printf ("param ");                          break;
      //case 25: printf ("read_id ");                        break;
        case t_integer:   printf ("integer ");               break;
        case t_real:      printf ("real ");                  break;
        case t_string:    printf ("string ");                break;
        case t_boolean:   printf ("boolean ");               break;
        case t_dollar:    printf ("$\n");                    break;
        case t_expr_int:  printf ("INTEGER " );              break;
        case t_expr_str:  printf ("STRING " );               break;
        case t_expr_dou:  printf ("DOUBLE ");                break;
        case t_expr_boo:  printf ("BOOL ");                  break;
        default: printf ("error input=%d\n", terminal);
    }
    printf("%s",KNRM );
}

/* Funkcia porovnávajúca spracovaný terminál so vstupom, následne žiada token */

void match (token tok, int terminal)
{
    if (tok -> type == terminal)
    {
        getNextToken (fd, tok);
        terminalis (terminal, tok);
        /*if (localTS==NULL)
            printf("LOCALNULL\n");
        else
            printf("LOCAL %d\n",&*localTS );*/
    }
    else
        {
            printf ("jebol match dostal v tokene typ %d\n", tok -> type);
            errorHandler (errSyn);
        }
}


void nt_program (token tok)
{
    //////////////////////////////////////////////////////////////////////RULE 1
    if (tok -> type == t_var      ||
        tok -> type == t_function ||
        tok -> type == t_begin     )
    {   
        if (tok -> type == t_var)
        {
            nt_var_def_block (tok);
        }
        if (tok -> type == t_function)
        {
            nt_fun_def_list (tok);
        }
        if (tok -> type == t_begin)
        {
            nt_main (tok);
            if (fwdDeclarations != 0)
                errorHandler (errSemDef);
        }
    }
    else
    {    
        printf ("nt_program\n");
        printf ("token bol %d a valstr bol %s\n", tok -> type, tok -> val_str);
        errorHandler (errSyn);
    }
}

void nt_var_def_block (token tok)
{
    if (tok -> type == t_var      ||
        tok -> type == t_function ||
        tok -> type == t_begin     )
    {   
        ///////////////////////////////////////////////////////////////////RULE2
        if (tok -> type == t_var)
        {
            match (tok, t_var);
            nt_var_def (tok);
            nt_var_def_list (tok);
        }
        ///////////////////////////////////////////////////////////////////RULE3
        else
            return;
    }
    else
    {    
        printf ("nt_var_def_block\n");
        errorHandler (errSyn);
    }
}

void nt_var_def_list (token tok)
{
    if (tok->type == t_function ||
        tok->type == t_begin    ||
        tok->type == t_var_id    )
    {

        ///////////////////////////////////////////////////////////////////RULE4
        if (tok -> type == t_var_id)
        {
            nt_var_def (tok);
            nt_var_def_list (tok);
        }
        ///////////////////////////////////////////////////////////////////RULE5
        else
            return;
    }
    else
    {    
        printf ("nt_var_def_list\n");
        errorHandler (errSyn);
    }
}

void nt_var_def (token tok)
{
    ///////////////////////////////////////////////////////////////////////RULE6
    
    if (tok -> type == t_var_id)
    {
        /* Pripravím si key pre ukladanie */

        char * key = createKey ("V", tok -> val_str);

        /* Rozhodnem sa s ktorou tabulkou idem pracovať */

        tNodePtr * currTS = NULL;
        currTS = (searchGlobalOnly == true) ? &rootTS : &localTS;

        /* Uložím premennú do odpovedajúcej tabulky symbolov */

        saveSymbol (&*currTS, key, tok -> val_str, t_var_id, 0, true);

        /* Porovnanie terminálov */

        match (tok, t_var_id);
        match (tok, t_colon);

        /* Priradenie typu práve uloženej premennej */
        /* searchType=1 hovorí, že ide o premennú   */

        nt_type (tok, key);

        /* Porovnanie zvyšného terminálu */

        match (tok, t_semicolon);

        //Sem pride instrukcia
        tNodePtr currentVar = searchSymbol(&*currTS, key);
        printf("%s",KYEL);
        if (localIL==NULL){
            insertInst (&IL, I_VAR, &currentVar->data, NULL, NULL);
            printf("GLOBAL\n");printf("Vlozil som instrukciu I_VAR s ukazatelom %u do IL %u\n", &currentVar->data,&IL);
        }

        else{
            insertInst (localIL, I_VAR, &currentVar->data, NULL, NULL);
            printf("LOCAL\n");printf("Vlozil som instrukciu I_VAR s ukazatelom %u do IL %u\n", &currentVar->data,localIL);
        }
        printf("%s",KNRM);
        free (key);

    }
    else
    {    
        printf ("nt_var_def\n");
        errorHandler (errSyn);
    }

}

void nt_fun_def_list (token tok)
{
    if (tok->type == t_function || tok->type == t_begin)
    {   

        /* Premenná ktorá rozhoduje či môže prísť forward deklarácia alebo nie*/

        bool nextMustBeBody    = false;
        char * rememberVarName =  NULL;
        ///////////////////////////////////////////////////////////////////RULE7

        if (tok -> type == t_function)
        {
            match (tok, t_function);

            char * key = createKey ("F", tok -> val_str);

            /* Ak podmienka je true, znamená to, že funkcia ešte nebola de-   */
            /* klarovaná a teda jej deklarácia prebehla v poriadku. Dodatočne */
            /* sa k nej dodeklarovala lokálna premenná s rovnakým menom v lo- */
            /* kálnej tabulke                                                 */

            if (saveSymbol (&rootTS, key, tok -> val_str, t_fun_id, 0, false) == true)
            {
                //Kedze vytvaram novu funkciu potrebujem novu lokalnu tabulku
                tNodePtr newLocalTS;
                init(&newLocalTS);
                //musel som tam nieco vloyit lebo nad prazdnou to blblo
                saveSymbol(&newLocalTS, "placeholder", "testname", 0,0,true);
                localTS=newLocalTS;
                //Do funkcie ulozim jej lokalnuTS
                //printf("localTS je teraz %d\n",&*localTS );
                searchSymbol(&rootTS, key)->data->localTSadr=newLocalTS;

                /*Uloženie lokálnej premennej patriacej k práve uloženej fun.*/

                char *funVarKey = createKey ("V", tok -> val_str);
                rememberVarName = createKey ("V", tok -> val_str);
                saveSymbol (&localTS, funVarKey, tok->val_str, t_var_id, 0, true);
                //printf("vnorena kokotina nasla %s\n",searchSymbol(&(searchSymbol(&rootTS, key)->data->localTSadr),funVarKey)->data->name);
                free (funVarKey);
            }

            /* V opačnom prípade ide buď o redeklaráciu alebo doplnenie for-  */
            /* ward deklarácie                                                */

            else
            {   
                localTS = searchSymbol (&rootTS, key) -> data -> localTSadr;
                //printf("kotrola priradil som %d\n",&*localTS ); //printf("Som v inej a nasiel som%s\n",searchSymbol(&localTS, "test")->data->name);
                /* Ak v rámci programu bola aspoň jedna forward deklarácia, */
                /* tak v tejto deklarácií už musí byť telo funkcie, inak by */
                /* šlo o redeklaráciu                                       */

                if (fwdDeclarations != 0)
                {
                    nextMustBeBody = true;
                }

                /* Ak podmienka nebola splnená, znamená to, že ide o redef. */

                else
                {
                    printf ("Redefinition of function %s\n", tok->val_str);
                    errorHandler (errSemDef);
                }
            }


            match (tok, t_fun_id);
            match (tok, t_l_parrent);

            /* Spracovanie parametrov funkcie */

            nt_param_list (tok, nextMustBeBody, key);
            
            /* Nulujem počítadlo aktuálneho param, lebo skončili parametre */
//bolo tu povodne argsRead
            //pocetArg = 0;

            /* Ak ide o prvú delkaráciu funkcie, uložím počet jej argumentov **
            ** do jej symbolu do premennej argCount                          */

            if (nextMustBeBody == false)
            {
                searchSymbol (&rootTS, key) -> data -> argCount = pocetArg;
                printf("uloil som %d\n",searchSymbol(&rootTS, key)->data->argCount );

                /* Vynulujem počítadlo argumentov lebo som skončil paramlist */

                
            }
pocetArg = 0;
            /* ) : */

            match (tok, t_r_parrent);
            match (tok, t_colon);

            /* Priradím alebo skontrolujem typ funkcie, viac info v nt_type */

            nt_type (tok, key);

            /* Nakoniec priradím správny typ rovnomennej lokálnej premennej   **
            ** rovnomennej funkcie. V case som musel použiť magické čísla,    **
            ** pretože case nepodporuje konštanty. Čísla 5 až 8 predstavujú   **
            ** funkciu, ktorá už má priradený typ, ale ešte neprišlo telo.    **
            ** Čísla 9 až 12 sú pre funkcie ktoré majú zatiaľ len forward de- **
            ** klaráciu a čísla 13 až 16 sú pre funkcie ktoré už majú aj typ, **
            ** aj riadne telo. Typy idú v poradí INTEGER, REAL, STRING, BOO-  **
            ** LEAN.                                                          */

            if (rememberVarName != NULL)
                switch (searchSymbol (&rootTS, key) -> data -> type)
                {
                    case 5:
                    case 9:
                    case 13:    searchSymbol (&localTS, rememberVarName) -> data -> type = sym_var_int;
                                break;
                    case 6:
                    case 10:
                    case 14:    searchSymbol (&localTS, rememberVarName) -> data -> type = sym_var_rea;
                                break;
                    case 7:
                    case 11:
                    case 15:    searchSymbol (&localTS, rememberVarName) -> data -> type = sym_var_str;
                                break;
                    case 8:
                    case 12:
                    case 16:    searchSymbol (&localTS, rememberVarName) -> data -> type = sym_var_boo;
                                break;
                    default:    errorHandler (errSemTypArg);
                                break;
                }

            /* ; */
            
            match (tok, t_semicolon);

            /* Do symbolu funkcie uložím ukazateľ na jej lokálnu tabuľku sym. */

            //searchSymbol (&rootTS, key) -> data -> localTSadr = localTS;


            /* Spracovanie tela funkcie. nextMustBeBody mi hovorí či už musí **
            ** prísť telo funkcie (true) alebo môže byť forward (false)      **
            ** premenná key sa predáva aby som vedel názov aktuálne funkcie  */

            nt_fun_body (tok, nextMustBeBody, key);

            /* Vynulujem globálnu TS, pre istotu */

            localTS = NULL;

            /* Uvoľnenie alokovaných stringov */

            free (key);
            free (rememberVarName);

            /* ; */

            match (tok, t_semicolon);

            /* Rekurzívne volanie tejto istej funkcie pre prípad, že by v kó- **
            ** de bolo viac deklarácií funkcií po sebe                        */

            nt_fun_def_list(tok);

        }
        ///////////////////////////////////////////////////////////////////RULE8
        else

            /* Eps. pravidlo - Deklarácie užívateľských funkcií niesú povinné */

            return;
    }

    /* Prípad keď prišiel nesprávny terminál */

    else
    {    
        printf ("nt_fun_def_list\n");
        errorHandler (errSyn);
    }
}

void nt_fun_body (token tok, bool nextMustBeBody, char * key)
{
    /* Zoznam terminálov na ktoré funkcia reaguje */

    if (tok -> type == t_forward ||
        tok -> type == t_begin   ||
        tok -> type == t_var      )
    {   
        
        ///////////////////////////////////////////////////////////////////RULE9 +++treba zaistit ze neskor bude definovana
        
        /* Ak som miesto tela funkcie dostal kľúčové slovo FORWARD a zároveň  **
        ** nextMustBeBody bolo false, čo znamená že forward deklarácia k tej- **
        ** to funkcii ešte nebola, takže može byť teraz                       */

        if (tok -> type == t_forward && nextMustBeBody == false)
        {
            /* Zvýšim globálne počítadlo fwd deklarácií čakajúcich na telo */

            fwdDeclarations++;

            /* K typu aktuálnej funkie pripočítam 4, čím sa dostane do stavu  **
            ** 9 až 12 čo znamená že ide o funkciu ktorá má forward deklará-  **
            ** cie a čaká na riadne telo                                      */

            searchSymbol (&rootTS, key) -> data -> type += 4;

            /* forward */

            match (tok, t_forward);

        }
        //////////////////////////////////////////////////////////////////RULE10
        
        /* Else znamená že prišlo VAR alebo BEGIN */

        else
        {
            // Potrebujem tabulku instrukcii
            //tInsList newLocalIL;

            tInsList *newLocalILptr=malloc (sizeof(tInsList));
            //InitList (&newLocalIL);

            /* Do "hledam" si uložím symbol aktuálnej funkcie */

            tNodePtr hledam = searchSymbol(&rootTS, key);

            hledam->data->localILadr=newLocalILptr;

            localIL=newLocalILptr;
            printf("localIL je teraz %u\n",&*localIL );

            /* Ak už teraz musí prísť telo, čiže ak aktuálna funkcia už mala  **
            ** forward deklaráciu, tak to znamená že tým že mi sem neprišlo   **
            ** forward, môžem túto funkciu zmeniť z typu 9 až 12 na 13 až 16, **
            ** čo znamená že funkcia má aj typ aj telo. Následne znížim glo-  **
            ** bálne počátadlo dopredne deklarovaných funkcií fwdDeclarations */ 

            if (nextMustBeBody == true)
            {    
                hledam -> data -> type += 4;
                fwdDeclarations--;
            }

            /* Globálnu premennú searchGlobalOnly nastavím na false, pretože  **
            ** pôjdem spracovávať telo funkcie ktoré bude využívať lokálnu    **
            ** tabuľku symbolov                                               */

            searchGlobalOnly = false;

            /* Pravidlo pre deklarácie lokálnych premenných a samotné telo */

            nt_var_def_block (tok);
            nt_body (tok);

            /* Akonáhle sa dostanem von z tela funkcie, opäť ma zaujímajú len **
            ** globálne premenné a funkcie                                    */

            searchGlobalOnly = true;


            /* Do symbolu funkcie uložím ukazateľ na jej lokálnu tabuľku sym- **
            ** bolov a vynuluje globálny ukazateľ                             */

             //(hledam -> data -> localTSadr) = localTS;
            localIL = NULL;

            //disposeTable(&localTS);
        }
    }

    /* Prípad keď prišiel nesprávny terminál */

    else
    {
        printf ("synerror in nt_fun_def_list\n");
        errorHandler (errSyn);
    }
}

void nt_body (token tok)
{
    //////////////////////////////////////////////////////////////////////RULE11
    
    /* Funkcia nt_body reaguje len na terminál begin */

    if (tok -> type == t_begin)
    {
        match (tok, t_begin);
        nt_stmt_list (tok);
        match (tok, t_end);
    }

    /* Prípad keď prišiel nesprávny terminál */

    else
    {
        printf ("syn error in nt_body doslo sem %d\n", tok -> type);
        errorHandler (errSyn);
    }
}

void nt_main (token tok)
{
    //////////////////////////////////////////////////////////////////////RULE12
    
    /* Funkcia nt_main reaguje len na terminál begin */

    if (tok -> type == t_begin)
    {
        nt_body (tok);
        match (tok, t_period);
    }

    /* Prípad keď prišiel nesprávny terminál */

    else
    {
        printf ("syn error in nt_main\n");
        errorHandler (errSyn);
    }
}

void nt_stmt_list (token tok)
{
    if (tok->type == t_if     ||
        tok->type == t_end    ||
        tok->type == t_write  ||
        tok->type == t_begin  ||
        tok->type == t_while  ||
        tok->type == t_readln ||
        tok->type == t_var_id  )

    {
        //////////////////////////////////////////////////////////////////RULE14
        
        /* Epsilon pravidlo - zoznam príkazov môže byť prázdny */

        if (tok -> type == t_end)
        {
            return;
        }
        //////////////////////////////////////////////////////////////////RULE13

        /* Ak je príkaz neprázdny, daľej ho roviniem pravidlom nt_stmt */

        else
        {
            nt_stmt(tok);
            nt_stmt_more(tok);
        }

    }

    /* Prípad keď prišiel nesprávny terminál */

    else
    {
        printf ("syn error in nt_stmt_list\n");
        errorHandler (errSyn);
    }
}

void nt_stmt_more (token tok)
{
    if (tok->type == t_semicolon ||
        tok->type == t_end        )
    {
        //////////////////////////////////////////////////////////////////RULE15
        
        if (tok->type == t_semicolon)
        {
            match(tok,t_semicolon);
            nt_stmt_list(tok);
        }
        //////////////////////////////////////////////////////////////////RULE16
        
        /* Epsilon pravidlo - ďalší príkaz už nemusí prísť */

        else
            return;
    }

    /* Prípad keď prišiel nesprávny terminál */

    else
    {
        printf ("nt_stmt_more doslo sem %d\n", tok->type);
        errorHandler (errSyn);
    }
}

void nt_stmt (token tok)
{
    if (tok->type == t_if     ||
        tok->type == t_begin  ||
        tok->type == t_write  ||
        tok->type == t_while  ||
        tok->type == t_readln ||
        tok->type == t_var_id  )
    {
        tNodePtr hledam  = NULL;
        tNodePtr *currTS = NULL;
        tInsList *currIL = NULL;
        char * key       = NULL;
        int precedenceResult = 0;
        tInsList *revert;
        switch (tok->type)
        {
            //////////////////////////////////////////////////////////////RULE17
            case t_begin:   nt_body (tok);
                            break;
            //////////////////////////////////////////////////////////////RULE18
            case t_var_id:  key = createKey ("V", tok -> val_str);            
                            currTS = (searchGlobalOnly) ? &rootTS : &localTS;
                            hledam = searchSymbol (currTS, key);

                            if (hledam == 0 && searchGlobalOnly == false)
                                hledam = searchSymbol(&rootTS, key);

                            if (hledam!=0)
                                if (hledam -> data -> type == t_var_id || (hledam->data->type >= 1 && hledam->data->type <= 4))
                                {
                                    match (tok,t_var_id);
                                }
                                else
                                {
                                    printf ("nedefnovana premenna\n");
                                    errorHandler (errSemDef);
                                    return;
                                }
                            match     (tok,t_assign);

                            //SEMANTICKA KONTROLA
                            int semControlVar = nt_assign (tok);
                            //printf("semkontrol je %d\n",semControlVar );
                            //printf("skontroloval som\n");
                            if (semControlVar != hledam->data->type+4 && semControlVar != hledam->data->type+12 && semControlVar != hledam->data->type)
                            {
                                printf("nesedeli typy\n");
                                errorHandler (errSemTypArg);
                            }
                            //else printf("ok\n");

                            //Tu je rozhodovanie ktoru instrukciu mam zavolat podla typu precedencie
                            int intype = 0;
                            switch (semControlVar)
                            {
                                case 1:    intype = I_ASGNI; break;
                                case 3:    intype = I_ASGNS; break;
                                case 2:    intype = I_ASGNR; break;
                                case 4:    intype = I_ASGNB; break;
                            }
                            printf("%s",KYEL);
                            if (localIL==NULL)
                            {   
                                printf("GLOBAL\n"); printf("Vlozil som instrukciu assign %d s result %u do IL %u\n",intype,&hledam->data,&IL);
                                insertInst (&IL, intype, NULL, NULL, &hledam->data);
                            }
                            else
                            {
                                insertInst (localIL, intype, NULL, NULL, &hledam->data);
                                printf("LOCAL\n"); printf("Vlozil som instrukciu assign %d s result %u do IL %u\n",intype,&hledam->data,localIL);
                            }
                            break;
                            printf("%s",KNRM);

                            free (key);
                            break;                
            //////////////////////////////////////////////////////////////RULE19               
            case t_if:      match   (tok,t_if);
                            precedenceResult = precedenceParser();

                            if (precedenceResult != t_expr_boo)
                                errorHandler(errSemTypArg);

                            terminalis (precedenceResult, NULL); //Debug výpis

                            /* Vytvorím nový instruction list pre then vetvu, **
                            ** odpamätám si aktuálny lokálny instruction list **
                            ** do ukazateľa revert a novo-vytvorený thenIL na-**
                            ** stavím ako aktuálny lokálny instruction list.  **
                            ** V nt_body sa všetky inštrukcie budú teda dávať **
                            ** do aktuálneho lokálneho instruction listu, čo  **
                            ** je práve teraz thenIL. Na záver sa obnoví pô-  **
                            ** vodný localIL, čím sa zaistí správna funkčnosť **
                            ** pri zanorení. Obdobne funguje else vetva.      */

                            match (tok, t_then);

                            tInsList  * thenIL = malloc (sizeof (tInsList));
                            InitList   (thenIL);
                            revert    = localIL;
                            localIL   =  thenIL;
                            nt_body       (tok);
                            localIL   =  revert;

                            match (tok, t_else);

                            tInsList  * elseIL = malloc (sizeof (tInsList)); 
                            InitList   (elseIL);
                            revert    = localIL;
                            localIL   =  elseIL;
                            nt_body       (tok);
                            localIL   =  revert;

                            /* Ak je aktuálny localIL NULL, znamená že idem   **
                            ** vkladať globálnu inštrukciu takže currIL si    **
                            ** nastavím na IL a v opačnom prípade ideme do    **
                            ** aktuálneho lokálneho listu na ktorý ukazuje    **
                            ** localIL.                                       */

                            currIL =   (localIL == NULL) ? &IL : localIL;
                            insertInst (currIL, I_IF, thenIL, elseIL, NULL);

                            /* Vypísanie práve vloženej inštrukcie pre debug  */

                            if (debug == true)
                                printf ("%sNew Instruction | %u | I_IF | %u | %u | NULL |%s\n", KYEL, currIL, thenIL, elseIL, KNRM);
                            
                            break;
            //////////////////////////////////////////////////////////////RULE20
            case t_while:   match (tok,t_while);
                            precedenceResult = precedenceParser();

                            if (precedenceResult != t_expr_boo)
                                errorHandler(errSyn);

                            terminalis (precedenceResult, NULL); //Debug výpis
                            
                            /* Vytvorenie novej tabuľky pre inštrukcie tela   **
                            ** while funguje rovnako ako pri then, či else.   */

                            match (tok,t_do);

                            tInsList * whileIL = malloc (sizeof (tInsList));
                            InitList  (whileIL);
                            revert   =  localIL;
                            localIL  =  whileIL;
                            nt_body       (tok);
                            localIL  =   revert;

                            /* Ak je aktuálny localIL NULL, znamená že idem   **
                            ** vkladať globálnu inštrukciu takže currIL si    **
                            ** nastavím na IL a v opačnom prípade ideme do    **
                            ** aktuálneho lokálneho listu na ktorý ukazuje    **
                            ** localIL.                                       */

                            currIL =   (localIL == NULL) ? &IL : localIL;
                            insertInst (currIL, I_WHILE, whileIL, NULL, NULL);

                            /* Vypísanie práve vloženej inštrukcie pre debug  */

                            if (debug == true)
                                printf ("%sNew Instruction | %u | I_WHILE | %u | NULL | NULL |%s\n", KYEL, currIL, whileIL, KNRM);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE21
            case t_readln:  match (tok,t_readln);
                            match (tok,t_l_parrent);
                            //Kontrola či premenná už je v tabulke
                            key = createKey ("V", tok->val_str);

                            currTS = (searchGlobalOnly == true) ? &rootTS : &localTS;
                            hledam = searchSymbol (&*currTS, key);
                            if (hledam == 0 && searchGlobalOnly == false)
                                hledam = searchSymbol(&rootTS, key);

                            if (hledam!=0)
                                
                                if (hledam->data->type==t_var_id || (hledam->data->type >= 1 && hledam->data->type <= 4))//toto reaguje len na typove premenne 
                                {
                                    match (tok,t_var_id);
                                }
                                else
                                {
                                    printf ("nedefnovana premenna pre readln\n");
                                    errorHandler (errSemDef);
                                    return;
                                }

                            //SEMANTICKA KONTROLA
                            if (hledam->data->type < 1 || hledam->data->type > 3)
                            {
                                printf("tuna nemoze byt bool\n");
                                errorHandler (errSemTypArg);
                            }



                            match (tok,t_r_parrent);
                            free (key);
                            break;
            //////////////////////////////////////////////////////////////RULE22
            case t_write:   match (tok, t_write);
                            match (tok, t_l_parrent);
                            tContent * * ptrArr;
                            ptrArr = malloc (sizeof (struct tContent * ) * 100);

                            nt_term_list (tok, "Fwrite", ptrArr);

                            pocetArg = 0;
                            match (tok,t_r_parrent);
                            break;
        }
    }    
    else
    {
        printf("syntax error in nt_stmt\n");
        errorHandler(errSyn);
    }
}

int j = 0;

//tContent contentArrOld[256];

int nt_assign (token tok)
{
    if (tok -> type == t_l_parrent ||
        tok -> type == t_expr_int  ||
        tok -> type == t_expr_str  ||
        tok -> type == t_expr_dou  ||
        tok -> type == t_fun_id    ||
        tok -> type == t_var_id     )
    {
        //////////////////////////////////////////////////////////////////RULE23
        if (tok -> type == t_l_parrent ||
            tok -> type == t_expr_int  ||
            tok -> type == t_expr_str  ||
            tok -> type == t_expr_dou  ||
            tok -> type == t_var_id     )
        {

            int precedenceResult = precedenceParser();
            terminalis (precedenceResult, NULL);
            switch (precedenceResult)
            {
                case t_expr_int: return sym_var_int;break;
                case t_expr_str: return sym_var_str;break;
                case t_expr_dou: return sym_var_rea;break;
                case t_expr_boo: return sym_var_boo;break;
                default: errorHandler(errSemDef);return 0;
            }
        }
        //////////////////////////////////////////////////////////////////RULE24
        else
        {
            char * key = createKey ("F", tok -> val_str);
            tNodePtr hledam = searchSymbol (&rootTS, key);
            
            if (hledam==0)
            {
                printf("Hladal som key %s\n", key);
                errorHandler(errSemDef);
            }

            match (tok, t_fun_id);
            match (tok, t_l_parrent);
            //printf("idem vytvorit pole\n");
            tContent **contentArr=malloc(sizeof(struct tContent*)*100);
            printf("vytvoril som doublepole %u\n____________________\n",&contentArr);
            //globalArr=contentArr;
            contentArr[0]=&hledam->data->nextArg->data->content;

            nt_term_list(tok, key, contentArr);
            pocetArg = 0;
            //termy su overene idem ich nahradit
            printf("\nhledam->data->argCount je %d\n",hledam->data->argCount);
            

            j=0;
            //povodne argsRead=0;
            match(tok,t_r_parrent);


            printf("%s",KYEL);                                                                                                          //treba ale este predat premenne
            if (localIL==NULL)
            {
                insertInst (&IL, I_FCE, hledam->data, contentArr, NULL);
                printf("GLOBAL\n");printf("Vlozil som instrukciu I_FCE s ukazatelom %u do IL %u\n", &hledam->data,&IL);
                                   printf("____________________________________________________\n");
            }
            else
            {
                insertInst (localIL, I_FCE, hledam->data, contentArr, NULL);
                printf("LOCAL %u\n",&*localIL);printf("Vlozil som instrukciu I_FCE s ukazatelom %u do IL %u\n", &hledam->data,localIL);
                printf("____________________________________________________\n");
            }
            printf("%s",KNRM);
            localIL=NULL;
            free(key);
            return hledam->data->type;
        }
    }
    else
    {
        printf("syn error in nt_assign\n");
        errorHandler(errSyn);
    }
    return -1;
}

void nt_term (token tok, char *currentFunctionKey, tContent **contentArr)
{
    if (tok->type == t_var_id || tok->type == t_expr_int || tok->type == t_expr_dou || tok->type == t_expr_str)
    {
        int comparison1;
        int comparison2;
        if (tok->type == t_var_id)
        {
            //printf("prislo varid\n____________________\n");
            pocetArg++;
            //povodne argsRead++;
            //printf("\n---IDEM KONTROLOVAT %d. PARAMETER vo funkcii %s\n",argsRead, currentFunctionKey);

            char * key = createKey("V",tok->val_str);
            //printf("premenna ktoru idem hladat je %s\n",key );


            //tNodePtr reload = searchSymbol (&rootTS,currentFunctionKey)->data->localTSadr;
            //printf("%s je key\n",key );
            tNodePtr hledam = NULL;
            if (searchGlobalOnly==1)
                hledam=(searchSymbol(&rootTS, key));
            else
            {
                hledam=(searchSymbol(&localTS, key));   
                if (hledam==0)
                    hledam=(searchSymbol(&rootTS, key));    
            }
            
            if (hledam!=0)
            {
                  comparison1 = hledam->data->type;
                  //printf("\n--overene %s ", hledam->data->name );
                  
                  contentArr[j]=&hledam->data->content;
                  //printf("uloxzil som ukayatel s hodnotou %d\n",contentArr[j]->integer);
                  j++;
            //printf("COMPARISON1 je %d\n",comparison1 );
            }else
            errorHandler(99);
            

            if (strcmp(currentFunctionKey, "Fwrite") !=0 &&
                strcmp(currentFunctionKey, "Fcopy")  !=0 &&
                strcmp(currentFunctionKey, "Flength")!=0 &&
                strcmp(currentFunctionKey, "Fsort")  !=0 &&
                strcmp(currentFunctionKey, "Ffind")  !=0)
            {
                //teraz potrebujem najst argument s cicslom argsread
                tNodePtr hledam2=searchSymbol(&rootTS, currentFunctionKey);
                if (hledam2 == 0)
                        errorHandler(errSemTypArg);

                tNodePtr currentFunction = searchSymbol(&rootTS, currentFunctionKey);
                for (int i = 1; i</*pvoodne argsRead*/pocetArg;i++)
                {
                    currentFunction=currentFunction->data->nextArg;
                }

                //printf("som na parametri cislo %d a mal by vyhovovat %s\n",argsRead,currentFunction->data->nextArg->data->name);
                comparison2 = currentFunction->data->nextArg->data->type;
                
                    if (comparison1!=comparison2)
                    {
                        printf("copmarizony sa dojebali %d!=%d %s\n",comparison1,comparison2,currentFunction->data->nextArg->data->name);

                        errorHandler(errSemTypArg);
                    }
                    else
                    {//idem ulozit content pre volanie
                        
                    }
            }

            else
                //printf("---Write kontrola preskocena\n");
                if (comparison1<1 || comparison1>4)
                    errorHandler(errSemTypArg);

            //printf("-----------------------------------mal som dostat %d aa bolo %d\n", comparison1,comparison2);

            //printf("hledam dva ma v sebe adresu name %d\n",hledam2->data->localTSadr );


            match (tok, t_var_id);
            //musi sa tu skontrolovat typ vyrazu
            free(key);
        }
        else
        {
            pocetArg++;
            comparison1=tok->type;
            printf("comparison 1 je %d\n",comparison1 );
            if (strcmp(currentFunctionKey, "Fwrite") !=0 &&
                strcmp(currentFunctionKey, "Fcopy")  !=0 &&
                strcmp(currentFunctionKey, "Flength")!=0 &&
                strcmp(currentFunctionKey, "Fsort")  !=0 &&
                strcmp(currentFunctionKey, "Ffind")  !=0)
            {
                //teraz potrebujem najst argument s cicslom argsread
                
                tNodePtr hledam2=searchSymbol(&rootTS, currentFunctionKey);
                if (hledam2 == 0)
                        errorHandler(errSemTypArg);

                tNodePtr currentFunction = searchSymbol(&rootTS, currentFunctionKey);
                for (int i = 1; i</*pvoodne argsRead*/pocetArg;i++)
                {
                    currentFunction=currentFunction->data->nextArg;
                }

                //printf("som na parametri cislo %d a mal by vyhovovat %s\n",argsRead,currentFunction->data->nextArg->data->name);
                comparison2 = currentFunction->data->nextArg->data->type;
                
                    if (comparison1!=comparison2)
                    {
                        printf("copmarizony sa dojebali %d!=%d %s\n",comparison1,comparison2,currentFunction->data->nextArg->data->name);

                        errorHandler(errSemTypArg);
                    }
                    else
                    {//idem ulozit content pre volanie
                        
                    }
            }
            else
                //printf("---Write kontrola preskocena\n");
                if (comparison1<41 || comparison1>44)
                {
                    printf("zly typ\n");
                    errorHandler(errSemTypArg);
                }
                    //printf("ELSE\n");
            //printf("\nTYP JE TU %d a currentFunctionKey je %s\n",tok->type,currentFunctionKey );
            //printf("tu som\n");
            match (tok, tok->type);
            //printf("couldnt may=tch\n");
        }
    }
    else
    {
        printf("syn error in nt_term\n");
        errorHandler(errSyn);
    }

}

void nt_term_list (token tok, char *currentFunctionKey, tContent **contentArr)
{
    if (tok->type == t_var_id || tok->type == t_r_parrent || tok->type == t_expr_int || tok->type == t_expr_dou || tok->type == t_expr_str)
    {

        /////////////////////////////////////////////////////////////////////RULE26
        if (tok->type == t_var_id || tok->type == t_expr_int || tok->type == t_expr_dou || tok->type == t_expr_str)
        {
            nt_term (tok, currentFunctionKey, contentArr);
            //printf("TERM BOL SPRACOVANY\n________________________\n");
            nt_term_more(tok, currentFunctionKey, contentArr);
        }
        /////////////////////////////////////////////////////////////////////RULE25
        else
        {
            
            return;
        }
    }
    else
    {
        printf("syn error in nt_term_list\n");
        errorHandler(errSyn);
    }
}

void nt_term_more (token tok, char *currentFunctionKey, tContent **contentArr)
{
    if (tok->type == t_comma || tok->type == t_r_parrent)
    {
        ////////////////////////////////////////////////////////////////////////RULE27
        if (tok->type == t_comma)
        {
            match(tok,t_comma);
            nt_term_list(tok, currentFunctionKey, contentArr);
        }
        ////////////////////////////////////////////////////////////////////////RULE28
        else
            return;
    }
    else
    {
        printf("syn error in nt_term_more\n");
        errorHandler(errSyn);
    }
}

void nt_param (token tok, bool testOnly, char * currentFunctionKey)
{
    if (tok->type == t_var_id)
    {   
        pocetArg++;
        //pvoodne argsRead++;
        char *key = createKey("V",tok->val_str);

        //printf("PRAMETER key je %s\n",key );
        //char *key = createKey ()

        ////////////////////////////////////////////SPRACOVANIE Parametru
        if (testOnly==false)//ak sa idu nove ukladat
        {  
            saveSymbol (&localTS, key, tok->val_str, t_var_id, pocetArg /*povodne argsRead*/, true);
            //printf("&&&&&&&&&&&&&&&&&&& ulozil som %s s argcountom %d\n",key,argsRead );
            match (tok, t_var_id);
            match (tok, t_colon);
            bool temp=searchGlobalOnly;
            searchGlobalOnly=false;
            nt_type(tok, key);
            searchGlobalOnly=temp;
            //pocetArg++;



            //tuna ide ulozenie ukazatela na parameter dalsi

            tNodePtr currentFunction = searchSymbol(&rootTS, currentFunctionKey);
            if (currentFunction->data->nextArg==NULL)
                currentFunction->data->nextArg=searchSymbol(&localTS, key);
            else
            {
                while (currentFunction->data->nextArg!=NULL)
                {
                    currentFunction=currentFunction->data->nextArg;
                }
                currentFunction->data->nextArg=searchSymbol(&localTS, key);
            }



        }
        else // ak sa len kontroluju z minulej fwd hlavicky
        {
            tNodePtr hledam=searchSymbol(&localTS, key);
            if (hledam==0)
            {
                printf("Error hlavicky sa lisia ty kokot\n");
                errorHandler(errSemTypArg);
            }
            else
            {
                match (tok, t_var_id);
                match (tok, t_colon);
                match (tok, hledam->data->type+25);
            }
        }
        ///////////////////////////////////////////////Premenna ulozena
        //searchSymbol(&rootTS, currentFunctionKey)->data->nextArg=key;
        //printf("podarilo sa \n");
        free(key);
    }
    else
    {
        printf("synerror in nt_param\n");
        errorHandler(errSyn);
    }
}

void nt_param_list (token tok, bool testOnly, char * currentFunctionKey)
{
    if (tok->type == t_r_parrent || tok->type == t_var_id)
    {
        ////////////////////////////////////////////////////////////////////////RULE29
        if (tok->type == t_r_parrent)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////RULE30
        else
        {
            nt_param(tok, testOnly, currentFunctionKey);
            nt_param_more(tok, testOnly, currentFunctionKey);
        }
    }
    else
    {
        printf("synerror in nt_param_list\n");
        errorHandler(errSyn);
    }
}

void nt_param_more (token tok, bool testOnly, char * currentFunctionKey)
{
    if (tok->type == t_r_parrent || tok->type == t_comma)
    {
        ////////////////////////////////////////////////////////////////////////RULE32
        if (tok->type == t_r_parrent)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////RULE31
        else
        {
            match(tok,t_comma);
            nt_param_list(tok, testOnly, currentFunctionKey);
        }

    }
    else
    {
        printf("synerror in nt_param_more\n");
        errorHandler(errSyn);
    }
}

void nt_type (token tok, char * key)
{
    if (tok->type == t_integer || tok->type == t_real ||
        tok->type == t_string  || tok->type == t_boolean)
    {
        tNodePtr hledam;
        if (searchGlobalOnly==true)
        {   
            hledam=searchSymbol(&rootTS,  key);
        }
        else
        {
            hledam=searchSymbol(&localTS, key);
        }

        if (hledam==0)
            errorHandler(errSemDef);
        else
            switch (tok->type)
            {
                //////////////////////////////////////////////////////////RULE33
                case 26:    match(tok,t_integer);
                            if (hledam->data->type==t_var_id)
                                hledam->data->type=1;
                            else
                                if (hledam->data->type==t_fun_id)
                                    hledam->data->type=5;
                            break;
                //////////////////////////////////////////////////////////RULE34
                case 27:    match(tok,t_real);
                            if (hledam->data->type==t_var_id)
                                hledam->data->type=2;
                            else
                                if (hledam->data->type==t_fun_id)
                                    hledam->data->type=6;
                            break;
                //////////////////////////////////////////////////////////RULE35
                case 28:    match(tok,t_string);
                            if (hledam->data->type==t_var_id)
                                hledam->data->type=3;
                            else
                                if (hledam->data->type==t_fun_id)
                                    hledam->data->type=7;
                            break;
                //////////////////////////////////////////////////////////RULE36
                case 29:    match(tok,t_boolean);
                            if (hledam->data->type==t_var_id)
                                hledam->data->type=4;
                            else
                                if (hledam->data->type==t_fun_id)
                                    hledam->data->type=8;
                            break;
            }
    }   
    else
    {
        printf("syntax error in nt_type\n");
        errorHandler(errSyn);
    }

}

void startTable () ///////////////////////////////////////////////Funkcia na rozbehanie tabulky symbolov
{
    init(&rootTS);
    //init(&localTS);
    /*Vetvenie stromu na globalnu a lokalnu polovicu*/
    
    //printf("╔═══════════════════╗\n║rootTS:     %d║\n║localTS:    %d║\n╚═══════════════════╝\n",&rootTS,&localTS);

    if (buildemin()!=0) //////////////////////////////////////////vlozenie vstavanych funkcii
    {
        printf("jebol startTable\n");errorHandler(errInt);
    }
}

/*
           _
          ((`)_.._     ,'-. _..._ _._
           \,'    '-._.-\  '     ` .-'
          .'            /         (
         /             |     _   _ \
        |              \     e   e  |
        ;                     .-.   /
         ;       ',       '-.( '')-'
          '.      |           ;-'
            \    /           /
            /   /-._  __,  7 |
            \  `\  \``  |  | |
             \   \_,\   |  |_,\
          jgs '-`'      \_,\

*/