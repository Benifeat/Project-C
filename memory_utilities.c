
#include "memory_utilities.h"

mcrHead *createMcrList(){

    mcrHead *list = malloc(sizeof(mcrHead));

    if(list == NULL)
        memoryError();

    list->head = NULL;
    return list;
}

data_base *definedMemory(){

    data_base *asmValues = malloc(sizeof(data_base));

    if(asmValues == NULL)
        memoryError();

    asmValues->DC = 0;
    asmValues->IC = 100;

    return asmValues;
}

symbolLine *createSymbolLine(){

    symbolLine *list = malloc(sizeof(symbolLine));

    if(list == NULL)
        memoryError();

    list->head = NULL;
    return list;
}

headLine *createHeadLine(){

    headLine *list = malloc(sizeof(headLine));

    if(list == NULL)
        memoryError();

    list->head = NULL;
    return list;
}

void addMcrList(mcrHead *headMcr, char name[MAX_LINE], int firstLineLocation, int amountOfLines){

    mcrList *pointer = malloc(sizeof(mcrList));
    mcrList *temp = headMcr->head;

    if (!pointer)
        memoryError();

    /* save all the parameters of the macro we want to save */
    strcpy(pointer->mcrName, name) ;
    pointer->mcrLocation = firstLineLocation;
    pointer->numberOfLines = amountOfLines;
    pointer->next = NULL;

    if(!(headMcr->head)) /* if the first macro is not install */
        headMcr->head = pointer;

    else{

        while (temp->next != NULL) /* we want to insert pointer to the last macro in the list */
            temp = temp->next;

        temp->next = pointer;
    } /* end else */
}

void addSymbolList(symbolLine *symbol, char *labelName, char *type, int IC){

    symbolList *pointer = malloc(sizeof(symbolList));
    symbolList  *temp ;

    if (!pointer)
        memoryError();

    /* save all the parameters of the Label we want to save */
    strcpy(pointer->symbolName, labelName) ;
    strcpy(pointer->mark, type);
    pointer->value = IC;
    pointer->next = NULL;

    if(!(symbol->head)) /* if the first label is not install */
        symbol->head = pointer;

    else{

        temp = symbol->head;

        while (temp->next != NULL) /* we want to insert pointer to the last label in the list */
            temp = temp->next;

        temp->next = pointer;
    } /* end else */
}

void addDataLine(headLine *headLineStart, int num, int IC){

    dataList *pointer = malloc(sizeof(dataList));
    dataList *temp = headLineStart->head;

    if(pointer == NULL)
        memoryError();

    /* save the parameter of the data line we want to save */
    pointer->data =  resultList();
    addDataValues(pointer->data, num);
    pointer->ic_start = IC;
    pointer->next = NULL;

    if (!headLineStart->head) /* if the first data line is not install */
        headLineStart->head = pointer;

    else{

        while(temp->next != NULL) /* we want to insert pointer to the last data line in the list */
            temp = temp->next;

        temp->next = pointer;
    } /* end else */
}

void addDataValues(opCode *opCoded, int num){

    int i;

    for(i = 0; i < 14; i++){

        if(num & (1<< i))
            opCoded->value |= (1 << i);
    } /* end for loop */
}

void setOpCode(opCode *opCoded){

    opCoded->value = 0;
}

opCode *resultList(){

    opCode *pointer = malloc(sizeof(opCode));

    if(!pointer)
        memoryError();

    pointer->value = 0;

    return pointer;
}

int badName(char *name){

    int i ;
    /* name of instructors, directives and registers */
    char *directives [MAX_DIRECTIVES] = {".data", ".string", ".entry", ".extern"};
    char *instructions [MAX_INSTRUCTIONS] = {"mov", "cmp", "add", "sub", "lea", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    char *registers [MAX_REGISTERS] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

    for (i = 0 ; i < MAX_INSTRUCTIONS ; i++){

        if(!strcmp(name, instructions[i]))
            return 1;
    } /* end for loop */

    for (i = 0 ; i < MAX_DIRECTIVES ; i++){

        if(!strcmp(name, directives[i]))
            return 1;
    } /* end for loop */

    for (i = 0 ; i < MAX_REGISTERS; i++){

        if(!strcmp(name, registers[i]))
            return 1;
    } /* end for loop */

    return 0;
}

void freeheadLineStart(headLine *list){

    dataList *temp = list->head;

    while(list->head != NULL){

        list->head = list->head->next;
        free(temp->data);
        free(temp);
        temp = list->head;
    } /* end while loop */

    free(list);
}

void freeMcrTbl(mcrHead *list){

    mcrList *temp = list->head;

    while(list->head != NULL){

        list->head = list->head->next;
        free(temp);
        temp = list->head;
    } /* end while loop */

    free(list);
}

void freeSymbolLine(symbolLine *list){

    symbolList *temp = list->head;

    while(list->head != NULL){

        list->head = list->head->next;
        free(temp);
        temp = list->head;
    } /* end while loop */

    free(list);
}
