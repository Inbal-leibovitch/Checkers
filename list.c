/*
 * list.c
 * This module implement a doubly-linked-list that is used as undo-redo list.
 * Each element of the list represents a move that was done by the user.
 * Element contains a list of changes and pointers to the next and the prev moves.
 * Each change represents a cell that was changed because of the move.
 */
#include "list.h"

/*remove all the moves beyond the current move from the undo-redo list*/
void clearMoves() {
	if (isEmpty()==1) { /*no moves to clear*/
		return;
	}
	while (last != current) {
		Move* temp = last;
		Change* tempChange = temp->headOfChanges;
		freeChanges(tempChange);

		last = last->prev;
		last->next= NULL;
		free(temp);
	}
}
/*add a move to the undo-redo list after the user sets a cell*/
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


/* return 1 if list is empty, else return 0 */
int isEmpty(){
	if (head->next== NULL){
		return 1;
	}
	return 0;
}
/*free a list of changes in one move*/
void freeChanges(Change* changes) {
	if (changes == NULL){
		return;
	}
	if (changes->next != NULL ) {
		freeChanges(changes->next);
	}
	free(changes);
}
