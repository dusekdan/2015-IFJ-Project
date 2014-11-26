////////////////////Roman Jaška
////////////////////rozrobené
#include "parser.h"
#include "errorHandler.c"

tNodePtr localTS;//ukazatel na localnu tabulku
bool searchGlobalOnly = true;//ci ukladam do globalnej tabulky
int fwdDeclarations=0;//pocet fwd
int pocetArg = 0;
bool debug=false;
char * textPreTerminalis = NULL;


/* Funkcia pre prichystanie key na prácu so symbolom */

char *createKey (char prefix[1], char *suffix)
{
    /* Alokovanie pamäti pre výsledný kľúč */

    char *key = malloc(sizeof(char)*(strlen(suffix)+1));

    /* Pri zlyhaní malloc volám internú chybu (99)         */
    /* Inak spojím prefix a suffix a vrátim výsledný string*/

    if (key == NULL)
        errorHandler(errInt);
    else
    {
        memset (key, 0, strlen(key));
        strcpy (key, prefix);
        strcat (key, suffix);
    }
    return (key);
}

/* Funkcia ktorá vytvorí nový symbol podľa parametrov ak ešte neexistuje */

bool saveSymbol (tNodePtr *currTS, char *key, char *name, int type, int argCount, bool errOnRedef)
{
    /* Otestujem či už náhodou neexistuje daný symbol */

    if (searchSymbol(currTS, key) == 0)
    {

    /* Alokujem si miesto pre data pre nový symbol v tabulke */

    tData newsymbol = malloc(sizeof(struct tData));
    
    /* Ak alokácia zlyhala alebo predaný name či key je NULL volám chybu 99 */
    /* Ak alokácia prebehla bez problémov a názvy sú neprázdne, pokračujem  */

    if (newsymbol != NULL || key != NULL || name != NULL)
    {
        /*Do dátovej štruktúry uložím dáta predané funkcii*/

        newsymbol->name     = name;
        newsymbol->type     = type;
        newsymbol->argCount = argCount;

        /*Uložím do aktuálnej tabulky nový symbol ktorý som si práve pripravil*/
        /*Ak vloženie zlyhá, vraciam internú chybu errInt(99)                 */

        if (insertSymbol (currTS, key, newsymbol) == 0)
            errorHandler (errInt);

        return true; 
    }
        else
        {    
            errorHandler (errInt);
        }
    }
    else
        if (errOnRedef == true)
            errorHandler(errSemDef);
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

/* Funkcia simulujúca činnnosť Lex analyzátora */

void gib_tok (token tok)
{
    int i=0;
    char *m;    
    scanf("%d",&i);
    if (i==t_var_id || i == t_fun_id)
    {
        scanf("%ms",&m);
        tok->val_str=m;
    }
    if (i ==t_expr_val)
    {
        scanf("%ms",&m);
        textPreTerminalis=m;
    }  
    tok->type=i;
}

/* Funkcia vypisujúca názov terminálu z jeho typu */

void terminalis (int terminal, token tok)
{
    switch (terminal)
    {
        case 1: printf("var\n");break;
        case 2: printf(": ");break;
        case 3: printf(";\n");break;
        case 4: printf("( ");break;
        case 5: printf(") ");break;
        case 6: printf("function ");break;
        case 7: printf("forward ");break;
        case 8: printf("begin\n");break;
        case 9: printf("end\n");break;
        case 10: printf(".\n");break;
        case 11: printf(", ");break;
        case 12: printf(":= ");break;
        case 13: printf("if ");break;
        case 14: printf("then\n");break;
        case 15: printf("else\n");break;
        case 16: printf("while ");break;
        case 17: printf("do ");break;
        case 18: printf("readln ");break;
        case 19: printf("write ");break;
        case 20: printf("%s ",tok->val_str);break;
        case 21: printf("expr ");break;
        case 22: printf("%s ",tok->val_str);break;
        //case 23: printf("term ");break;
        //case 24: printf("param ");break;
        //case 25: printf("read_id ");break;
        case 26: printf("integer ");break;
        case 27: printf("real ");break;
        case 28: printf("string ");break;
        case 29: printf("boolean ");break;
        case 30: printf("$\n");break;
        case 41: printf("'%s' ", textPreTerminalis );break;
        default: printf("error input=%d\n", terminal);
    }
}

/* Funkcia porovnávajúca spracovaný terminál so vstupom, následne žiada token */

void match (token tok, int terminal)
{
    if (terminal!=30)
    {
        gib_tok (tok);
        terminalis (terminal, tok);
    }
    else printf("Reach the end of file.\n");
}


void nt_program (token tok)
{
    //////////////////////////////////////////////////////////////////////RULE 1
    if (tok->type == t_var || tok->type == t_function || tok->type == t_begin)
    {   
        if (tok->type == t_var)
        {
            nt_var_def_block (tok);
        }
        if (tok->type == t_function)
        {
            nt_fun_def_list (tok);
        }
        if (tok->type == t_begin)
        {
            nt_main (tok);
            if (fwdDeclarations!=0)
                errorHandler(errSemDef);
        }
    }
    else
    {    
        printf("nt_program\n");
        errorHandler(errSyn);
    }
}

void nt_var_def_block (token tok)
{
    if (tok->type == t_var || tok->type == t_function || tok->type == t_begin)
    {   
        ///////////////////////////////////////////////////////////////////RULE2
        if (tok->type == t_var)
        {
            match(tok, t_var);
            nt_var_def(tok);
            nt_var_def_list(tok);
        }
        ///////////////////////////////////////////////////////////////////RULE3
        else
            return;
    }
    else
    {    
        printf("nt_var_def_block\n");
        errorHandler(errSyn);
    }
}

void nt_var_def_list (token tok)
{
    if (tok->type == t_function || tok->type == t_begin || tok->type == t_var_id)
    {

        ///////////////////////////////////////////////////////////////////RULE4
        if (tok->type == t_var_id)
        {
            nt_var_def(tok);
            nt_var_def_list(tok);
        }
        ///////////////////////////////////////////////////////////////////RULE5
        else
            return;
    }
    else
    {    
        printf("nt_var_def_list\n");
        errorHandler(errSyn);
    }
}

void nt_var_def (token tok)
{
    ///////////////////////////////////////////////////////////////////////RULE6
    
    if (tok->type == t_var_id)
    {
        /* Pripravím si key pre ukladanie */

        char * key = createKey ("V",tok->val_str);

        /* Rozhodnem sa s ktorou tabulkou idem pracovať */

        tNodePtr *currTS = NULL;
        currTS = (searchGlobalOnly == true) ? &rootTS : &localTS;

        /* Uložím premennú do odpovedajúcej tabulky symbolov */

        saveSymbol (&*currTS, key, tok->val_str, t_var_id, 0, true);

        /* Porovnanie terminálov */

        match(tok,t_var_id);
        match (tok, t_colon);

        /* Priradenie typu práve uloženej premennej */
        /* searchType=1 hovorí, že ide o premennú   */

        nt_type (tok, key);

        /* Porovnanie zvyšného terminálu */

        match (tok, t_semicolon);
        free(key);

    }
    else
    {    
        printf("nt_var_def\n");
        errorHandler(errSyn);
    }

}

void nt_fun_def_list (token tok)
{
    if (tok->type == t_function || tok->type == t_begin)
    {   

        /* Premenná ktorá rozhoduje či môže prísť forward deklarácia alebo nie*/

        bool nextMustBeBody = false;
        char *rememberVarName = NULL;
        ///////////////////////////////////////////////////////////////////RULE7

        if (tok->type == t_function)
        {
            match (tok, t_function);

            char *key = createKey ("F", tok->val_str);

            /* Ak podmienka je true, znamená to, že funkcia ešte nebola de-   */
            /* klarovaná a teda jej deklarácia prebehla v poriadku. Dodatočne */
            /* sa k nej dodeklarovala lokálna premenná s rovnakým menom v lo- */
            /* kálnej tabulke                                                 */

            if (saveSymbol (&rootTS, key, tok->val_str, t_fun_id, 0, false)==true)
            {

                /*Uloženie lokálnej premennej patriacej k práve uloženej fun.*/

                char *funVarKey = createKey ("V", tok->val_str);
                rememberVarName = createKey ("V", tok->val_str);
                saveSymbol (&localTS, funVarKey, tok->val_str, t_var_id, 0, true);
                free (funVarKey);
            }

            /* V opačnom prípade ide buď o redeklaráciu alebo doplnenie for-  */
            /* ward deklarácie                                                */

            else
            {   

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
                    printf("Redefinition of function %s\n", tok->val_str);
                    errorHandler(errSemDef);
                }
            }


            match (tok, t_fun_id);
            match (tok, t_l_parrent);

            /* Spracovanie parametrov funkcie */

            nt_param_list (tok, nextMustBeBody);
            if (nextMustBeBody==false)
            {
                searchSymbol (&rootTS, key)->data->argCount=pocetArg;
                pocetArg=0;
            }

            match (tok, t_r_parrent);
            match (tok, t_colon);

            nt_type (tok, key);
            //Nakoniec este priradim typ premennej rovnomennej
            if (rememberVarName!=NULL)
            switch (searchSymbol(&rootTS, key)->data->type)
                {
                    case 5:
                    case 9:
                    case 13:    searchSymbol(&localTS, rememberVarName)->data->type=1;break;
                    case 6:
                    case 10:
                    case 14:    searchSymbol(&localTS, rememberVarName)->data->type=2;break;
                    case 7:
                    case 11:
                    case 15:    searchSymbol(&localTS, rememberVarName)->data->type=3;break;
                    case 8:
                    case 12:
                    case 16:    searchSymbol(&localTS, rememberVarName)->data->type=4;break;
                    default: errorHandler(errSemTypArg);break;
                }

            printf("key je teraz %s\n", key );

            
            match (tok, t_semicolon);


            nt_fun_body (tok, nextMustBeBody, key);

            free (key);

            match (tok, t_semicolon);


            nt_fun_def_list(tok);








        }
        ///////////////////////////////////////////////////////////////////RULE8
        else
            return;
    }
    else
    {    
        printf("nt_fun_def_list\n");
        errorHandler(errSyn);
    }
}

