#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "game.h"
#include "actions.h"
#include "list.h"
#include "stack.h"
#include "gurobi.h"
#include "actionsUtils.h"

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
	int prevValue;
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
	prevValue = board.gameBoard[y - 1][x - 1].value;
	if (z == prevValue) {
		printBoard();
		return;
	}
	/** if value is 0, delete current value in cell*/
	prevCell.value = board.gameBoard[y - 1][x - 1].value;
	prevCell.error = board.gameBoard[y - 1][x - 1].error;
	prevCell.fixed = board.gameBoard[y - 1][x - 1].fixed;

	board.gameBoard[y - 1][x - 1].value = 0;
	board.gameBoard[y - 1][x - 1].error = 0;

	unErrorBoard();

	if (z == 0) {
		board.gameBoard[y - 1][x - 1].value = 0;
		board.gameBoard[y - 1][x - 1].error = 0;
		if (prevValue != 0) {
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
		if (prevValue == 0) {
			board.numBlanks--;
		}
	}

	clearMoves();
	addMove(x, y, z, prevCell);

	printBoard();
	/* if game is in solve mode and there are no more blanks cells the board is validated **/
	if (board.numBlanks == 0) {
		if (GameMode == 1) { /**if game is in solve mode - validate board */
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


/*
 * TODO: the function return 1 if the board has no errors, else return 0
 * use ILP
 */

/* *****************************************/
void printErrorNotInRange(int X) {
	printf("Error: value not in range 0-%d\n", X);
}
/* *****************************************/

void validate() {

	if (isErroneous() == 1) {
		printf(ERRONEOUS);
		return;
	}
	clearTempSol();
	if (ILPSolver() == 1) {
		printf(SOLVABLE);
		return;
	}
	printf(UNSOLVABLE);
}

void generate(int x, int y) {
	int i;
	if (x > board.numBlanks || y > board.numBlanks) {
		printErrorNotInRange(board.numBlanks);
		return;
	}
	if (board.numBlanks != board.N * board.N) {
		printf("Error: board is not empty\n");
		return;
	}
	clearBoard();
	for (i = 0; i < 1000; i++) {
		if (fillXCells(x) == 0) { /*a cell has no legal value*/
			clearBoard();
			continue;
		}
		if (ILPSolver() == 0) { /*board not solvable*/
			clearBoard();
			continue;
		}
		/*tempSol has solution*/
		chooseYCells(y);
		clearTempSol();
		unErrorBoard(); /*validValue adds unnecessary errors*/
		printBoard();
		return;
	}
	printf("Error: puzzle generator failed\n");
	return;
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
	unErrorBoard();
	if (print == 1) {
		printBoard();
		while (spare != NULL ) {
			if (spare->after.value == 0 && spare->before.value == 0) {
				printf("Undo %d,%d: from _ to _\n", spare->col + 1,
						spare->row + 1);

			} else if (spare->after.value == 0) {
				printf("Undo %d,%d: from _ to %d\n", spare->col + 1,
						spare->row + 1, spare->before.value);
				board.numBlanks--;
			} else if (spare->before.value == 0) {
				printf("Undo %d,%d: from %d to _\n", spare->col + 1,
						spare->row + 1, spare->after.value);
				board.numBlanks++;
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
	CheckForErrors();
	printBoard();
	while (spare != NULL ) {

		if (spare->after.value == 0 && spare->before.value == 0) {
			printf("Redo %d,%d: from _ to _\n", spare->col + 1, spare->row + 1);

		} else if (spare->before.value == 0) {
			printf("Redo %d,%d: from _ to %d\n", spare->col + 1, spare->row + 1,
					spare->after.value);
			board.numBlanks--;
		} else if (spare->after.value == 0) {
			printf("Redo %d,%d: from %d to _\n", spare->col + 1, spare->row + 1,
					spare->before.value);
			board.numBlanks++;
		} else {
			printf("Redo %d,%d: from %d to %d\n", spare->col + 1,
					spare->row + 1, spare->before.value, spare->after.value);
		}
		spare = spare->next;
	}

}

void save(char* fileName) {
	FILE* fp = NULL;

	if (GameMode == 2) {
		if (isErroneous() == 1) {
			printf(ERRONEOUS);
			return;
		}
		clearTempSol();
		if (ILPSolver() == 0) {
			printf(VALIDATIONFAILED);
			return;
		}
	}
	fp = fopen(fileName, "w");
	if (fp == NULL ) {
		printf("Error: File cannot be created or modified\n");
		return;
	}
	saveTofile(fp);
	printf("Saved to: %s\n", fileName);
	return;
}

void hint(int x, int y) {
	int solvable = 0;
	if (isErroneous() == 1) {
		printf(ERRONEOUS);
		return;
	}
	if (board.gameBoard[y - 1][x - 1].fixed == 1) {
		printf(FIXED);
		return;
	}
	if (board.gameBoard[y - 1][x - 1].value != 0) {
		printf(CONTAINSVALUE);
		return;
	}
	clearTempSol();
	solvable = ILPSolver();
	if (solvable != 1) {
		printf(UNSOLVABLE);
		return;
	}
	printf("Hint: set cell to %d\n", board.gameBoard[y - 1][x - 1].tempSol);
}

void numSolutions() {
	int numSolutions;
	if (isErroneous() == 1) {
		printf(ERRONEOUS);
		return;
	}
	/*exhustive backtracking*/
	numSolutions = iterativeBT();
	printf("Number of solutions: %d\n", numSolutions);
	if (numSolutions == 1) {
		printf(GOODBOARD);
	}
	if (numSolutions > 1) {
		printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
}

void autofill() {
	clearAutofill();
	if (isErroneous() == 1) {
		printf(ERRONEOUS);
		return;
	}
	fillValuesInAutofill();
	updateBoardAndList();

	unErrorBoard(); /*needed because validValue adds unnecessary errors*/
	printBoard();
	if (board.numBlanks == 0) {
		printf(SUCCESS);
		GameMode = 0;
		return;
	}
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
	freeResources();
	printf(EXITING);
	exit(0);
}
