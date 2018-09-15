/*
 * actionsUtils.c
 *
 *this module contains functions that are used by the actions.c module
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

/**
 * checks all cell in board if they are valid with validValue function
 * return 1 if board is valid and 0 if it is not
 */
int validateFullBoard() {

	int i = 0, j = 0, tempValue;
	int valid = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			tempValue = board.gameBoard[i][j].value;
			if (tempValue != 0) {
				board.gameBoard[i][j].value = 0;
				valid = validValue(j + 1, i + 1, tempValue);
				board.gameBoard[i][j].value = tempValue;
				if (valid == 0) {
					return 0;
				}
			}
		}
	}
	return 1;
}

/*
 * checks if there are any cell in the board that are marked as error and they are not
 * if such cell is found then set error to 0
 */
void unErrorBoard() {
	int i = 0, j = 0, value;
	int valid = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			value = board.gameBoard[i][j].value;
			if (value != 0) {
				board.gameBoard[i][j].value = 0;
				valid = validValue(j + 1, i + 1, value);
				board.gameBoard[i][j].value = value;
				if (valid == 1) {
					board.gameBoard[i][j].error = 0;
				}
			} else {
				board.gameBoard[i][j].error = 0;
			}
		}
	}
	return;
}
/**
 * check board for cells that are erronous and not marked with error,
 * if such cell if found set error to 1
 */
void CheckForErrors() {
	int i = 0, j = 0, value;
	int valid = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			value = board.gameBoard[i][j].value;
			if (value != 0) {
				board.gameBoard[i][j].value = 0;
				valid = validValue(j + 1, i + 1, value);
				board.gameBoard[i][j].value = value;
				if (valid == 0) {
					board.gameBoard[i][j].error = 1;
				}
			} else {
				board.gameBoard[i][j].error = 0;
			}
		}
	}
	return;
}

/*
 * check if the value z is valid in cell with row=y col=x
 * returns 1 if the value is valid, else return 0.
  */
int validValue(int x, int y, int z) {
	int i = 0, j = 0;
	int row = 0, col = 0;
	int foundConflict = 0;

	/** check row */
	for (i = 0; i < board.N; i++) {
		if (board.gameBoard[y - 1][i].value == z) {
			foundConflict = 1;
			if (board.gameBoard[y - 1][i].fixed == 0) {
				board.gameBoard[y - 1][i].error = 1;
			}
		}
	}
	/** check col */
	for (i = 0; i < board.N; i++) {
		if (board.gameBoard[i][x - 1].value == z) {
			foundConflict = 1;
			if (board.gameBoard[i][x - 1].fixed == 0) {
				board.gameBoard[i][x - 1].error = 1;
			}
		}
	}
	/**  check square */
	row = ((y - 1) / board.row) * board.row;
	col = ((x - 1) / board.col) * board.col;
	for (i = row; i < row + board.row; i++) {
		for (j = col; j < col + board.col; j++) {
			if (board.gameBoard[i][j].value == z) {
				foundConflict = 1;
				if (board.gameBoard[i][j].fixed == 0) {
					board.gameBoard[i][j].error = 1;
				}
			}
		}
	}
	if (foundConflict == 1) {
		return 0;
	}
	return 1;
}

/*
 * check if there is a cell with error in board
 * return 1 if board an erroneous cell is found
 */
int isErroneous() {
	int i = 0, j = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			if (board.gameBoard[i][j].error == 1) {
				return 1;
			}
		}
	}
	return 0;
}

/*
 * print error message with range X
 */
void printErrorNotInRange(int X) {
	printf("Error: value not in range 0-%d\n", X);
}

/*
 * changes pointers i and j to point to a random row and col
 *  that do not have a value in the board yet
 */
void randEmptyCell(int* i, int* j) {
	do {
		*i = rand() % board.N;
		*j = rand() % board.N;
	} while (board.gameBoard[*i][*j].value != 0);
}

