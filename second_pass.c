

#include "second_pass.h"
#define _CRT_SECURE_NO_WARNINGS
void editDotEnt(FILE *entFile, symbolLine *symbolLine, char *firstLabelName, int countLines, int *error) {

    symbolList *temp = symbolLine->head;
    char line[MAX_LINE] = {'\0'};

    while (temp) {
        if (!strcmp(firstLabelName, temp->symbolName)) {

            if (!strcmp("ext", temp->mark)) {

                extCantBeEnt(error, countLines);
                return;
            } /* end second if */

            sprintf(line, "%s\t\t%d\n", firstLabelName, temp->value);
            fputs(line, entFile);
            break;
        } /* end first if */
        temp = temp->next;
    } /* end while loop */
}

void editDotExt(FILE *extFile, char *firstLabelName, int IC){

    char line[MAX_LINE] = {'\0'};

    sprintf(line, "%s\t\t%d\n", firstLabelName, IC);
    fputs(line, extFile);

}

int lastLabelChecks(symbolLine *symbolLine, char *firstLabelName, int type, int *error, int countLines){

    symbolList *temp = symbolLine->head;
    int len = strlen(firstLabelName);

    while(temp && len != 0) {

        if (!strncmp(firstLabelName, temp->symbolName, len)) {

            if (type && !strcmp(temp->mark, "ext")) { /* .extern label statement in .entry statement */

                extCantBeEnt(error, countLines);
                return 0;
            } /* end second if */
            return 1;
        } /* end first if */

        temp = temp->next;
    } /* end while  loop */

    unDefienedLabel(error, countLines);
    return 0;
}

void second(data_base *asmValues, symbolLine *symbolLineStart, headLine *headLineStart, char *amFileName, char *extFileName, char *entFileName, char *obFileName, int *error, int *ext_Flag, int *ent_Flag){

    FILE *amFile = NULL;
    FILE *extFile = NULL;
    FILE *entFile = NULL;
    FILE *obFile = NULL;
    char line[MAX_LINE] ={'\0'};
    char *ptrLine = NULL;
    char *op1[OP1_LENGTH] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
    char *op2[OP2_LENGTH] = {"mov", "cmp", "add", "sub", "lea"};
    int countLines = 0;
    asmValues->IC = 100;

    if(!(amFile = fileNotCreated(amFile, amFileName, "r+", error)))
        return;

    if(ext_Flag) {

        if (!(extFile = fileNotCreated(extFile, extFileName, "w+", error)))
            return;
    } /* end if */

    if(ent_Flag) {

        if (!(entFile = fileNotCreated(entFile, entFileName, "w+", error)))
            return;
    } /* end if */

    if(!(obFile = fileNotCreated(obFile, obFileName, "w+", error)))
        return;

    while(fgets(line, MAX_LINE, amFile) != NULL) { /*check line after line in the as file*/

        char firstLabelName[MAX_SYMBOL] = {'\0'};
        int firstLabelFlag;
        int op1Flag = 0;
        int op2Flag = 0;
        int i;
        ptrLine = line;
        countLines++;

        no_tabs(ptrLine); /* we want to point on the first char in the line text */

        /* if it is an empty line we skip */
        if(*ptrLine == '\n' || *ptrLine == '\0')
            continue;

        /* if it is a comment line we skip */
        if(*ptrLine == ';') \
            continue;

        firstLabelFlag = label(ptrLine);

        if(firstLabelFlag) {

            saveLabel(ptrLine,firstLabelName);
            ptrLine += strlen(firstLabelName); /* pass the label name and the character ':' */
            no_tabs(ptrLine);
        } /* end if */

        if (*ptrLine == '.') { /* the start of the data, string, extern, entry directive command */

            /*if there is an .entry directive command we will check if its valid and add the entries lines to the .ent file */
            if (checkWord(ptrLine,".entry")) {

                ptrLine += 6;
                no_tabs(ptrLine);
                saveLabelToSymbol(ptrLine, firstLabelName);

                if(lastLabelChecks(symbolLineStart, firstLabelName, 1, error, countLines)) /* check if the label name exists in the symbol table */
                    editDotEnt(entFile, symbolLineStart, firstLabelName, countLines, error);

                continue;
            } /* end second if */

            if (checkWord(ptrLine,".extern"))
                continue;


            /*we already have .data and .string in the data linked list, so we skip them */
            if(checkWord(ptrLine, ".data") || checkWord(ptrLine, ".string")){

                editDotObFile(obFile, asmValues, headLineStart, asmValues->IC);
                continue;
            } /* end if */
        } /* end first if */

        /* instruction command with zero operand: "rts", "stop" */
        if(checkWord(ptrLine, "rts")){

            opCode *tempLine = resultList();
            addOp(14, tempLine);
            printOpToDotObj(obFile, asmValues, tempLine);
            asmValues->IC += 1;
            continue;
        } /* end if */

        if(checkWord(ptrLine, "stop")){

            opCode *tempLine = resultList();
            addOp(15, tempLine);
            printOpToDotObj(obFile, asmValues, tempLine);
            asmValues->IC += 1;
            continue;
        } /* end if */

        /* instruction command with 1 operand: "not", "clr", "inc", "dec", "red", "prn"
         * and "jmp", "bne", "jsr" that can be jump address */
        for(i = 0 ; i < OP1_LENGTH ; i++){

            if(checkWord(ptrLine, op1[i])){

                ptrLine += strlen(op1[i]);
                getOp1(obFile, extFile, asmValues, symbolLineStart, ptrLine, i, error, countLines);
                op1Flag = 1;
                break;
            } /* end if */
        } /* end for loop */

        if(op1Flag)
            continue;


        /* instruction command with 2 operand: "mov", "cmp", "add", "sub", "lea" */
        for(i = 0 ; i < OP2_LENGTH ; i++){

            if(checkWord(ptrLine, op2[i])){

                ptrLine += strlen(op2[i]);
                getOp2(obFile, extFile, asmValues, symbolLineStart, ptrLine, i, error, countLines);
                op2Flag = 1;
                break;
            } /* end if */
        } /* end for */

        if(op2Flag)
            continue;
    }

    if(asmValues->IC - 100 > MAX_LENGTH){

        dataOverbound(error);
    } /* end if */

    fclose(amFile);
    fclose(obFile);

    if(ext_Flag)
        fclose(extFile);

    if(ent_Flag)
        fclose(entFile);
}