void nt_fun_body (token tok, bool nextMustBeBody, char *key) //fwd1 body2
{
    if (tok->type == t_forward || tok->type == t_begin || tok->type == t_var)
    {   
        //////////////////////////////////////////////////////////////////RULE9 +++treba zaistit ze neskor bude definovana
        if (tok->type == t_forward && nextMustBeBody==false)//forward a zaroven nebol fwd
        {
            fwdDeclarations++;//zvysil sa pocet funkcii cakajucich na fwd
            //printf("found forward\n");
            searchSymbol(&rootTS, key)->data->type+=4;
            match(tok,t_forward);
            //printf("nasiel som fwd\n");
            //return 1;
        }
        //////////////////////////////////////////////////////////////////////RULE10
        else
        {   
            tNodePtr hledam = searchSymbol(&rootTS, key);
            if (nextMustBeBody==true)
            {    
                hledam->data->type+=4;//hotovokonec 13-16
                fwdDeclarations--;
            }
            searchGlobalOnly = false;
            nt_var_def_block (tok);
            nt_body (tok);
            disposeTable(&localTS);
            searchGlobalOnly=true;            
        }
    }
    else
    {
        printf("synerror in nt_fun_def_list\n");
        errorHandler(errSyn);
    }
}

void nt_body (token tok)
{
    ////////////////////////////////////////////////////////////////////////////RULE11
    if (tok->type == t_begin)
    {

        match(tok,t_begin);
        nt_stmt_list(tok);
        match(tok,t_end);
    }
    else printf("syn error in nt_body\n");
}

