/*
 * parser.h
 * This module parse a command that was received from the user.
 * If the command is valid, the command is executed.
 */

#ifndef PARSER_H_
#define PARSER_H_

#define MAXCommand 257
#define INVALIDCOMMAND "ERROR: invalid command\n"
#define MARKERROR "Error: the value should be 0 or 1\n"

/* input is String command from user
 * checks if command is valid and calls the corresponding parsing function
 */
void parse (char str[MAXCommand]);
void parseSolve ();
void parseEdit();
void parseMarkErrors();
void parsePrintBoard();
void parseSet();
void parseValidate();
void parseGenerate();
void parseSave();
void parseHint();
void parseUndo();
void parseRedo();
void parseNumSolutions();
void parseAutofill();
void parseReset();
#endif /* PARSER_H_ */
