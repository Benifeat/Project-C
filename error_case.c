
#include "error_case.h"

void memoryError(void){

    printf("ERROR : You have memory allocation error.\n");
    exit(0);
}

void chkNumArgs (int args){
    if (args < 2){
        printf("ERROR : You need to write at list one file name.\n ");
        exit(0);
    } /* end if */
}

FILE *fileNotCreated(FILE *fd, char *fileToOpen, char *mode, int *error){

    if (!(fd = fopen(fileToOpen, mode))) {

        printf("ERROR : the file %s is not open.\n", fileToOpen);
        *error = 1;
        return NULL;
    } /* end if */
    return fd;
}

void badLabelName(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("The label name is the name of a register or directive or instruction command\n");
    printf("or the label has characters other than A-Z or a-z or 0-9 \n\n");
    *error = 1;
}

void chkLineLen(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("The line length is longer than 81 characters.\n\n");
    *error = 1;
}

void LabelToLong(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("The label name is longer than 30 characters.\n\n");
    *error = 1;
}

void badMcr(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("The macro name is the name of a register or directive or instruction command\n");
    printf("or the macro has characters other than A-Z or a-z or 0-9\n\n");
    *error = 1;
}

void noSpace(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("There is no space or tab after command word, before the parameters.\n\n");
    *error = 1;
}

void noParameter(int * error, int line){

    printf("Error in line : %d\n", line);
    printf("You didn't insert parameters after command word.\n\n");
    *error = 1;
}

void spacedLabel(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("There are spaces in the label statement.\n\n");
    *error = 1;
}

void existingLabelName(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Two different statements about a label with the same label name.\n\n");
    *error = 1;
}

void textOverBound(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Extraneous text.\n\n");
    *error = 1;
}

void unDefienedcommand(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Undefined directive command.\n\n");
    *error = 1;
}

void undefinedInstruction(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Undefined instruction command.\n\n");
    *error = 1;
}

void badComma(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Invalid comma.\n\n");
    *error = 1;
}

void invalidDataParam(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Invalid data parameter, Only integers preceded by '-' or '+' or nothing are allowed.\n\n");
    *error = 1;
}

void badString(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Invalid string, string must to start with \" and finish with \".\n\n");
    *error = 1;
}

void toManyCommas(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Multiple consecutive commas.\n\n");
    *error = 1;
}

void noComma(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Missing a comma.\n\n");
    *error = 1;
}

void dumpLabel(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Setting a label before .extern or .entry makes the label irrelevant.\n\n");
    *error = 1;
}

void badValue(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Illegal parameter.\n\n");
    *error = 1;
}

void badNumber(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Invalid parameter of type number, numbers are defined as follows : start with '#' and \n");
    printf("can include the symbols '-', '+' and any integer number \n\n");
    *error = 1;
}

void badRegister(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("Invalid parameter of register type, the registers begin with 'r' and after a number from 0 to 7.\n\n");
    *error = 1;
}

void dataOverbound(int *error){

    printf("ERROR : We have exceeded the maximum amount of data that can be used to run the program.\n\n");
    *error = 1;
}

void unDefienedLabel(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("There is no label statement or the name of the label does \n");
    printf("not exist in the symbol table.\n\n");
    *error = 1;
}

void extCantBeEnt(int *error, int line){

    printf("Error in line : %d\n", line);
    printf("The label name is defined as external.\n\n");
    *error = 1;
}
