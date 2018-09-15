/*
 * mainUtils.c
 * functions used by main.c
 */
#include "parser.h"
#include "mainUtils.h"
#include <stdio.h>

/*
 * return -1 if EOF found
 * return 0 if not found
 */
int checkForEOF(char* str){
	int i=0;
	for (i=0; i<MAXCommand; i++){
		printf("%c",str[i]);
		if (str[i]==EOF){
			printf("found\n");
			return -1;
		}
	}
	printf("\n");
	return 0;
}

/*
 * sets all chars in str to 0
 */
void clearSTR(char* str){
	int i=0;
	for (i=0; i<MAXCommand; i++){
		str[i]=0;
	}
}