void editDotObFile(FILE *obFile, data_base *asmValues, headLine *headLineStart, int currentIC){

    dataList *temp = headLineStart->head;

    while(temp != NULL && temp->ic_start == currentIC){

        printOpToDotObj(obFile,asmValues, temp->data);
        asmValues->IC += 1;
        headLineStart->head = headLineStart->head->next;
        free(temp->data);
        free(temp);
        temp = headLineStart->head;
    } /* end while loop*/
}

void saveLabelToSymbol(char *line, char *firstLabelName){

    int i = 0;

    for(i = 0 ; line[i] != '\n' && line[i] != '\0' && line[i] != '\t' && line[i] != ' ' && line[i] != ':' && line[i] != ')' && line[i] != ',' && i < MAX_SYMBOL ; i++){

        firstLabelName[i] = line[i];

        if(line[i] == '('){

            firstLabelName[i+1] = '\0';
            return;
        } /* end if */
    } /* end for */

    firstLabelName[i] = '\0';
}

void jumpToLabelAddress(char *firstLabelName, int *flag){

    int i = 0;

    for(i = 0 ; firstLabelName[i] != '\0' ; i++){

        if(firstLabelName[i] == '('){

            firstLabelName[i] = '\0';
            *flag = 1;
            return;
        } /* end if */

        if(firstLabelName[i] == ',' || firstLabelName[i] == ')'){

            firstLabelName[i] = '\0';
            return;
        } /* end if */
    } /* end for loop */
}

