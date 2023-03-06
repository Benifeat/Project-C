

#include "second_pass.h"

void putEntFile(FILE *entFile, symHead *symHead, char *lblName, int lineCnt, int *error) {

    symTbl *tmp = symHead->head;
    char line[MAX_LINE] = {'\0'};

    while (tmp) {
        if (!strcmp(lblName, tmp->symName)) {

            if (!strcmp("ext", tmp->sign)) {

                lblCantEnt(error, lineCnt);
                return;
            } /* end second if */

            sprintf(line, "%s\t\t%d\n", lblName, tmp->value);
            fputs(line, entFile);
            break;
        } /* end first if */
        tmp = tmp->next;
    } /* end while loop */
}

void putExtFile(FILE *extFile, char *lblName, int IC){

    char line[MAX_LINE] = {'\0'};

    sprintf(line, "%s\t\t%d\n", lblName, IC);
    fputs(line, extFile);

}

int existLbl(symHead *symHead, char *lblName, int type, int *error, int lineCnt){

    symTbl *tmp = symHead->head;
    int len = strlen(lblName);

    while(tmp && len != 0) {

        if (!strncmp(lblName, tmp->symName, len)) {

            if (type && !strcmp(tmp->sign, "ext")) { /* .extern label statement in .entry statement */

                lblCantEnt(error, lineCnt);
                return 0;
            } /* end second if */
            return 1;
        } /* end first if */

        tmp = tmp->next;
    } /* end while  loop */

    noLblStatement(error, lineCnt);
    return 0;
}

