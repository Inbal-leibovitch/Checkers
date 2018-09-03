/*
 * actions.h
 *
 *  Created on: 20 баев 2018
 *      Author: inbal
 */

#ifndef ACTIONS_H_
#define ACTIONS_H_

#include<stdlib.h>
#include "parser.h"

#define FILEERROR "Error: File doesn't exist or cannot be opened\n"
#define FIXED "Error: cell is fixed\n"
#define SUCCESS "Puzzle solved successfully\n"
#define UNDOERORR "Error: no moves to undo\n"
#define REDOERORR "Error: no moves to redo\n"
#define BOARDRESET "Board reset\n"
#define EXITING "Exiting...\n"
#define ERRONEOUS "Error: board contains erroneous values\n"
#define SOLVABLE "Validation passed: board is solvable\n"
#define UNSOLVABLE "Validation failed: board is unsolvable\n"

void solve (char* fileName);
void edit(char* fileName);
void editEmpty();
void markErrors(int x);
void printBoard();
void set(int x, int y, int z);
void printErrorNotInRange();
void validate();
void generate(int x, int y);
void printErrorEmptyCells();
void undo();
void redo();
void save(char* fileName);
void hint (int x, int y);
void numSolutions();
void autofill();
void reset();
void exitGame();
int validateFullBoard();
int validValue(int x, int y, int z);
void freeUndoRedo();
void freeBoard();
void freeStack();
void unErrorBoard();
int isErroneous();

#endif /* ACTIONS_H_ */