void nt_main (token tok)
{
    ///////////////////////////////////////////////////////////////////////////////RULE12
    if (tok->type == t_begin)
    {
        nt_body(tok);
        match(tok,t_period);
        match(tok,t_dollar);
    }
    else
    {
        printf("syn error in nt_main\n");
        errorHandler(errSyn);
    }
}

void nt_stmt_list (token tok)
{
    if (tok->type == t_begin || tok->type == t_end    || tok->type == t_if    ||
        tok->type == t_while || tok->type == t_readln || tok->type == t_write ||
        tok->type == t_var_id)
    {
        ////////////////////////////////////////////////////////////////////////////RULE14
        if (tok->type == t_end)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////////RULE13
        else
        {
            nt_stmt(tok);
            nt_stmt_more(tok);
        }

    }
    else
    {
        printf("syn error in nt_stmt_list\n");
        errorHandler(errSyn);
    }
}

void nt_stmt_more (token tok)
{
    if (tok->type == t_semicolon || tok->type == t_end)
    {
        //////////////////////////////////////////////////////////////////////////////////RULE15
        if (tok->type == t_semicolon)
        {
            match(tok,t_semicolon);
            nt_stmt_list(tok);
        }
        //////////////////////////////////////////////////////////////////////////////////RULE16
        else
            return;
    }
    else
    {
        printf("nt_stmt_more\n");
        errorHandler(errSyn);
    }
}

