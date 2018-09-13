/*
 * stack.c
 *
 *  Created on: 1 בספט 2018
 *      Author: inbal
 */
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "parser.h"
extern int GameMode;

void printBoardTempSol() {
	int i = 0, j = 0, k = 0;
	for (i = 0; i < board.N * 4 + board.row + 1; i++) {
		printf("-");
	}
	printf("\n");

	for (i = 0; i < board.N; i++) {
		printf("|");
		for (j = 0; j < board.N; j++) {
			printf(" ");
			if (board.gameBoard[i][j].tempSol == 0) {
				printf("  ");
			} else {
				printf("%2d", board.gameBoard[i][j].tempSol);
			}
			if (board.gameBoard[i][j].fixed == 1) {
				printf(".");
			} else if (GameMode == 2 || board.markError == 1) {
				if (board.gameBoard[i][j].error == 1) {
					printf("*");
				} else {
					printf(" ");
				}
			} else {
				printf(" ");
			}

			if (j % board.col == (board.col - 1)) {
				printf("|");
			}
		}
		printf("\n");
		if (i % board.row == (board.row - 1)) {
			for (k = 0; k < board.N * 4 + board.row + 1; k++) {
				printf("-");
			}
			printf("\n");
		}
	}
}

int iterativeBT(){
	Element* current = NULL;
	int i=0, j=0, val=0, numOfSol=0;
	int flagBreak =0;
 	copyToTemp();
 	printBoardTempSol();
	push(-1,-1,0);
	while (isStackEmpty()==0){
		if (findEmptyCell()==0){
			numOfSol++;
		}
		current = pop();
		if (current->row==-1 && current->col == -1){
			current->row++;
			current->col++;
		}
		else{
			/*printf("undo row=%d, col=%d, value=%d\n", current->row, current->col, board.gameBoard[current->row][current->col].tempSol);*/
			board.gameBoard[current->row][current->col].tempSol=0;

		}
 		for (i=0; i<board.N; i++){
 			/*printf("i=%d", i);*/
			for ( j=0; j<board.N; j++){
				/*printf("j=%d\n" ,j);*/
				if (board.gameBoard[i][j].tempSol==0 && board.gameBoard[i][j].value==0){
					for (val=current->value+1; val<=board.N; val++){
						/*printf("try val=%d\n",val);*/
						if (isSafe(i,j, val)){
							/*printf("success=%d\n",val);*/
							board.gameBoard[i][j].tempSol=val;
							/*printBoardTempSol();*/
							push(i,j,val);
							current->value=0;
							/*printf("break1");*/
							break;
						}
					}
					if (board.gameBoard[i][j].tempSol==0){
						flagBreak=1;
						/*printf("break2 i=%d, j=%d", i, j);*/
						break;
					}
 				}
 			}
			if (flagBreak==1){
				flagBreak=0;
				/*printf("break3 i=%d, j=%d", i, j);*/
				break;
			}
		}
 		free(current);
	}
	freeStack();
	return numOfSol;
 }

void copyToTemp(){
	int i=0, j=0;
	for (i=0; i<board.N; i++){
		for (j=0; j<board.N; j++){
			board.gameBoard[i][j].tempSol=board.gameBoard[i][j].value;
		}
	}
}

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

/** return 1  if found unassigned cell
 * return 0 if board is full
 *
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


Element* pop(){
	Element* temp = top;
	if (isStackEmpty()==1){
		return NULL;
	}
	top = top->prev;
	return temp;
}

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

/*return 1 if stack is empty*/
int isStackEmpty(){
	if (top == NULL){
		return 1;
	}
	return 0;
}

void freeStack(){
	Element* temp;
	while (top!=NULL){
		temp = top;
		top = top->prev;
		free(temp);
	}
}
