

#ifndef MAMAN14_ENCODER_H
#define MAMAN14_ENCODER_H

#include "memory_utilities.h"

/* relevant in the case of a jump address
 * change the relevant bit (8 - 11) depending on the parameters */
void addParam(int param, int whichParam, encoder *enc);
/* print label to .ob file */
void addLblEncode(symHead *headSymTbl, encoder *enc, char *lblName, int *ext_Flag);
/* print label classify to .ob file */
void addClassify(encoder *enc, int whichClassify);
/* converts the string of bits into a unique code */
void convertEncode(encoder *enc, char bitLineStr[14]);
/* print opcode to .ob file */
void addOp(int instNum, encoder *enc);
/* print to .ob file */
void prnObjFile(FILE *obFile, essentials *asmParam, encoder *tmpLine);
/* change the relevant bit (0 - 3) depending on the parameters */
void addAddr(int whichOper, int whichAddr, encoder *enc);
/* print register to .ob file */
void addRegEncode(encoder *enc, int regNum, int whichParam);
/* print numbers to .ob file */
void addNumEncode(encoder *enc, int num);

#endif /*MAMAN14_ENCODER_H*/
