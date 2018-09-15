/*
 * stack.c
 * this module implements a stack that is used for counting the number of solutions to the board using exhaustive backtracking
 * each stack element is (i,j,val) where i,j is the index and val is value of cell
 * each element has a pointer to it's prev element
 */
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

/*
 * uses stack to compute the number of possible solutions to a given board
 * the computation is iterative.
 */
int iterativeBT(){
	Element* current = NULL;
	int i=0, j=0, val=0, numOfSol=0;
	int flagBreak =0;
 	copyToTemp();
	push(-1,-1,0); /*initialize stack */
	while (isStackEmpty()==0){
		if (findEmptyCell()==0){ /*if board is full then a solution is found*/
			numOfSol++;
		}
		current = pop(); /*go back to prev element in stack*/
		if (current->row==-1 && current->col == -1){
			current->row++;
			current->col++;
		}
		else{
			board.gameBoard[current->row][current->col].tempSol=0;
		}
 		for (i=0; i<board.N; i++){
			for ( j=0; j<board.N; j++){
				if (board.gameBoard[i][j].tempSol==0 && board.gameBoard[i][j].value==0){
					for (val=current->value+1; val<=board.N; val++){
						if (isSafe(i,j, val)){
							board.gameBoard[i][j].tempSol=val;
							push(i,j,val);
							current->value=0;
							break;
						}
					}
					if (board.gameBoard[i][j].tempSol==0){
						flagBreak=1;
						break;
					}
 				}
 			}
			if (flagBreak==1){
				flagBreak=0;
				break;
			}
		}
 		free(current);
	}
	freeStack();
	return numOfSol;
 }

/*
 * copies values in gameboard.value to gameboard.tempSol
 */
void copyToTemp(){
	int i=0, j=0;
	for (i=0; i<board.N; i++){
		for (j=0; j<board.N; j++){
			board.gameBoard[i][j].tempSol=board.gameBoard[i][j].value;
		}
	}
}

/*
 * checks if a value z is valid in the cell x,y
 * return 1 if safe, otherwise return 0
 */
int isSafe(int x, int y, int z){
	int i=0, j=0;
	int row=0, col=0;

	/**  check row	 */
	for (i=0; i<board.N; i++){
		if (board.gameBoard[x][i].tempSol == z){
			return 0;
		}
	}

	/** check col */
	for (i=0; i<board.N; i++){
		if (board.gameBoard[i][y].tempSol == z){
			return 0;
		}
	}

	/**	 check square */
	row = ((x)/board.row)*board.row;
	col = ((y)/board.col)*board.col;
	for (i=row ; i< row+board.row; i++)
	{
		for (j=col ; j< col+board.col; j++){
			if (board.gameBoard[i][j].tempSol== z){
				return 0;
			}
		}
	}
	return 1;
}

/*
 * return 1  if found unassigned cell
 * return 0 if board is full
 */
int findEmptyCell(){
	int a=0,b=0;
	for (a=0; a < board.N; a++){
		for (b=0; b<board.N; b++){
			if (board.gameBoard[a][b].tempSol==0){
				return 1;
			}
		}
	}
	return 0;
}

/*
 * return the first element in the stack and removes it from stack
 */
Element* pop(){
	Element* temp = top;
	if (isStackEmpty()==1){
		return NULL;
	}
	top = top->prev;
	return temp;
}

/*
 * add element to the top of the stack with the values (i,j,val)
 */
void push(int i, int j, int val){

	Element* element = (Element*)malloc(sizeof(Element));
	if (element == NULL){
		printf(CALLOC);
	}
	element->row=i;
	element->col=j;
	element->value=val;

	if (isStackEmpty() == 1) {
		top = element;
		element->prev=NULL;
	}
	else{
		element->prev = top;
		top = element;
	}
}

/*
 * return 1 if stack is empty, else return 0
 * */
int isStackEmpty(){
	if (top == NULL){
		return 1;
	}
	return 0;
}
/*
 * frees all elements in the stack
 */
void freeStack(){
	Element* temp;
	while (top!=NULL){
		temp = top;
		top = top->prev;
		free(temp);
	}
}
