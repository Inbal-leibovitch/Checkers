/*
 * actionsUtils.h
 *
 *  Created on: 15 בספט 2018
 *      Author: Tal
 */

#ifndef ACTIONSUTILS_H_
#define ACTIONSUTILS_H_

int validateFullBoard();
void unErrorBoard();
void CheckForErrors();
int validValue(int x, int y, int z);
int isErroneous();
void randEmptyCell(int* i, int* j);
int fillXCells(int x);
void clearBoard();
void chooseYCells(int y);
void clearValue();
void clearTempSol();
void saveTofile(FILE* fp);
void updateBoardAndList();
void fillValuesInAutofill();
void clearAutofill();
void freeUndoRedo();
void freeBoard();
void freeResources();


#endif /* ACTIONSUTILS_H_ */
