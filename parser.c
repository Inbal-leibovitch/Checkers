/*
 * parser.c
 * This module parse a command that was received from the user.
 * If the command is valid, the command is executed.
 */

#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "actions.h"
#include "game.h"

extern int GameMode;

/* input is String command from user
 * checks if command is valid and calls the corresponding parsing function
 */
void parse(char str[MAXCommand]) {
	char* token;
	const char s[5] = " \t\r\n";

	/**  first token */
	token = strtok(str, s);

	if (token == NULL ) {
		return;
	}
	if (strcmp(token, "solve") == 0) {
		parseSolve(str);
		return;
	} else if (strcmp(token, "edit") == 0) {
		parseEdit();
		return;
	} else if (strcmp(token, "mark_errors") == 0) {
		parseMarkErrors();
		return;
	} else if (strcmp(token, "print_board") == 0) {
		parsePrintBoard();
		return;
	} else if (strcmp(token, "set") == 0) {
		parseSet();
		return;
	} else if (strcmp(token, "validate") == 0) {
		parseValidate();
		return;
	} else if (strcmp(token, "generate") == 0) {
		parseGenerate();
		return;
	} else if (strcmp(token, "undo") == 0) {
		parseUndo();
		return;
	} else if (strcmp(token, "redo") == 0) {
		parseRedo();
		return;
	} else if (strcmp(token, "save") == 0) {
		parseSave();
		return;
	} else if (strcmp(token, "hint") == 0) {
		parseHint();
		return;
	} else if (strcmp(token, "num_solutions") == 0) {
		parseNumSolutions();
		return;
	} else if (strcmp(token, "autofill") == 0) {
		parseAutofill();
		return;
	} else if (strcmp(token, "reset") == 0) {
		parseReset();
		return;
	} else if (strcmp(token, "exit") == 0) {
		exitGame();
		return;
	} else {
		printf(INVALIDCOMMAND);
	}
}
/*parse solve command*/
void parseSolve() {
	char* fileName;
	char* token;
	const char s[5] = " \t\r\n";
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
/*parse edit command*/
void parseEdit() {
	char* fileName;
	char* token;
	const char s[5] = " \t\r\n";
	token = strtok(NULL, s);
	if (token != NULL ) {
		fileName = token;
		edit(fileName);
		return;
	}
	editEmpty();
	return;
}

/*parse mark_errors command*/
void parseMarkErrors() {
	char* token;
	const char s[5] = " \t\r\n";
	char* end;
	int mark = 0;
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
	printf(INVALIDCOMMAND);
	return;
}

/*parse print_board command*/
void parsePrintBoard() {
	if (GameMode != 0) {
		printBoard();
	} else {
		printf(INVALIDCOMMAND);
	}
	return;
}

/*parse set command*/
void parseSet() {
	char* token;
	const char s[5] = " \t\r\n";
	int x = 0, y = 0, z = 0;
	char* end;
	int notInRange = 0; /*notInRange is 1 if x, y or z are not in range*/
	if (GameMode == 0) {
		printf(INVALIDCOMMAND);
		return;
	}
	token = strtok(NULL, s);
	if (token != NULL ) {
		x = strtol(token, &end, 10);
		if (end == token) {
			notInRange = 1;
		}
	} else {
		printf(INVALIDCOMMAND);
		return;
	}
	token = strtok(NULL, s);
	if (token != NULL ) {
		y = strtol(token, &end, 10);
		if (end == token) {
			notInRange = 1;
		}
	} else {
		printf(INVALIDCOMMAND);
		return;
	}
	token = strtok(NULL, s);
	if (token != NULL ) {
		z = strtol(token, &end, 10);
		if (end == token) {
			notInRange = 1;
		}
	} else {
		printf(INVALIDCOMMAND);
		return;
	}
	if (notInRange == 1) {
		printErrorNotInRange(board.N);
		return;
	}
	set(x, y, z);
	return;
}

/*parse validate command*/
void parseValidate() {
	if (GameMode == 0) {
		printf(INVALIDCOMMAND);
		return;
	}
	validate();
	return;
}

/*parse generate command*/
void parseGenerate() {
	char* token;
	const char s[5] = " \t\r\n";
	int x = 0, y = 0;
	char* end;
	int notInRange = 0; /*notInRange is 1 if x, y or z are not in range*/
	if (GameMode != 2) {
		printf(INVALIDCOMMAND);
		return;
	}
	token = strtok(NULL, s);
	if (token != NULL ) {
		x = strtol(token, &end, 10);
		if (end == token) {
			notInRange = 1;
		}
	} else {
		printf(INVALIDCOMMAND);
		return;
	}
	token = strtok(NULL, s);
	if (token != NULL ) {
		y = strtol(token, &end, 10);
		if (end == token) {
			notInRange = 1;
		}
	} else {
		printf(INVALIDCOMMAND);
		return;
	}
	if (notInRange == 1) {
		printErrorNotInRange(board.numBlanks);
		return;
	}
	generate(x, y);
	return;
}

/*parse undo command*/
void parseUndo() {
	if (GameMode != 0) {
		undo(1);
	} else {
		printf(INVALIDCOMMAND);
	}
	return;
}

/*parse redo command*/
void parseRedo() {
	if (GameMode != 0) {
		redo();
	} else {
		printf(INVALIDCOMMAND);
	}
	return;
}

/*parse save command*/
void parseSave() {
	char* fileName;
	char* token;
	const char s[5] = " \t\r\n";
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

/*parse hint command*/
void parseHint() {
	char* token;
	const char s[5] = " \t\r\n";
	int x = 0, y = 0;
	char* end;
	int notInRange = 0; /*notInRange is 1 if x, y or z are not in range*/
	if (GameMode != 1) {
		printf(INVALIDCOMMAND);
		return;
	}
	token = strtok(NULL, s);
	if (token != NULL ) {
		x = strtol(token, &end, 10);
		if (end == token) {
			notInRange = 1;

		}
	} else {
		printf(INVALIDCOMMAND);
		return;
	}
	token = strtok(NULL, s);
	if (token != NULL ) {
		y = strtol(token, &end, 10);
		if (end == token) {
			notInRange = 1;

		}
	} else {
		printf(INVALIDCOMMAND);
		return;
	}
	if (notInRange == 1) {
		printf("Error: value not in range 1-%d\n", board.N);
		return;
	}
	hint(x, y);
	return;
}

/*parse num_solutions command*/
void parseNumSolutions() {
	if (GameMode == 0) {
		printf(INVALIDCOMMAND);
		return;
	}
	numSolutions();
	return;
}

/*parse autofill command*/
void parseAutofill() {
	if (GameMode != 1) {
		printf(INVALIDCOMMAND);
		return;
	}
	autofill();
	return;
}

/*parse reset command*/
void parseReset() {
	if (GameMode == 0) {
		printf(INVALIDCOMMAND);
		return;
	}
	reset();
	return;
}

