
#ifndef MAMAN14_AUXILIARY_STRUCTURES_H
#define MAMAN14_AUXILIARY_STRUCTURES_H

#include "error_case.h"

/* macro parameters */
typedef struct macro{
    char mcrName[MAX_LINE];
    int mcrLocation;
    int numberOfLines;
    struct macro *next;
} mcrList;
/* macro table head pointer */
typedef struct mcrTblHead{
    mcrList *head;
}mcrHead;
/* symbol parameters */
typedef struct symbol{
    char symbolName[MAX_SYMBOL];
    int value;
    char mark[MAX_TYPE];
    struct symbol *next;
}symbolList;
/* symbol table head pointer */
typedef struct symTblHead{
    symbolList *head;
}symbolLine;
/* number of bit in the incoder - 14 */
typedef struct opCode{
    unsigned int value:14;
}opCode;
/* data table parameters */
typedef struct dataList{
    opCode *data;
    int ic_start;
    struct dataList *next;
}dataList;
/* data table head pointer */
typedef struct dataTblHead{
    dataList *head;
}headLine;
/* assembler parameters */
typedef struct esnParam{
    int IC;
    int DC;
} data_base;
/* in this function we allocate memory to head of the list of macros */
mcrHead *createMcrList();
/* in this function we allocate memory for the essential variables: IC, DC */
data_base *definedMemory();
/* in this function we allocate memory to head of the list of symbols */
symbolLine *createSymbolLine();
/* in this function we allocate memory to head of the list of data lines */
headLine *createHeadLine();
/* if we do not have an allocation error we put all the data of the macro in the macro table, else we return error */
void addMcrList(mcrHead *headMcr, char *name, int firstLineLocation, int amountOfLines);
/* if we do not have an allocation error we put all the symbol parameters in the symbol table, else we return error */
void addSymbolList(symbolLine *symbol, char *labelName, char *type, int IC);
/* add the data line */
void addDataLine(headLine *headLineStart, int num, int IC);
/* add the data parameter */
void addDataValues(opCode *opCoded, int num);
/* initilize opCode value to 0 */
void setOpCode(opCode *opCoded);
/* if we do not have an allocation error initialize all the data line parameters and return a pointer to this line */
opCode *resultList();
/* check if the name is instruction or guidance or register name, else return false */
int badName(char *name);
/* free the data table */
void freeheadLineStart(headLine *list);
/* free the macro table */
void freeMcrTbl(mcrHead *list);
/* free the symbol table */
void freeSymbolLine(symbolLine *list);


#endif /*MAMAN14_AUXILIARY_STRUCTURES_H*/
