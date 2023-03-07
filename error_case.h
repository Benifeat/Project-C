
#ifndef MAMAN14_ERR_MESS_H
#define MAMAN14_ERR_MESS_H

#include "defined.h"

/* print memory allocation error */
void memoryError(void);
/* check if we get enough arguments (>=2) and return ERROR if not */
void chkNumArgs (int args);
/* check if we can open the file and return ERROR if not */
FILE *fileNotCreated(FILE *fd, char *fileToOpen, char *mode, int *error);
/* print illegal label name error */
void badLabelName(int *error, int line);
/* print line length error */
void chkLineLen(int *error, int line);
/* print long label name error */
void LabelToLong(int *error, int line);
/* print illegal macro name error */
void badMcr(int *error, int line);
/* print no space error */
void noSpace(int *error, int line);
/* print no parameter error */
void noParameter(int *error, int line);
/* print space in label error */
void spacedLabel(int *error, int line);
/* print double label name error */
void existingLabelName(int *error, int line);
/* print extraneous text error */
void textOverBound(int *error, int line);
/* print undefined directive command error */
void unDefienedcommand(int *error, int line);
/* print undefined instruction command error */
void undefinedInstruction(int *error, int line);
/* print invalid comma error */
void badComma(int *error, int line);
/* print invalid dara parameters error */
void invalidDataParam(int *error, int line);
/* print invalid string error */
void badString(int *error, int line);
/* print multiple comma error */
void toManyCommas(int *error, int line);
/* print missing comma error */
void noComma(int *error, int line);
/* print irrelevant label error */
void dumpLabel(int *error, int line);
/* print illegal parameter error */
void badValue(int *error, int line);
/* print invalid number parameter error */
void badNumber(int *error, int line);
/* print invalid register parameter error */
void badRegister(int *error, int line);
/* print exceeded data amount eror */
void dataOverbound(int *error);
/* print no label statement error */
void unDefienedLabel(int *error, int line);
/* print label cannot be entry error */
void extCantBeEnt(int *error, int line);


#endif /*MAMAN14_ERR_MESS_H*/
