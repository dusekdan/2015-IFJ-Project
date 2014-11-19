////////////////////Roman Jaška
////////////////////rozrobené
#include "parser.h"



void gib_tok (int* typ_ptr, char* att_ptr)
{
    //printf("gib token\n");
    int i=0;
    scanf("%d",&i);
    /*printf("Got: ");
    terminalis(i);*/
    *typ_ptr=i;
    /*
    *att_ptr='a';*/
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

void match (int* typ_ptr, char* att_ptr, int terminal)
{
    if (terminal<30)
    {
        gib_tok(typ_ptr, att_ptr);
        //printf("Did: ");
        terminalis(terminal);
    }
    else printf("done\n");
}


void nt_program (int* typ_ptr, char* att_ptr)
{
    gib_tok (typ_ptr, att_ptr);
    /////////////////////////////////////////////////////////////////////////////RULE 1
    if (*typ_ptr == t_var || *typ_ptr == t_function || *typ_ptr == t_begin)
    {   
        if (*typ_ptr == t_var)
        {
            nt_var_def_block (typ_ptr, att_ptr);
        }
        if (*typ_ptr == t_function)
        {
            nt_fun_def_list (typ_ptr, att_ptr);
        }
        if (*typ_ptr == t_begin)
        {
            nt_main (typ_ptr, att_ptr);
        }
    }
    else
        printf("syntax error in nt_program\n");
}

void nt_var_def_block (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_var || *typ_ptr == t_function || *typ_ptr == t_begin)
    {   
        ////////////////////////////////////////////////////////////////////////RULE 2
        if (*typ_ptr == t_var)
        {
            match(typ_ptr, att_ptr, 1);
            nt_var_def(typ_ptr, att_ptr);
            nt_var_def_list(typ_ptr, att_ptr);
        }
        //////////////////////////////////////////////////////////////////////////RULE3
        else
            return;
    }
    else
        printf("syntax error in nt_var_def_block\n");
}

void nt_var_def_list (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_function || *typ_ptr == t_begin || *typ_ptr == t_var_id)
    {
        /////////////////////////////////////////////////////////////////////////RULE4
        if (*typ_ptr == t_var_id)
        {
            nt_var_def(typ_ptr, att_ptr);
            nt_var_def_list(typ_ptr, att_ptr);
        }
        /////////////////////////////////////////////////////////////////////////RULE5
        else
            return;
    }
    else
        printf("syntax error in nt_var_def_list\n");
}

void nt_var_def (int* typ_ptr, char* att_ptr)
{
    ///////////////////////////////////////////////////////////////////////////RULE6
    if (*typ_ptr == t_var_id)
    {
        match (typ_ptr, att_ptr, t_var_id);
        match (typ_ptr, att_ptr, t_colon);
        nt_type (typ_ptr, att_ptr);
        match (typ_ptr, att_ptr, t_semicolon);
    }
    else
        printf("syntax error in nt_var_def\n");

}

void nt_fun_def_list (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_function || *typ_ptr == t_begin)
    {
        //////////////////////////////////////////////////////////////////RULE7
        if (*typ_ptr == t_function)
        {
            match(typ_ptr,att_ptr,t_function);
            match(typ_ptr,att_ptr,t_fun_id);
            match(typ_ptr,att_ptr,t_l_parrent);
            nt_param_list(typ_ptr,att_ptr);
            match(typ_ptr,att_ptr,t_r_parrent);
            match(typ_ptr,att_ptr,t_colon);
            nt_type(typ_ptr,att_ptr);
            match(typ_ptr,att_ptr,t_semicolon);
            nt_fun_body(typ_ptr,att_ptr);
            match(typ_ptr,att_ptr,t_semicolon);
            nt_fun_def_list(typ_ptr,att_ptr);
        }
        //////////////////////////////////////////////////////////////////////RULE8
        else
            return;
    }
    else
        printf("synerror in nt_fun_def_list\n");
}


void nt_fun_body (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_forward || *typ_ptr == t_begin || *typ_ptr == t_var)
    {
        //////////////////////////////////////////////////////////////////RULE9 +++treba zaistit ze neskor bude definovana
        if (*typ_ptr == t_forward)
        {
            match(typ_ptr,att_ptr,t_function);
            match(typ_ptr,att_ptr,t_fun_id);
            match(typ_ptr,att_ptr,t_l_parrent);
            nt_param_list(typ_ptr,att_ptr);
            match(typ_ptr,att_ptr,t_r_parrent);
            match(typ_ptr,att_ptr,t_colon);
            nt_type(typ_ptr,att_ptr);
            match(typ_ptr,att_ptr,t_semicolon);
            match(typ_ptr,att_ptr,t_forward);
            match(typ_ptr,att_ptr,t_semicolon);
            nt_fun_def_list(typ_ptr,att_ptr);
        }
        //////////////////////////////////////////////////////////////////////RULE10
        else
        {
            nt_var_def_block  (typ_ptr,att_ptr);
            nt_body (typ_ptr,att_ptr);
        }
    }
    else
        printf("synerror in nt_fun_def_list\n");
}