void getOp1(FILE *obFile, FILE *extFile, data_base *asmValues, symbolLine *symbolLineStart, char *ptrLine, int commandIndex, int *error, int countLines){

    enum{not = 0, clr, inc, dec, jmp, bne, red, prn, jsr};
    enum{IMMEDIATE = 0, PATH, JUMP, DIRECT_REGISTER}; /* address */
    enum{SOURCE = 1, DESTINATION}; /* which operand */
    enum{LABEL = 1, REGISTER}; /* which parameter */
    enum {E = 1, R}; /* which classify */

    int num = 0;
    int ext_Flag = 0; /* equal to 1 if we reach to external file */
    int jumpToAddress = 0; /* equal to 1 if we reach '(' after label name */
    int firstParam = 0;
    int registerFlag = 0; /* equal to 1 if the first parameter in the jump address is register */
    char firstLabelName[MAX_SYMBOL] = {'\0'};
    opCode *tempLine = resultList();

    switch (commandIndex) {

        case not:
            addOp(4, tempLine);
            break;

        case clr:
            addOp(5, tempLine);
            break;

        case inc:
            addOp(7, tempLine);
            break;

        case dec:
            addOp(8, tempLine);
            break;

        case jmp:
            addOp(9, tempLine);
            break;

        case bne:
            addOp(10, tempLine);
            break;

        case red:
            addOp(11, tempLine);
            break;

        case prn:
            addOp(12, tempLine);
            break;

        case jsr:
            addOp(13, tempLine);
            break;

        default:
            break;
    } /* end switch */

    no_tabs(ptrLine);

    if(*ptrLine == 'r'){

        if(checkRegister(ptrLine[1]) != -1) {

            num = checkRegister(ptrLine[1]);
            addAddress(DESTINATION, DIRECT_REGISTER, tempLine);
            printOpToDotObj(obFile, asmValues, tempLine);
            asmValues->IC += 1;
            setOpCode(tempLine);
            editOpCodeNumbers(tempLine, num, 1);
            printOpToDotObj(obFile, asmValues, tempLine);
            asmValues->IC += 1;
            free(tempLine);
            return;
        } /* end second if */
    } /* end first if */

    else if(*ptrLine == '#'){

        ptrLine += 1; /* point on the number */
        num = atoi(ptrLine);
        printOpToDotObj(obFile, asmValues, tempLine);
        asmValues->IC += 1;
        setOpCode(tempLine);
        addNumEncode(tempLine, num);
        printOpToDotObj(obFile, asmValues, tempLine);
        asmValues->IC += 1;
        free(tempLine);
        return;
    } /* end else if */

    else {

        saveLabelToSymbol(ptrLine, firstLabelName);
        jumpToLabelAddress(firstLabelName, &jumpToAddress);

        if (!lastLabelChecks(symbolLineStart, firstLabelName, 0, error, countLines)) {

            free(tempLine);
            return;
        } /* end if */

        if (jumpToAddress) {

            skipChars(ptrLine);
            ptrLine++;
	    opCode *secondString;
            secondString = resultList();
            opCode *thirdString; 
	    thirdString = resultList();

            addAddress(DESTINATION, JUMP, tempLine);
            editLabelOpcode(symbolLineStart, secondString, firstLabelName, &ext_Flag);

            if(ext_Flag) {

                editDotExt(extFile, firstLabelName, asmValues->IC + 1);
                ext_Flag = 0;
            } /* end second if */

            while (*ptrLine != '\n' && *ptrLine != '\0') {

                if (*ptrLine == 'r') {

                    if (checkRegister(ptrLine[1]) != -1) {

                        num = checkRegister(ptrLine[1]);

                        if (registerFlag || firstParam) {

                            editValues(REGISTER, 2, tempLine);
                            printOpToDotObj(obFile, asmValues, tempLine);
                            asmValues->IC += 1;
                            printOpToDotObj(obFile, asmValues, secondString);
                            asmValues->IC += 1;

                            if (firstParam) {

                                printOpToDotObj(obFile, asmValues, thirdString);
                                asmValues->IC += 1;
                                setOpCode(thirdString);
                            } /* end fourth if */

                            editOpCodeNumbers(thirdString, num, 2);
                            printOpToDotObj(obFile, asmValues, thirdString);
                            asmValues->IC += 1;
                            free(tempLine);
                            free(secondString);
                            free(thirdString);
                            return;
                        } /* end third if */

                        editValues(REGISTER, 1, tempLine);
                        editOpCodeNumbers(thirdString, num, 1);
                        registerFlag = 1;
                        skipChars(ptrLine); /* skip the register */
                        no_tabs(ptrLine);
                        ptrLine += 1; /* skip the comma */
                        no_tabs(ptrLine);
                    } /* end second if */
                } /* end first if */

                else if (*ptrLine == '#') {

                    ptrLine += 1;
                    num = atoi(ptrLine); /* point on the number */

                    if (registerFlag || firstParam) {

                        printOpToDotObj(obFile, asmValues, tempLine);
                        asmValues->IC += 1;
                        printOpToDotObj(obFile, asmValues, secondString);
                        asmValues->IC += 1;
                        printOpToDotObj(obFile, asmValues, thirdString);
                        asmValues->IC += 1;
                        setOpCode(thirdString);
                        addNumEncode(thirdString, num);
                        printOpToDotObj(obFile, asmValues, thirdString);
                        asmValues->IC += 1;
                        free(tempLine);
                        free(secondString);
                        free(thirdString);
                        return;
                    } /* end if */

                    addNumEncode(thirdString, num);
                    firstParam = 1;
                    skipChars(ptrLine); /* skip the number */
                    no_tabs(ptrLine);
                    ptrLine += 1; /* skip the comma */
                    no_tabs(ptrLine);
                } /* end else if */

                else {

                    saveLabelToSymbol(ptrLine, firstLabelName);

                    if (!lastLabelChecks(symbolLineStart, firstLabelName, 0, error, countLines)) {

                        free(tempLine);
                        free(secondString);
                        free(thirdString);
                        return;
                    } /* end if */

                    if (registerFlag || firstParam) {

                        editValues(LABEL, 2, tempLine);
                        printOpToDotObj(obFile, asmValues, tempLine);
                        asmValues->IC += 1;
                        printOpToDotObj(obFile, asmValues, secondString);
                        asmValues->IC += 1;
                        printOpToDotObj(obFile, asmValues, thirdString);
                        asmValues->IC += 1;
                        setOpCode(thirdString);
                        editLabelOpcode(symbolLineStart, thirdString, firstLabelName, &ext_Flag);

                        if(ext_Flag)
                            editDotExt(extFile, firstLabelName, asmValues->IC);

                        printOpToDotObj(obFile, asmValues, thirdString);
                        asmValues->IC += 1;
                        free(tempLine);
                        free(secondString);
                        free(thirdString);
                        return;
                    } /* end if */

                    editValues(LABEL, 1, tempLine);
                    editLabelOpcode(symbolLineStart, thirdString, firstLabelName, &ext_Flag);

                    if(ext_Flag) {

                        editDotExt(extFile, firstLabelName, asmValues->IC + 2);
                        ext_Flag = 0;
                    } /* end if */

                    firstParam = 1;
                    skipChars(ptrLine); /* skip the label name */
                    no_tabs(ptrLine);
                    ptrLine += 1; /* skip the comma */
                    no_tabs(ptrLine);
                } /* end else */
            } /* end while loop */
        } /* end first if */

        addAddress(DESTINATION, PATH, tempLine);
        printOpToDotObj(obFile, asmValues, tempLine);
        asmValues->IC += 1;
        setOpCode(tempLine);
        editLabelOpcode(symbolLineStart, tempLine, firstLabelName, &ext_Flag);

        if(ext_Flag) {

            editDotExt(extFile, firstLabelName, asmValues->IC);
            ext_Flag = 0;
        } /* end if */

        printOpToDotObj(obFile, asmValues, tempLine);
        asmValues->IC += 1;
        free(tempLine);
    } /* end else */
}

