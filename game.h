/*
 * game.h
 *
 *  Created on: 22 баев 2018
 *      Author: inbal
 */

#ifndef GAME_H_
#define GAME_H_
#include <stdlib.h>
#include <stdio.h>

#define CALLOC "Error: calloc has failed\n"
typedef struct{
	int value;
	int fixed; 	/** fixed is 1 if value is fixed. else- 0*/
	int error; /** error=1 if cell is erroneous*/
	int tempSol;
	int autofill;
}Cell;

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

void createBoard(FILE* fp);
void createEmptyBoard();


#endif /* GAME_H_ */