void second(essentials *asmParam, symHead *headSymTbl, headData *headDataTbl, char *amFileName, char *extFileName, char *entFileName, char *obFileName, int *error, int *ext_Flag, int *ent_Flag){

    FILE *amFile = NULL;
    FILE *extFile = NULL;
    FILE *entFile = NULL;
    FILE *obFile = NULL;
    char line[MAX_LINE] ={'\0'};
    char *ptrLine = NULL;
    char *op1[OP1_LENGTH] = {"not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
    char *op2[OP2_LENGTH] = {"mov", "cmp", "add", "sub", "lea"};
    int lineCnt = 0;
    asmParam->IC = 100;

    if(!(amFile = chkFileOpen(amFile, amFileName, "r+", error)))
        return;

    if(ext_Flag) {

        if (!(extFile = chkFileOpen(extFile, extFileName, "w+", error)))
            return;
    } /* end if */

    if(ent_Flag) {

        if (!(entFile = chkFileOpen(entFile, entFileName, "w+", error)))
            return;
    } /* end if */

    if(!(obFile = chkFileOpen(obFile, obFileName, "w+", error)))
        return;

    while(fgets(line, MAX_LINE, amFile) != NULL) { /*check line after line in the as file*/

        char lblName[MAX_SYMBOL] = {'\0'};
        int lblFlag;
        int op1Flag = 0;
        int op2Flag = 0;
        int i;
        ptrLine = line;
        lineCnt++;

        skipTabSpace(ptrLine); /* we want to point on the first char in the line text */

        /* if it is an empty line we skip */
        if(*ptrLine == '\n' || *ptrLine == '\0')
            continue;

        /* if it is a comment line we skip */
        if(*ptrLine == ';') \
            continue;

        lblFlag = isLbl(ptrLine);

        if(lblFlag) {

            saveLblName(ptrLine,lblName);
            ptrLine += strlen(lblName); /* pass the label name and the character ':' */
            skipTabSpace(ptrLine);
        } /* end if */

        if (*ptrLine == '.') { /* the start of the data, string, extern, entry directive command */

            /*if there is an .entry directive command we will check if its valid and add the entries lines to the .ent file */
            if (checkWord(ptrLine,".entry")) {

                ptrLine += 6;
                skipTabSpace(ptrLine);
                saveSym(ptrLine, lblName);

                if(existLbl(headSymTbl, lblName, 1, error, lineCnt)) /* check if the label name exists in the symbol table */
                    putEntFile(entFile, headSymTbl, lblName, lineCnt, error);

                continue;
            } /* end second if */

            if (checkWord(ptrLine,".extern"))
                continue;


            /*we already have .data and .string in the data linked list, so we skip them */
            if(checkWord(ptrLine, ".data") || checkWord(ptrLine, ".string")){

                prnDataLine(obFile, asmParam, headDataTbl, asmParam->IC);
                continue;
            } /* end if */
        } /* end first if */

        /* instruction command with zero operand: "rts", "stop" */
        if(checkWord(ptrLine, "rts")){

            encoder *tmpLine = crtEnc();
            addOp(14, tmpLine);
            prnObjFile(obFile, asmParam, tmpLine);
            asmParam->IC += 1;
            continue;
        } /* end if */

        if(checkWord(ptrLine, "stop")){

            encoder *tmpLine = crtEnc();
            addOp(15, tmpLine);
            prnObjFile(obFile, asmParam, tmpLine);
            asmParam->IC += 1;
            continue;
        } /* end if */

        /* instruction command with 1 operand: "not", "clr", "inc", "dec", "red", "prn"
         * and "jmp", "bne", "jsr" that can be jump address */
        for(i = 0 ; i < OP1_LENGTH ; i++){

            if(checkWord(ptrLine, op1[i])){

                ptrLine += strlen(op1[i]);
                readOp1(obFile, extFile, asmParam, headSymTbl, ptrLine, i, error, lineCnt);
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
                readOp2(obFile, extFile, asmParam, headSymTbl, ptrLine, i, error, lineCnt);
                op2Flag = 1;
                break;
            } /* end if */
        } /* end for */

        if(op2Flag)
            continue;
    }

    if(asmParam->IC - 100 > MAX_LENGTH){

        exceededDataAmount(error);
    } /* end if */

    fclose(amFile);
    fclose(obFile);

    if(ext_Flag)
        fclose(extFile);

    if(ent_Flag)
        fclose(entFile);
}

void prnDataLine(FILE *obFile, essentials *asmParam, headData *headDataTbl, int currIC){

    dataTbl *tmp = headDataTbl->head;

    while(tmp != NULL && tmp->firstIC == currIC){

        prnObjFile(obFile,asmParam, tmp->data);
        asmParam->IC += 1;
        headDataTbl->head = headDataTbl->head->next;
        free(tmp->data);
        free(tmp);
        tmp = headDataTbl->head;
    } /* end while loop*/
}

void saveSym(char *line, char *lblName){

    int i = 0;

    for(i = 0 ; line[i] != '\n' && line[i] != '\0' && line[i] != '\t' && line[i] != ' ' && line[i] != ':' && line[i] != ')' && line[i] != ',' && i < MAX_SYMBOL ; i++){

        lblName[i] = line[i];

        if(line[i] == '('){

            lblName[i+1] = '\0';
            return;
        } /* end if */
    } /* end for */

    lblName[i] = '\0';
}

void jmpLbl(char *lblName, int *flag){

    int i = 0;

    for(i = 0 ; lblName[i] != '\0' ; i++){

        if(lblName[i] == '('){

            lblName[i] = '\0';
            *flag = 1;
            return;
        } /* end if */

        if(lblName[i] == ',' || lblName[i] == ')'){

            lblName[i] = '\0';
            return;
        } /* end if */
    } /* end for loop */
}

void readOp1(FILE *obFile, FILE *extFile, essentials *asmParam, symHead *headSymTbl, char *ptrLine, int instInd, int *error, int lineCnt){

    enum{not = 0, clr, inc, dec, jmp, bne, red, prn, jsr};
    enum{IMMEDIATE = 0, DIRECT, JUMP, DIR_REGI}; /* address */
    enum{SOURCE = 1, DEST}; /* which operand */
    enum{LABEL = 1, REGISTER}; /* which parameter */
    enum {E = 1, R}; /* which classify */

    int num = 0;
    int ext_Flag = 0; /* equal to 1 if we reach to external file */
    int jmpAddrFlag = 0; /* equal to 1 if we reach '(' after label name */
    int firstParam = 0;
    int regi = 0; /* equal to 1 if the first parameter in the jump address is register */
    char lblName[MAX_SYMBOL] = {'\0'};
    encoder *tmpLine = crtEnc();

    switch (instInd) {

        case not:
            addOp(4, tmpLine);
            break;

        case clr:
            addOp(5, tmpLine);
            break;

        case inc:
            addOp(7, tmpLine);
            break;

        case dec:
            addOp(8, tmpLine);
            break;

        case jmp:
            addOp(9, tmpLine);
            break;

        case bne:
            addOp(10, tmpLine);
            break;

        case red:
            addOp(11, tmpLine);
            break;

        case prn:
            addOp(12, tmpLine);
            break;

        case jsr:
            addOp(13, tmpLine);
            break;

        default:
            break;
    } /* end switch */

    skipTabSpace(ptrLine);

    if(*ptrLine == 'r'){

        if(chkReg(ptrLine[1]) != -1) {

            num = chkReg(ptrLine[1]);
            addAddr(DEST, DIR_REGI, tmpLine);
            prnObjFile(obFile, asmParam, tmpLine);
            asmParam->IC += 1;
            initEncode(tmpLine);
            addRegEncode(tmpLine, num, 1);
            prnObjFile(obFile, asmParam, tmpLine);
            asmParam->IC += 1;
            free(tmpLine);
            return;
        } /* end second if */
    } /* end first if */

    else if(*ptrLine == '#'){

        ptrLine += 1; /* point on the number */
        num = atoi(ptrLine);
        prnObjFile(obFile, asmParam, tmpLine);
        asmParam->IC += 1;
        initEncode(tmpLine);
        addNumEncode(tmpLine, num);
        prnObjFile(obFile, asmParam, tmpLine);
        asmParam->IC += 1;
        free(tmpLine);
        return;
    } /* end else if */

    else {

        saveSym(ptrLine, lblName);
        jmpLbl(lblName, &jmpAddrFlag);

        if (!existLbl(headSymTbl, lblName, 0, error, lineCnt)) {

            free(tmpLine);
            return;
        } /* end if */

        if (jmpAddrFlag) {

            skipChars(ptrLine);
            ptrLine++;
            encoder *secondLine = crtEnc();
            encoder *thirdLine = crtEnc();

            addAddr(DEST, JUMP, tmpLine);
            addLblEncode(headSymTbl, secondLine, lblName, &ext_Flag);

            if(ext_Flag) {

                putExtFile(extFile, lblName, asmParam->IC + 1);
                ext_Flag = 0;
            } /* end second if */

            while (*ptrLine != '\n' && *ptrLine != '\0') {

                if (*ptrLine == 'r') {

                    if (chkReg(ptrLine[1]) != -1) {

                        num = chkReg(ptrLine[1]);

                        if (regi || firstParam) {

                            addParam(REGISTER, 2, tmpLine);
                            prnObjFile(obFile, asmParam, tmpLine);
                            asmParam->IC += 1;
                            prnObjFile(obFile, asmParam, secondLine);
                            asmParam->IC += 1;

                            if (firstParam) {

                                prnObjFile(obFile, asmParam, thirdLine);
                                asmParam->IC += 1;
                                initEncode(thirdLine);
                            } /* end fourth if */

                            addRegEncode(thirdLine, num, 2);
                            prnObjFile(obFile, asmParam, thirdLine);
                            asmParam->IC += 1;
                            free(tmpLine);
                            free(secondLine);
                            free(thirdLine);
                            return;
                        } /* end third if */

                        addParam(REGISTER, 1, tmpLine);
                        addRegEncode(thirdLine, num, 1);
                        regi = 1;
                        skipChars(ptrLine); /* skip the register */
                        skipTabSpace(ptrLine);
                        ptrLine += 1; /* skip the comma */
                        skipTabSpace(ptrLine);
                    } /* end second if */
                } /* end first if */

                else if (*ptrLine == '#') {

                    ptrLine += 1;
                    num = atoi(ptrLine); /* point on the number */

                    if (regi || firstParam) {

                        prnObjFile(obFile, asmParam, tmpLine);
                        asmParam->IC += 1;
                        prnObjFile(obFile, asmParam, secondLine);
                        asmParam->IC += 1;
                        prnObjFile(obFile, asmParam, thirdLine);
                        asmParam->IC += 1;
                        initEncode(thirdLine);
                        addNumEncode(thirdLine, num);
                        prnObjFile(obFile, asmParam, thirdLine);
                        asmParam->IC += 1;
                        free(tmpLine);
                        free(secondLine);
                        free(thirdLine);
                        return;
                    } /* end if */

                    addNumEncode(thirdLine, num);
                    firstParam = 1;
                    skipChars(ptrLine); /* skip the number */
                    skipTabSpace(ptrLine);
                    ptrLine += 1; /* skip the comma */
                    skipTabSpace(ptrLine);
                } /* end else if */

                else {

                    saveSym(ptrLine, lblName);

                    if (!existLbl(headSymTbl, lblName, 0, error, lineCnt)) {

                        free(tmpLine);
                        free(secondLine);
                        free(thirdLine);
                        return;
                    } /* end if */

                    if (regi || firstParam) {

                        addParam(LABEL, 2, tmpLine);
                        prnObjFile(obFile, asmParam, tmpLine);
                        asmParam->IC += 1;
                        prnObjFile(obFile, asmParam, secondLine);
                        asmParam->IC += 1;
                        prnObjFile(obFile, asmParam, thirdLine);
                        asmParam->IC += 1;
                        initEncode(thirdLine);
                        addLblEncode(headSymTbl, thirdLine, lblName, &ext_Flag);

                        if(ext_Flag)
                            putExtFile(extFile, lblName, asmParam->IC);

                        prnObjFile(obFile, asmParam, thirdLine);
                        asmParam->IC += 1;
                        free(tmpLine);
                        free(secondLine);
                        free(thirdLine);
                        return;
                    } /* end if */

                    addParam(LABEL, 1, tmpLine);
                    addLblEncode(headSymTbl, thirdLine, lblName, &ext_Flag);

                    if(ext_Flag) {

                        putExtFile(extFile, lblName, asmParam->IC + 2);
                        ext_Flag = 0;
                    } /* end if */

                    firstParam = 1;
                    skipChars(ptrLine); /* skip the label name */
                    skipTabSpace(ptrLine);
                    ptrLine += 1; /* skip the comma */
                    skipTabSpace(ptrLine);
                } /* end else */
            } /* end while loop */
        } /* end first if */

        addAddr(DEST, DIRECT, tmpLine);
        prnObjFile(obFile, asmParam, tmpLine);
        asmParam->IC += 1;
        initEncode(tmpLine);
        addLblEncode(headSymTbl, tmpLine, lblName, &ext_Flag);

        if(ext_Flag) {

            putExtFile(extFile, lblName, asmParam->IC);
            ext_Flag = 0;
        } /* end if */

        prnObjFile(obFile, asmParam, tmpLine);
        asmParam->IC += 1;
        free(tmpLine);
    } /* end else */
}

void readOp2(FILE *obFile, FILE *extFile, essentials *asmParam, symHead *headSymTbl, char *ptrLine, int instInd, int *error, int lineCnt) {

    enum {mov = 0, cmp, add, sub, lea};
    enum {IMMEDIATE = 0, DIRECT, JUMP, DIR_REGI}; /* address */
    enum {SOURCE = 1, DEST}; /* which operand */
    enum {LABEL = 1, REGISTER}; /* which parameter */

    int num = 0;
    int ext_Flag = 0; /* equal to 1 if we reach to external file */
    int firstParam = 0;
    int regi = 0; /* equal to 1 if the first parameter is register */
    char lblName[MAX_SYMBOL] = {'\0'};
    encoder *firstLine = crtEnc();
    encoder *tmpLine = crtEnc();


    switch (instInd) {

        case mov:
            break;

        case cmp:
            addOp(1, firstLine);
            break;

        case add:
            addOp(2, firstLine);
            break;

        case sub:
            addOp(3, firstLine);
            break;

        case lea:
            addOp(6, firstLine);
            break;

        default:
            break;
    } /* end switch */

    skipTabSpace(ptrLine); /* point on the first parameter */

    while (*ptrLine != '\n' && *ptrLine != '\0') {

        if (*ptrLine == 'r') {

            if (chkReg(ptrLine[1]) != -1) {

                num = chkReg(ptrLine[1]);

                if (regi || firstParam) {

                    addAddr(DEST, DIR_REGI, firstLine);
                    prnObjFile(obFile, asmParam, firstLine);
                    asmParam->IC += 1; /* for the first parameter */

                    if (firstParam) {

                        prnObjFile(obFile, asmParam, tmpLine);
                        asmParam->IC += 1;
                        initEncode(tmpLine);
                    } /* end fourth if */

                    addRegEncode(tmpLine, num, 2);
                    prnObjFile(obFile, asmParam, tmpLine);
                    asmParam->IC += 1;
                    free(firstLine);
                    free(tmpLine);
                    return;
                } /* end third if */

                addAddr(SOURCE, DIR_REGI, firstLine);
                addRegEncode(tmpLine, num, 1);

                regi = 1;
                skipChars(ptrLine); /* skip the register */
                skipTabSpace(ptrLine);
                ptrLine += 1; /* skip the comma */
                skipTabSpace(ptrLine);
            } /* end second if */
        } /* end first if */

        else if (*ptrLine == '#') {

            ptrLine += 1; /* point on the number */
            num = atoi(ptrLine);

            if (regi || firstParam) {

                prnObjFile(obFile, asmParam, firstLine);
                asmParam->IC += 1;
                prnObjFile(obFile, asmParam, tmpLine);
                asmParam->IC += 1;
                initEncode(tmpLine);
                addNumEncode(tmpLine, num);
                prnObjFile(obFile, asmParam, tmpLine);
                asmParam->IC += 1;
                free(tmpLine);
                free(firstLine);
                return;
            } /* end if */

            addNumEncode(tmpLine, num);
            firstParam = 1;
            skipChars(ptrLine); /* skip the number */
            skipTabSpace(ptrLine);
            ptrLine += 1; /* skip the comma */
            skipTabSpace(ptrLine);
        } /* end else if */

        else {

            saveSym(ptrLine, lblName);

            if (!existLbl(headSymTbl, lblName, 0, error, lineCnt)) {

                free(firstLine);
                free(tmpLine);
                return;
            } /* end if */

            if (regi || firstParam) {

                addAddr(DEST, DIRECT, firstLine);
                prnObjFile(obFile, asmParam, firstLine);
                asmParam->IC += 1;
                prnObjFile(obFile, asmParam, tmpLine);
                asmParam->IC += 1;
                initEncode(tmpLine);
                addLblEncode(headSymTbl, tmpLine, lblName, &ext_Flag);

                if(ext_Flag) {

                    putExtFile(extFile, lblName, asmParam->IC);
                    ext_Flag = 0;
                } /* end second if */

                prnObjFile(obFile, asmParam, tmpLine);
                asmParam->IC += 1;
                free(tmpLine);
                free(firstLine);
                return;
            } /* end first if */

            addAddr(SOURCE, DIRECT, firstLine);
            addLblEncode(headSymTbl, tmpLine, lblName, &ext_Flag);

            if(ext_Flag) {

                putExtFile(extFile, lblName, asmParam->IC + 1);
                ext_Flag = 0;
            } /* end if */

            firstParam = 1;
            skipChars(ptrLine); /* skip the number */
            skipTabSpace(ptrLine);
            ptrLine += 1; /* skip the comma */
            skipTabSpace(ptrLine);
        } /* end else */
    } /* end while loop */
}

