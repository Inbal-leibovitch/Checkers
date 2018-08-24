/*
 * game.h
 *
 *  Created on: 22 ���� 2018
 *      Author: inbal
 */

#ifndef GAME_H_
#define GAME_H_

#define CALLOC "Error: calloc has failed\n"
typedef struct{
	int value;
	int fixed; 	/** fixed is 1 if value is fixed. else- 0*/
}Cell;

typedef struct{
	Cell** gameBoard;
	int markError;
	int row;
	int col;
	int N;
	int numBlanks;
	int solved;/** solved =0 if puzzle not solved, else 1 */
}Board;

Board board;

#endif /* GAME_H_ */
