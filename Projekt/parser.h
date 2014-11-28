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
//static const int t_term      = 23;
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
static const int t_expr_int  = 41;
static const int t_expr_str  = 42;
static const int t_expr_dou  = 43;                                    // tento terminal ak prislo nieco konecne ako cislo alebo string, vtomto pripade bude v odpovedajucej casti struktury token obsah
                                  // ale moze sa tam vyskytnut aj premenna cize, var_id a v tom pripade je v *val_str bude obsahovat nazov premennej a po vyhladani
                                  // v tabulke symbolov zistis typ, (hledam->data->type), tam su typy int 1 real 2 string 3 bool 4, iny typ nieje platna premenna

enum tableIDs
{
    placeholder,
    //var = premenna 1 - 4
    sym_var_int,
    sym_var_rea,
    sym_var_str,
    sym_var_boo,
    //fun = funkce 5 - 8
    sym_fun_int,
    sym_fun_rea,
    sym_fun_str,
    sym_fun_boo,
    //fwd = forward deklarace funkce 9 - 12
    sym_fwd_int,
    sym_fwd_rea,
    sym_fwd_str,
    sym_fwd_boo,
    //fok = funkce ok 13 - 16
    sym_fok_int,
    sym_fok_rea,
    sym_fok_str,
    sym_fok_boo

};

typedef struct token
{
    int type;      //Typ podla zoznamu terminalov
    char *val_str; //Názov v prípade že ide o id, tak sem pojde jeho nazov, ak ide o string sem ide obsah stringu
    int   val_int; //Hodnota int ak ide o integer alebo 0/1 ak ide o bool
    double val_flo; //Hodnota na float (real)
    //////////String netreba nato pozijeme val_str
    //////////Bool netreba naten pouzijeme val_int
}*token;

void gib_tok (token tok);
void terminalis (int terminal, token tok); //na debug
void nt_var_def_block (token tok);
void nt_var_def (token tok);
void nt_var_def_list (token tok);
void nt_fun_def_list (token tok);
void nt_fun_body (token tok, bool nextMustBeBody, char *key);
void nt_body (token tok);
void nt_main (token tok);
void nt_stmt_list (token tok);
void nt_stmt_more (token tok);
void nt_stmt (token tok);
int nt_assign (token tok);
void nt_term (token tok, char * currentFunctionKey);
void nt_term_list (token tok, char * currentFunctionKey);
void nt_term_more (token tok, char * currentFunctionKey);
void nt_type (token tok, char * key);
void nt_param_list (token tok, bool testOnly);
void nt_param_more (token tok, bool testOnly);
void nt_param (token tok, bool testOnly);

int buildemin ();



