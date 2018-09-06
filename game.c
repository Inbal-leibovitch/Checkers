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
	/*if (board!=NULL){
		freeUndoRedo();
		freeBoard();
		freeStack();
	}*/
	/** row is number of row in small block  row=m , col=n */
	fscanf(fp, "%d %d", &row, &col);
	board.solved = 0;
	board.row = row;
	board.col = col;
	board.N = row * col;
	board.numBlanks = board.N*board.N;
	board.markError=1;
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
				}
				if (c == '.') {
					if (GameMode==2){
						board.gameBoard[i][j - 1].fixed = 0;
					}
					else{
						board.gameBoard[i][j - 1].fixed = 1;
					}
				}
			}
			if (c == EOF || c == '\n') {
				if (j != 0) {
					i++;
				}
				break;
			}
		}
	}
	printBoard();
}

void createEmptyBoard() {
	int row = 3, col = 3;
	int i, j;
	/** row is number of row in small block  row=m , col=n */
	/*if (board!=NULL){
		freeUndoRedo();
		freeBoard();
		freeStack();
	}*/
	board.solved = 0;

	board.row = row;
	board.col = col;
	board.N = row * col;
	board.numBlanks = board.N*board.N;
	board.markError=1;
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
	printBoard();
}