void nt_stmt (token tok)
{
    if (tok->type == t_begin  || tok->type == t_var_id || tok->type == t_if    ||
        tok->type == t_while  || tok->type == t_readln || tok->type == t_write)
    {
        tNodePtr hledam  = NULL;
        tNodePtr *currTS = NULL;
        char * key       = NULL;
        switch (tok->type)
        {
            ////////////////////////////////////////////////////////////////////////////////RULE17
            case 8:         nt_body (tok);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE18
            case 20:        key = createKey ("V", tok->val_str);            
                            currTS = (searchGlobalOnly == true) ? &rootTS : &localTS;
                            hledam = searchSymbol (&*currTS, key);

                            if (hledam == 0 && searchGlobalOnly == false)
                                hledam = searchSymbol(&rootTS, key);

                            if (hledam!=0)
                                if (hledam->data->type==t_var_id || (hledam->data->type >= 1 && hledam->data->type <= 4))
                                {
                                    printf("\n\nSnazim sa priradit do premennej typu %d\n\n",hledam->data->type);


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
                            if (semControlVar != hledam->data->type+4 && semControlVar != hledam->data->type+12)
                            {
                                printf("nesedeli typy\n");
                                errorHandler (errSemTypArg);
                            }


                            free (key);
                            break;                
            ////////////////////////////////////////////////////////////////////////////////RULE19               
            case 13:        match   (tok,t_if);
                            match   (tok,t_expr);
                            match   (tok,t_then);
                            nt_body (tok);
                            match   (tok,t_else);
                            nt_body (tok);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE20
            case 16:        match   (tok,t_while);
                            match   (tok,t_expr);
                            match   (tok,t_do);
                            nt_body (tok);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE21
            case 18:        match (tok,t_readln);
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
            ////////////////////////////////////////////////////////////////////////////////RULE22
            case 19:        match        (tok,t_write);
                            match        (tok,t_l_parrent);
                            nt_term_list (tok);
                            match        (tok,t_r_parrent);
                            break;
        }
    }    
    else
    {
        printf("syntax error in nt_stmt\n");
        errorHandler(errSyn);
    }
}

int nt_assign (token tok)
{
    if (tok->type == t_expr || tok->type == t_fun_id)
    {
        ///////////////////////////////////////////////////////////////////////RULE23
        if (tok->type == t_expr)
        {
            match(tok,t_expr);
            return 0;
        }
        ////////////////////////////////////////////////////////////////////////RULE24
        else
        {
            char * key = createKey ("F",tok->val_str);
            tNodePtr hledam = searchSymbol(&rootTS, key);
            if (hledam==0)
            {
                printf("Hladal som key %s\n",key );
                errorHandler(errSemDef);
            }
            free(key);
            match(tok,t_fun_id);
            match(tok,t_l_parrent);
            nt_term_list(tok);
            match(tok,t_r_parrent);
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

void nt_term (token tok)
{
    if (tok->type == t_var_id || tok->type == t_expr_val)
    {
        if (tok->type == t_var_id)
        {
            match (tok, t_var_id);
        }
        else
        {
            match (tok, t_expr_val);
        }
    }
    else
    {
        printf("syn error in nt_term\n");
        errorHandler(errSyn);
    }

}

void nt_term_list (token tok)
{
    if (tok->type == t_var_id || tok->type == t_r_parrent || tok->type == t_expr_val)
    {
        /////////////////////////////////////////////////////////////////////RULE26
        if (tok->type == t_var_id || tok->type == t_expr_val)
        {
            nt_term (tok);
            nt_term_more(tok);
        }
        /////////////////////////////////////////////////////////////////////RULE25
        else
            return;
    }
    else
    {
        printf("syn error in nt_term_list\n");
        errorHandler(errSyn);
    }
}

void nt_term_more (token tok)
{
    if (tok->type == t_comma || tok->type == t_r_parrent)
    {
        ////////////////////////////////////////////////////////////////////////RULE27
        if (tok->type == t_comma)
        {
            match(tok,t_comma);
            nt_term_list(tok);
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

void nt_param (token tok, bool testOnly)
{
    if (tok->type == t_var_id)
    {   
        char *key = createKey ("P", tok->val_str);

        ////////////////////////////////////////////SPRACOVANIE Parametru
        if (testOnly==false)//ak sa idu nove ukladat
        {   
            saveSymbol (&localTS, key, tok->val_str, t_var_id, 0, true);
            match (tok, t_var_id);
            match (tok, t_colon);
            bool temp=searchGlobalOnly;
            searchGlobalOnly=false;
            nt_type(tok, key);
            searchGlobalOnly=temp;
            pocetArg++;
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
                //printf("type co idem matchovat je %d\n",hledam->data->type );
                match (tok, t_colon);

                match (tok, hledam->data->type+25);
            }

        }
        ///////////////////////////////////////////////Premenna ulozena




        free(key);
    }
    else
    {
        printf("synerror in nt_param\n");
        errorHandler(errSyn);
    }
}

void nt_param_list (token tok, bool testOnly)
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
            nt_param(tok, testOnly);
            nt_param_more(tok, testOnly);
        }
    }
    else
    {
        printf("synerror in nt_param_list\n");
        errorHandler(errSyn);
    }
}

void nt_param_more (token tok, bool testOnly)
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
            nt_param_list(tok, testOnly);
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
    /*Vetvenie stromu na globalnu a lokalnu polovicu*/
    init(&localTS);
    printf("╔═══════════════════╗\n║rootTS:     %d║\n║localTS:    %d║\n╚═══════════════════╝\n",&rootTS,&localTS);

    if (buildemin()!=0) //////////////////////////////////////////vlozenie vstavanych funkcii
        errorHandler(errInt);

}


int main(int argc, char const *argv[])
{
    if (argc!=1 && strcmp(argv[1],"-d")==0)debug=true;
    startTable();
    token tok=malloc(sizeof(struct token));
    if (tok!=0)
    {
        gib_tok (tok);
        nt_program (tok);
        free(tok);
    }
    else
    {
        errorHandler(errInt);
        return 1;
    }
    disposeTable(&rootTS);
    if (localTS!=0) disposeTable(&localTS);
    return 0;
}