void nt_body (int* typ_ptr, char* att_ptr)
{
    ////////////////////////////////////////////////////////////////////////////RULE11
    if (*typ_ptr == t_begin)
    {
        match(typ_ptr,att_ptr,t_begin);
        nt_stmt_list(typ_ptr,att_ptr);
        match(typ_ptr,att_ptr,t_end);
    }
    else printf("syn error in nt_body\n");
}

void nt_main (int* typ_ptr, char* att_ptr)
{
    ///////////////////////////////////////////////////////////////////////////////RULE12
    if (*typ_ptr == t_begin)
    {
        nt_body(typ_ptr,att_ptr);
        match(typ_ptr,att_ptr,t_period);
        match(typ_ptr,att_ptr,t_dollar);
    }
    else printf("syn error in nt_main\n");
}

void nt_stmt_list (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_begin || *typ_ptr == t_end    || *typ_ptr == t_if    ||
        *typ_ptr == t_while || *typ_ptr == t_readln || *typ_ptr == t_write ||
        *typ_ptr == t_var_id)
    {
        ////////////////////////////////////////////////////////////////////////////RULE14
        if (*typ_ptr == t_end)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////////RULE13
        else
        {
            nt_stmt(typ_ptr,att_ptr);
            nt_stmt_more(typ_ptr,att_ptr);
        }

    }
    else
        printf("syn error in nt_stmt_list\n");
}

void nt_stmt_more (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_semicolon || *typ_ptr == t_end)
    {
        //////////////////////////////////////////////////////////////////////////////////RULE15
        if (*typ_ptr == t_semicolon)
        {
            match(typ_ptr,att_ptr,t_semicolon);
            nt_stmt_list(typ_ptr,att_ptr);
        }
        //////////////////////////////////////////////////////////////////////////////////RULE16
        else
            return;
    }
}

void nt_stmt (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_begin  || *typ_ptr == t_var_id || *typ_ptr == t_if    ||
        *typ_ptr == t_while  || *typ_ptr == t_readln || *typ_ptr == t_write/* ||
        *typ_ptr == t_length || *typ_ptr == t_copy   || *typ_ptr == t_find  ||
        *typ_ptr == t_sort*/)
    {
        switch (*typ_ptr)
        {
            ////////////////////////////////////////////////////////////////////////////////RULE17
            case 8:         nt_body(typ_ptr,att_ptr);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE18
            case 20:        match(typ_ptr,att_ptr,t_var_id);   
                            match(typ_ptr,att_ptr,t_assign);
                            nt_assign(typ_ptr,att_ptr);
                            break;                
            ////////////////////////////////////////////////////////////////////////////////RULE19               
            case 13:        match(typ_ptr,att_ptr,t_if);
                            match(typ_ptr,att_ptr,t_expr);
                            match(typ_ptr,att_ptr,t_then);
                            nt_body(typ_ptr,att_ptr);
                            match(typ_ptr,att_ptr,t_else);
                            nt_body(typ_ptr,att_ptr);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE20
            case 16:        match(typ_ptr,att_ptr,t_while);
                            match(typ_ptr,att_ptr,t_expr);
                            match(typ_ptr,att_ptr,t_do);
                            nt_body(typ_ptr,att_ptr);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE21
            case 18:        match(typ_ptr,att_ptr,t_readln);
                            match(typ_ptr,att_ptr,t_l_parrent);
                            match(typ_ptr,att_ptr,t_var_id);
                            match(typ_ptr,att_ptr,t_r_parrent);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE22
            case 19:        match(typ_ptr,att_ptr,t_write);
                            match(typ_ptr,att_ptr,t_l_parrent);
                            nt_term_list(typ_ptr,att_ptr);
                            match(typ_ptr,att_ptr,t_r_parrent);
                            break;
            /*////////////////////////////////////////////////////////////////////////////////RULE37
            case 30:        match(typ_ptr,att_ptr,t_length);
                            match(typ_ptr,att_ptr,t_l_parrent);
                            match(typ_ptr,att_ptr,t_term);
                            match(typ_ptr,att_ptr,t_r_parrent);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE38
            case 31:        match(typ_ptr,att_ptr,t_copy);
                            match(typ_ptr,att_ptr,t_l_parrent);
                            match(typ_ptr,att_ptr,t_term);
                            match(typ_ptr,att_ptr,t_semicolon);
                            match(typ_ptr,att_ptr,t_term);
                            match(typ_ptr,att_ptr,t_semicolon);
                            match(typ_ptr,att_ptr,t_term);
                            match(typ_ptr,att_ptr,t_r_parrent);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE39
            case 32:        match(typ_ptr,att_ptr,t_find);
                            match(typ_ptr,att_ptr,t_l_parrent);
                            match(typ_ptr,att_ptr,t_term);
                            match(typ_ptr,att_ptr,t_semicolon);
                            match(typ_ptr,att_ptr,t_term);
                            match(typ_ptr,att_ptr,t_r_parrent);
                            break;
            ////////////////////////////////////////////////////////////////////////////////RULE40
            case 33:        match(typ_ptr,att_ptr,t_sort);
                            match(typ_ptr,att_ptr,t_l_parrent);
                            match(typ_ptr,att_ptr,t_term);
                            match(typ_ptr,att_ptr,t_r_parrent);
                            break;*/
        }
    }    
    else
        printf("syntax error in nt_stmt\n");
}

