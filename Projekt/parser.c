////////////////////Roman Jaška
////////////////////rozrobené
#include "parser.h"
#include "ial.c"



void gib_tok (token tok)
{
    //printf("gib token\n");
    int i=0;
    
    scanf("%d",&i);

    //tok->type=i;
    tok->type=i;
    }

void terminalis (int terminal)
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
        case 20: printf("var_id ");break;
        case 21: printf("expr ");break;
        case 22: printf("fun_id ");break;
        case 23: printf("term ");break;
        case 24: printf("param ");break;
        //case 25: printf("read_id ");break;
        case 26: printf("integer ");break;
        case 27: printf("real ");break;
        case 28: printf("string ");break;
        case 29: printf("boolean ");break;
        /*case 30: printf("length ");break;
        case 31: printf("copy ");break;
        case 32: printf("find ");break;
        case 33: printf("sort ");break;*/
        case 30: printf("$\n");break;
        default: printf("error input=%d\n", terminal);
    }
}

void match (token tok, int terminal)
{
    if (terminal<30)
    {
        gib_tok(tok);
        //printf("Did: ");
        terminalis(terminal);
    }
    else printf("done\n");
}


void nt_program (token tok)
{
    gib_tok (tok);
    /////////////////////////////////////////////////////////////////////////////RULE 1
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
        }
    }
    else
        printf("syntax error in nt_program\n");
}

void nt_var_def_block (token tok)
{
    if (tok->type == t_var || tok->type == t_function || tok->type == t_begin)
    {   
        ////////////////////////////////////////////////////////////////////////RULE 2
        if (tok->type == t_var)
        {
            match(tok, t_var);
            nt_var_def(tok);
            nt_var_def_list(tok);
        }
        //////////////////////////////////////////////////////////////////////////RULE3
        else
            return;
    }
    else
        printf("syntax error in nt_var_def_block\n");
}

void nt_var_def_list (token tok)
{
    if (tok->type == t_function || tok->type == t_begin || tok->type == t_var_id)
    {
        /////////////////////////////////////////////////////////////////////////RULE4
        if (tok->type == t_var_id)
        {
            nt_var_def(tok);
            nt_var_def_list(tok);
        }
        /////////////////////////////////////////////////////////////////////////RULE5
        else
            return;
    }
    else
        printf("syntax error in nt_var_def_list\n");
}

void nt_var_def (token tok)
{
    ///////////////////////////////////////////////////////////////////////////RULE6
    if (tok->type == t_var_id)
    {
        ////////////////////////////////////////////SPRACOVANIE PREMENNEJ
        if (searchSymbol(&rootTS, tok->val_str)==0)
        //////////////////////////////////////////////IDEME UKLADAT
        {
            tNodePtr temp;
            tData premenna = malloc(sizeof(struct tData));
            if (premenna!=NULL)
            {
                premenna->name=tok->val_str;
                premenna->type=t_var_id;
                temp=insertSymbol(&rootTS, tok->val_str,premenna);
                printf("premenna %s bola vytvorena\n", tok->val_str);
            } else printf("dojebal sa malloc\n");
        }else printf("premmenna uy existuje\n");

        match (tok, t_var_id);
        


        match (tok, t_colon);
        nt_type (tok);
        match (tok, t_semicolon);
    }
    else
        printf("syntax error in nt_var_def\n");

}

void nt_fun_def_list (token tok)
{
    if (tok->type == t_function || tok->type == t_begin)
    {
        //////////////////////////////////////////////////////////////////RULE7
        if (tok->type == t_function)
        {
            match(tok,t_function);
            match(tok,t_fun_id);
            match(tok,t_l_parrent);
            nt_param_list(tok);
            match(tok,t_r_parrent);
            match(tok,t_colon);
            nt_type(tok);
            match(tok,t_semicolon);
            nt_fun_body(tok);
            match(tok,t_semicolon);
            nt_fun_def_list(tok);
        }
        //////////////////////////////////////////////////////////////////////RULE8
        else
            return;
    }
    else
        printf("synerror in nt_fun_def_list\n");
}


