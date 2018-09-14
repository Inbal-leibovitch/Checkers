/*
 * game.c
 *
 *  Created on: 22 баев 2018
 *      Author: inbal
 */
#include "game.h"
#include "actions.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

extern int GameMode;

/*
 * asks user for number of cells to fill until a valild number is given.
 * allocates memory for puzzle and solution boards.
 * creates a new board with random back tracking and prints the board.
 */

void createBoard(FILE* fp) {
	int row = 0, col = 0;
	int i = 0, j = 0;
	int c, flag = 1; /*flag=0--->whileNumber, flag=1--->whileSpace/. 	*/
	int number = 0;
	int countRow =0; /*number of numbers in row so far*/
	/** row is number of row in small block  row=m , col=n */
	fscanf(fp, "%d %d", &row, &col);
	if (board.BoardAllocated == 1) {
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
	while (i < board.N) {
		j = 0;
		while ((c = fgetc(fp))) {
			if (c >= '0' && c <= '9') {
				flag = 0;
				number = number * 10 + c - '0';
			} else {
				if (flag == 0) {
					board.gameBoard[i][j].value = number;
					board.gameBoard[i][j].error = 0;
					board.gameBoard[i][j].autofill =0;
					board.gameBoard[i][j].tempSol=0;
					if (number!=0){
						board.numBlanks--;
					}
					j++;
					flag = 1;
					number = 0;
					countRow++;
				}
				if (c == '.') {
					if (GameMode==2){
						board.gameBoard[i][j - 1].fixed = 0;
					}
					else{
						board.gameBoard[i][j - 1].fixed = 1;
					}
				}
				/*if (c== '*'){
					board.gameBoard[i][j-1].error =1;
				}*/
			}
			if (c == EOF || countRow == board.N) {
				if (j != 0) {
					i++;
					countRow=0;
				}
				break;
			}
		}
	}
	board.BoardAllocated=1;
	CheckForErrors();
	printBoard();
}

void createEmptyBoard() {
	int row = 3, col = 3;
	int i, j;
	/** row is number of row in small block  row=m , col=n */
	if (board.BoardAllocated == 1) {
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
