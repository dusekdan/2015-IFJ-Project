#include "stdio.h"
#include "string.h"
#include "ial.c"

static const int t_var       =  1;
static const int t_colon     =  2;
static const int t_semicolon =  3;
static const int t_l_parrent =  4;
static const int t_r_parrent =  5;
static const int t_function  =  6;
static const int t_forward   =  7;
static const int t_begin     =  8;
static const int t_end       =  9;
static const int t_period    = 10;
static const int t_comma     = 11;
static const int t_assign    = 12;
static const int t_if        = 13;
static const int t_then      = 14;
static const int t_else      = 15;
static const int t_while     = 16;
static const int t_do        = 17;
static const int t_readln    = 18;
static const int t_write     = 19;
static const int t_var_id    = 20;
static const int t_expr      = 21;
static const int t_fun_id    = 22;
static const int t_term      = 23;
//static const int t_param     = 24;
//static const int t_read_id   = 25;  //////////////////////////////read id nakoniec nebude treba
static const int t_integer   = 26;
static const int t_real      = 27;
static const int t_string    = 28;
static const int t_boolean   = 29;
static const int t_dollar    = 30;

static const int t_plus      = 31;// +
static const int t_minus     = 32;// -
static const int t_mul       = 33;// *
static const int t_div       = 34;// /
/// zatvorky su t_l_parrent a rparrent cize 4 a 5
static const int t_less      = 35;//<
static const int t_more      = 36;//>
static const int t_lesseq    = 37;//<=
static const int t_moreeq    = 38;//>=
static const int t_equal     = 39;//=
static const int t_nequal    = 40;//<>
static const int t_expr_val  = 41;// tento terminal ak prislo nieco konecne ako cislo alebo string, vtomto pripade bude v odpovedajucej casti struktury token obsah
                                  // ale moze sa tam vyskytnut aj premenna cize, var_id a v tom pripade je v *val_str bude obsahovat nazov premennej a po vyhladani
                                  // v tabulke symbolov zistis typ, (hledam->data->type), tam su typy int 1 real 2 string 3 bool 4, iny typ nieje platna premenna



typedef struct token
{
    int type;      //Typ podla zoznamu terminalov
    char *val_str; //Názov v prípade že ide o id, tak sem pojde jeho nazov, ak ide o string sem ide obsah stringu
    int   val_int; //Hodnota int ak ide o integer alebo 0/1 ak ide o bool
    float val_flo; //Hodnota na float (real)
    //////////String netreba nato pozijeme val_str
    //////////Bool netreba naten pouzijeme val_int
}*token;


void terminalis (int terminal, token tok); //na debug
void nt_var_def_block (token tok);
void nt_var_def (token tok);
void nt_var_def_list (token tok);
void nt_fun_def_list (token tok);
void nt_fun_body (token tok);
void nt_body (token tok);
void nt_main (token tok);
void nt_stmt_list (token tok);
void nt_stmt_more (token tok);
void nt_stmt (token tok);
void nt_assign (token tok);
void nt_term_list (token tok);
void nt_term_more (token tok);
void nt_type (token tok);
void nt_param_list (token tok);
void nt_param_more (token tok);
void nt_param (token tok);

int buildemin ();


int buildemin()
{
    tNodePtr newnode;
    tData    newsymbol = malloc(sizeof(struct tData));
    if (newsymbol != NULL)
    {
        newsymbol->name = "length";
        newsymbol->type = 5;//funkcia integrova
        newsymbol->argCount = 1;
        newnode = insertSymbol(&rootTS, "length", newsymbol);
    }else return 1;
    newsymbol=NULL;
             newnode;
             newsymbol = malloc(sizeof(struct tData));
    if (newsymbol != NULL)
    {
        newsymbol->name = "copy";
        newsymbol->type = 7;//funkcia stringova
        newsymbol->argCount = 3;
        newnode = insertSymbol(&rootTS, "length", newsymbol);
    }else return 1;
    newsymbol=NULL;
             newnode;
             newsymbol = malloc(sizeof(struct tData));
    if (newsymbol != NULL)
    {
        newsymbol->name = "find";
        newsymbol->type = 5;//funkcia intefgrova
        newsymbol->argCount = 2;
        newnode = insertSymbol(&rootTS, "find", newsymbol);
    }else return 1;
    newsymbol=NULL;
             newnode;
             newsymbol = malloc(sizeof(struct tData));
    if (newsymbol != NULL)
    {
        newsymbol->name = "sort";
        newsymbol->type = 7;//funkcia stringova
        newsymbol->argCount = 1;
        newnode = insertSymbol(&rootTS, "length", newsymbol);
    }else return 1;
    return 0;
}

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
    tok->type=i;
}

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
        case 23: printf("term ");break;
        //case 24: printf("param ");break;
        //case 25: printf("read_id ");break;
        case 26: printf("integer ");break;
        case 27: printf("real ");break;
        case 28: printf("string ");break;
        case 29: printf("boolean ");break;
        case 30: printf("$\n");break;
        default: printf("error input=%d\n", terminal);
    }
}
