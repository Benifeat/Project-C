

#ifndef MAMAN14_ENCODER_H
#define MAMAN14_ENCODER_H

#include "memory_utilities.h"

/* relevant in the case of a jump address
 * change the relevant bit (8 - 11) depending on the parameters */
void editValues(int value, int spesificValue, opCode *opCoded);
/* print label to .ob file */
void editLabelOpcode(symbolLine *symbolLineStart, opCode *opCoded, char *firstLabelName, int *ext_Flag);
/* print label classify to .ob file */
void editRelation(opCode *opCoded, int relation);
/* converts the string of bits into a unique code */
void changeToOpCode(opCode *opCoded, char opCodeString[14]);
/* print opcode to .ob file */
void addOp(int commandNumber, opCode *opCoded);
/* print to .ob file */
void printOpToDotObj(FILE *obFile, data_base *asmValues, opCode *tempLine);
/* change the relevant bit (0 - 3) depending on the parameters */
void addAddress(int whichOp, int address, opCode *opCoded);
/* print register to .ob file */
void editOpCodeNumbers(opCode *opCoded, int regNum, int spesificValue);
/* print numbers to .ob file */
void addNumEncode(opCode *opCoded, int num);

#endif /*MAMAN14_ENCODER_H*/
