
#include "first_pass.h"

int isLbl (char *line){

    int i;
    int spaceFlag = 0;

    for(i = 0 ; line[i] != '\n' && line[i] != '\0' ; i++){ /* check if there is ':' in label name */

        if(line[i] == ':'){

            if(spaceFlag)
                return -1;

            return 1;
        } /* end if */

        if(line[i] == ' ' || line[i] == '\t') {

            spaceFlag = 1;
            continue;
        } /* end if */

        if(!((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z') || (line[i] >= '0' && line[i] <= '9')))
            return 0;
    } /* end for loop */

    return 0;
}

int validLblName(symHead *sym, char *labelName, int type, int *error , int lineCnt){

    symTbl  *tmp = sym->head;
    int len = strlen(labelName);
    int i;

    if (labelName[0] == ' '){

        lngLblName(error, lineCnt);
        return 0;
    } /* end if */

    if(type) {/* equal to 1 if the label end with ':' */

        labelName[len - 1] = '\0';
        len--;
    } /* end if */

    for (i = 0; i < len ; i++) {

        if(labelName[i] == ' ' || labelName[i] == '\t'){

            extraTxt(error, lineCnt);
            return 0;
        } /* end if */

        if(!((labelName[i] >= 'A' && labelName[i] <= 'Z') || (labelName[i] >= 'a' && labelName[i] <= 'z') || (labelName[i] >= '0' && labelName[i] <= '9'))) {

            illLblName(error, lineCnt);
            return 0;
        } /* end if */
    } /* end for loop */

    if(invalidName(labelName)) { /* check if the name of the label is register name or instruction or directive command or if not exists label name */

        illLblName(error, lineCnt);
        return 0;
    } /* end if */

    if (!tmp)
        return 1;

    while(tmp != NULL) { /* check all the names in the table of label name  */

        if(len == strlen(tmp->symName)) {

            if (!strcmp(labelName, tmp->symName)) { /* check if the new label name is in the table */

                dblLblName(error, lineCnt); /* error message */
                return 0;
            } /* end second if */
        } /* end first if */

        tmp = tmp->next;
    } /* end while loop */

    return 1;
}

void first(char *asFileName, essentials *asmParam, symHead *sym, headData *dataTblLineHead, int *error, int *entFlag, int *extFlag) {

    FILE *asFile = NULL;
    int lineCnt = 0;
    char line[MAX_LINE] = {'\0'};

    if(!(asFile = chkFileOpen(asFile, asFileName, "r+", error)))
        return;

    while(fgets(line, MAX_LINE, asFile) != NULL) { /* check line after line in the as file */

        char lblName[MAX_SYMBOL] = {'\0'};
        char lblType[MAX_TYPE] = {'\0'}; /* save the type of the symbol ( code / data / external) */
        int lblFlag = 0; /* turned on if we find label name in the line */
        char *ptrLine = line;
        lineCnt++;

        skipTabSpace(ptrLine); /* we want to point on the first char in the line text */

        /* if it is an empty line we skip */
        if (*ptrLine == '\n' || *ptrLine == '\0')
            continue;

        /* if it is a comment line we skip */
        if (*ptrLine == ';')
            continue;

        if(strlen(ptrLine) == MAX_LINE-1){

            chkLineLen(error, lineCnt);

            while(strlen(ptrLine) == MAX_LINE-1) { /* we will go through the entire line before moving on to the next line in the file */

                if (fgets(line, MAX_LINE, asFile) == NULL) {

                    fclose(asFile);
                    return;
                } /* end if */
            } /* end while loop */

            continue;
        } /* end if */

        lblFlag = isLbl(ptrLine); /* if the name is valid we point on it with ptr_label and return 1 */

        if(lblFlag == -1){

            spaceInLbl(error, lineCnt);
            continue;
        } /* end if */

        if (lblFlag == 1) {

            saveLblName(ptrLine, lblName);

            if (!validLblName(sym, lblName, 1, error, lineCnt))
                continue;

            ptrLine += strlen(lblName) + 1; /* point after the label name and the ':' */
            skipTabSpace(ptrLine);
        } /* end if */

        if (*ptrLine == '.') { /* the start of the data, string, extern, entry directive command */

            if (checkWord(ptrLine, ".data") || checkWord(ptrLine, ".string")) {

                if (lblFlag) {

                    strcpy(lblType, "data"); /* save the label type */
                    addSymTbl(sym, lblName, lblType, asmParam->IC);
                } /* end if */

                if (checkWord(ptrLine, ".data")) {

                    ptrLine += 5; /* point after the .data directive */
                    readDataParam(asmParam, dataTblLineHead, ptrLine, error, lineCnt);
                    continue;
                } /* end if */

                if (checkWord(ptrLine, ".string")) {

                    ptrLine += 7; /* point after the .string directive */
                    readStrParam(asmParam, dataTblLineHead, ptrLine, error, lineCnt);
                    continue;
                } /* end if */
            } /* end labels if */

            if (checkWord(ptrLine, ".entry")) {

                *entFlag = 1;
                ptrLine += 6;

                if (lblFlag) { /* if we pass a label name before .entry */

                    irrLbl(error, lineCnt);
                    continue;
                } /* end if */

                skipTabSpace(ptrLine);

                if (*ptrLine == '\n' || *ptrLine == '\0') {

                    noLblStatement(error, lineCnt);
                    continue;
                } /* end if */

                skipChars(ptrLine); /* skip the label name */
                skipTabSpace(ptrLine);

                if (*ptrLine != '\n' && *ptrLine != '\0') {

                    extraTxt(error, lineCnt);
                    continue;
                } /* end if */

                continue;
            } /* end entry if */

            if (checkWord(ptrLine, ".extern")) {

                *extFlag = 1;
                strcpy(lblType, "ext");
                ptrLine += 7;

                if (lblFlag) { /* if we pass a label name before .extern */

                    irrLbl(error, lineCnt);
                    continue;
                } /* end if */

                skipTabSpace(ptrLine);

                if (*ptrLine == '\n' || *ptrLine == '\0') {

                    noLblStatement(error, lineCnt);
                    continue;
                } /* end if */

                saveLblName(ptrLine, lblName);

                if (validLblName(sym, lblName, 0, error, lineCnt)) {

                    skipChars(ptrLine);
                    skipTabSpace(ptrLine);

                    if (*ptrLine != '\n' && *ptrLine != '\0') {

                        extraTxt(error, lineCnt);
                        continue;
                    } /* end second if */

                    addSymTbl(sym, lblName, lblType, 0);
                    continue;
                } /* end first if */
                continue;
            } /* end extern if */

            else { /* the word from the text file start with '.' but it is not a directive command */

                undefDirCmd(error, lineCnt);
                continue;
            } /* end else */
        } /* end first if */

        if (lblFlag) {

            strcpy(lblType, "code");
            addSymTbl(sym, lblName, lblType, asmParam->IC);
            readInst(asmParam, dataTblLineHead, ptrLine, error, lineCnt);
            continue;
        } /* end if */

        readInst(asmParam, dataTblLineHead, ptrLine, error, lineCnt);
    } /* end while loop */
    fclose(asFile);
}

void saveLblName(char *line, char *lblName){

    int i;

    for(i = 0 ; line[i] != '\n' && line[i] != '\0' && i < MAX_SYMBOL ; i++){

        lblName[i] = line[i];

        if(line[i] == ':' || line[i] == ')' || line[i] == ',' ){

            lblName[i] = line[i];
            lblName[i+1] = '\0';
            return;
        } /* end if */
    } /* end for loop */

    if(lblName[MAX_SYMBOL-1] != '\0')
        lblName[0] = ' ';

}

int checkWord(char *line, char *command){

    int i = 0;

    while(line[i] && line[i] != ',' && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
        i++;

    if(strlen(command) != i)
        return 0;

    if(!strncmp(command, line, i))
        return 1;

    return 0;
}

void readStrParam(essentials *asmParam, headData *headDataTbl, char *line, int *error, int lineCnt){

    char letter = '\0' ;
    int i = 0;
    int firstIC = asmParam->IC;
    int lastStrInd = -1; /* save the index of the closing " */
    int noString = 0; /* equal to 1 if there are more characters after " */

    if (*line != ' ' && *line != '\t') {

        if(*line == '\n' || *line == '\0') { /* check if after the .string command we do not have param */

            noParam(error, lineCnt); /* error message */
            return;
        } /* end second if */
        noSpace(error, lineCnt); /* error message */
        return;
    } /* end first if */

    skipTabSpace(line); /* now we point on the parameter or end of line */

    if(*line == '\n' || *line == '\0') {

        noParam(error, lineCnt); /* error message */
        return;
    } /* end if */

    if (*line != '\"'){ /* the string should start with " */

        invalidStr(error, lineCnt); /* error message */
        return;
    } /* end if */

    line++; /* point on the first char in the string */

    while(line[i] != '\n' && line[i] != '\0') { /* the first pass over the string is to check closing of the " */

        if(line[i] == '\"') { /* save the index of the closing " */

            lastStrInd = i;
            noString = 0;
        } /* end if */

        if(i > lastStrInd && (line[i] != ' ' && line[i] != '\t')) /* if we reached to " but there ara more characters  */
            noString = 1;

        i++;
    } /* end while loop */

    if(lastStrInd == -1 || noString){

        invalidStr(error, lineCnt); /* error message */
        return;
    } /* end if */

    i = 0;

    while(i < lastStrInd) { /* We save the data line of the string */

        letter = *line;
        addDataLine(headDataTbl, (int) letter, firstIC);
        asmParam->DC += 1;
        asmParam->IC += 1;
        line++;
        i++;
    } /* end while loop */

    addDataLine(headDataTbl, 0, firstIC); /* add '\0' to the end of the string */
    asmParam->DC += 1;
    asmParam->IC += 1;
}

void readDataParam(essentials *asmParam, headData *headDataTbl, char *line, int *error, int lineCnt){

    int num = 0;
    int firstIC = asmParam->IC;
    char *comma = NULL; /* will point on the char after the num */
    errno = 0;

    if (*line != ' ' && *line != '\t') {

        if(*line == '\n' || *line == '\0') { /* check if after the .data command we do not have param */

            noParam(error, lineCnt); /* error message */
            return;
        } /* end second if */
        noSpace(error, lineCnt); /* error message */
        return;
    } /* end first if */

    skipTabSpace(line); /* now we point on the parameter or end of line */

    if(*line == '\n' || *line == '\0') {

        noParam(error, lineCnt); /* error message */
        return;
    } /* end if */

    if (*line == ',') {

        invalidComma(error, lineCnt); /* error message */
        return;
    } /* end if */

    while(*line != '\n' && *line != '\0') {

        num = strtol(line, &comma, 10); /* we want to check the number and the comma */

        if (line == comma || (errno != 0 && num == 0)) { /* the parameters are not numbers*/

            invalidDataParam(error, lineCnt); /* error message */
            return;
        } /* end if */

        else {

            skipTabSpace(comma);

            if(*comma == '\n' || *comma == '\0'){ /* we reach to end of line */

                addDataLine(headDataTbl, num, firstIC);
                asmParam->DC += 1;
                asmParam->IC += 1;
                return;
            } /* end if */

            if (*comma == '.') { /* if this is a real number (example: 0.1) */

                invalidDataParam(error, lineCnt); /* error message */
                return;
            } /* end if */

            else if (*comma != ',') {

                missingComma(error, lineCnt); /* error message */
                return;
            } /* end else if */

            else { /* *comma = ',' */

                comma++;
                skipTabSpace(comma); /* we want to check if the user enter 2 commas in row */

                if(*comma == '\n' || *comma == '\0'){

                    extraTxt(error, lineCnt); /* error message */
                    return;
                } /* end if */

                if (*comma == ',') {

                    multipleCommas(error, lineCnt); /* error message */
                    return;
                } /* end if */
            } /* end else */

            line = comma;
            addDataLine(headDataTbl, num, firstIC);
            asmParam->DC += 1;
            asmParam->IC += 1;
        } /* end else */
    } /* end while loop */
}

void readInst(essentials *asmParam, headData *headDataTbl, char *line , int *error, int lineCnt){

    int i = 0;
    int instructInd = -1;
    /* name of instructors */
    char *instructions [MAX_INSTRUCTIONS] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    for (i = 0; i < MAX_INSTRUCTIONS; i++) {

        if(checkWord(line, instructions[i])){ /* if we find the instruction name */

            instructInd = i; /* save the index of the instruction name */
            break;
        } /* end if */
    } /* end for loop */

    if(instructInd == -1){ /* we do not reach to valid instruction name */

        undefInsCmd(error, lineCnt); /* error message */
        return;
    } /* end if */

    line += strlen(instructions[i]); /* pass the instruction command */

    if(*line != ' ' && *line != '\t' && *line != '\n' && *line != '\0'){

        noSpace(error, lineCnt); /* error message */
        return;
    } /* end if */

    skipTabSpace(line);

    if(*line == ','){ /* if there is a comma before the parameters */

        invalidComma(error, lineCnt); /* error message */
        return;
    } /* end if */

    if(13 < instructInd){ /* there should be not operands */

        if(*line != '\n' && *line != '\0'){

            extraTxt(error, lineCnt); /* error message */
            return;
        } /* end second if */
        asmParam->IC++; /* we add one line in the final table */
        return;
    } /* end first if */

    checkInstParam(asmParam, line , error, lineCnt, instructInd);
}

void checkInstParam(essentials *asmParam, char *line , int *error, int lineCnt, int instructInd) {

    int jmpAddr = 0; /* equal to 1 when we reach to jump address in the line */
    int regi = 0; /* equal to 1 when we pass register */
    int passParam = 0; /* equal to 1 if we pass a parameter */
    int i = 0;

    if (*line == '\n' || *line == '\0') {

        noParam(error, lineCnt); /* error message */
        return;
    } /* end if */

    if(3 < instructInd && instructInd != 6){

        if (*line == 'r') { /* check if the first parameter is a register or label name */

            if (chkReg(line[1]) == -2) {

                invalidRegParam(error, lineCnt); /* error message */
                return;
            } /* end third if */

            if (chkReg(line[1]) != -1) {/* the parameter is register */

                asmParam->IC += 2;
                skipChars(line);
                skipTabSpace(line);

                if (*line != '\n' && *line != '\0')
                    extraTxt(error, lineCnt);

                return;
            } /* end third if */
        } /* end second if */

        else if (*line == '#') { /* the parameter is a number */

            line++;

            if (!chkNum(line)) {

                invalidNumParam(error, lineCnt); /* error message */
                return;
            } /* end if */

            if(instructInd == 12) { /* number can be a destination operand only in prn command */

                asmParam->IC += 2;
                skipChars(line);
                skipTabSpace(line);

                if (*line != '\n' && *line != '\0')
                    extraTxt(error, lineCnt);

                return;
            } /* end first if */

            illParam(error, lineCnt);
            return;
        } /* end else if */

        else { /* the parameter is label name or beginning of jump address */

            while (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0') { /* check if the line is a jump address */

                if (line[i] == '(') {

                    jmpAddr = 1;
                    i++;
                    break;
                } /* end if */
                i++;
            } /* end while loop */

            if ((instructInd == 9 || instructInd == 10 || instructInd == 13) && jmpAddr) { /* jmp or bne or jsr */

                line += i; /* point on the first parameter in ( ) */

                while (1) {

                    if (*line == 'r') { /* first parameter in the jump address is register or label name */

                        if (chkReg(line[1]) == -2) {

                            invalidRegParam(error, lineCnt); /* error message */
                            return;
                        } /* end if */

                        if (chkReg(line[1]) != -1) {/* the parameter is register */

                            if (regi || passParam) {

                                if (regi)
                                    asmParam->IC += 3;
                                if (passParam)
                                    asmParam->IC += 4;

                                skipChars(line);
                                skipTabSpace(line);
                                break;
                            } /* end third if */

                            regi = 1;
                            skipChars(line);
                            skipTabSpace(line);
                        } /* end second if */
                    } /* end first if */

                    else if (*line == '#') { /* the parameter is a number */

                        line++;

                        if (!chkNum(line)) {

                            invalidNumParam(error, lineCnt); /* error message */
                            return;
                        } /* end if */

                        if (regi || passParam) {

                            asmParam->IC += 4;
                            skipChars(line);
                            skipTabSpace(line);
                            break;
                        } /* end if */

                        passParam = 1;
                        skipChars(line);
                        skipTabSpace(line);
                    } /* end else if */

                    else { /* label name */

                        if (regi || passParam) {

                            asmParam->IC += 4;
                            skipChars(line);
                            skipTabSpace(line);
                            break;
                        } /* end if */

                        skipChars(line); /* skip label name */
                        skipTabSpace(line);
                        passParam = 1;
                    } /* end else */

                    if (*line != ',' && (passParam || regi)) {

                        missingComma(error, lineCnt); /* error message */
                        return;
                    } /* end if */

                    line++;
                    skipTabSpace(line);

                    if (*line == ',') {

                        multipleCommas(error, lineCnt); /* error message */
                        return;
                    } /* end if */
                } /* end while loop */

                if (*line != ')') {

                    extraTxt(error, lineCnt);
                    return;
                } /* end if */

                line++;
                skipTabSpace(line);

                if (*line != '\n' && *line != '\0')
                    extraTxt(error, lineCnt);

                return;
            } /* end first if */

            else {

                if (jmpAddr) { /* if we have '(' in the label name */

                    illLblName(error, lineCnt); /* error message */
                    return;
                } /* end if */

                asmParam->IC += 2;
                skipChars(line);
                skipTabSpace(line);

                if (*line != '\n' && *line != '\0')
                    extraTxt(error, lineCnt);

                return;
            } /* end else */
        } /* end major else */
    } /* end first if */

    else { /* the instruction include 2 param */

        while (1) {

            if (*line == 'r') {

                if (chkReg(line[1]) == -2) {


                    invalidRegParam(error, lineCnt); /* error message */
                    return;
                } /* end if */

                if (chkReg(line[1]) != -1) {

                    if (regi || passParam) {

                        if (regi)
                            asmParam->IC += 2;

                        if (passParam)
                            asmParam->IC += 3;

                        skipChars(line);
                        skipTabSpace(line);
                        break;
                    } /* end third if */

                    if(instructInd == 6){ /* register cant be source operand in lea command */

                        illParam(error, lineCnt);
                        return;
                    } /* end third if */

                    regi = 1;
                    skipChars(line);
                    skipTabSpace(line);
                } /* end second if */
            } /* end first if */

            else if (*line == '#') { /* the parameter is a number */

                line++;

                if (!chkNum(line)) {

                    invalidNumParam(error, lineCnt); /* error message */
                    return;
                } /* end if */

                if (regi || passParam) {

                    if(instructInd != 1){ /* number can be a destination operand only in cmp command */

                        illParam(error, lineCnt);
                        return;
                    } /* end second if */

                    asmParam->IC += 3;
                    skipChars(line);
                    skipTabSpace(line);
                    break;
                } /* end first if */

                if(instructInd == 6){ /* number cant be source operand in lea command */

                    illParam(error, lineCnt);
                    return;
                } /* end if */

                passParam = 1;
                skipChars(line);
                skipTabSpace(line);
            } /* end else if */

            else {

                skipChars(line); /* label name */
                skipTabSpace(line);

                if (regi || passParam) {

                    asmParam->IC += 3;
                    break;
                } /* end if */

                passParam = 1;
            } /* end else */

            if (*line != ',' && (passParam || regi)) {

                missingComma(error, lineCnt);
                return;
            } /* end if */

            line++;
            skipTabSpace(line);

            if (*line == ',') {

                multipleCommas(error, lineCnt);
                return;
            } /* end if */
        } /* end while loop */

        if (*line != '\n' && *line != '\0')
            extraTxt(error, lineCnt);

        return;
    } /* end major else */
}

void skipChars(char *line){

    while(*line && *line != '\n' && *line != '\0' && *line != '\t' && *line != ' '&& *line != ',' && *line != '(' && *line != ')'){

        memmove(line, line+1, strlen(line));
    } /* end while loop */
}

int chkReg(char num){

    if(isdigit(num) == 0) /* maybe it is a label name */
        return -1;

    else{

        int regNum = num - '0';

        if(0 <= regNum && regNum <= 7)
            return regNum;

        return -2;
    } /* end else */
}

int chkNum(char *line){

    int i = 0;

    if(line[i] == '-' || line[i] == '+')
        i++;

    while(line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0' && line[i] != ','){

        if(isdigit(line[i]) == 0) /* the char not digit */
            return  0;

        i++;
    } /* end while loop */

    return 1;
}