/*
 * chooses X random cell and fills them with random values
 * return 1 if all cell were filled
 * 0 if a cell has no legal value available
 */
int fillXCells(int x) {
	int i = 0, j = 0, numOfOptions = 0, chosen = 0, index = 0, k = 0;
	int num;
	int* options;

	for (k = 0; k < x; k++) {
		randEmptyCell(&i, &j);

		/**options is an array the size of board.N. if cell i is 1 then i is a valid option.*/
		options = calloc(board.N, sizeof(int));
		for (num = 1; num <= board.N; num++) {
			if (validValue(j + 1, i + 1, num) == 1) {
				options[num - 1]++;
				numOfOptions++;
			}
		}
		if (numOfOptions == 0) {
			return 0; /*cell has no legal value*/
		}
		chosen = rand() % (numOfOptions);
		num = 0;
		for (index = 0; index < board.N; index++) {
			if (options[index] == 1) {
				num++;
			}
			if (num == chosen + 1) {
				break;
			}
		}
		board.gameBoard[i][j].value = index + 1;
		free(options);
	}
	return 1;
}

/*
 * clears all value and tempSol fields in cells to 0
 */
void clearBoard() {
	int i = 0, j = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			board.gameBoard[i][j].value = 0;
			board.gameBoard[i][j].tempSol = 0;
		}
	}
}

/*
 * choose Y cells randomly and clears all other cells
 * saves the values of these Y cells and add them to a new move for the
 * undo/redo list
 */
void chooseYCells(int y) {
	int k = 0, i = 0, j = 0;
	Change* currentChange = NULL;
	Change* temp = NULL;
	Move* move = NULL;
	clearValue();
	if (y == 0) {
		return;
	}
	for (k = 0; k < y; k++) {
		randEmptyCell(&i, &j);
		board.gameBoard[i][j].value = board.gameBoard[i][j].tempSol;
		board.numBlanks--;
		if (move == NULL ) { /*if a new move was not allocated yet*/
			move = (Move*) malloc(sizeof(Move));
			if (move == NULL ) {
				printf(CALLOC);
				return;
			}
			move->headOfChanges = NULL;
		}
		temp = (Change*) malloc(sizeof(Change));
		if (temp == NULL ) {
			printf(CALLOC);
			return;
		}
		temp->row = i;
		temp->col = j;
		/*the values of the cell before generate*/
		temp->before.autofill = 0;
		temp->before.error = 0;
		temp->before.fixed = 0;
		temp->before.tempSol = 0;
		temp->before.value = 0;
		/*the values of the cell after generate*/
		temp->after.autofill = 0;
		temp->after.error = 0;
		temp->after.fixed = 0;
		temp->after.tempSol = 0;
		temp->after.value = board.gameBoard[i][j].value;

		temp->next = NULL;
		/*add the change to list of changes in the move*/
		if (move->headOfChanges == NULL ) {
			move->headOfChanges = temp;
			currentChange = temp;
		} else {
			currentChange->next = temp;
			currentChange = temp;
		}
	}
	/*add move to list of moves*/
	if (move != NULL ) {
		clearMoves(); /*clear rest of moves list*/
		if (isEmpty() == 1) {
			head->next = move;
			move->prev = head;
		} else {
			last->next = move;
			move->prev = last;
		}
		move->next = NULL;
		current = move;
		last = current;
	}
}
/*
 * sets all cells value in board to 0
 */
void clearValue() {
	int i = 0, j = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			board.gameBoard[i][j].value = 0;
		}
	}
}
/*
 * sets all cells tempSol in board to 0
 */
void clearTempSol() {
	int i = 0, j = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			board.gameBoard[i][j].tempSol = 0;
		}
	}
}

/*
 *saves the current board to a file in the specified format
 *if in edit mode all cells are marked as fixed.
 */
