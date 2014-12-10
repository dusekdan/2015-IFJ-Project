#define t_var       1
#define t_colon     2
#define t_semicolon 3
#define t_l_parrent 4
#define t_r_parrent 5
#define t_function  6
#define t_forward   7
#define t_begin     8
#define t_end       9
#define t_period   10
#define t_comma    11
#define t_assign   12
#define t_if       13
#define t_then     14
#define t_else     15
#define t_while    16
#define t_do       17
#define t_readln   18
#define t_write    19
#define t_var_id   20
//#define t_expr   21
#define t_fun_id   22
//#define t_term   23
//#define t_param  24
//#define t_read_id25
#define t_integer  26
#define t_real     27
#define t_string   28
#define t_boolean  29
#define t_dollar   30

#define t_plus     31// +
#define t_minus    32// -
#define t_mul      33// *
#define t_div      34// /
/// zatvorky su t_l_prrent a rparrent cize 4 a 5
#define t_less     35//<
#define t_more     36//>
#define t_lesseq   37//<=
#define t_moreeq   38//>=
#define t_equal    39//=
#define t_nequal   40//<>
#define t_expr_int 41
#define t_expr_str 42
#define t_expr_dou 43
#define t_expr_boo 44

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

int getNextToken (FILE*fd, token TToken);
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
void nt_term (token tok, char * currentFunctionKey, tContent *contentArr);
void nt_term_list (token tok, char * currentFunctionKey, tContent *contentArr);
void nt_term_more (token tok, char * currentFunctionKey, tContent *contentArr);
void nt_type (token tok, char * key);
void nt_param_list (token tok, bool testOnly, char * currentFunctionKey);
void nt_param_more (token tok, bool testOnly, char * currentFunctionKey);
void nt_param (token tok, bool testOnly, char * currentFunctionKey);

int buildemin ();



