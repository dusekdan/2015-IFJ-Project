#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

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
	//int id;
	tType type;
	char* value;
} tToken;