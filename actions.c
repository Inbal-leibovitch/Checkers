#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "game.h"
#include "actions.h"

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
	createBoard(fp);
	fclose(fp);
}

void editEmpty() {
	GameMode = 2;
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
	printf("printboard");
}

/*
 * Set value z to row=x, col=y, if legal.
 * Must be in edit or solve mode before calling this function.
 */
void set(int x, int y, int z) {
	/*
	 * Checks if all three parameters are in the right range
	 */
	if (x <= 0 || x > board.N || y <= 0 || y > board.N || z < 0
			|| z > board.N) {
		printErrorNotInRange(board.N);
		return;
	}
	/** if cell if fixed, ignore command*/
	if (board.gameBoard[y - 1][x - 1].fixed == 1) {
		printf(FIXED);
		return;
	}
	/** if value is 0, delete current value in cell*/
	if (z == 0) {
		board.gameBoard[y - 1][x - 1].value = 0;
		board.numBlanks++;
	} else {
		/** set the value and mark error if value is wrong */
		board.gameBoard[y - 1][x - 1].value = z;
		board.numBlanks--;
		if (validValue(x, y, z) == 1) {
			board.gameBoard[y - 1][x - 1].error = 1;
		}
	}
	/**
	 *TODO: clear any move beyond current move from
	 * undo\redo list
	 */
	printBoard();
	/* if game is in solve mode and there are no more blanks cells the board is validated **/
	if (board.numBlanks == 0) {
		if (GameMode == 1) { /**if game is in solve mode - validate board */
			if (validateBoard() == 0) {
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
 * returns 1 if value is valid, else retuen 0.
 * x = col, y=row
 */
int validValue(int x, int y, int z) {
	int i = 0, j = 0;
	int row = 0, col = 0;

	/** check row */
	for (i = 0; i < board.row * board.col; i++) {
		if (board.gameBoard[y - 1][i].value == z) {
			return 0;
		}
	}
	/** check col */
	for (i = 0; i < board.row * board.col; i++) {
		if (board.gameBoard[i][x - 1].value == z) {
			return 0;
		}
	}
	/**  check square */
	row = ((y - 1) / board.row) * row;
	col = ((x - 1) / board.col) * board.col;
	for (i = row; i < row + board.row; i++) {
		for (j = col; j < col + board.col; j++) {
			if (board.gameBoard[i][j].value == z) {
				return 0;
			}
		}
	}
	return 1;
}
/*
 * TODO: the function return 1 if the board has no errors, else return 0
 * use ILP
 */
int validateBoard() {
	return 0;
}

void printErrorNotInRange(int X) {
	printf("Error: value not in range 0-%d\n",X);
}

void validate() {
	printf("validate");
}

void generate(int x, int y) {
	printf("generate: %d, %d", x, y);
}
void printErrorEmptyCells() {
	printf("error empty cells");
}
void undo() {
	printf("undo");
}
void redo() {
	printf("redo");
}
void save(char* fileName) {
	printf("save: %s", fileName);
}
void hint(int x, int y) {
	printf("hint: %d, %d", x, y);
}
void numSolutions() {
	printf("numsolutions");
}
void autofill() {
	printf("autofill");
}
void reset() {
	printf("reset");
}
void exitGame() {
	printf("exit");
}

