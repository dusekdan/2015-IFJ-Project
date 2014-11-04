#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void getToken(FILE* fd);

typedef enum
{
	IDENTIFIER,
	STRING,
	REAL,
	INTEGER,
	BOOLEAN,
	OPERATOR
} tType;

typedef struct
{
	//tType type;
	char* attribute;
} tToken;