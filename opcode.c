

#include "opcode.h"

void addParam(int param, int whichParam, encoder *enc){

    int i;

    if(whichParam == 1) /* param 1 */
        i = 12;
    else /* param 2 */
        i = 10;

    switch (param) {

        case 1: /* it is a label name as a parameter */
            enc->param |= (1 << i);
            break;

        case 2: /* it is a register as a parameter */
            enc->param |= (1 << (i+1)) + (1 << i);
            break;

        default:
            break;
    } /* end switch */
}

void addLblEncode(symHead *headSymTbl, encoder *enc, char *lblName, int *ext_Flag){

    symTbl *tmp = headSymTbl->head;
    int whichClassify = 2; /* R - relocatable */

    while (tmp != NULL){

        if(!strcmp(tmp->symName, lblName)){

            if(!strcmp(tmp->sign, "ext")){

                whichClassify = 1; /* E - external */
                *ext_Flag = 1;
            } /* end second if */

            break;
        } /* end first if */

        tmp = tmp->next;
    } /* end while loop */

    addClassify(enc, whichClassify);
    addNumEncode(enc, tmp->value);
}

void addClassify(encoder *enc, int whichClassify){

    int i = 0;

    switch (whichClassify) {

        case 1:
            enc->param |= (1 << i);
            break;

        case 2:
            enc->param |= (1 << (i+1));
            break;

        default:
            break;
    } /* end switch */
}

void convertEncode(encoder *enc, char bitLineStr[14]){

    int i, j = 13;

    for(i = 0 ; i < 14 ; i++){

        if(enc->param & (1 << i))
            bitLineStr[j] = '/';
        else
            bitLineStr[j] = '.';

        j--;
    } /* end for loop */
}

void addOp(int instNum, encoder *enc){

    int i = 6; /* the opcode bit are 6 - 9 in extras */

    switch (instNum) {

        case 1: /* cmp */
            enc->param |= (1 << i);
            break;

        case 2: /* add */
            enc->param |= (1 << (i+1));
            break;

        case 3: /* sub */
            enc->param |= (1 << (i+1)) + (1 << i);
            break;

        case 4: /* not */
            enc->param |= (1 << (i+2));
            break;

        case 5: /* clr */
            enc->param |= (1 << (i+2)) + (1 << i);
            break;

        case 6: /* lea */
            enc->param |= (1 << (i+2)) + (1 << (i+1));
            break;

        case 7: /* inc */
            enc->param |= (1 << (i+2)) + (1 << (i+1)) + (1 << i);
            break;

        case 8: /* dec */
            enc->param |= (1 << (i+3));
            break;

        case 9: /* jmp */
            enc->param |= (1 << (i+3)) + (1 << i);
            break;

        case 10: /* bne */
            enc->param |= (1 << (i+3)) + (1 << (i+1));
            break;

        case 11: /* red */
            enc->param |= (1 << (i+3)) + (1 << (i+1)) + (1 << i);
            break;

        case 12: /* prn */
            enc->param |= (1 << (i+3)) + (1 << (i+2));
            break;

        case 13: /* jsr */
            enc->param |= (1 << (i+3)) + (1 << (i+2)) + (1 << i);
            break;

        case 14: /* rts */
            enc->param |= (1 << (i+3)) + (1 << (i+2)) + (1 << (i+1));
            break;

        case 15: /* stop */
            enc->param |= (1 << (i+3)) + (1 << (i+2)) + (1 << (i+1)) + (1 << i);
            break;

        default:
            break;
    } /* end switch */
}

void prnObjFile(FILE *obFile, essentials *asmParam, encoder *tmpLine){

    char bitLineStr[14] = {'\0'};
    convertEncode(tmpLine, bitLineStr);
    fprintf(obFile, "\t0%d\t\t%s\n", asmParam->IC, bitLineStr);
}

void addAddr(int whichOper, int whichAddr, encoder *enc){

    int i;

    if(whichOper == 1) /* source operand */
        i = 4;
    else /* destination operand */
        i = 2;

    switch (whichAddr) {

        case 1: /* direct address */
            enc->param |= (1 << i);
            break;

        case 2: /* jump address */
            enc->param |= (1 << (i+1));
            break;

        case 3: /* direct register address */
            enc->param |= (1 << i) + (1 << (i+1));
            break;

        default:
            break;

    } /* end switch */
}

void addRegEncode(encoder *enc, int regNum, int whichParam){

    int i;

    if (whichParam == 1)
        i = 8;
    else
        i = 2;

    switch (regNum) {

        case 1:
            enc->param |= (1 << i);
            break;

        case 2:
            enc->param |= (1 << (i+1));
            break;

        case 3:
            enc->param |= (1 << (i+1)) + (1 << i);
            break;

        case 4:
            enc->param |= (1 << (i+2));
            break;

        case 5:
            enc->param |= (1 << (i+2)) + (1 << i);
            break;

        case 6:
            enc->param |= (1 << (i+2)) + (1 << (i+1));
            break;

        case 7:
            enc->param |= (1 << (i+2)) + (1 << (i+1)) + (1 << i);
            break;

        default:
            break;
    } /* end switch */
}

void addNumEncode(encoder *enc, int num){

    int i;

    for(i = 2; i < 14; i++){

        if(num & (1<< (i-2)))
            enc->param |= (1 << i);
    } /* end for loop */
}


