#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "game.h"
#include "actions.h"
#include "list.h"
#include "stack.h"
#include "solver.h"

extern int GameMode;

void solve(char* fileName) {
	FILE* fp = NULL;
	fp = fopen(fileName, "r");
	if (fp == NULL ) {
		printf(FILEERROR);
		return;
	}
	GameMode = 1; /*solve mode*/
	createBoard(fp);
	fclose(fp);
}

void edit(char* fileName) {
	FILE* fp = NULL;
	fp = fopen(fileName, "r");
	if (fp == NULL ) {
		printf("Error: File cannot be opened\n");
		/*
		 * Different error than in solve..
		 */
		return;
	}
	GameMode = 2; /*edit mode*/
	/**board.markError = 1;*/
	createBoard(fp);

	fclose(fp);
}

void editEmpty() {
	GameMode = 2;
	board.markError = 1;
	createEmptyBoard();
}

/*checked in parser that we are in solve mode, and that
 * this is a number */
void markErrors(int x) {
	if (x != 0 && x != 1) {
		printf(MARKERROR);
		return;
	}
	board.markError = x;

}

void printBoard() {
	int i = 0, j = 0, k = 0;
	for (i = 0; i < board.N * 4 + board.row + 1; i++) {
		printf("-");
	}
	printf("\n");

	for (i = 0; i < board.N; i++) {
		printf("|");
		for (j = 0; j < board.N; j++) {
			printf(" ");
			if (board.gameBoard[i][j].value == 0) {
				printf("  ");
			} else {
				printf("%2d", board.gameBoard[i][j].value);
			}
			if (board.gameBoard[i][j].fixed == 1) {
				printf(".");
			} else if (GameMode == 2 || board.markError == 1) {
				if (board.gameBoard[i][j].error == 1) {
					printf("*");
				} else {
					printf(" ");
				}
			} else {
				printf(" ");
			}

			if (j % board.col == (board.col - 1)) {
				printf("|");
			}
		}
		printf("\n");
		if (i % board.row == (board.row - 1)) {
			for (k = 0; k < board.N * 4 + board.row + 1; k++) {
				printf("-");
			}
			printf("\n");
		}
	}
}

/*
 * Set value z to row=x, col=y, if legal.
 * Must be in edit or solve mode before calling this function.
 */
void set(int x, int y, int z) {
	Cell prevCell;
	int prevValue = board.gameBoard[y-1][x-1].value;

	/** Checks if all three parameters are in the right range */
	if (x <= 0 || x > board.N || y <= 0 || y > board.N || z < 0
			|| z > board.N) {
		printErrorNotInRange(board.N);
		return;
	}
	/** if cell if fixed, ignore command*/
	if (board.gameBoard[y - 1][x - 1].fixed == 1 && GameMode == 1) {
		printf(FIXED);
		return;
	}
	/** if value is 0, delete current value in cell*/
	prevCell.value = board.gameBoard[y - 1][x - 1].value;
	prevCell.error = board.gameBoard[y - 1][x - 1].error;
	prevCell.fixed = board.gameBoard[y - 1][x - 1].fixed;

	board.gameBoard[y - 1][x - 1].value=0;
	board.gameBoard[y - 1][x - 1].error=0;

	unErrorBoard();

	if (z == 0) {
		board.gameBoard[y - 1][x - 1].value = 0;
		board.gameBoard[y - 1][x - 1].error = 0;
		if (prevValue !=0){
			board.numBlanks++;
		}
	} else {
		/** set the value and mark error if value is wrong */
		if (validValue(x, y, z) == 0) {
			board.gameBoard[y - 1][x - 1].error = 1;
		} else {
			board.gameBoard[y - 1][x - 1].error = 0;
		}
		board.gameBoard[y - 1][x - 1].value = z;
		if (prevValue==0){
			board.numBlanks--;
		}
	}

	clearMoves();
	addMove(x, y, z, prevCell);

	printBoard();
	/* if game is in solve mode and there are no more blanks cells the board is validated **/
	if (board.numBlanks == 0) {
		if (GameMode == 1) { /**if game is in solve mode - validate board */
			printf("here");
			if (validateFullBoard() == 0) {
				printf("Puzzle solution erroneous\n");
			} else {
				printf(SUCCESS);
				GameMode = 0;
				return;
			}
		}
	}
}

