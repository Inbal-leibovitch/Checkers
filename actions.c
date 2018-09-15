/*
 * This module contains functions that executes all the commands received from the user.
 */

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

/* load a game from the file and start it in solve mode*/
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

/* load a game from the file and start it in edit mode*/
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

/*load empty board and start it in edit mode*/
void editEmpty() {
	GameMode = 2;
	createEmptyBoard();
}

/*
 * change the value of mark_errors to the value of x
 * the value of  is 0 or 1 only
 */
void markErrors(int x) {
	if (x != 0 && x != 1) {
		printf(MARKERROR);
		return;
	}
	board.markError = x;
}

/*print the board*/
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
			/*mark erroneous values in edit mode or if mark_errors==1*/
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
 * Set value z to x,y, if z is valid value
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
	/** if value doesn't change - ignore the command */
	prevValue = board.gameBoard[y - 1][x - 1].value;
	if (z == prevValue) {
		printBoard();
		return;
	}
	/*create prev cell for the undo-redo list*/
	prevCell.value = board.gameBoard[y - 1][x - 1].value;
	prevCell.error = board.gameBoard[y - 1][x - 1].error;
	prevCell.fixed = board.gameBoard[y - 1][x - 1].fixed;

	board.gameBoard[y - 1][x - 1].value = 0;
	board.gameBoard[y - 1][x - 1].error = 0;

	/*redmove erors from the board after changing the cell value to 0*/
	unErrorBoard();
	/** if value is 0, delete current value in cell*/
	if (z == 0) {
		board.gameBoard[y - 1][x - 1].value = 0;
		board.gameBoard[y - 1][x - 1].error = 0;
		if (prevValue != 0) {
			board.numBlanks++;
		}
	}
	/* set the value and mark error if value is wrong
	 * validValue function also mark errors if necessary
	 */
	else {
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
	/*clear undo-redo list after set move*/
	clearMoves();
	/*add move to the indo-redo list*/
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

/* validate the board is solvable using ILP */
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

/*generate a board by randomly choosing x cells the fill, solving the board using ILP and leave in it y cells
 * only available in edit mode
 * if x>number of empty cells or if the board is not empty the command is not executed
 */
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

/* undo a move from the undo-redo list, if there is a move to undo
 * if print==1 , prints output is provided
 */
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
/* undo a move from the undo-redo list, if there is a move to undo*/
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
/*save the current board to a file named filename */
void save(char* fileName) {
	FILE* fp = NULL;

	/* check in edit mode if the board is erroneous*/
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

/* provide a hint to cell x,y, using ILP to solve the board
 	 only available in solve mode*
 */
void hint(int x, int y) {
	int solvable = 0;
	/*the command is not executed if x or y are not in range*/
	if (x <= 0 || x > board.N || y <= 0 || y > board.N ) {
		printf("Error: value not in range 1-%d\n", board.N);
		return;
	}
	/*the command is not executed if the board is erroneous*/
	if (isErroneous() == 1) {
		printf(ERRONEOUS);
		return;
	}
	/*the command is not executed if the cell is fixed*/
	if (board.gameBoard[y - 1][x - 1].fixed == 1) {
		printf(FIXED);
		return;
	}
	/*the command is not executed if the cell contains value*/
	if (board.gameBoard[y - 1][x - 1].value != 0) {
		printf(CONTAINSVALUE);
		return;
	}
	clearTempSol();
	solvable = ILPSolver();
	/*checks if the board is solvable using ILP*/
	if (solvable != 1) {
		printf(UNSOLVABLE);
		return;
	}
	printf("Hint: set cell to %d\n", board.gameBoard[y - 1][x - 1].tempSol);
}

/*return the number of possible solutions to the current board*/
void numSolutions() {
	int numSolutions;
	/*the command is not executed if the board is erroneous*/
	if (isErroneous() == 1) {
		printf(ERRONEOUS);
		return;
	}
	/*computing the number of solutions using exhaustive backtracking*/
	numSolutions = iterativeBT();
	printf("Number of solutions: %d\n", numSolutions);
	if (numSolutions == 1) {
		printf(GOODBOARD);
	}
	if (numSolutions > 1) {
		printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
}

/*autofill all the cells that have only one valid value*/
void autofill() {
	clearAutofill();
	/*the command is not executed if the board is erroneous*/
	if (isErroneous() == 1) {
		printf(ERRONEOUS);
		return;
	}
	/*fill values in autofill field of each cell*/
	fillValuesInAutofill();
	/*copy the solution from cell.autofill to cell.value and updates undo-redo list */
	updateBoardAndList();

	unErrorBoard(); /*needed because validValue adds unnecessary errors*/
	printBoard();
	if (board.numBlanks == 0) {
		printf(SUCCESS);
		GameMode = 0;
		return;
	}
}

/*reset the board to it's initial state by undoing all moves in the undo-redo list*/
void reset() {
	while (current != head) {
		undo(0);

	}
	clearMoves();
	printf(BOARDRESET);
	printBoard();
}
/*exit the programs and frees all resources that were allocated*/
void exitGame() {
	freeResources();
	printf(EXITING);
	exit(0);
}
