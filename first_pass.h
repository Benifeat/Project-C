

#ifndef MAMAN14_FIRST_H
#define MAMAN14_FIRST_H

#include "preassembler.h"
#include "memory_utilities.h"
#include "defined.h"
#include <errno.h>

/* check if we have reached a declaration of a valid symbol according to the symbol type */
int label (char *line);
/* check if the label name is not exist in the symbol table, is not equal to familiar names in the program */
int goodLabelName(symbolLine *symbol, char *labelName, int type, int *error , int countLines);
/* we add all the label name from the file to the symbol table
 * we check errors in the ".as" file and return message if we find one */
void first(char *amFileName, data_base *asmValues, symbolLine *symbol, headLine *dataListStart, int *error, int *entFlag, int *extFlag);
/* save the label name from the file */
void saveLabel(char *line, char *firstLabelName);
/* return true if the command name is the same as the word from the text file */
int checkWord(char *line, char *command);
/* read string parameters and update IC and DC */
void checkStringValues(data_base *asmValues, headLine *headLineStart, char *line, int *error, int countLines);
/* if all the parameter write right in the text file we save them in the data table, else we return error */
void checkDataValues(data_base *asmValues, headLine *headLineStart, char *line, int *error, int countLines);
/* find if the instruction in the file valid */
void checkInstructions(data_base *asmValues, headLine *headLineStart, char *line , int *error, int countLines);
/* check the instruction parameters : number, register (label name we check in the second pass */
void checkInstructionValues(data_base *asmValues, char *line , int *error, int countLines, int instructionIndex);
/* skip all characters that are not : '\t', ' ', '\0', '\n', ',' */
void skipChars(char *line);
/* check if the parameter is valid register
 * return -1 if the next character is not a number
 * return -2 if the next character is not a valid number
 * else return the number of the register */
int checkRegister(char num);
/* check if the parameter is valid number */
int checkNumber(char *line);

#endif /*MAMAN14_FIRST_H*/