void getOp2(FILE *obFile, FILE *extFile, data_base *asmValues, symbolLine *symbolLineStart, char *ptrLine, int commandIndex, int *error, int countLines) {

    enum {mov = 0, cmp, add, sub, lea};
    enum {IMMEDIATE = 0, PATH, JUMP, DIRECT_REGISTER}; /* address */
    enum {SOURCE = 1, DESTINATION}; /* which operand */
    enum {LABEL = 1, REGISTER}; /* which parameter */

    int num = 0;
    int ext_Flag = 0; /* equal to 1 if we reach to external file */
    int firstParam = 0;
    int registerFlag = 0; /* equal to 1 if the first parameter is register */
    char firstLabelName[MAX_SYMBOL] = {'\0'};
    opCode *firstString = resultList();
    opCode *tempLine = resultList();


    switch (commandIndex) {

        case mov:
            break;

        case cmp:
            addOp(1, firstString);
            break;

        case add:
            addOp(2, firstString);
            break;

        case sub:
            addOp(3, firstString);
            break;

        case lea:
            addOp(6, firstString);
            break;

        default:
            break;
    } /* end switch */

    no_tabs(ptrLine); /* point on the first parameter */

    while (*ptrLine != '\n' && *ptrLine != '\0') {

        if (*ptrLine == 'r') {

            if (checkRegister(ptrLine[1]) != -1) {

                num = checkRegister(ptrLine[1]);

                if (registerFlag || firstParam) {

                    addAddress(DESTINATION, DIRECT_REGISTER, firstString);
                    printOpToDotObj(obFile, asmValues, firstString);
                    asmValues->IC += 1; /* for the first parameter */

                    if (firstParam) {

                        printOpToDotObj(obFile, asmValues, tempLine);
                        asmValues->IC += 1;
                        setOpCode(tempLine);
                    } /* end fourth if */

                    editOpCodeNumbers(tempLine, num, 2);
                    printOpToDotObj(obFile, asmValues, tempLine);
                    asmValues->IC += 1;
                    free(firstString);
                    free(tempLine);
                    return;
                } /* end third if */

                addAddress(SOURCE, DIRECT_REGISTER, firstString);
                editOpCodeNumbers(tempLine, num, 1);

                registerFlag = 1;
                skipChars(ptrLine); /* skip the register */
                no_tabs(ptrLine);
                ptrLine += 1; /* skip the comma */
                no_tabs(ptrLine);
            } /* end second if */
        } /* end first if */

        else if (*ptrLine == '#') {

            ptrLine += 1; /* point on the number */
            num = atoi(ptrLine);

            if (registerFlag || firstParam) {

                printOpToDotObj(obFile, asmValues, firstString);
                asmValues->IC += 1;
                printOpToDotObj(obFile, asmValues, tempLine);
                asmValues->IC += 1;
                setOpCode(tempLine);
                addNumEncode(tempLine, num);
                printOpToDotObj(obFile, asmValues, tempLine);
                asmValues->IC += 1;
                free(tempLine);
                free(firstString);
                return;
            } /* end if */

            addNumEncode(tempLine, num);
            firstParam = 1;
            skipChars(ptrLine); /* skip the number */
            no_tabs(ptrLine);
            ptrLine += 1; /* skip the comma */
            no_tabs(ptrLine);
        } /* end else if */

        else {

            saveLabelToSymbol(ptrLine, firstLabelName);

            if (!lastLabelChecks(symbolLineStart, firstLabelName, 0, error, countLines)) {

                free(firstString);
                free(tempLine);
                return;
            } /* end if */

            if (registerFlag || firstParam) {

                addAddress(DESTINATION, PATH, firstString);
                printOpToDotObj(obFile, asmValues, firstString);
                asmValues->IC += 1;
                printOpToDotObj(obFile, asmValues, tempLine);
                asmValues->IC += 1;
                setOpCode(tempLine);
                editLabelOpcode(symbolLineStart, tempLine, firstLabelName, &ext_Flag);

                if(ext_Flag) {

                    editDotExt(extFile, firstLabelName, asmValues->IC);
                    ext_Flag = 0;
                } /* end second if */

                printOpToDotObj(obFile, asmValues, tempLine);
                asmValues->IC += 1;
                free(tempLine);
                free(firstString);
                return;
            } /* end first if */

            addAddress(SOURCE, PATH, firstString);
            editLabelOpcode(symbolLineStart, tempLine, firstLabelName, &ext_Flag);

            if(ext_Flag) {

                editDotExt(extFile, firstLabelName, asmValues->IC + 1);
                ext_Flag = 0;
            } /* end if */

            firstParam = 1;
            skipChars(ptrLine); /* skip the number */
            no_tabs(ptrLine);
            ptrLine += 1; /* skip the comma */
            no_tabs(ptrLine);
        } /* end else */
    } /* end while loop */
}

