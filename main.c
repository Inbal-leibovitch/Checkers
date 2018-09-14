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
#include "actions.h"
#include "mainUtils.h"
/*
 * gets command from user and call parse.
 */
int GameMode=0;

int main(){
	char str[MAXCommand];
	top = NULL;
	printf("Sudoku\n------\n");
	SP_BUFF_SET();
	srand(time(NULL));

	/*initialize undo/redo doubly linked list*/
	clearSTR(str);
	board.BoardAllocated=0;
	head = (Move*) malloc(sizeof(Move));
	if (head == NULL){
		printf(CALLOC);
		exit(1);
	}
	head->next = NULL; /*head->next is the first element in the list*/
	head->prev = NULL;
	head->headOfChanges = NULL;
	current = head;
	last = head;

	printf("Enter your command:\n");

	while(fgets(str, MAXCommand, stdin)!=NULL)
	{
		parse(str);
		printf("Enter your command:\n");
		clearSTR(str);
	}
	exitGame();


	return 0;
}