void nt_assign (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_expr || *typ_ptr == t_fun_id)
    {
        ///////////////////////////////////////////////////////////////////////RULE23
        if (*typ_ptr == t_expr)
        {
            match(typ_ptr,att_ptr,t_expr);
        }
        ////////////////////////////////////////////////////////////////////////RULE24
        else
        {
            match(typ_ptr,att_ptr,t_fun_id);
            match(typ_ptr,att_ptr,t_l_parrent);
            nt_term_list(typ_ptr,att_ptr);
            match(typ_ptr,att_ptr,t_r_parrent);
        }
    }
    else printf("syn error in nt_assign\n");
}

void nt_term_list (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_term || *typ_ptr == t_r_parrent)
    {
        /////////////////////////////////////////////////////////////////////RULE26
        if (*typ_ptr == t_term)
        {
            match(typ_ptr,att_ptr,t_term);
            nt_term_more(typ_ptr,att_ptr);
        }
        /////////////////////////////////////////////////////////////////////RULE25
        else
            return;
    }
    else
        printf("syn error in nt_term_list\n");
}

void nt_term_more (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_comma || *typ_ptr == t_r_parrent)
    {
        ////////////////////////////////////////////////////////////////////////RULE27
        if (*typ_ptr == t_comma)
        {
            match(typ_ptr,att_ptr,t_comma);
            nt_term_list(typ_ptr,att_ptr);
        }
        ////////////////////////////////////////////////////////////////////////RULE28
        else
            return;
    }
    else
        printf("syn error in nt_term_more\n");
}

void nt_param_list (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_r_parrent || *typ_ptr == t_param)
    {
        ////////////////////////////////////////////////////////////////////////RULE29
        if (*typ_ptr == t_r_parrent)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////RULE30
        else
        {
            match(typ_ptr,att_ptr,t_param);
            nt_param_more(typ_ptr,att_ptr);
        }
    }
    else
        printf("synerror in nt_param_list\n");
}

void nt_param_more (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_r_parrent || *typ_ptr == t_comma)
    {
        ////////////////////////////////////////////////////////////////////////RULE32
        if (*typ_ptr == t_r_parrent)
        {
            return;
        }
        ////////////////////////////////////////////////////////////////////////RULE31
        else
        {
            match(typ_ptr,att_ptr,t_comma);
            nt_param_list(typ_ptr,att_ptr);
        }

    }
    else
        printf("synerror in nt_param_more\n");
}

void nt_type (int* typ_ptr, char* att_ptr)
{
    if (*typ_ptr == t_integer || *typ_ptr == t_real ||
        *typ_ptr == t_string  || *typ_ptr == t_boolean)
    {
        switch (*typ_ptr)
        {
            /////////////////////////////////////////////////////////////RULE33
            case 26:    match(typ_ptr,att_ptr,t_integer);
                        break;
            /////////////////////////////////////////////////////////////RULE34
            case 27:    match(typ_ptr,att_ptr,t_real);
                        break;
            /////////////////////////////////////////////////////////////RULE35
            case 28:    match(typ_ptr,att_ptr,t_string);
                        break;
            /////////////////////////////////////////////////////////////RULE36
            case 29:    match(typ_ptr,att_ptr,t_boolean);
                        break;
        }
    }
    else
        printf("syntax error in nt_type\n");

}



int main(int argc, char const *argv[])
{
    char attribute;
    char *att_ptr = &attribute;
    int typ ;/*= *argv[1]-48;*/
    //printf("%d\n", typ );
    int *typ_ptr = &typ;
    nt_program (typ_ptr,att_ptr);
    return 0;
}