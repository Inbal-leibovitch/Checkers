/*
 * This module contains functions that executes all the commands received from the user.
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
#define GOODBOARD "This is a good board!\n"
#define CONTAINSVALUE "Error: cell already contains a value\n"
#define VALIDATIONFAILED "Error: board validation failed\n"

/* load a game from the file and start it in solve mode*/
void solve (char* fileName);
/* load a game from the file and start it in edit mode*/
void edit(char* fileName);
/*load empty board and start it in edit mode*/
void editEmpty();
/* change the value of mark_errors to the value of x*/
void markErrors(int x);
/*print the board*/
void printBoard();
/*set value z to cell x,y, if z is a valid value*/
void set(int x, int y, int z);
/* validate the board is solvable */
void validate();
/*generate new board from an empty board*/
void generate(int x, int y);
/*undo the last move*/
void undo();
/*redo a move*/
void redo();
/*save the board to a new file */
void save(char* fileName);
/*prints hint to cell x,y */
void hint (int x, int y);
/*prints the number of possible solutions to the current board */
void numSolutions();
/*autofill all the empty cells that have only one valid value */
void autofill();
/*reset the board to it's initial state*/
void reset();
/*exit the game*/
void exitGame();


#endif /* ACTIONS_H_ */
