/***********************parser.c*********************************/
/* Soubor: parser.c - syntaktická/sémantická analýza a generá-  */
/*                    tor inštrukcií.                           */
/* Předmět: Formalní jazyky a překladače (IFJ)                  */
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I                                       */
/* Datum: prosinec 2014                                         */
/* Kódování: UTF-8                                              */
/* Autoři:          Roman Jaška (xjaska00)                      */
/* ASCII Art:       http://ascii.co.uk/art/pig                  */                
/****************************************************************/
/*         ___                                                  */
/*       ',_`""\        .---,     _ __  _ __ __ _ ___  ___ _ __ */
/*          \   :-""``/`    |    | '_ \| '__/ _` / __|/ _ \ '__|*/
/*           `;'     //`\   /    | |_) | | | (_| \__ \  __/ |   */
/*           /   __     |   ('.  | .__/|_|  \__,_|___/\___|_|   */
/*          |_ ./O)\     \  `) \ | |                   ___      */
/*         _/-.    `      `"`  |`|_|                  / __|     */  
/*     .-=; `                  /   `-.            _  | (__      */
/*    /o o \   ,_,           .        '.         (_)  \___|     */
/*    L._._;_.-'           .            `'-.                    */        
/*      `'-.`             '                 `'-.                */
/*          `.         '                        `-._            */
/*            '-._. -'                              '.          */
/*               \                                    `\        */
/*                |                                     \       */
/*                |    |                                 ;   _. */
/*                \    |           |                     |-.((  */
/*                 ;.  \           /    /                |-.`\) */
/*                 | '. ;         /    |                 |(_) ) */
/*                 |   \ \       /`    |                 ;'--'  */
/*                  \   '.\    /`      |                /       */
/*                   |   /`|  ;        \               /        */
/*                   |  |  |  |-._      '.           .'         */
/*                   /  |  |  |__.`'---"_;'-.     .-'           */
/*                  //__/  /  |    .-'``     _.-'`              */
/*                jgs     //__/   //___.--''`                   */
/*                                                              */
/****************************************************************/

/* Premenná rozhodujúca či mám hladať len v globálnej tabuľke */

bool searchGlobalOnly = true;

/* Aktuálny počet forward deklarácií čakajúcich na riadne telá */

int fwdDeclarations = 0;

/* Počítadlo počtu argumentov funkcie pri ich spracovaní v deklarácií **
** a pri následnom volaní                                             */

int pocetArg = 0;

/* Token pre lexikálny analyzátor */

token tok;

bool debug = false;

#include "precedence.c"

/* Funkcia pre prichystanie key na prácu so symbolom */

char * createKey (char * prefix, char * suffix)
{
    /* Alokovanie pamäti pre výsledný kľúč = suffix + prefix */

    char * key = malloc (sizeof (char) * (strlen (suffix) + strlen (prefix)));
    InsertLastMarius (& Smetisko, key);

    /* Pri zlyhaní malloc volám internú chybu (99)         */
    /* Inak spojím prefix a suffix a vrátim výsledný string*/

    if (key == NULL)
    {
        fprintf (stderr, "Could not allocate a new key.\n");
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
    //InsertLastMarius(&Smetisko, newsymbol);
    
    /* Ak alokácia zlyhala alebo predaný name či key je NULL volám chybu 99 */
    /* Ak alokácia prebehla bez problémov a názvy sú neprázdne, pokračujem  */

    if (newsymbol != NULL || key != NULL || name != NULL)
    {
        /*Do dátovej štruktúry uložím dáta predané funkcii*/

        newsymbol -> name        =     name;
        newsymbol -> type        =     type;
        newsymbol -> nextArg     =     NULL;
        newsymbol -> argCount    = argCount;
        newsymbol -> localTSadr  =     NULL;
        newsymbol -> localILadr  =     NULL;
        newsymbol -> initialized =    false;

        /*Uložím do aktuálnej tabulky nový symbol ktorý som si práve pripravil*/
        /*Ak vloženie zlyhá, vraciam internú chybu errInt(99)                 */

        if (insertSymbol (currTS, key, newsymbol) == 0)
        {
            fprintf(stderr, "Could not insert a new symbol.\n");
            errorHandler (errInt);
        }
        return true; 
    }
    else
        {    
            fprintf(stderr, "Could not allocate a new symbol.\n");
            errorHandler (errInt);
        }
    }
    else
        if (errOnRedef == true)
        {   
            fprintf(stderr, "Redefinition of a symbol.\n");
            errorHandler (errSemDef);
            }
    return false;
}

/* Funkcia pre pridanie vstavaných funkcií do globálnej TS */ 

