

#ifndef MAMAN14_SECOND_H
#define MAMAN14_SECOND_H

#include "first_pass.h"
#include "opcode.h"

/* we copy the name of the label and its value to the entry file */
void putEntFile(FILE *entfile, symHead *symHead, char *lblName, int lineCnt, int *error);
/* we copy the name of the label and its value to the external file */
void putExtFile(FILE *extfile, char *lblName, int IC);
/* check if the label name exists in the symbol table */
int existLbl(symHead *symHead, char *lblName, int type, int *error, int lineCnt);
/* in this pass we check the label's , entry and extern, and write this on the correct file
 * we build the final table for the obj file */
void second(essentials *asmParam, symHead *headSymTbl, headData *headDataTbl, char *asFileName, char *extFileName, char *entFileName, char *obFileName, int *error, int *ext_Flag, int *ent_Flag);
/* adds the relevant data line as a bit line to the object file */
void prnDataLine(FILE *obFile, essentials *asmParam, headData *headDataTbl, int currIC);
/* save the symbol name in the label name */
void saveSym(char *line, char *lblName);
/* check if we reach to the start or end of jump address */
void jmpLbl(char *lblName, int *flag);
/* reads who is the only operand and check if the label name exist in the symbol table
 * if it is a jump address check the first label name and the two operands in the "( )"
 * adds the special bit line to the object file */
void readOp1(FILE *obFile, FILE *extFile, essentials *asmParam, symHead *headSymTbl, char *ptrLine, int instInd, int *error, int lineCnt);
/* reads the two operands are and check if the label name exist in the symbol table
 * adds the special bit line to the object file */
void readOp2(FILE *obFile, FILE *extFile, essentials *asmParam, symHead *headSymTbl, char *ptrLine, int instInd, int *error, int lineCnt);

#endif /*MAMAN14_SECOND_H*/
