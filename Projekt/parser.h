#include "stdio.h"
#include "string.h"

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
static const int t_param     = 24;
//static const int t_read_id   = 25;  //////////////////////////////read id nakoniec nebude treba
static const int t_integer   = 26;
static const int t_real      = 27;
static const int t_string    = 28;
static const int t_boolean   = 29;
static const int t_dollar    = 30;

typedef struct
{
    int type;      //Typ podla zoznamu terminalov
    *char val_str; //Názov v prípade že ide o id, tak sem pojde jeho nazov, ak ide o string sem ide obsah stringu
    int   val_int; //Hodnota int ak ide o integer alebo 0/1 ak ide o bool
    float val_flo; //Hodnota na float (real)
    //////////String netreba nato pozijeme val_str
    //////////Bool netreba naten pouzijeme val_int
}token;


void terminalis (int terminal); //na debug
void nt_var_def_block (int* typ_ptr, char* att_ptr);
void nt_var_def (int* typ_ptr, char* att_ptr);
void nt_var_def_list (int* typ_ptr, char* att_ptr);
void nt_fun_def_list (int* typ_ptr, char* att_ptr);
void nt_fun_body (int* typ_ptr, char* att_ptr);
void nt_body (int* typ_ptr, char* att_ptr);
void nt_main (int* typ_ptr, char* att_ptr);
void nt_stmt_list (int* typ_ptr, char* att_ptr);
void nt_stmt_more (int* typ_ptr, char* att_ptr);
void nt_stmt (int* typ_ptr, char* att_ptr);
void nt_assign (int* typ_ptr, char* att_ptr);
void nt_term_list (int* typ_ptr, char* att_ptr);
void nt_term_more (int* typ_ptr, char* att_ptr);
void nt_type (int* typ_ptr, char* att_ptr);
void nt_param_list (int* typ_ptr, char* att_ptr);
void nt_param_more (int* typ_ptr, char* att_ptr);