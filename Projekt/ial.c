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


tNodePtr *rootTS;



int main(int argc, char *argv[]) {

	//int sizeArray = sizeof(argv) / sizeof(argv[0]);

	int pole[argc - 1];

	for(int i = 1; i < argc; i++) {

		pole[i - 1] = atoi(argv[i]);
	}

	
	quickSort(pole, 0, argc - 2);		// volani quicksortu



	for(int j = 1; j < argc; j++) {

		printf("%d\n", pole[j - 1]);
	}

	return 0;
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


void partition(int pole[], int *i, int *j) { 	// rozdeleni

	int pivot = pole[(*i + *j) / 2];

	do {

		while(pole[*i] < pivot) 	// prvky mensi jak pivot
			(*i)++;

		while(pole[*j] > pivot)		// prvky vetsi jak pivot
			(*j)--;

		if((*i) <= (*j)) {

			int help = pole[*i];
			pole[(*i)++] = pole[*j];
			pole[(*j)--] = help;
		}

	} while(*i < *j);
}

void quickSort(int pole[], int l, int r) { // razeni

	int i, j;

	i = l; j = r;

	partition(pole, &i, &j);

	if(j > l)
		quickSort(pole, l, j);

	if(i < r)
		quickSort(pole, i, r);
}
