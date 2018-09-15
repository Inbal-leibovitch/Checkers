/*
 * actionsUtils.h
 *this module contains functions that are used by the actions.c module
 */

#ifndef ACTIONSUTILS_H_
#define ACTIONSUTILS_H_
/*returns 1 if board is valid, else 0*/
int validateFullBoard();
/*remove unnecessary errors from board */
void unErrorBoard();
/*check board for errouneous cell that are not marked as erronous and marks them*/
void CheckForErrors();
/*check if value z is valid in row=y col=x*/
int validValue(int x, int y, int z);
/*return 1 if there is a cell with error in board, else return 0*/
int isErroneous();
/*changes pointers i and j to point to a random row and col
 *  that do not have a value in the board yet*/
void randEmptyCell(int* i, int* j);
/*print message not in range X*/
void printErrorNotInRange(int X);
/*chooses X random cell and fills them with random values, return 1 if all cell were filled and 0 if a cell has no legal value available*/
int fillXCells(int x);
/*clears all value and tempSol fields in cells to 0*/
void clearBoard();
/*choose Y cells randomly and clears all other cells
 * saves the values of these Y cells and add them to a new move for the
 * undo/redo list*/
void chooseYCells(int y);
/*ets all cells value in board to 0*/
void clearValue();
/*sets all cells tempSol in board to 0*/
void clearTempSol();
/*saves the board to a file in the specified format*/
void saveTofile(FILE* fp);
/*copies values from autofill field in cells to value field*/
void updateBoardAndList();
/*checks for each cell if there is only one number that is valid for that cell, and add it*/
void fillValuesInAutofill();
/*sets all cells autofill in board to 0*/
void clearAutofill();
/*frees undo/redo list*/
void freeUndoRedo();
/*frees board*/
void freeBoard();
/*frees board and undo/redo list*/
void freeResources();


#endif /* ACTIONSUTILS_H_ */
