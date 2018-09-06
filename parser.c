/*
 * parser.c

 *
 *  Created on: 13 במאי 2018
 *      Author: inbal
 */

#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "actions.h"
#include "game.h"

extern int GameMode;
/**
 * input is String command from user
 * checks if command is valid and calls the corresponding function from game.
 */
void parse(char str[MAXCommand]) {
	char* token;
	const char s[5] = " \t\r\n";
	char* fileName;
	int x = 0, y = 0, z = 0;
	char* end;
	int mark = 0;
	/**  first token */
	token = strtok(str, s);

	if (token == NULL ) {
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< SOLVE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	if (strcmp(token, "solve") == 0) {
		token = strtok(NULL, s);
		if (token != NULL ) {
			fileName = token;
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		solve(fileName);
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< EDIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "edit") == 0) {
		token = strtok(NULL, s);
		if (token != NULL ) {
			fileName = token;
			edit(fileName);
			return;
		}
		editEmpty();
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< MARK_ERRORS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "mark_errors") == 0) {
		/******
		 * ^^^^^^^^^^^^^^^^^^^^^^^^^ gamemode: 0=init, 1=solve, 2=edit ^^^^^^^^^^^^^^^^^^^^^^^^^
		 * 							define GameMode! somewhere
		 */
		if (GameMode != 1) {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			mark = strtol(token, &end, 10);
			if (end == token) {
				printf(MARKERROR);
				return;
			}
			markErrors(mark);
			return;
		}
		/**
		 * &&&&&&&&&&&&&&& DILLEMA &&&&&&&&&&&&&&&
		 *
		 * if x is null, is it invalid command, or markerror ? &&&&&&
		 */
		printf(INVALIDCOMMAND);
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< PRINT_BOARD >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "print_board")==0) {
		if(GameMode!=0){
			printBoard();
		}
		else{
			printf(INVALIDCOMMAND);
		}
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< SET >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "set") == 0) {
		if (GameMode == 0) {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			x = strtol(token, &end, 10);
			if (end == token) {
				printErrorNotInRange();
				/**
				 *  5d in set, page 7
				 **/
				return;
			}
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			y = strtol(token, &end, 10);
			if (end == token) {
				printErrorNotInRange();
				/**
				 *  5d in set, page 7
				 **/
				return;
			}
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			z = strtol(token, &end, 10);
			if (end == token) {
				printErrorNotInRange();
				/**
				 *  5d in set, page 7
				 **/
				return;
			}
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		set(x, y, z);
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< VALIDATE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */

	else if (strcmp(token, "validate") == 0) {
		if (GameMode==0){
			printf(INVALIDCOMMAND);
			return;
		}
		validate();
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< GENERATE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "generate") == 0) {
		if (GameMode != 2) {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			x = strtol(token, &end, 10);
			if (end == token) {
				printErrorNotInRange(board.numBlanks);
				return;
			}
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			y = strtol(token, &end, 10);
			if (end == token) {
				printErrorNotInRange(board.numBlanks);
				return;
			}
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		generate(x, y);
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< UNDO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "undo") == 0) {
		if (GameMode != 0) {
			undo(1);
		} else {
			printf(INVALIDCOMMAND);
		}
		return;

	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< REDO >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "redo") == 0) {
		if (GameMode != 0) {
			redo();
		} else {
			printf(INVALIDCOMMAND);
		}
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< SAVE >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "save") == 0) {
		if (GameMode == 0) {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			fileName = token;
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		save(fileName);
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< HINT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "hint") == 0) {
		if (GameMode != 1) {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			x = strtol(token, &end, 10);
			if (end == token) {
				printErrorNotInRange();
				/**
				 * 11d in hint, page 9
				 **/

				return;
			}
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		token = strtok(NULL, s);
		if (token != NULL ) {
			y = strtol(token, &end, 10);
			if (end == token) {
				printErrorNotInRange();
				/**
				 * 11d in hint, page 9
				 **/
				return;
			}
		} else {
			printf(INVALIDCOMMAND);
			return;
		}
		hint(x, y);
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< NUM_SOLUTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "num_solutions") == 0) {
		if (GameMode == 0){
			printf(INVALIDCOMMAND);
			return;
		}
		numSolutions();
		return;
	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< AUTOFILL >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "autofill") == 0) {
		if (GameMode!=1){
			printf(INVALIDCOMMAND);
			return;
		}
		autofill();
		return;

	}
	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< RESET >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "reset") == 0) {
		if (GameMode == 0){
			printf(INVALIDCOMMAND);
			return;
		}
		reset();
		return;
	}

	/**
	 *<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< EXIT >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	 */
	else if (strcmp(token, "exit") == 0) {
		exitGame();
		return;
	}

	else {
		printf(INVALIDCOMMAND);
	}
}

