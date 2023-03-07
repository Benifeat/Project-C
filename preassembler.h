

#ifndef MAMAN14_PRE_ASM_H
#define MAMAN14_PRE_ASM_H

#include "memory_utilities.h"
#include "error_case.h"
#include "assembler.h"

/* skip the spaces and tabs in the beginning */
void no_tabs(char *pointer);
/* remove all spaces from a given string */
void no_spaces(char *pointer);
/* process to open the macros */
void openMcr(char *amFileName, char *asFileName, int *error);
/* check if we get to macro name after we past the block of this macro */
int checkMcr(mcrHead *mcr, char *line, FILE *asFile, FILE *source);
/* check if we get to macro block */
int goodMcr (char *line, char *word);

#endif /*MAMAN14_PRE_ASM_H*/
