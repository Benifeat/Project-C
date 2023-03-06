

#ifndef MAMAN14_FIRST_H
#define MAMAN14_FIRST_H

#include "preassembler.h"
#include "memory_utilities.h"
#include "defined.h"
#include <errno.h>

/* check if we have reached a declaration of a valid symbol according to the symbol type */
int isLbl (char *line);
/* check if the label name is not exist in the symbol table, is not equal to familiar names in the program */
int validLblName(symHead *sym, char *labelName, int type, int *error , int lineCnt);
/* we add all the label name from the file to the symbol table
 * we check errors in the ".as" file and return message if we find one */
void first(char *amFileName, essentials *asmParam, symHead *sym, headData *dataTblLineHead, int *error, int *entFlag, int *extFlag);
/* save the label name from the file */
void saveLblName(char *line, char *lblName);
/* return true if the command name is the same as the word from the text file */
int checkWord(char *line, char *command);
/* read string parameters and update IC and DC */
void readStrParam(essentials *asmParam, headData *headDataTbl, char *line, int *error, int lineCnt);
/* if all the parameter write right in the text file we save them in the data table, else we return error */
void readDataParam(essentials *asmParam, headData *headDataTbl, char *line, int *error, int lineCnt);
/* find if the instruction in the file valid */
void readInst(essentials *asmParam, headData *headDataTbl, char *line , int *error, int lineCnt);
/* check the instruction parameters : number, register (label name we check in the second pass */
void checkInstParam(essentials *asmParam, char *line , int *error, int lineCnt, int instructInd);
/* skip all characters that are not : '\t', ' ', '\0', '\n', ',' */
void skipChars(char *line);
/* check if the parameter is valid register
 * return -1 if the next character is not a number
 * return -2 if the next character is not a valid number
 * else return the number of the register */
int chkReg(char num);
/* check if the parameter is valid number */
int chkNum(char *line);

#endif /*MAMAN14_FIRST_H*/
