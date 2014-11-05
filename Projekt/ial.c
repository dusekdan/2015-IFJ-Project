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

void searchSymbol() {


	if((*rootTS)->rptr != NULL)
		searchSymbol(&((*rootTS)->rptr));

	if((*rootTS)->lptr != NULL)
		searchSymbol(&(*rootTS)->lptr));


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
