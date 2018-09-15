/*
 * mainUtils.h
 * functions used by main.c
 */

#ifndef MAINUTILS_H_
#define MAINUTILS_H_

/* return 1 if EOF is found*/
int checkForNewLine(char str[MAXCommand]);
/* sets all chars in str to 0*/
void clearSTR(char* str);

#endif /* MAINUTILS_H_ */
