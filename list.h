/*
 * list.h
 *
 *  Created on: 1 בספט 2018
 *      Author: inbal
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "game.h"
#include "parser.h"

typedef struct change{
	int row;
	int col;
	Cell before;
	Cell after;
	struct change* next;
}Change;

typedef struct move{
	Change* headOfChanges;
	struct move* next;
	struct move* prev;
}Move;

Move *head;
Move *last;
Move *current;

void clearMoves();
void addMove(int x,int y,int z,Cell prevCell);
void freeChanges(Change* changes);
int isEmpty();


#endif /* LIST_H_ */
