

#ifndef MAMAN14_PRE_ASM_H
#define MAMAN14_PRE_ASM_H

#include "memory_utilities.h"
#include "error_case.h"
#include "assembler.h"

/* skip the spaces and tabs in the beginning */
void skipTabSpace(char *p);
/* remove all spaces from a given string */
void remSpaces(char *p);
/* process to open the macros */
void openMcr(char *amFileName, char *asFileName, int *error);
/* check if we get to macro name after we past the block of this macro */
int chkMcrName(mcrHead *mcr, char *line, FILE *asFile, FILE *source);
/* check if we get to macro block */
int chkWord (char *line, char *word);

#endif /*MAMAN14_PRE_ASM_H*/
