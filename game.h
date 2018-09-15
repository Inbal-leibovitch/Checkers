/*
 * game.h
 *this module creates  a new board. if a file is specified than the board is filled according to the file. if no file was spcified
 *an empty board with row=3 and col=3 is created
 */

#ifndef GAME_H_
#define GAME_H_
#include <stdlib.h>
#include <stdio.h>

#define CALLOC "Error: calloc has failed\n"

/* cell struct*/
typedef struct{
	int value;
	int fixed; 	/** fixed is 1 if value is fixed. else- 0*/
	int error; /** error=1 if cell is erroneous*/
	int tempSol;
	int autofill;
}Cell;

/*board struct*/
typedef struct{
	Cell** gameBoard;
	int markError;
	int row;
	int col;
	int N;
	int numBlanks;
	int solved;/** solved =0 if puzzle not solved, else 1 */
	int BoardAllocated; /*is 0 before first board is allocated*/
}Board;

Board board;

/*creates a new board and fills it accordint to the file specified*/
void createBoard(FILE* fp);
/*creates a new empty board where row and col equal 3*/
void createEmptyBoard();


#endif /* GAME_H_ */