/**
 * return 1 if board is valid
 */
int validateFullBoard() {

	int i=0, j=0, tempValue;
	int valid=0;
	for (i=0; i<board.N; i++){
		for (j=0; j<board.N; j++){
			tempValue = board.gameBoard[i][j].value;
			if (tempValue!=0){
				board.gameBoard[i][j].value=0;
				valid=validValue(j+1,i+1,tempValue);
				board.gameBoard[i][j].value = tempValue;
				if (valid ==0){
					return 0;
				}
			}
		}
	}
	return 1;
}

void unErrorBoard(){
	int i=0, j=0, value;
	int valid=0;
	for (i=0; i<board.N; i++){
		for (j=0; j<board.N; j++){
			value = board.gameBoard[i][j].value ;
			if (value!=0){
				board.gameBoard[i][j].value=0;
				valid=validValue(j+1,i+1,value);
				board.gameBoard[i][j].value = value;
				if (valid == 1) {
					board.gameBoard[i][j].error = 0;
				}
			}
		}
	}
	return;
}

/**
 * returns 1 if value is valid, else return 0.
 * x = col, y=row
 * autofill if autofill=1
 */
int validValue(int x, int y, int z) {
	int i = 0, j = 0;
	int row = 0, col = 0;
	int foundConflict =0;

	/** check row */
	for (i = 0; i < board.N; i++) {
		if (board.gameBoard[y - 1][i].value == z) {
			foundConflict =1;
			if (board.gameBoard[y - 1][i].fixed==0){
				board.gameBoard[y - 1][i].error =1;
			}
		}
	}
	/** check col */
	for (i = 0; i < board.N; i++) {
		if (board.gameBoard[i][x - 1].value == z) {
			foundConflict =1;
			if(board.gameBoard[i][x - 1].fixed==0){
				board.gameBoard[i][x - 1].error =1;
			}
		}
	}
	/**  check square */
	row = ((y - 1) / board.row) * board.row;
	col = ((x - 1) / board.col) * board.col;
	for (i = row; i < row + board.row; i++) {
		for (j = col; j < col + board.col; j++) {
			if (board.gameBoard[i][j].value == z) {
				foundConflict =1;
				if (board.gameBoard[i][j].fixed==0){
					board.gameBoard[i][j].error =1;
				}
			}
		}
	}
	if (foundConflict==1){
		return 0;
	}
	return 1;
}
/*
 * TODO: the function return 1 if the board has no errors, else return 0
 * use ILP
 */

void printErrorNotInRange(int X) {
	printf("Error: value not in range 0-%d\n", X);
}

void validate() {
	if (isErroneous()==1){
		printf(ERRONEOUS);
		return;
	}
	if (ILPValidate()==1){
		printf(SOLVABLE);
		return;
	}
	printf(UNSOLVABLE);
}

/*
 * return 1 if board is erroneous
 */
int isErroneous(){
	int i=0, j=0;
	for (i=0; i<board.N; i++){
		for (j=0; j<board.N; j++){
			if (board.gameBoard[i][j].error==1){
				printf("i=%d,j=%d", i,j);
				return 1;
			}
		}
	}
	return 0;
}

void generate(int x, int y) {
	printf("generate: %d, %d", x, y);
}
void printErrorEmptyCells() {
	printf("error empty cells");
}
/*if print==1 - prints output is provided*/
void undo(int print) {
	Change* change;
	Change* spare;
	if (current == head) {
		printf(UNDOERORR);
		return;
	}

	change = current->headOfChanges;
	spare = change;
	while (change != NULL ) {
		board.gameBoard[change->row][change->col].value = change->before.value;
		board.gameBoard[change->row][change->col].fixed = change->before.fixed;
		board.gameBoard[change->row][change->col].error = change->before.error;
		change = change->next;
	}

	if (print == 1) {
		printBoard();
		while (spare != NULL ) {
			if (spare->after.value == 0 && spare->before.value == 0) {
				printf("Undo %d,%d: from _ to _\n", spare->col + 1,
						spare->row + 1);

			} else if (spare->after.value == 0) {
				printf("Undo %d,%d: from _ to %d\n", spare->col + 1,
						spare->row + 1, spare->before.value);
			} else if (spare->before.value == 0) {
				printf("Undo %d,%d: from %d to _\n", spare->col + 1,
						spare->row + 1, spare->after.value);
			} else {
				printf("Undo %d,%d: from %d to %d\n", spare->col + 1,
						spare->row + 1, spare->after.value,
						spare->before.value);
			}
			spare = spare->next;
		}

	}
	current = current->prev;
}

