/*
 * list.c
 *
 *  Created on: 1 בספט 2018
 *      Author: inbal
 */
#include "list.h"



void clearMoves() {
	if (isEmpty()==1) { /*no moves to clear*/
		return;
	}
	while (last != current) {
		Move* temp = last;
		Change* tempChange = temp->headOfChanges;
		freeChanges(tempChange);
		free(temp);

		last = last->prev;
		last->next= NULL;
	}
}

void addMove(int x, int y, int z, Cell prevCell) {
	Move* move = (Move*) malloc(sizeof(Move));
	if (move == NULL ) {
		printf(CALLOC);
		return;
	}
	move->headOfChanges = (Change*) malloc(sizeof(Change));
	move->headOfChanges->before.value = prevCell.value;
	move->headOfChanges->before.fixed = prevCell.fixed;
	move->headOfChanges->before.error = prevCell.error;

	move->headOfChanges->after.value = z;
	move->headOfChanges->after.fixed = board.gameBoard[y - 1][x - 1].fixed;
	move->headOfChanges->after.error = board.gameBoard[y - 1][x - 1].error;

	move->headOfChanges->col = x - 1;
	move->headOfChanges->row = y - 1;
	move->headOfChanges->next = NULL;

	if (isEmpty()==1){
		head->next=move;
		move->prev=head;
	}else{
		last->next = move;
		move->prev =last;
	}
	move->next = NULL;
	current = move;
	last = current;
}

/**
 * return 1 if list is empty
 */
int isEmpty(){
	if (head->next== NULL){
		return 1;
	}
	return 0;
}

void freeChanges(Change* changes) {
	if (changes->next != NULL ) {
		freeChanges(changes->next);
	}
	free(changes);
}
