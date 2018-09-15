/*
 * game.c
 *this module creates a new gameboard by allocating memory to the 2-d array of Cells.
 *each cell is filled with a number according to the file. if no file is given then an empty board is created.
 */
#include "game.h"
#include "actions.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "actionsUtils.h"

extern int GameMode;

/*
 * creates a new board and fill up the values of the cells according to the file given.
 */
void createBoard(FILE* fp) {
	int row = 0, col = 0;
	int i = 0, j = 0;
	int c, flag = 1; /*when reading the file, flag=0 while a number is read, flag=1 while space is read*/
	int number = 0;
	int countRow =0; /*number of numbers in row so far*/

	/** row is number of row in small block  row=m , col=n */
	fscanf(fp, "%d %d", &row, &col);
	if (board.BoardAllocated == 1) { /*check if we have already allocated a board in this run of the program*/
		while (current != head) {
			undo(0);
		}
		clearMoves();
		freeBoard();
	}
	board.solved = 0;
	board.row = row;
	board.col = col;
	board.N = row * col;
	board.numBlanks = board.N*board.N;
	if (board.BoardAllocated == 0){
		board.markError=1;
	}
	/**allocate memory for boards*/

	board.gameBoard = calloc(board.N, sizeof(Cell*));
	if (board.gameBoard == NULL ) {
		printf(CALLOC);
		exit(1);
	}
	for (i = 0; i < board.N; i++) {
		board.gameBoard[i] = calloc(board.N, sizeof(Cell));
		if (board.gameBoard[i] == NULL ) {
			printf(CALLOC);
			exit(1);
		}
	}
	i = 0;
	/*read the file given and add set for each cell it's value, error, autofill and temSol*/
	while (i < board.N) {
		j = 0;
		while ((c = fgetc(fp))) {
			if (c >= '0' && c <= '9') { /*read a number*/
				flag = 0;
				number = number * 10 + c - '0';
			} else {
				if (flag == 0) { /*add the value to the relevant cell*/
					board.gameBoard[i][j].value = number;
					board.gameBoard[i][j].error = 0;
					board.gameBoard[i][j].autofill =0;
					board.gameBoard[i][j].tempSol=0;
					if (number!=0){ /*decrease num blanks by 1*/
						board.numBlanks--;
					}
					j++;
					flag = 1;
					number = 0;
					countRow++;
				}
				if (c == '.') { /*if cell is fixed*/
					if (GameMode==2){
						board.gameBoard[i][j - 1].fixed = 0;
					}
					else{
						board.gameBoard[i][j - 1].fixed = 1;
					}
				}
			}
			if (c == EOF || countRow == board.N) { /*if enough numbers were read for a row or EOF reached break*/
				if (j != 0) {
					i++;
					countRow=0;
				}
				break;
			}
		}
	}
	board.BoardAllocated=1;
	CheckForErrors(); /*check if there are any errors in the board before print*/
	printBoard();
}

/*
 * creates a new empty board with row=3 and col=3
 */
void createEmptyBoard() {
	int row = 3, col = 3;
	int i, j;
	/** row is number of row in small block  row=m , col=n */
	if (board.BoardAllocated == 1) { /*check if a board was already allocated in this run of the program*/
		while (current != head) {
			undo(0);
		}
		clearMoves();
		freeBoard();
	}
	board.solved = 0;
	board.row = row;
	board.col = col;
	board.N = row * col;
	board.numBlanks = board.N*board.N;
	if (board.BoardAllocated==0){
		board.markError=1;
	}
	/**allocate memory for boards*/

	board.gameBoard = calloc(board.N, sizeof(Cell*));
	if (board.gameBoard == NULL ) {
		printf(CALLOC);
		exit(1);
	}
	for (i = 0; i < board.N; i++) {
		board.gameBoard[i] = calloc(board.N, sizeof(Cell));
		if (board.gameBoard[i] == NULL ) {
			printf(CALLOC);
			exit(1);
		}
	}
	/*set all values of the cells to 0*/
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			board.gameBoard[i][j].value = 0;
			board.gameBoard[i][j].fixed = 0;
			board.gameBoard[i][j].error = 0;
			board.gameBoard[i][j].autofill =0;
			board.gameBoard[i][j].tempSol=0;
		}
	}
	board.BoardAllocated=1;
	printBoard();
}
