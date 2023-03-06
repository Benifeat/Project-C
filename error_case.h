
#ifndef MAMAN14_ERR_MESS_H
#define MAMAN14_ERR_MESS_H

#include "defined.h"

/* print memory allocation error */
void memAllocFail(void);
/* check if we get enough arguments (>=2) and return ERROR if not */
void chkNumArgs (int args);
/* check if we can open the file and return ERROR if not */
FILE *chkFileOpen(FILE *fd, char *fileToOpen, char *mode, int *error);
/* print illegal label name error */
void illLblName(int *error, int line);
/* print line length error */
void chkLineLen(int *error, int line);
/* print long label name error */
void lngLblName(int *error, int line);
/* print illegal macro name error */
void illMcrName(int *error, int line);
/* print no space error */
void noSpace(int *error, int line);
/* print no parameter error */
void noParam(int *error, int line);
/* print space in label error */
void spaceInLbl(int *error, int line);
/* print double label name error */
void dblLblName(int *error, int line);
/* print extraneous text error */
void extraTxt(int *error, int line);
/* print undefined directive command error */
void undefDirCmd(int *error, int line);
/* print undefined instruction command error */
void undefInsCmd(int *error, int line);
/* print invalid comma error */
void invalidComma(int *error, int line);
/* print invalid dara parameters error */
void invalidDataParam(int *error, int line);
/* print invalid string error */
void invalidStr(int *error, int line);
/* print multiple comma error */
void multipleCommas(int *error, int line);
/* print missing comma error */
void missingComma(int *error, int line);
/* print irrelevant label error */
void irrLbl(int *error, int line);
/* print illegal parameter error */
void illParam(int *error, int line);
/* print invalid number parameter error */
void invalidNumParam(int *error, int line);
/* print invalid register parameter error */
void invalidRegParam(int *error, int line);
/* print exceeded data amount eror */
void exceededDataAmount(int *error);
/* print no label statement error */
void noLblStatement(int *error, int line);
/* print label cannot be entry error */
void lblCantEnt(int *error, int line);


#endif /*MAMAN14_ERR_MESS_H*/
