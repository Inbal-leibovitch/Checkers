/*
 * stack.h
 * implements a stack. each stack element is (row,col,val) where row and col are the index in the board and val is value of cell
 * each element has a pointer to it's prev element
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

/* returns the last element in stack and removes it from stack*/
Element* pop();
/* add element to stack with row=x, col=y, val=z*/
void push(int x, int y, int z);
/* returns 1 if stack is empty*/
int isStackEmpty();
/* return 1 if an empty cell is found in the board*/
int findEmptyCell();
/* return the number of solutions to the sudoku board*/
int iterativeBT();
/* copies values from value field to tempSol filed for all cells in board*/
void copyToTemp();
/* check if a value z is valid in row x and col y by sudoku rules*/
int isSafe(int x, int y, int z);
/* frees all elments in the stack*/
void freeStack();

#endif /* STACK_H_ */
