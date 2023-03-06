
#ifndef MAMAN14_AUXILIARY_STRUCTURES_H
#define MAMAN14_AUXILIARY_STRUCTURES_H

#include "error_case.h"

/* macro parameters */
typedef struct macro{
    char mcrName[MAX_LINE];
    int pos_FirstMcrLine;
    int numOfLines;
    struct macro *next;
} mcrTbl;
/* macro table head pointer */
typedef struct mcrTblHead{
    mcrTbl *head;
}mcrHead;
/* symbol parameters */
typedef struct symbol{
    char symName[MAX_SYMBOL];
    int value;
    char sign[MAX_TYPE];
    struct symbol *next;
}symTbl;
/* symbol table head pointer */
typedef struct symTblHead{
    symTbl *head;
}symHead;
/* number of bit in the incoder - 14 */
typedef struct encoder{
    unsigned int param:14;
}encoder;
/* data table parameters */
typedef struct dataTbl{
    encoder *data;
    int firstIC;
    struct dataTbl *next;
}dataTbl;
/* data table head pointer */
typedef struct dataTblHead{
    dataTbl *head;
}headData;
/* assembler parameters */
typedef struct esnParam{
    int IC;
    int DC;
} essentials;
/* in this function we allocate memory to head of the list of macros */
mcrHead *crtMcrTbl();
/* in this function we allocate memory for the essential variables: IC, DC */
essentials *crtEsn();
/* in this function we allocate memory to head of the list of symbols */
symHead *crtSymTbl();
/* in this function we allocate memory to head of the list of data lines */
headData *crtDataTbl();
/* if we do not have an allocation error we put all the data of the macro in the macro table, else we return error */
void addMcrTbl(mcrHead *headMcr, char *name, int posFirstLine, int linesAmount);
/* if we do not have an allocation error we put all the symbol parameters in the symbol table, else we return error */
void addSymTbl(symHead *sym, char *labelName, char *type, int IC);
/* add the data line */
void addDataLine(headData *headDataTbl, int num, int IC);
/* add the data parameter */
void addDataParam(encoder *enc, int num);
/* initilize encoder param to 0 */
void initEncode(encoder *enc);
/* if we do not have an allocation error initialize all the data line parameters and return a pointer to this line */
encoder *crtEnc();
/* check if the name is instruction or guidance or register name, else return false */
int invalidName(char *name);
/* free the data table */
void freeDataTbl(headData *list);
/* free the macro table */
void freeMcrTbl(mcrHead *list);
/* free the symbol table */
void freeSymTbl(symHead *list);


#endif /*MAMAN14_AUXILIARY_STRUCTURES_H*/