int buildemin ()
{
/*
  _                  _   _     
 | |                | | | |    
 | | ___ _ __   __ _| |_| |__  
 | |/ _ \ '_ \ / _` | __| '_ \ 
 | |  __/ | | | (_| | |_| | | |
 |_|\___|_| |_|\__, |\__|_| |_|
                __/ |          
               |___/           
*/
    saveSymbol (&rootTS, "Flength", "length", sym_fok_int, 1, true);

        tNodePtr      newLocalTS;
        init       (& newLocalTS);
        saveSymbol (& newLocalTS, "placeholder", NULL, 0, 0, true);

        tData Flength               = searchSymbol (& rootTS, "Flength") -> data;
              Flength -> localTSadr = newLocalTS;

        saveSymbol (& newLocalTS, "Vs", "s", sym_var_str, 0, true);

        Flength -> nextArg = searchSymbol (& newLocalTS, "Vs");
        Flength -> nextArg -> data -> nextArg = NULL;
/*
   ___ ___  _ __  _   _ 
  / __/ _ \| '_ \| | | |
 | (_| (_) | |_) | |_| |
  \___\___/| .__/ \__, |
           | |     __/ |
           |_|    |___/ 

*/
    saveSymbol (&rootTS, "Fcopy",   "copy",   sym_fok_str, 3, true);
        tNodePtr      newLocalTS2;
        init       (& newLocalTS2);
        saveSymbol (& newLocalTS2, "placeholder", NULL, 0, 0, true);

        tData Fcopy               = searchSymbol (& rootTS, "Fcopy") -> data;
              Fcopy -> localTSadr = newLocalTS2;

        saveSymbol (& newLocalTS2, "Vs", "s", sym_var_str, 0, true);

        Fcopy -> nextArg = searchSymbol (& newLocalTS2, "Vs");

        saveSymbol   (& newLocalTS2, "Vi", "i", sym_var_int, 0, true);
        Fcopy -> nextArg -> data -> nextArg = searchSymbol (& newLocalTS2, "Vi");
        saveSymbol   (& newLocalTS2, "Vn", "n", sym_var_int, 0, true);
        searchSymbol (& newLocalTS2, "Vi") -> data -> nextArg = searchSymbol (& newLocalTS2, "Vn");
        searchSymbol (& newLocalTS2, "Vn") -> data -> nextArg = NULL;
/*
   __ _           _ 
  / _(_)         | |
 | |_ _ _ __   __| |
 |  _| | '_ \ / _` |
 | | | | | | | (_| |
 |_| |_|_| |_|\__,_|

*/                                        
    saveSymbol (&rootTS, "Ffind",   "find",   sym_fok_int, 2, true);
        tNodePtr      newLocalTS3;
        init       (& newLocalTS3);
        saveSymbol (& newLocalTS3, "placeholder", NULL, 0, 0, true);

        tData Ffind               = searchSymbol (& rootTS, "Ffind") -> data;
              Ffind -> localTSadr = newLocalTS3;

        saveSymbol (& newLocalTS3, "Vs", "s", sym_var_str, 0, true);

        Ffind -> nextArg = searchSymbol (& newLocalTS3, "Vs");
        saveSymbol   (& newLocalTS3, "Vsearch", "search", sym_var_str, 0, true);
        Ffind -> nextArg -> data -> nextArg = searchSymbol (& newLocalTS3, "Vsearch");
        searchSymbol (& newLocalTS3, "Vsearch") -> data -> nextArg = NULL;

/*               _   
                | |  
  ___  ___  _ __| |_ 
 / __|/ _ \| '__| __|
 \__ \ (_) | |  | |_ 
 |___/\___/|_|   \__|

*/
    saveSymbol (&rootTS, "Fsort",   "sort",   sym_fok_str, 1, true);
        tNodePtr newLocalTS4;
        init  (& newLocalTS4);
        saveSymbol (& newLocalTS4, "placeholder", "testname", 0, 0, true);

        tData Fsort               = searchSymbol (& rootTS, "Fsort") -> data;
              Fsort -> localTSadr = newLocalTS4;

        saveSymbol (& newLocalTS4, "Vs", "s", sym_var_str, 0, true);
        Fsort -> nextArg = searchSymbol (& newLocalTS4, "Vs");
        Fsort -> nextArg -> data -> nextArg = NULL;

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
        default: fprintf(stderr, "Invalid input.\n");
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
    }
    else
        {
            fprintf(stderr, "Terminal does not match. Expected t_%d, received t_%d.\n", terminal, tok -> type);
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
            {   
                fprintf(stderr, "Ended with %d pending forward declaration(s).\n", fwdDeclarations);
                errorHandler (errSemDef);
            }   
        }
    }
    else
    {    
        fprintf (stderr, "Syntax error in nt_program. Received t_%d.\n", tok -> type);
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
        fprintf (stderr, "Syntax error in nt_var_def_block. Received t_%d.\n", tok -> type);
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
        fprintf (stderr, "Syntax error in nt_var_def_list. Received t_%d.\n", tok -> type);
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


        tInsList * currIL =   (localIL == NULL) ? &IL : localIL;
        insertInst (currIL, I_VAR, currentVar->data, NULL, NULL);

        /* Vypísanie práve vloženej inštrukcie pre debug  */

        if (debug == true)
            printf ("\n%sNew Instruction | %p | I_VAR | %p | NULL | NULL |%s\n", KYEL, (void *) currIL, (void *) currentVar->data, KNRM);

    }
    else
    {    
        fprintf (stderr, "Syntax error in nt_var_def. Received t_%d.\n", tok -> type);
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
                //free (funVarKey);
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
                    fprintf (stderr, "Attempted redefinition of function %s.\n", key);
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
                //printf("uloil som %d\n",searchSymbol(&rootTS, key)->data->argCount );

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
                    default:    fprintf (stderr, "Type mismatch in %s.\n", key);
                                errorHandler (errSemTypArg);
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

            //free (key);
            //free (rememberVarName);

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
        fprintf (stderr, "Syntax error in nt_fun_def_list, received %d\n", tok -> type);
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

            tInsList * newLocalIL = malloc (sizeof(tInsList));
            InsertLastMarius (& Smetisko, newLocalIL);
            InitList (newLocalIL);

            /* Do "hledam" si uložím symbol aktuálnej funkcie */

            tNodePtr hledam = searchSymbol(&rootTS, key);

            hledam->data->localILadr = newLocalIL;

            //localIL=newLocalILptr;
            //printf("localIL je teraz %p\n",&*localIL );

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

            tInsList * revert = localIL;
            localIL = newLocalIL;

            nt_body (tok);

            localIL = revert;
            /* Akonáhle sa dostanem von z tela funkcie, opäť ma zaujímajú len **
            ** globálne premenné a funkcie                                    */

            searchGlobalOnly = true;


            /* Do symbolu funkcie uložím ukazateľ na jej lokálnu tabuľku sym- **
            ** bolov a vynuluje globálny ukazateľ                             */

             //(hledam -> data -> localTSadr) = localTS;
            //localIL = NULL;

            //disposeTable(&localTS);
        }
    }

    /* Prípad keď prišiel nesprávny terminál */

    else
    {
        fprintf (stderr, "Syntax error in nt_fun_body. Received t_%d.\n", tok -> type);
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
        fprintf (stderr, "Syntax error in nt_body. Received t_%d.\n", tok -> type);
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
        fprintf (stderr, "Syntax error in nt_main. Received t_%d.\n", tok -> type);
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
            if (debug == true) printf("stmtMustntBeEmpty je %d\n",stmtMustntBeEmpty );
            if (stmtMustntBeEmpty==true)
            {
                fprintf(stderr, "Expected another statement after semicolon.\n");
                errorHandler(errSyn);
            }

            //Instrukcia NOP

            /* Ak je aktuálny localIL NULL, znamená že idem   **
            ** vkladať globálnu inštrukciu takže currIL si    **
            ** nastavím na IL a v opačnom prípade ideme do    **
            ** aktuálneho lokálneho listu na ktorý ukazuje    **
            ** localIL.                                       */

            tInsList * currIL =   (localIL == NULL) ? &IL : localIL;
            insertInst (currIL, I_NOP, NULL, NULL, NULL);

            /* Vypísanie práve vloženej inštrukcie pre debug  */

            if (debug == true)
                printf ("\n%sNew Instruction | %p | I_NOP | NULL | NULL | NULL |%s\n", KYEL, (void *) currIL, KNRM);

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
        fprintf (stderr, "Syntax error in nt_stmt_list. Received t_%d.\n", tok -> type);
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
            if (debug == true) printf("NASTAVIL TRU\n");
            stmtMustntBeEmpty=true;

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
        fprintf (stderr, "Syntax error in nt_stmt_more. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }
}
int j = 0;
void nt_stmt (token tok)
{   
    stmtMustntBeEmpty=false;
    
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
        int intype;
        int fificounter=0;
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
                                hledam = searchSymbol (&rootTS, key);

                            if (hledam!=0)
                            {
                                if (hledam -> data -> type == t_var_id || (hledam->data->type >= 1 && hledam->data->type <= 4))
                                {
                                    match (tok,t_var_id);
                                }
                            }
                            else
                            {
                                fprintf (stderr, "Undefined variable '%s' in nt_stmt.\n", hledam -> data -> name);
                                errorHandler (errSemDef);
                            }
                            match     (tok,t_assign);

                            //SEMANTICKA KONTROLA
                            int semControlVar = nt_assign (tok);
                            //printf("Is the variable %s initialized? %d\n",hledam->data->name, hledam->data->initialized);
                            hledam -> data -> initialized = true;
                            //printf("Is the variable %s initialized? %d\n",hledam->data->name, hledam->data->initialized);

                            if (semControlVar != hledam->data->type+4 && semControlVar != hledam->data->type+12 && semControlVar != hledam->data->type)
                            {
                                fprintf (stderr, "Type mismatch while trying to assign a value to '%s' in nt_stmt.\n", hledam -> data -> name);
                                errorHandler (errSemTypArg);
                            }

                            //Tu je rozhodovanie ktoru instrukciu mam zavolat podla typu precedencie
                            switch (semControlVar)
                            {
                                case sym_fun_int:
                                case sym_fok_int:
                                case sym_var_int:   intype = I_ASGNI; break;
                                case sym_fun_str:
                                case sym_fok_str:
                                case sym_var_str:   intype = I_ASGNS; break;
                                case sym_fun_rea:
                                case sym_fok_rea:
                                case sym_var_rea:   intype = I_ASGNR; break;
                                case sym_fun_boo:
                                case sym_fok_boo:
                                case sym_var_boo:   intype = I_ASGNB; break;

                                default:    errorHandler(errInt);
                                            break;
                            }


                            /* Ak je aktuálny localIL NULL, znamená že idem   **
                            ** vkladať globálnu inštrukciu takže currIL si    **
                            ** nastavím na IL a v opačnom prípade ideme do    **
                            ** aktuálneho lokálneho listu na ktorý ukazuje    **
                            ** localIL.                                       */

                            currIL =   (localIL == NULL) ? &IL : localIL;
                            insertInst (currIL, intype, NULL, NULL, hledam -> data);

                            /* Vypísanie práve vloženej inštrukcie pre debug  */

                            if (debug == true)
                                printf ("\n%sNew Instruction | %p | I_%d | NULL | NULL | %p |%s\n", KYEL, (void *) currIL, intype, (void *) & hledam -> data, KNRM);

                            //free (key);
                            break;                
            //////////////////////////////////////////////////////////////RULE19               
            case t_if:      match   (tok,t_if);
                            precedenceResult = precedenceParser();

                            if (precedenceResult != t_expr_boo)
                            {   
                                fprintf (stderr, "Expression was not boolean.\n");
                                errorHandler(errSemTypArg);
                            }

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
                            InsertLastMarius (& Smetisko, thenIL);
                            InitList   (thenIL);
                            revert    = localIL;
                            localIL   =  thenIL;
                            nt_body       (tok);
                            localIL   =  revert;

                            match (tok, t_else);

                            tInsList  * elseIL = malloc (sizeof (tInsList));
                            InsertLastMarius (& Smetisko, elseIL);
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
                                printf ("\n%sNew Instruction | %p | I_IF | %p | %p | NULL |%s\n", KYEL, (void *) currIL, (void *) thenIL, (void *) elseIL, KNRM);
                            
                            break;
            //////////////////////////////////////////////////////////////RULE20
            case t_while:   match (tok,t_while);
                            
                            //odpamatam aktualnu insytrukciu
                            //fifi pocitadlo na 0
                            tListItem * remember = (localIL==NULL) ? (&IL)->last : localIL->last;

                            precedenceResult = precedenceParser();
                            // ???? fifi pocitadlo 
                            fificounter = numberOfExprInsts;
                            if (precedenceResult != t_expr_boo)
                            {   
                                fprintf (stderr, "Expression was not boolean.\n");
                                errorHandler(errSemTypArg);
                            }

                            terminalis (precedenceResult, NULL); //Debug výpis
                            
                            /* Vytvorenie novej tabuľky pre inštrukcie tela   **
                            ** while funguje rovnako ako pri then, či else.   */

                            match (tok,t_do);

                            tInsList * whileIL = malloc (sizeof (tInsList));
                            InsertLastMarius (& Smetisko, whileIL);
                            InitList  (whileIL);
                            revert   =  localIL;
                            localIL  =  whileIL;
                            
                            nt_body       (tok);

                            //idem dat podmienku este
                            if (revert!=NULL)
                                Replicator (revert,localIL,remember,fificounter);
                            else
                                Replicator (&IL,localIL,remember,fificounter);


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
                                printf ("\n%sNew Instruction | %p | I_WHILE | %p | NULL | NULL |%s\n", KYEL, (void *) currIL, (void *) whileIL, KNRM);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE21
            case t_readln:  match (tok,t_readln);
                            match (tok,t_l_parrent);
                            //Kontrola či premenná už je v tabulke
                            key = createKey ("V", tok->val_str);

                            currTS = (searchGlobalOnly == true) ? &rootTS : &localTS;
                            hledam = searchSymbol (currTS, key);
                            if (hledam == 0 && searchGlobalOnly == false)
                                hledam = searchSymbol(&rootTS, key);

                            if (hledam != 0)
                            {    
                                if (hledam->data->type==t_var_id || (hledam->data->type >= 1 && hledam->data->type <= 3))//toto reaguje len na typove premenne 
                                {
                                    match (tok,t_var_id);
                                }
                            }
                            else
                            {
                                fprintf (stderr, "Undefined variable '%s' in nt_stmt readln.\n", tok -> val_str);
                                errorHandler (errSemDef);
                                return;
                            }

                            //SEMANTICKA KONTROLA
                            if (hledam->data->type < 1 || hledam->data->type > 3)
                            {
                                fprintf (stderr, "Incompatible type of readln variable.\n");
                                errorHandler (errSemTypArg);
                            }
                            match (tok,t_r_parrent);

                            switch (hledam -> data -> type)
                            {
                                case 1: intype = I_READI; break;
                                case 2: intype = I_READR; break;
                                case 3: intype = I_READS; break;
                            }

                            /* Ak je aktuálny localIL NULL, znamená že idem   **
                            ** vkladať globálnu inštrukciu takže currIL si    **
                            ** nastavím na IL a v opačnom prípade ideme do    **
                            ** aktuálneho lokálneho listu na ktorý ukazuje    **
                            ** localIL.                                       */

                            currIL =   (localIL == NULL) ? &IL : localIL;
                            insertInst (currIL, intype, NULL, NULL, hledam -> data);

                            /* Vypísanie práve vloženej inštrukcie pre debug  */

                            if (debug == true)
                                printf ("\n%sNew Instruction | %p | I_READ_%d | NULL | NULL | %p |%s\n", KYEL, (void *) currIL, intype, (void *) & hledam -> data, KNRM);

                            //free (key);
                            break;
            //////////////////////////////////////////////////////////////RULE22
            case t_write:   match (tok, t_write);
                            match (tok, t_l_parrent);
                            tData ** dataArr = (tData**) malloc (sizeof (tData*) * 10);
                            InsertLastMarius (& Smetisko, dataArr);

                            nt_term_list (tok, "Fwrite", NULL, dataArr);
                            
                            pocetArg = 0;
                            match (tok,t_r_parrent);
                            int * jp = malloc(sizeof(int));
                            InsertLastMarius (& Smetisko, jp);
                            *jp=j;

                            /* Ak je aktuálny localIL NULL, znamená že idem   **
                            ** vkladať globálnu inštrukciu takže currIL si    **
                            ** nastavím na IL a v opačnom prípade ideme do    **
                            ** aktuálneho lokálneho listu na ktorý ukazuje    **
                            ** localIL.                                       */

                            currIL =   (localIL == NULL) ? &IL : localIL;
                            insertInst (currIL, I_WRITE, jp, dataArr, NULL);
                            j=0;//toto bolo povodne pred pocetarg
                            /* Vypísanie práve vloženej inštrukcie pre debug  */

                            if (debug == true)
                                printf ("\n%sNew Instruction | %p | I_WRITE | %i | %p | NULL |%s\n", KYEL, (void *) currIL, *(int *) jp, (void *) dataArr, KNRM);

                            //free (key);
                            
                            break;
        }
    }    
    else
    {
        fprintf (stderr, "Syntax error in nt_stmt. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }
}



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

            int precedenceResult = precedenceParser ();
            terminalis (precedenceResult, NULL);
            switch (precedenceResult)
            {
                case t_expr_int: return sym_var_int;break;
                case t_expr_str: return sym_var_str;break;
                case t_expr_dou: return sym_var_rea;break;
                case t_expr_boo: return sym_var_boo;break;
                default: errorHandler (errSyn);
                         break;
            }
        }
        //////////////////////////////////////////////////////////////////RULE24
        else
        {
            char * key = createKey ("F", tok -> val_str);
            char * key2 = createKey ("V", tok -> val_str);
            tNodePtr hledam = searchSymbol (&rootTS, key);
            //printf("funkcia je %s\n",key );
            
            if (hledam == 0)
            {
                fprintf (stderr, "Semantic error in nt_assign. No function called '%s'.\n", tok -> val_str);
                errorHandler(errSemDef);
            }

            match (tok, t_fun_id);
            match (tok, t_l_parrent);


            tContent * selfVarCon;
            if (strcmp(key,"Fcopy")   != 0 &&
                strcmp(key,"Flength") != 0 &&
                strcmp(key,"Ffind")   != 0 &&
                strcmp(key,"Fsort")   != 0  )
            selfVarCon = &(searchSymbol(&hledam->data->localTSadr, key2)->data->content);
            
            //free (key2);

            tContent ** contentArr = (tContent**) malloc (sizeof (tContent*) * 10);
            InsertLastMarius (& Smetisko, contentArr);
            nt_term_list (tok, key, contentArr, NULL);
            pocetArg = 0;
            
            //termy su overene idem ich nahradit
            //printf("\nhledam->data->argCount je %d\n",hledam->data->argCount);
            
            
            j = 0;

            match (tok, t_r_parrent);


            /* Ak je aktuálny localIL NULL, znamená že idem   **
            ** vkladať globálnu inštrukciu takže currIL si    **
            ** nastavím na IL a v opačnom prípade ideme do    **
            ** aktuálneho lokálneho listu na ktorý ukazuje    **
            ** localIL.                                       */

            tInsList * currIL =   (localIL == NULL) ? &IL : localIL;
            if (strcmp(key,"Fcopy")   == 0 ||
                strcmp(key,"Flength") == 0 ||
                strcmp(key,"Ffind")   == 0 ||
                strcmp(key,"Fsort")   == 0  )
            selfVarCon=NULL;
            
            insertInst (currIL, I_FCE, hledam -> data, contentArr, selfVarCon);

            /* Vypísanie práve vloženej inštrukcie pre debug  */

            if (debug == true)
                printf ("\n%sNew Instruction | %p | I_FCE | %p | %p | %p |%s\n", KYEL, (void *) currIL, (void *) & hledam -> data, (void *) contentArr, (void *) selfVarCon, KNRM);

            //free(key);
            return hledam->data->type;

        }
    }
    else
    {
        fprintf (stderr, "Syntax error in nt_assign. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }
    return -1;
}

void nt_term (token tok, char *currentFunctionKey, tContent **contentArr, tData **dataArr)
{
    if (tok -> type == t_var_id   ||
        tok -> type == t_expr_int ||
        tok -> type == t_expr_dou ||
        tok -> type == t_expr_str ||
        tok -> type == t_expr_boo  )
    {
        int comparison1;
        int comparison2;
        if (tok->type == t_var_id)
        {
            pocetArg++;

            char * key = createKey("V",tok->val_str);


            tNodePtr hledam = NULL;
            if (searchGlobalOnly==1)
                hledam=(searchSymbol(&rootTS, key));
            else
            {
                hledam=(searchSymbol(&localTS, key));   
                if (hledam==0)
                    hledam=(searchSymbol(&rootTS, key));    
            }
            
            if (hledam != 0)
            {   
                tContent * currCon = &(hledam -> data -> content);
                comparison1  = hledam -> data -> type;
                if (contentArr!=NULL)
                    {
                        contentArr[j]= currCon;
                        if (debug == true) printf("Do contentArr[%d].integer som ulozil %d\n",j,(*contentArr[j]).integer);
                    }
                if (dataArr!=NULL)
                    {
                        if (debug == true) printf("davam do dataru\n");
                        dataArr[j]=&(hledam -> data);
                    }


                
                j++;
            //printf("COMPARISON1 je %d\n",comparison1 );
            }
            else
            {   
                fprintf (stderr, "Variable not found.\n");
                errorHandler (errSemDef);
            }
            

            if (strcmp(currentFunctionKey, "Fwrite") != 0/* &&
                strcmp(currentFunctionKey, "Fcopy")  != 0 &&
                strcmp(currentFunctionKey, "Flength")!= 0 &&
                strcmp(currentFunctionKey, "Fsort")  != 0 &&
                strcmp(currentFunctionKey, "Ffind")  != 0  */)
            {
                //teraz potrebujem najst argument s cicslom argsread
                tNodePtr hledam2 = searchSymbol (&rootTS, currentFunctionKey);
                if (hledam2 == 0)
                {
                    fprintf (stderr, "Argument mismatch.\n");
                    errorHandler (errSemTypArg);
                }

                tNodePtr currentFunction = searchSymbol (&rootTS, currentFunctionKey);
                for (int i = 1; i < pocetArg; i++)
                {
                    currentFunction = currentFunction -> data -> nextArg;
                }

                //printf("som na parametri cislo %d a mal by vyhovovat %s\n",argsRead,currentFunction->data->nextArg->data->name);
                comparison2 = currentFunction -> data -> nextArg -> data -> type;
                
                    if (comparison1 != comparison2)
                    {
                        fprintf (stderr, "Tyme mismatch in nt_assign\n");
                        errorHandler (errSemTypArg);
                    }
            }
            else
                //printf("---Write kontrola preskocena\n");
                if (comparison1 < 1 || comparison1 > 4)
                {

                    fprintf (stderr, "Type mismatch.\n");
                    errorHandler (errSemTypArg);
                }
            match (tok, t_var_id);
            //free (key);
        }
        else
        {
            pocetArg++;
            comparison1=tok->type;

            if (strcmp(currentFunctionKey, "Fwrite") != 0/* &&
                strcmp(currentFunctionKey, "Fcopy")  != 0 &&
                strcmp(currentFunctionKey, "Flength")!= 0 &&
                strcmp(currentFunctionKey, "Fsort")  != 0 &&
                strcmp(currentFunctionKey, "Ffind")  != 0  */)
            {
                //teraz potrebujem najst argument s cicslom argsread
                
                tNodePtr hledam2 = searchSymbol (&rootTS, currentFunctionKey);
                if (hledam2 == 0)
                {
                    fprintf (stderr, "Argument mismatch.\n");
                    errorHandler(errSemTypArg);
                }

                tNodePtr currentFunction = searchSymbol (&rootTS, currentFunctionKey);
                for (int i = 1; i < pocetArg; i++)
                {
                    currentFunction = currentFunction -> data -> nextArg;
                }

                comparison2 = currentFunction -> data -> nextArg -> data -> type;
                switch (comparison2)
                    {
                        case sym_var_int: comparison2 = t_expr_int; break;
                        case sym_var_str: comparison2 = t_expr_str; break;
                        case sym_var_boo: comparison2 = t_expr_boo; break;
                        case sym_var_rea: comparison2 = t_expr_dou; break;
                    }
                
                    if (comparison1 != comparison2)
                    {
                        fprintf (stderr, "Tyme mismatch in nt_assign\n");
                        //printf("comparison2 je %d\n",comparison2);
                        //printf("comparison1 je %d\n",comparison1);
                        errorHandler (errSemTypArg);
                    }
                    else
                    {


                    //idem ulozit content pre volanie
                        
                        char * randomKey = randstring(20);
                        if (debug == true) printf("randomkey je %s\n",randomKey );

                        if (searchSymbol(&rootTS, randomKey)!=0)
                        {
                            fprintf (stderr, "Randomkey error\n");
                            errorHandler (errInt);
                        }

                        saveSymbol (&rootTS, randomKey, NULL, tok -> type, 0, true);

                        tData * currData = &(searchSymbol(&rootTS, randomKey) -> data);
                        switch (tok -> type)
                        {
                            case t_expr_int:    (*currData) -> content.integer = tok -> val_int;
                                                break;
                            case t_expr_str:    (*currData) -> content.string  = tok -> val_str;
                                                break;
                            case t_expr_dou:    (*currData) -> content.real    = tok -> val_flo;
                                                break;
                            case t_expr_boo:    (*currData) -> content.boolean = tok -> val_int;
                                                break;
                        }

                        tContent * currCon = &((*currData) -> content);
                        contentArr[j]      = currCon;
                        //printf("Do contentArr[%d].integer som ulozil kokotne cislo %d\n",j,(*contentArr[j]).integer);
                        j++;
                        //printf("COMPARISON1 je %d\n",comparison1 );
                        //free (randomKey);
                    }
            }
            else
                //printf("---Write kontrola preskocena\n");
                if (comparison1 < 41 || comparison1 > 44)
                {

                    fprintf (stderr, "Type mismatch.\n");
                    errorHandler(errSemTypArg);
                }
                else////////////NEVZKOKOTILO CHYBU IDEMM ULOZIT LITERAL DO TABULKZ
                    {
                                            //idem ulozit content pre volanie
                        
                        char * randomKey2 = randstring(20);
                        if (debug == true) printf("randomkey2 je %s\n",randomKey2 );

                        if (searchSymbol(&rootTS, randomKey2)!=0)
                        {
                            fprintf (stderr, "Randomkey2 error\n");
                            errorHandler (errInt);
                        }

                        saveSymbol (&rootTS, randomKey2, NULL, tok -> type, 0, true);

                        tData * currData2 = &(searchSymbol(&rootTS, randomKey2) -> data);
                        switch (tok -> type)
                        {
                            case t_expr_int:    (*currData2) -> content.integer = tok -> val_int;
                                                break;
                            case t_expr_str:    (*currData2) -> content.string  = tok -> val_str;
                                                break;
                            case t_expr_dou:    (*currData2) -> content.real    = tok -> val_flo;
                                                break;
                            case t_expr_boo:    (*currData2) -> content.boolean = tok -> val_int;
                                                break;
                        }

                        //tData * currDat = &((*currData2) -> content);
                        dataArr[j]       = currData2;
                        //printf("Do contentArr[%d].integer som ulozil kokotne cislo %d\n",j,(*contentArr[j]).integer);
                        j++;
                        //printf("COMPARISON1 je %d\n",comparison1 );
                        //free (randomKey);

                    }
            match (tok, tok->type);
        }
    }
    else
    {
        fprintf (stderr, "Syntax error in nt_term. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }

}

void nt_term_list (token tok, char * currentFunctionKey, tContent * * contentArr, tData **dataArr)
{
    if (tok -> type == t_var_id   ||
        tok -> type == t_expr_int ||
        tok -> type == t_expr_dou ||
        tok -> type == t_expr_str ||
        tok -> type == t_r_parrent )
    {

        /////////////////////////////////////////////////////////////////////RULE26
        if (tok -> type == t_var_id   ||
            tok -> type == t_expr_int ||
            tok -> type == t_expr_dou ||
            tok -> type == t_expr_str  )
        {
            nt_term      (tok, currentFunctionKey, contentArr, dataArr);
            nt_term_more (tok, currentFunctionKey, contentArr, dataArr);
        }
        /////////////////////////////////////////////////////////////////////RULE25
        else
            return;
    }
    else
    {
        fprintf (stderr, "Syntax error in nt_term_list. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }
}

void nt_term_more (token tok, char * currentFunctionKey, tContent * * contentArr, tData **dataArr)
{
    if (tok -> type == t_comma    ||
        tok -> type == t_r_parrent )
    {
        ////////////////////////////////////////////////////////////////////////RULE27
        if (tok -> type == t_comma)
        {
            match        (tok, t_comma);
            nt_term_list (tok, currentFunctionKey, contentArr, dataArr);
        }
        ////////////////////////////////////////////////////////////////////////RULE28
        else
            return;
    }
    else
    {
        fprintf (stderr, "Syntax error in nt_term_more. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }
}

void nt_param (token tok, bool testOnly, char * currentFunctionKey)
{
    if (tok->type == t_var_id)
    {   
        pocetArg++;
        char * key = createKey ("V", tok -> val_str);
        ////////////////////////////////////////////SPRACOVANIE Parametru
        if (testOnly == false)//ak sa idu nove ukladat
        {  
            saveSymbol (&localTS, key, tok -> val_str, t_var_id, pocetArg, true);
            //printf("&&&&&&&&&&&&&&&&&&& ulozil som %s s argcountom %d\n",key,argsRead );
            match (tok, t_var_id);
            match (tok, t_colon);
            bool temp = searchGlobalOnly;
            searchGlobalOnly = false;
            nt_type (tok, key);
            searchGlobalOnly = temp;
            
            //tuna ide ulozenie ukazatela na parameter dalsi

            tNodePtr currentFunction = searchSymbol (&rootTS, currentFunctionKey);
            if (currentFunction -> data -> nextArg == NULL)
                currentFunction -> data -> nextArg = searchSymbol (&localTS, key);
            else
            {
                while (currentFunction -> data -> nextArg != NULL)
                {
                    currentFunction = currentFunction -> data -> nextArg;
                }
                currentFunction -> data -> nextArg = searchSymbol (&localTS, key);
            }
        }
        else // ak sa len kontroluju z minulej fwd hlavicky
        {
            tNodePtr hledam = searchSymbol (&localTS, key);
            if (hledam == 0)
            {
                fprintf (stderr, "Different function head.\n");
                errorHandler (errSemTypArg);
            }
            else
            {
                match (tok, t_var_id);
                match (tok, t_colon);
                match (tok, hledam -> data -> type + 25);
            }
        }
        //free (key);
    }
    else
    {
        fprintf (stderr, "Syntax error in nt_param. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }
}

void nt_param_list (token tok, bool testOnly, char * currentFunctionKey)
{
    if (tok -> type == t_r_parrent ||
        tok -> type == t_var_id     )
    {
        ////////////////////////////////////////////////////////////////////////RULE29
        if (tok -> type == t_r_parrent)
            return;
        ////////////////////////////////////////////////////////////////////////RULE30
        else
        {
            nt_param      (tok, testOnly, currentFunctionKey);
            nt_param_more (tok, testOnly, currentFunctionKey);
        }
    }
    else
    {
        fprintf (stderr, "Syntax error in nt_param_list. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }
}

void nt_param_more (token tok, bool testOnly, char * currentFunctionKey)
{
    if (tok -> type == t_r_parrent ||
        tok -> type == t_comma      )
    {
        ////////////////////////////////////////////////////////////////////////RULE32
        if (tok -> type == t_r_parrent)
            return;
        ////////////////////////////////////////////////////////////////////////RULE31
        else
        {
            match (tok, t_comma);
            nt_param_list (tok, testOnly, currentFunctionKey);
        }

    }
    else
    {
        fprintf (stderr, "Syntax error in nt_param_more. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }
}

void nt_type (token tok, char * key)
{
    if (tok -> type == t_integer ||
        tok -> type == t_real    ||
        tok -> type == t_string  ||
        tok -> type == t_boolean  )
    {
        tNodePtr hledam;
        if (searchGlobalOnly == true)
            hledam = searchSymbol (&rootTS, key);
        else
            hledam = searchSymbol (&localTS, key);

        if (hledam == 0)
        {
            fprintf (stderr, "Invalid variable.\n");
            errorHandler (errSemDef);
        }

        else
            switch (tok -> type)
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
        fprintf (stderr, "Syntax error in nt_type. Received t_%d.\n", tok -> type);
        errorHandler (errSyn);
    }

}

void startTable () ///////////////////////////////////////////////Funkcia na rozbehanie tabulky symbolov
{
    init (&rootTS);
    if (buildemin () != 0) //////////////////////////////////////////vlozenie vstavanych funkcii
    {
        fprintf (stderr, "Error in startTable.\n");
        errorHandler (errInt);
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