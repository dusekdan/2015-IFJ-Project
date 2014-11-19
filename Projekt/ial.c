/***********************ial.c************************************/
/* Soubor: ial.c - Obsahujicí funkce do předmětu IAL 			*/
/* Předmět: Formalní jazyky a překladače (IFJ) 					*/
/* Projekt: Implementace interpretu imperativního jazyka IFJ14  */
/* Varianta zadání: b/1/I 										*/
/* Datum: prosinec 2014											*/
/* Kódování: UTF-8												*/
/* Autoři:			Filip Kalous (xkalou03)						*/
/****************************************************************/


#include "ial.h"

#define ASCIISIZE 256


tNodePtr *rootTS;



int main() {

	char *pattern = "alibaba";

	char *text = "dnesba je alibaba otealibabavrena";

	int find = BMASearch(text, pattern);
	
	if(find > 0)
		printf("Nasli jsme podretezec. Jeho pozice je %d.\n", find);
	else
		printf("Nenasli jsme podretezec.\n");

	//int sizeArray = sizeof(argv) / sizeof(argv[0]);

	int delka = strlen(text);

	printf("volam qucik\n");

	quickSort(text, 0, delka - 1);		// volani quicksortu

	return 0;
}


int BMASearch(char *text, char *pattern) { 		// vraci jeho pozici, indexovano od 1


	int textLength, patternLength;
	int i;
	int j = 0;

	textLength = strlen(text);				// delka retezce 
	patternLength = strlen(pattern);		// delka hledaneho retezce

	int BCShift[ASCIISIZE], GSShift[patternLength];

	BMACountBadChar(pattern, patternLength, BCShift);
	BMACountGoodSuffix(pattern, patternLength, GSShift);


	if(textLength < patternLength)		// text je mensi jak vyhledavany podretezec
		return 0;

	if(patternLength == 0)				// podretezec je prazdny - pozice 1
		return 1;


	while(j <= (textLength - patternLength)) {

		for(i = (patternLength - 1); i >= 0 && pattern[i] == text[i+j]; --i);
		
		if(i < 0) {

			return (j + 1);
			j += GSShift[0];
			
		}

		else
			j += max(GSShift[i], BCShift[(int) text[i + j]] - patternLength + 1 + i);
	}

	return 0;		
} 			

void BMACountBadChar(char *pattern, int patternLength, int shiftBC[]) { 	// heuristika bad-character shift

	int i;

	for(i = 0; i < ASCIISIZE; ++i)		// inicializovani pole
		shiftBC[i] = patternLength;

	for(i = 0; i < (patternLength - 1); ++i)
		shiftBC[(int) pattern[i]] = patternLength - i - 1;

}

void BMACountGoodSuffix(char *pattern, int patternLength, int shiftGS[]) {	// heuristika good-suffix shift

	int i, j, suffix[patternLength];
	j = 0;

	BMASuffixes(pattern, patternLength, suffix);


	for(i = 0; i < patternLength; ++i)			// inicializovani pole
		shiftGS[i] = patternLength;

	for(i = (patternLength - 1); i >= 0; --i)
		if(suffix[i] == (i + 1))
			for(; j < (patternLength - 1 - i); ++j)
				if(shiftGS[j] == patternLength)
					shiftGS[j] = patternLength - 1 - i;

	for(i = 0; i <= patternLength - 2; ++i)
		shiftGS[patternLength - 1 - suffix[i]] = patternLength - 1 - i;

}

void BMASuffixes(char *pattern, int patternLength, int *suffix) {			// pomocna funkce pro good-suffix shift

	int f, g;

	suffix[patternLength - 1] = patternLength;
	g = patternLength - 1;

	for(int i = (patternLength - 2); i >= 0; --i) {

		if(i > g && suffix[i + patternLength - 1 - f] < (i - g))
			suffix[i] = suffix[i + patternLength - 1 -f];

		else {

			if(i < g)
				g = i;

			f = i;

			while(g >= 0 && pattern[g] == pattern[g + patternLength - 1 - f])
				--g;

			suffix[i] = f - g;
		}
	}
}


void initTable(void) {

	(*rootTS) = NULL;	
}

void disposeTable(tNodePtr *rootTS) {


	if((*rootTS)->rptr != NULL) {

		disposeTable(&((*rootTS)->rptr)); 		// vycistime pravou stranu stromu
	}

	if((*rootTS)->lptr != NULL) {	
	
		disposeTable(&((*rootTS)->lptr));		// vycistime levou stranu stromu	
	}

	free((*rootTS)->data.name);
	free(*rootTS);
}

tNodePtr searchSymbol(tNodePtr *rootTS, char* key) {

	if((*rootTS != NULL)) {

		int strCompare = strcmp(key, ((*rootTS)->key));
	
		if(strCompare < 0)
			return searchSymbol(&((*rootTS)->lptr), key);

		else if(strCompare > 0)
			return searchSymbol(&((*rootTS)->rptr), key);

		else 
			return (*rootTS);
	}

	return 0;
}

tNodePtr createNode(tNodePtr *tempRoot) {
	
	tNodePtr new;
	if((new = malloc(sizeof(struct tUzel))) == NULL)
		
		return 0;
	
	else {
	
	new->key = (*tempRoot)->key;
	new->data = (*tempRoot)->data; 
	new->lptr = NULL;
	new->rptr = NULL;
	return new;
	}
}

tNodePtr insertSymbol(tNodePtr *rootTS, char *key, tData data) {

	int cmp;
	tNodePtr tmp;
	
	if(*rootTS == NULL) {

		tmp = createNode(rootTS);
		printf("%s\n", tmp->data.name);
		return tmp;
	}
	else {

		cmp = strcmp((*rootTS)->key,key);

		if(cmp > 0)
		return insertSymbol(&((*rootTS)->rptr), key, data);	
		
		if(cmp < 0)
		return insertSymbol(&((*rootTS)->lptr), key, data);	
	    
	    if(key == (*rootTS)->key)
	    	(*rootTS)->data = data;

	}

	return 0;
}

tNodePtr readSymbol(tNodePtr *rootTS, char *key) {

	tNodePtr temp;

	if(*rootTS != NULL) {

		temp = searchSymbol(rootTS, key);

		return temp;			
	}

	return 0;
}


void partition(char text[], int *i, int *j) { 	// rozdeleni

	int pivot = text[(*i + *j) / 2];

	do {

		while(text[*i] < pivot) 	// prvky mensi jak pivot
			(*i)++;

		while(text[*j] > pivot)		// prvky vetsi jak pivot
			(*j)--;

		if((*i) <= (*j)) {

			int help = text[*i];
			text[(*i)++] = text[*j];
			text[(*j)--] = help;
		}

	} while(*i < *j);
}


 void quickSort(char *text, int l, int r) { // razeni


	int i, j;

	i = l; j = r;

	partition(text, &i, &j);

	if(j > l)
		quickSort(text, l, j);

	if(i < r)
		quickSort(text, i, r);


	printf("%s\n", text);
}


