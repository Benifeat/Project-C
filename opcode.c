

#include "opcode.h"

void editValues(int value, int spesificValue, opCode *opCoded){

    int i;

    if(spesificValue == 1) /* value 1 */
        i = 12;
    else /* value 2 */
        i = 10;

    switch (value) {

        case 1: /* it is a label name as a parameter */
            opCoded->value |= (1 << i);
            break;

        case 2: /* it is a register as a parameter */
            opCoded->value |= (1 << (i+1)) + (1 << i);
            break;

        default:
            break;
    } /* end switch */
}

void editLabelOpcode(symbolLine *symbolLineStart, opCode *opCoded, char *firstLabelName, int *ext_Flag){

    symbolList *temp = symbolLineStart->head;
    int relation = 2; /* R - relocatable */

    while (temp != NULL){

        if(!strcmp(temp->symbolName, firstLabelName)){

            if(!strcmp(temp->mark, "ext")){

                relation = 1; /* E - external */
                *ext_Flag = 1;
            } /* end second if */

            break;
        } /* end first if */

        temp = temp->next;
    } /* end while loop */

    editRelation(opCoded, relation);
    addNumEncode(opCoded, temp->value);
}

void editRelation(opCode *opCoded, int relation){

    int i = 0;

    switch (relation) {

        case 1:
            opCoded->value |= (1 << i);
            break;

        case 2:
            opCoded->value |= (1 << (i+1));
            break;

        default:
            break;
    } /* end switch */
}

void changeToOpCode(opCode *opCoded, char opCodeString[14]){

    int i, j = 13;

    for(i = 0 ; i < 14 ; i++){

        if(opCoded->value & (1 << i))
            opCodeString[j] = '/';
        else
            opCodeString[j] = '.';

        j--;
    } /* end for loop */
}

void addOp(int commandNumber, opCode *opCoded){

    int i = 6; /* the opcode bit are 6 - 9 in extras */

    switch (commandNumber) {

        case 1: /* cmp */
            opCoded->value |= (1 << i);
            break;

        case 2: /* add */
            opCoded->value |= (1 << (i+1));
            break;

        case 3: /* sub */
            opCoded->value |= (1 << (i+1)) + (1 << i);
            break;

        case 4: /* not */
            opCoded->value |= (1 << (i+2));
            break;

        case 5: /* clr */
            opCoded->value |= (1 << (i+2)) + (1 << i);
            break;

        case 6: /* lea */
            opCoded->value |= (1 << (i+2)) + (1 << (i+1));
            break;

        case 7: /* inc */
            opCoded->value |= (1 << (i+2)) + (1 << (i+1)) + (1 << i);
            break;

        case 8: /* dec */
            opCoded->value |= (1 << (i+3));
            break;

        case 9: /* jmp */
            opCoded->value |= (1 << (i+3)) + (1 << i);
            break;

        case 10: /* bne */
            opCoded->value |= (1 << (i+3)) + (1 << (i+1));
            break;

        case 11: /* red */
            opCoded->value |= (1 << (i+3)) + (1 << (i+1)) + (1 << i);
            break;

        case 12: /* prn */
            opCoded->value |= (1 << (i+3)) + (1 << (i+2));
            break;

        case 13: /* jsr */
            opCoded->value |= (1 << (i+3)) + (1 << (i+2)) + (1 << i);
            break;

        case 14: /* rts */
            opCoded->value |= (1 << (i+3)) + (1 << (i+2)) + (1 << (i+1));
            break;

        case 15: /* stop */
            opCoded->value |= (1 << (i+3)) + (1 << (i+2)) + (1 << (i+1)) + (1 << i);
            break;

        default:
            break;
    } /* end switch */
}

void printOpToDotObj(FILE *obFile, data_base *asmValues, opCode *tempLine){

    char opCodeString[14] = {'\0'};
    changeToOpCode(tempLine, opCodeString);
    fprintf(obFile, "\t0%d\t\t%s\n", asmValues->IC, opCodeString);
}

void addAddress(int whichOp, int address, opCode *opCoded){

    int i;

    if(whichOp == 1) /* source operand */
        i = 4;
    else /* destination operand */
        i = 2;

    switch (address) {

        case 1: /* direct address */
            opCoded->value |= (1 << i);
            break;

        case 2: /* jump address */
            opCoded->value |= (1 << (i+1));
            break;

        case 3: /* direct register address */
            opCoded->value |= (1 << i) + (1 << (i+1));
            break;

        default:
            break;

    } /* end switch */
}

void editOpCodeNumbers(opCode *opCoded, int regNum, int spesificValue){

    int i;

    if (spesificValue == 1)
        i = 8;
    else
        i = 2;

    switch (regNum) {

        case 1:
            opCoded->value |= (1 << i);
            break;

        case 2:
            opCoded->value |= (1 << (i+1));
            break;

        case 3:
            opCoded->value |= (1 << (i+1)) + (1 << i);
            break;

        case 4:
            opCoded->value |= (1 << (i+2));
            break;

        case 5:
            opCoded->value |= (1 << (i+2)) + (1 << i);
            break;

        case 6:
            opCoded->value |= (1 << (i+2)) + (1 << (i+1));
            break;

        case 7:
            opCoded->value |= (1 << (i+2)) + (1 << (i+1)) + (1 << i);
            break;

        default:
            break;
    } /* end switch */
}

void addNumEncode(opCode *opCoded, int num){

    int i;

    for(i = 2; i < 14; i++){

        if(num & (1<< (i-2)))
            opCoded->value |= (1 << i);
    } /* end for loop */
}


