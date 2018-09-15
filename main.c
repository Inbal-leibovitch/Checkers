/*
 * main.c
 * starts a sudoku console game
 * asks the user for commands and waits until one is given
 * then call parser to parse and execute the command
 *
 */

#include "parser.h"
#include "SPBufferset.h"
#include<time.h>
#include <stdio.h>
#include<stdlib.h>
#include "game.h"
#include "list.h"
#include "mainUtils.h"
#include "stack.h"
#include "actions.h"
#include "actionsUtils.h"

/* GameMode - for game mode init GameMode=0, for game mode solve GameMode=1, for game mode edit GameMode=2*/
int GameMode=0;

int main(){
	char str[MAXCommand];
	char strCopy[MAXCommand];
	int eofFound =0;
	top = NULL;

	printf("Sudoku\n------\n");
	SP_BUFF_SET();
	srand(time(NULL));

	clearSTR(str); /*initialize str to be empty*/
	board.BoardAllocated=0; /*no board allocated yet*/

	/*initialize undo/redo doubly linked list*/
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
		strcpy(strCopy, str);
		parse(str);
		if (feof(stdin)==0){ /*end of line not reached*/
			while (checkForNewLine(strCopy)==0){ /*no new line char found*/
				clearSTR(str);
				fgets(str, MAXCommand, stdin);
				strcpy(strCopy, str);
				if (feof(stdin)){
					eofFound =1;
					break;
				}
			}
			if (eofFound ==1){
				break;
			}
			printf("Enter your command:\n");
			clearSTR(str);
		}
		else{
			break;
		}
	}
	/* where EOF is reached*/
	exitGame();
	return 0;
}

