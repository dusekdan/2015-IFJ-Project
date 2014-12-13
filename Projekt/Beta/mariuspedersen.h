typedef struct Kontajner
{
    void * Odpad;
    struct Kontajner *next;
} *tKontajner;

typedef struct 
{
    struct Kontajner *first;
    struct Kontajner *active;
    struct Kontajner *last;
} tSmetisko;
