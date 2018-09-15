/*
 * mainUtils.c
 * functions used by main.c
 */
#include "parser.h"
#include "mainUtils.h"
#include <stdio.h>

/*
 * return -1 if newline found
 * return 0 if not found
 */
int checkForNewLine(char* str){
	int i=0;
	for (i=0; i<MAXCommand; i++){
		if (str[i]=='\n'){
			return -1;
		}
	}
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