void redo() {
	Change* change;
	Change* spare;
	if (current->next == NULL ) { /*no moves to redo*/
		printf(REDOERORR);
		return;
	}
	current = current->next;
	change = current->headOfChanges;
	spare = change;

	while (change != NULL ) {
		board.gameBoard[change->row][change->col].value = change->after.value;
		board.gameBoard[change->row][change->col].fixed = change->after.fixed;
		board.gameBoard[change->row][change->col].error = change->after.error;
		change = change->next;
	}

	printBoard();
	while (spare != NULL ) {

		if (spare->after.value == 0 && spare->before.value == 0) {
			printf("Redo %d,%d: from _ to _\n", spare->col + 1, spare->row + 1);

		} else if (spare->before.value == 0) {
			printf("Redo %d,%d: from _ to %d\n", spare->col + 1, spare->row + 1,
					spare->after.value);
		} else if (spare->after.value == 0) {
			printf("Redo %d,%d: from %d to _\n", spare->col + 1, spare->row + 1,
					spare->before.value);
		} else {
			printf("Redo %d,%d: from %d to %d\n", spare->col + 1,
					spare->row + 1, spare->before.value, spare->after.value);
		}
		spare = spare->next;
	}

}
void save(char* fileName) {
	printf("save: %s", fileName);
}
void hint(int x, int y) {
	printf("hint: %d, %d", x, y);
}
/*
 * TODO: fix;
 */
void numSolutions() {
	printf("Number of solutions: %d\n",iterativeBT());
}

/*
 * TODO: fix autofill more than one
 */
void autofill() {
	int i=0, j=0, k=0;
	int value=0;
	if (isErroneous()==1){
		printf(ERRONEOUS);
		return;
	}
	for (i=0; i<board.N; i++){
		for (j=0; j<board.N; j++){
			if (board.gameBoard[i][j].value==0){
				for (k=1; k<=board.N; k++){
					if (validValue(j+1, i+1, k)==1){
						if (value==0){
							value = k;
						}
						else{
							value = -1;
						}
					}
				}
				if (value != -1 && value !=0){
					board.gameBoard[i][j].autofill=value;
					printf("Cell <%d,%d> set to %d\n", j+1, i+1, value);
				}
			}
		}
	}
	for (i=0; i<board.N; i++){
		for (j=0; j<board.N; j++){
			if (board.gameBoard[i][j].value==0 && board.gameBoard[i][j].autofill!=0 ){
				board.gameBoard[i][j].value=board.gameBoard[i][j].autofill;
				board.gameBoard[i][j].autofill=0;
			}
		}
	}
	unErrorBoard();
	printBoard();
}
void reset() {
	while (current != head) {
		undo(0);

	}
	clearMoves();
	printf(BOARDRESET);
	printBoard();
}
void exitGame() {
	freeUndoRedo();
	freeBoard();
	freeStack();
	printf(EXITING);
}

void freeUndoRedo() {
	while (last != head) {
		Move* temp = last;
		Change* tempChange = temp->headOfChanges;
		freeChanges(tempChange);
		free(temp);

		last = last->prev;
		last->next = NULL;
	}
	if (head!=NULL){
		free(head);
	}
}

void freeBoard() {
	int i;
	/*if (board!=NULL){*/
		for (i = 0; i < board.N; i++) {
			free(board.gameBoard[i]);
		}
		free(board.gameBoard);
	/*}*/
}

void freeStack(){
	Element* temp;
	while (top!=NULL){
		temp = top;
		top = top->prev;
		free(temp);
	}
}


