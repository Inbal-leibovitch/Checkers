/*
 * stack.h
 *
 *  Created on: 1 בספט 2018
 *      Author: inbal
 */
#include <stdlib.h>
#ifndef STACK_H_
#define STACK_H_



typedef struct element {
	int row;
	int col;
	int value;
	struct element* prev;
}Element;

Element* top;

Element* pop();
void push(int x, int y, int z);
int isStackEmpty();
int findEmptyCell();
int iterativeBT();
void copyToTemp();
int isSafe(int x, int y, int z);

#endif /* STACK_H_ */
