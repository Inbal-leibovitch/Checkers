/*
 * main.c
 *
 *  Created on: 22 במאי 2018
 *      Author: inbal
 */
/**
#include "parser.h"
#include "game.h"*/
#include "parser.h"
#include "SPBufferset.h"
#include<time.h>
#include <stdio.h>
#include<stdlib.h>
#include "game.h"
#include "list.h"
#include "stack.h"
/*
 * gets command from user and call parse.
 */
int GameMode=0;

int main(){
	char str[256];
	top = NULL;
	printf("Sudoku\n------\n");
	SP_BUFF_SET();
	srand(time(NULL));

	/*initialize undo/redo doubly linked list*/
	last = NULL;


	head = (Move*) malloc(sizeof(Move));
	head->next = NULL; /*head->next is the first element in the list*/
	head->prev = NULL;
	current = head;

	printf("Enter your command:\n");
	while(fgets(str, MAXCommand, stdin)!=NULL)
	{
		parse(str);
		printf("Enter your command:\n");
	}
	/**
	 * exitGame();
	 */

	return 0;
}

