

#ifndef MAMAN14_SECOND_H
#define MAMAN14_SECOND_H

#include "first_pass.h"
#include "opcode.h"

/* we copy the name of the label and its value to the entry file */
void editDotEnt(FILE *entfile, symbolLine *symbolLine, char *firstLabelName, int countLines, int *error);
/* we copy the name of the label and its value to the external file */
void editDotExt(FILE *extfile, char *firstLabelName, int IC);
/* check if the label name exists in the symbol table */
int lastLabelChecks(symbolLine *symbolLine, char *firstLabelName, int type, int *error, int countLines);
/* in this pass we check the label's , entry and extern, and write this on the correct file
 * we build the final table for the obj file */
void second(data_base *asmValues, symbolLine *symbolLineStart, headLine *headLineStart, char *asFileName, char *extFileName, char *entFileName, char *obFileName, int *error, int *ext_Flag, int *ent_Flag);
/* adds the relevant data line as a bit line to the object file */
void editDotObFile(FILE *obFile, data_base *asmValues, headLine *headLineStart, int currentIC);
/* save the symbol name in the label name */
void saveLabelToSymbol(char *line, char *firstLabelName);
/* check if we reach to the start or end of jump address */
void jumpToLabelAddress(char *firstLabelName, int *flag);
/* reads who is the only operand and check if the label name exist in the symbol table
 * if it is a jump address check the first label name and the two operands in the "( )"
 * adds the special bit line to the object file */
void getOp1(FILE *obFile, FILE *extFile, data_base *asmValues, symbolLine *symbolLineStart, char *ptrLine, int commandIndex, int *error, int countLines);
/* reads the two operands are and check if the label name exist in the symbol table
 * adds the special bit line to the object file */
void getOp2(FILE *obFile, FILE *extFile, data_base *asmValues, symbolLine *symbolLineStart, char *ptrLine, int commandIndex, int *error, int countLines);

#endif /*MAMAN14_SECOND_H*/
