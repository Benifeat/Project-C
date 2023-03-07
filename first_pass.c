
#include "first_pass.h"

int label (char *line){

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

int goodLabelName(symbolLine *symbol, char *labelName, int type, int *error , int countLines){

    symbolList  *temp = symbol->head;
    int len = strlen(labelName);
    int i;

    if (labelName[0] == ' '){

        LabelToLong(error, countLines);
        return 0;
    } /* end if */

    if(type) {/* equal to 1 if the label end with ':' */

        labelName[len - 1] = '\0';
        len--;
    } /* end if */

    for (i = 0; i < len ; i++) {

        if(labelName[i] == ' ' || labelName[i] == '\t'){

            textOverBound(error, countLines);
            return 0;
        } /* end if */

        if(!((labelName[i] >= 'A' && labelName[i] <= 'Z') || (labelName[i] >= 'a' && labelName[i] <= 'z') || (labelName[i] >= '0' && labelName[i] <= '9'))) {

            badLabelName(error, countLines);
            return 0;
        } /* end if */
    } /* end for loop */

    if(badName(labelName)) { /* check if the name of the label is register name or instruction or directive command or if not exists label name */

        badLabelName(error, countLines);
        return 0;
    } /* end if */

    if (!temp)
        return 1;

    while(temp != NULL) { /* check all the names in the table of label name  */

        if(len == strlen(temp->symbolName)) {

            if (!strcmp(labelName, temp->symbolName)) { /* check if the new label name is in the table */

                existingLabelName(error, countLines); /* error message */
                return 0;
            } /* end second if */
        } /* end first if */

        temp = temp->next;
    } /* end while loop */

    return 1;
}

void first(char *asFileName, data_base *asmValues, symbolLine *symbol, headLine *dataListStart, int *error, int *entFlag, int *extFlag) {

    FILE *asFile = NULL;
    int countLines = 0;
    char line[MAX_LINE] = {'\0'};

    if(!(asFile = fileNotCreated(asFile, asFileName, "r+", error)))
        return;

    while(fgets(line, MAX_LINE, asFile) != NULL) { /* check line after line in the as file */

        char firstLabelName[MAX_SYMBOL] = {'\0'};
        char firstLabelType[MAX_TYPE] = {'\0'}; /* save the type of the symbol ( code / data / external) */
        int firstLabelFlag = 0; /* turned on if we find label name in the line */
        char *ptrLine = line;
        countLines++;

        no_tabs(ptrLine); /* we want to point on the first char in the line text */

        /* if it is an empty line we skip */
        if (*ptrLine == '\n' || *ptrLine == '\0')
            continue;

        /* if it is a comment line we skip */
        if (*ptrLine == ';')
            continue;

        if(strlen(ptrLine) == MAX_LINE-1){

            chkLineLen(error, countLines);

            while(strlen(ptrLine) == MAX_LINE-1) { /* we will go through the entire line before moving on to the next line in the file */

                if (fgets(line, MAX_LINE, asFile) == NULL) {

                    fclose(asFile);
                    return;
                } /* end if */
            } /* end while loop */

            continue;
        } /* end if */

        firstLabelFlag = label(ptrLine); /* if the name is valid we point on it with ptr_label and return 1 */

        if(firstLabelFlag == -1){

            spacedLabel(error, countLines);
            continue;
        } /* end if */

        if (firstLabelFlag == 1) {

            saveLabel(ptrLine, firstLabelName);

            if (!goodLabelName(symbol, firstLabelName, 1, error, countLines))
                continue;

            ptrLine += strlen(firstLabelName) + 1; /* point after the label name and the ':' */
            no_tabs(ptrLine);
        } /* end if */

        if (*ptrLine == '.') { /* the start of the data, string, extern, entry directive command */

            if (checkWord(ptrLine, ".data") || checkWord(ptrLine, ".string")) {

                if (firstLabelFlag) {

                    strcpy(firstLabelType, "data"); /* save the label type */
                    addSymbolList(symbol, firstLabelName, firstLabelType, asmValues->IC);
                } /* end if */

                if (checkWord(ptrLine, ".data")) {

                    ptrLine += 5; /* point after the .data directive */
                    checkDataValues(asmValues, dataListStart, ptrLine, error, countLines);
                    continue;
                } /* end if */

                if (checkWord(ptrLine, ".string")) {

                    ptrLine += 7; /* point after the .string directive */
                    checkStringValues(asmValues, dataListStart, ptrLine, error, countLines);
                    continue;
                } /* end if */
            } /* end labels if */

            if (checkWord(ptrLine, ".entry")) {

                *entFlag = 1;
                ptrLine += 6;

                if (firstLabelFlag) { /* if we pass a label name before .entry */

                    dumpLabel(error, countLines);
                    continue;
                } /* end if */

                no_tabs(ptrLine);

                if (*ptrLine == '\n' || *ptrLine == '\0') {

                    unDefienedLabel(error, countLines);
                    continue;
                } /* end if */

                skipChars(ptrLine); /* skip the label name */
                no_tabs(ptrLine);

                if (*ptrLine != '\n' && *ptrLine != '\0') {

                    textOverBound(error, countLines);
                    continue;
                } /* end if */

                continue;
            } /* end entry if */

            if (checkWord(ptrLine, ".extern")) {

                *extFlag = 1;
                strcpy(firstLabelType, "ext");
                ptrLine += 7;

                if (firstLabelFlag) { /* if we pass a label name before .extern */

                    dumpLabel(error, countLines);
                    continue;
                } /* end if */

                no_tabs(ptrLine);

                if (*ptrLine == '\n' || *ptrLine == '\0') {

                    unDefienedLabel(error, countLines);
                    continue;
                } /* end if */

                saveLabel(ptrLine, firstLabelName);

                if (goodLabelName(symbol, firstLabelName, 0, error, countLines)) {

                    skipChars(ptrLine);
                    no_tabs(ptrLine);

                    if (*ptrLine != '\n' && *ptrLine != '\0') {

                        textOverBound(error, countLines);
                        continue;
                    } /* end second if */

                    addSymbolList(symbol, firstLabelName, firstLabelType, 0);
                    continue;
                } /* end first if */
                continue;
            } /* end extern if */

            else { /* the word from the text file start with '.' but it is not a directive command */

                unDefienedcommand(error, countLines);
                continue;
            } /* end else */
        } /* end first if */

        if (firstLabelFlag) {

            strcpy(firstLabelType, "code");
            addSymbolList(symbol, firstLabelName, firstLabelType, asmValues->IC);
            checkInstructions(asmValues, dataListStart, ptrLine, error, countLines);
            continue;
        } /* end if */

        checkInstructions(asmValues, dataListStart, ptrLine, error, countLines);
    } /* end while loop */
    fclose(asFile);
}

void saveLabel(char *line, char *firstLabelName){

    int i;

    for(i = 0 ; line[i] != '\n' && line[i] != '\0' && i < MAX_SYMBOL ; i++){

        firstLabelName[i] = line[i];

        if(line[i] == ':' || line[i] == ')' || line[i] == ',' ){

            firstLabelName[i] = line[i];
            firstLabelName[i+1] = '\0';
            return;
        } /* end if */
    } /* end for loop */

    if(firstLabelName[MAX_SYMBOL-1] != '\0')
        firstLabelName[0] = ' ';

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

void checkStringValues(data_base *asmValues, headLine *headLineStart, char *line, int *error, int countLines){

    char character = '\0' ;
    int i = 0;
    int ic_start = asmValues->IC;
    int saveLastIndex = -1; /* save the index of the closing " */
    int noString = 0; /* equal to 1 if there are more characters after " */

    if (*line != ' ' && *line != '\t') {

        if(*line == '\n' || *line == '\0') { /* check if after the .string command we do not have value */

            noParameter(error, countLines); /* error message */
            return;
        } /* end second if */
        noSpace(error, countLines); /* error message */
        return;
    } /* end first if */

    no_tabs(line); /* now we point on the parameter or end of line */

    if(*line == '\n' || *line == '\0') {

        noParameter(error, countLines); /* error message */
        return;
    } /* end if */

    if (*line != '\"'){ /* the string should start with " */

        badString(error, countLines); /* error message */
        return;
    } /* end if */

    line++; /* point on the first char in the string */

    while(line[i] != '\n' && line[i] != '\0') { /* the first pass over the string is to check closing of the " */

        if(line[i] == '\"') { /* save the index of the closing " */

            saveLastIndex = i;
            noString = 0;
        } /* end if */

        if(i > saveLastIndex && (line[i] != ' ' && line[i] != '\t')) /* if we reached to " but there ara more characters  */
            noString = 1;

        i++;
    } /* end while loop */

    if(saveLastIndex == -1 || noString){

        badString(error, countLines); /* error message */
        return;
    } /* end if */

    i = 0;

    while(i < saveLastIndex) { /* We save the data line of the string */

        character = *line;
        addDataLine(headLineStart, (int) character, ic_start);
        asmValues->DC += 1;
        asmValues->IC += 1;
        line++;
        i++;
    } /* end while loop */

    addDataLine(headLineStart, 0, ic_start); /* add '\0' to the end of the string */
    asmValues->DC += 1;
    asmValues->IC += 1;
}

void checkDataValues(data_base *asmValues, headLine *headLineStart, char *line, int *error, int countLines){

    int num = 0;
    int ic_start = asmValues->IC;
    char *comma = NULL; /* will point on the char after the num */
    errno = 0;

    if (*line != ' ' && *line != '\t') {

        if(*line == '\n' || *line == '\0') { /* check if after the .data command we do not have value */

            noParameter(error, countLines); /* error message */
            return;
        } /* end second if */
        noSpace(error, countLines); /* error message */
        return;
    } /* end first if */

    no_tabs(line); /* now we point on the parameter or end of line */

    if(*line == '\n' || *line == '\0') {

        noParameter(error, countLines); /* error message */
        return;
    } /* end if */

    if (*line == ',') {

        badComma(error, countLines); /* error message */
        return;
    } /* end if */

    while(*line != '\n' && *line != '\0') {

        num = strtol(line, &comma, 10); /* we want to check the number and the comma */

        if (line == comma || (errno != 0 && num == 0)) { /* the parameters are not numbers*/

            invalidDataParam(error, countLines); /* error message */
            return;
        } /* end if */

        else {

            no_tabs(comma);

            if(*comma == '\n' || *comma == '\0'){ /* we reach to end of line */

                addDataLine(headLineStart, num, ic_start);
                asmValues->DC += 1;
                asmValues->IC += 1;
                return;
            } /* end if */

            if (*comma == '.') { /* if this is a real number (example: 0.1) */

                invalidDataParam(error, countLines); /* error message */
                return;
            } /* end if */

            else if (*comma != ',') {

                noComma(error, countLines); /* error message */
                return;
            } /* end else if */

            else { /* *comma = ',' */

                comma++;
                no_tabs(comma); /* we want to check if the user enter 2 commas in row */

                if(*comma == '\n' || *comma == '\0'){

                    textOverBound(error, countLines); /* error message */
                    return;
                } /* end if */

                if (*comma == ',') {

                    toManyCommas(error, countLines); /* error message */
                    return;
                } /* end if */
            } /* end else */

            line = comma;
            addDataLine(headLineStart, num, ic_start);
            asmValues->DC += 1;
            asmValues->IC += 1;
        } /* end else */
    } /* end while loop */
}

void checkInstructions(data_base *asmValues, headLine *headLineStart, char *line , int *error, int countLines){

    int i = 0;
    int instructionIndex = -1;
    /* name of instructors */
    char *instructions [MAX_INSTRUCTIONS] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    for (i = 0; i < MAX_INSTRUCTIONS; i++) {

        if(checkWord(line, instructions[i])){ /* if we find the instruction name */

            instructionIndex = i; /* save the index of the instruction name */
            break;
        } /* end if */
    } /* end for loop */

    if(instructionIndex == -1){ /* we do not reach to valid instruction name */

        undefinedInstruction(error, countLines); /* error message */
        return;
    } /* end if */

    line += strlen(instructions[i]); /* pass the instruction command */

    if(*line != ' ' && *line != '\t' && *line != '\n' && *line != '\0'){

        noSpace(error, countLines); /* error message */
        return;
    } /* end if */

    no_tabs(line);

    if(*line == ','){ /* if there is a comma before the parameters */

        badComma(error, countLines); /* error message */
        return;
    } /* end if */

    if(13 < instructionIndex){ /* there should be not operands */

        if(*line != '\n' && *line != '\0'){

            textOverBound(error, countLines); /* error message */
            return;
        } /* end second if */
        asmValues->IC++; /* we add one line in the final table */
        return;
    } /* end first if */

    checkInstructionValues(asmValues, line , error, countLines, instructionIndex);
}

void checkInstructionValues(data_base *asmValues, char *line , int *error, int countLines, int instructionIndex) {

    int addressFlag = 0; /* equal to 1 when we reach to jump address in the line */
    int registerFlag = 0; /* equal to 1 when we pass register */
    int parameterFlag = 0; /* equal to 1 if we pass a parameter */
    int i = 0;

    if (*line == '\n' || *line == '\0') {

        noParameter(error, countLines); /* error message */
        return;
    } /* end if */

    if(3 < instructionIndex && instructionIndex != 6){

        if (*line == 'r') { /* check if the first parameter is a register or label name */

            if (checkRegister(line[1]) == -2) {

                badRegister(error, countLines); /* error message */
                return;
            } /* end third if */

            if (checkRegister(line[1]) != -1) {/* the parameter is register */

                asmValues->IC += 2;
                skipChars(line);
                no_tabs(line);

                if (*line != '\n' && *line != '\0')
                    textOverBound(error, countLines);

                return;
            } /* end third if */
        } /* end second if */

        else if (*line == '#') { /* the parameter is a number */

            line++;

            if (!checkNumber(line)) {

                badNumber(error, countLines); /* error message */
                return;
            } /* end if */

            if(instructionIndex == 12) { /* number can be a destination operand only in prn command */

                asmValues->IC += 2;
                skipChars(line);
                no_tabs(line);

                if (*line != '\n' && *line != '\0')
                    textOverBound(error, countLines);

                return;
            } /* end first if */

            badValue(error, countLines);
            return;
        } /* end else if */

        else { /* the parameter is label name or beginning of jump address */

            while (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0') { /* check if the line is a jump address */

                if (line[i] == '(') {

                    addressFlag = 1;
                    i++;
                    break;
                } /* end if */
                i++;
            } /* end while loop */

            if ((instructionIndex == 9 || instructionIndex == 10 || instructionIndex == 13) && addressFlag) { /* jmp or bne or jsr */

                line += i; /* point on the first parameter in ( ) */

                while (1) {

                    if (*line == 'r') { /* first parameter in the jump address is register or label name */

                        if (checkRegister(line[1]) == -2) {

                            badRegister(error, countLines); /* error message */
                            return;
                        } /* end if */

                        if (checkRegister(line[1]) != -1) {/* the parameter is register */

                            if (registerFlag || parameterFlag) {

                                if (registerFlag)
                                    asmValues->IC += 3;
                                if (parameterFlag)
                                    asmValues->IC += 4;

                                skipChars(line);
                                no_tabs(line);
                                break;
                            } /* end third if */

                            registerFlag = 1;
                            skipChars(line);
                            no_tabs(line);
                        } /* end second if */
                    } /* end first if */

                    else if (*line == '#') { /* the parameter is a number */

                        line++;

                        if (!checkNumber(line)) {

                            badNumber(error, countLines); /* error message */
                            return;
                        } /* end if */

                        if (registerFlag || parameterFlag) {

                            asmValues->IC += 4;
                            skipChars(line);
                            no_tabs(line);
                            break;
                        } /* end if */

                        parameterFlag = 1;
                        skipChars(line);
                        no_tabs(line);
                    } /* end else if */

                    else { /* label name */

                        if (registerFlag || parameterFlag) {

                            asmValues->IC += 4;
                            skipChars(line);
                            no_tabs(line);
                            break;
                        } /* end if */

                        skipChars(line); /* skip label name */
                        no_tabs(line);
                        parameterFlag = 1;
                    } /* end else */

                    if (*line != ',' && (parameterFlag || registerFlag)) {

                        noComma(error, countLines); /* error message */
                        return;
                    } /* end if */

                    line++;
                    no_tabs(line);

                    if (*line == ',') {

                        toManyCommas(error, countLines); /* error message */
                        return;
                    } /* end if */
                } /* end while loop */

                if (*line != ')') {

                    textOverBound(error, countLines);
                    return;
                } /* end if */

                line++;
                no_tabs(line);

                if (*line != '\n' && *line != '\0')
                    textOverBound(error, countLines);

                return;
            } /* end first if */

            else {

                if (addressFlag) { /* if we have '(' in the label name */

                    badLabelName(error, countLines); /* error message */
                    return;
                } /* end if */

                asmValues->IC += 2;
                skipChars(line);
                no_tabs(line);

                if (*line != '\n' && *line != '\0')
                    textOverBound(error, countLines);

                return;
            } /* end else */
        } /* end major else */
    } /* end first if */

    else { /* the instruction include 2 value */

        while (1) {

            if (*line == 'r') {

                if (checkRegister(line[1]) == -2) {


                    badRegister(error, countLines); /* error message */
                    return;
                } /* end if */

                if (checkRegister(line[1]) != -1) {

                    if (registerFlag || parameterFlag) {

                        if (registerFlag)
                            asmValues->IC += 2;

                        if (parameterFlag)
                            asmValues->IC += 3;

                        skipChars(line);
                        no_tabs(line);
                        break;
                    } /* end third if */

                    if(instructionIndex == 6){ /* register cant be source operand in lea command */

                        badValue(error, countLines);
                        return;
                    } /* end third if */

                    registerFlag = 1;
                    skipChars(line);
                    no_tabs(line);
                } /* end second if */
            } /* end first if */

            else if (*line == '#') { /* the parameter is a number */

                line++;

                if (!checkNumber(line)) {

                    badNumber(error, countLines); /* error message */
                    return;
                } /* end if */

                if (registerFlag || parameterFlag) {

                    if(instructionIndex != 1){ /* number can be a destination operand only in cmp command */

                        badValue(error, countLines);
                        return;
                    } /* end second if */

                    asmValues->IC += 3;
                    skipChars(line);
                    no_tabs(line);
                    break;
                } /* end first if */

                if(instructionIndex == 6){ /* number cant be source operand in lea command */

                    badValue(error, countLines);
                    return;
                } /* end if */

                parameterFlag = 1;
                skipChars(line);
                no_tabs(line);
            } /* end else if */

            else {

                skipChars(line); /* label name */
                no_tabs(line);

                if (registerFlag || parameterFlag) {

                    asmValues->IC += 3;
                    break;
                } /* end if */

                parameterFlag = 1;
            } /* end else */

            if (*line != ',' && (parameterFlag || registerFlag)) {

                noComma(error, countLines);
                return;
            } /* end if */

            line++;
            no_tabs(line);

            if (*line == ',') {

                toManyCommas(error, countLines);
                return;
            } /* end if */
        } /* end while loop */

        if (*line != '\n' && *line != '\0')
            textOverBound(error, countLines);

        return;
    } /* end major else */
}

void skipChars(char *line){

    while(*line && *line != '\n' && *line != '\0' && *line != '\t' && *line != ' '&& *line != ',' && *line != '(' && *line != ')'){

        memmove(line, line+1, strlen(line));
    } /* end while loop */
}

int checkRegister(char num){

    if(isdigit(num) == 0) /* maybe it is a label name */
        return -1;

    else{

        int regNum = num - '0';

        if(0 <= regNum && regNum <= 7)
            return regNum;

        return -2;
    } /* end else */
}

int checkNumber(char *line){

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
