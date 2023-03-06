

#ifndef MAMAN14_ASM_H
#define MAMAN14_ASM_H

#include "first_pass.h"
#include "second_pass.h"
#include "memory_utilities.h"

/* check if the file is empty - check if we have prompt lines */
void removeEmptyFile(char *filename);
/* change the ending of the file name */
void createFile (char *newFileName, char *currentFile, char *endFileName);
/* manages the transitions and runs the program and there is an error and exits the program */
void process(char * currentFile);

#endif /*MAMAN14_ASM_H*/