void saveTofile(FILE* fp) {
	int i = 0, j = 0;
	fputc(board.row + '0', fp);
	fputc(' ', fp);
	fputc(board.col + '0', fp);
	fputc('\n', fp);
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			fputc(board.gameBoard[i][j].value + '0', fp);
			if (board.gameBoard[i][j].fixed == 1
					|| (GameMode == 2 && board.gameBoard[i][j].value != 0)) {
				fputc('.', fp);
			}
			fputc(' ', fp);
		}
		if (i != board.N - 1) {
			fputc('\n', fp);
		}
	}
	fclose(fp);
}

/*
 * copies values from autofill field in cells to value field
 * and creates the list of changes and a new move to add to the undo/redo list
 */
void updateBoardAndList() {
	int i = 0, j = 0;
	Change* currentChange = NULL;
	Change* temp = NULL;
	Move* move = NULL;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			if (board.gameBoard[i][j].value == 0
					&& board.gameBoard[i][j].autofill != 0) {
				board.gameBoard[i][j].value = board.gameBoard[i][j].autofill;
				board.gameBoard[i][j].autofill = 0;
				board.numBlanks--;
				if (move == NULL ) {
					move = (Move*) malloc(sizeof(Move));
					if (move == NULL ) {
						printf(CALLOC);
						return;
					}
					move->headOfChanges = NULL;
					move->prev = NULL;
					move->next = NULL;
				}
				temp = (Change*) malloc(sizeof(Change));
				if (temp == NULL ) {
					printf(CALLOC);
					return;
				}
				temp->row = i;
				temp->col = j;

				temp->before.autofill = 0;
				temp->before.error = 0;
				temp->before.fixed = 0;
				temp->before.tempSol = 0;
				temp->before.value = 0;

				temp->after.autofill = 0;
				temp->after.error = 0;
				temp->after.fixed = 0;
				temp->after.tempSol = 0;
				temp->after.value = board.gameBoard[i][j].value;

				temp->next = NULL;

				if (move->headOfChanges == NULL ) {
					move->headOfChanges = temp;
					currentChange = temp;
				} else {
					currentChange->next = temp;
					currentChange = temp;
				}
			}
		}
	}
	if (move != NULL ) {
		clearMoves(); /*clear rest of moves list*/
		if (isEmpty() == 1) {
			head->next = move;
			move->prev = head;
		} else {
			last->next = move;
			move->prev = last;
		}
		move->next = NULL;
		current = move;
		last = current;
	}
}
/*
 * checks for each cell if there is only one number that is valid for that cell
 * if only one value is valid then it sets autofill field in that cell to the value
 */
void fillValuesInAutofill() {
	int i = 0, j = 0, k = 0, value = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			if (board.gameBoard[i][j].value == 0) {
				for (k = 1; k <= board.N; k++) {
					if (validValue(j + 1, i + 1, k) == 1) {
						if (value == 0) {
							value = k;
						} else {
							value = -1;
						}
					}
				}
				if (value != -1 && value != 0) {
					board.gameBoard[i][j].autofill = value;
					printf("Cell <%d,%d> set to %d\n", j + 1, i + 1, value);

				}
				value = 0;
			}
		}
	}
}

/*
 * sets all autofill fields in all cells in the board to 0
 */
void clearAutofill() {
	int i = 0, j = 0;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			board.gameBoard[i][j].autofill = 0;
		}
	}
}

/*
 * frees all moves in the undo/redo list and all changes in every move.
 */
void freeUndoRedo() {
	while (last != head) {
		Move* temp = last;
		Change* tempChange = temp->headOfChanges;
		freeChanges(tempChange);
		last = last->prev;
		last->next = NULL;
		free(temp);
	}
	if (head != NULL ) {
		free(head);
	}
}

/*
 * frees all rows in board.gameBoard and also frees board.gameBoard
 */
void freeBoard() {
	int i;
	if (board.BoardAllocated == 1) {
		for (i = 0; i < board.N; i++) {
			free(board.gameBoard[i]);
		}
		free(board.gameBoard);
	}
}

/*
 * frees board resources and undo/redo list
 */
void freeResources() {
	freeUndoRedo();
	freeBoard();
}

