static const int errLex       =  1; // chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
static const int errSyn       =  2; // chyba v programu v rámci syntaktické analýzy (chybná syntaxe struktury programu)
static const int errSemDef    =  3; // sémantická chyba v programu – nedeﬁnovaná funkce/proměnná, pokus o redeﬁnici funkce/proměnné, atd.
static const int errSemTypArg =  4; // sémantická chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech, příp. špatný počet či typ parametrů u volání funkce
static const int errSemRest   =  5; // ostatní sémantické chyby
static const int errRunLoad   =  6; // běhová chyba při načítání číselné hodnoty ze vstupu
static const int errRunUnin   =  7; // běhová chyba při práci s neinicializovanou proměnnou
static const int errRunZdiv   =  8; // běhová chyba dělení nulou
static const int errRunRest   =  9; // ostatní běhové chyby
static const int errInt       =  99;// interní chyba interpretu tj. neovlivněná vstupním programem (např. chyba alokace paměti, chyba při otvírání souboru s řídicím programem, špatné parametry příkazové řádky atd.)

int errorHandler (int errorType);