void nt_fun_body (token tok)
{
    if (tok->type == t_forward || tok->type == t_begin || tok->type == t_var)
    {
        //////////////////////////////////////////////////////////////////RULE9 +++treba zaistit ze neskor bude definovana
        if (tok->type == t_forward)
        {
            match(tok,t_function);
            match(tok,t_fun_id);
            match(tok,t_l_parrent);
            nt_param_list(tok);
            match(tok,t_r_parrent);
            match(tok,t_colon);
            nt_type(tok);
            match(tok,t_semicolon);
            match(tok,t_forward);
            match(tok,t_semicolon);
            nt_fun_def_list(tok);
        }
        //////////////////////////////////////////////////////////////////////RULE10
        else
        {
            nt_var_def_block (tok);
            nt_body (tok);
        }
    }
    else
        printf("synerror in nt_fun_def_list\n");
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
    else printf("syn error in nt_main\n");
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
        printf("syn error in nt_stmt_list\n");
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
}

void nt_stmt (token tok)
{
    if (tok->type == t_begin  || tok->type == t_var_id || tok->type == t_if    ||
        tok->type == t_while  || tok->type == t_readln || tok->type == t_write)
    {
        switch (tok->type)
        {
            ////////////////////////////////////////////////////////////////////////////////RULE17
            case 8:         nt_body (tok);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE18
            case 20:        match     (tok,t_var_id);   
                            match     (tok,t_assign);
                            nt_assign (tok);
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
                            match (tok,t_var_id);
                            match (tok,t_r_parrent);
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
        printf("syntax error in nt_stmt\n");
}

void nt_assign (token tok)
{
    if (tok->type == t_expr || tok->type == t_fun_id)
    {
        ///////////////////////////////////////////////////////////////////////RULE23
        if (tok->type == t_expr)
        {
            match(tok,t_expr);
        }
        ////////////////////////////////////////////////////////////////////////RULE24
        else
        {
            match(tok,t_fun_id);
            match(tok,t_l_parrent);
            nt_term_list(tok);
            match(tok,t_r_parrent);
        }
    }
    else printf("syn error in nt_assign\n");
}

void nt_term_list (token tok)
{
    if (tok->type == t_term || tok->type == t_r_parrent)
    {
        /////////////////////////////////////////////////////////////////////RULE26
        if (tok->type == t_term)
        {
            match(tok,t_term);
            nt_term_more(tok);
        }
        /////////////////////////////////////////////////////////////////////RULE25
        else
            return;
    }
    else
        printf("syn error in nt_term_list\n");
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
        printf("syn error in nt_term_more\n");
}

void nt_param_list (token tok)
{
    if (tok->type == t_r_parrent || tok->type == t_param)
    {
        ////////////////////////////////////////////////////////////////////////RULE29
        if (tok->type == t_r_parrent)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////RULE30
        else
        {
            match(tok,t_param);
            nt_param_more(tok);
        }
    }
    else
        printf("synerror in nt_param_list\n");
}

void nt_param_more (token tok)
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
            nt_param_list(tok);
        }

    }
    else
        printf("synerror in nt_param_more\n");
}

void nt_type (token tok)
{
    if (tok->type == t_integer || tok->type == t_real ||
        tok->type == t_string  || tok->type == t_boolean)
    {
        switch (tok->type)
        {
            /////////////////////////////////////////////////////////////RULE33
            case 26:    match(tok,t_integer);
                        break;
            /////////////////////////////////////////////////////////////RULE34
            case 27:    match(tok,t_real);
                        break;
            /////////////////////////////////////////////////////////////RULE35
            case 28:    match(tok,t_string);
                        break;
            /////////////////////////////////////////////////////////////RULE36
            case 29:    match(tok,t_boolean);
                        break;
        }
    }
    else
        printf("syntax error in nt_type\n");

}

int main(/*int argc, char const *argv[]*/)
{
    init(&rootTS);
    /*char key='a';
    char *key=&key;*/
    token tok=malloc(sizeof(struct token));
    if (tok!=0)
    {
        tok->val_str="abc";
        printf("tokenoken\n");
        nt_program (tok);
    }
    else
    {
        printf("tokenkokot\n");
        return 1;
    }
    return 0;
}