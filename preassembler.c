
#include "preassembler.h"

void no_spaces(char *pointer) {

    int i, count = 0; /* To keep track of non-space character count */

    /* Traverse the given string. If current character
     * is not space, then place it at index 'count++' */
    for (i = 0; pointer[i] != '\n' ; i++) {

        if (pointer[i] != ' ' && pointer[i] != '\t')
            pointer[count++] = pointer[i]; /* here count is incremented */
    } /* end for loop */

    pointer[count] = '\0'; /* finish the string */
}

void no_tabs(char *pointer) {

    int i;

    for (i = 0;  *pointer == ' ' || *pointer == '\t' ; i++) {
        memmove(pointer, pointer+1, strlen(pointer));
    } /* end for loop */
}

void openMcr(char *amFileName, char *asFileName, int *error){

    FILE *amFile = NULL;
    FILE *asFile = NULL;
    char line [MAX_LINE]= {'\0'};
    char mcrName[MAX_LINE] = {'\0'};
    char copyLine [MAX_LINE]= {'\0'};
    int firstString = -1 ; /* the position of amFile in the file with ftell() */
    int numberOfLines = 0; /* count the amount lines in the macro */
    int inMcr = 0, skipMcr = 0; /* flag if we are point in macro block, flag if we past at list one macro */
    int countLines = 0;
    mcrHead *mcr = createMcrList(); /* head of the macro list */

    if(!(amFile = fileNotCreated(amFile, amFileName, "w+", error)))
        return;

    if(!(asFile = fileNotCreated(asFile, asFileName, "r+", error)))
        return;

    while (fgets(line, MAX_LINE, asFile) != NULL) { /* we run until we get to the end of file */

        char *ptrLine = line;
        fpos_t pos;
        fgetpos(asFile, &pos);
        strcpy(copyLine, line);
        countLines++;

        no_tabs(ptrLine);
        /* we check if we get to macro block */
        if (goodMcr (ptrLine, "mcr")) {

            ptrLine += 3;
            no_spaces(ptrLine); /* we want only the name of the macro */

            if (!badName(ptrLine)){

                inMcr = 1;
                skipMcr = 1;

                if (firstString == -1)
                    firstString = (int) ftell(asFile); /* save the position of the file pointer from the beginning of the file to the first line of the macro  */

                strcpy(mcrName, ptrLine);
                continue;
            } /* end second if */

            badMcr(error, countLines);
            continue;
        } /* end first if */

        /* we get to the end of the macro */
        if (goodMcr (ptrLine, "endmcr") && inMcr) {

            inMcr = 0;
            addMcrList(mcr, mcrName, firstString, numberOfLines);
            numberOfLines = 0;
            firstString = -1 ;
            continue;
        } /* end if */

        /* we in the macro block */
        if (inMcr) {

            numberOfLines++;
            continue;
        } /* end if */

        if (skipMcr) {

            if (checkMcr(mcr, ptrLine, amFile, asFile)) {

                fsetpos(asFile, &pos);
                continue;
            } /* end second if */
        } /* end first if */

        fputs(copyLine, amFile);
    } /* end while loop */

    freeMcrTbl(mcr);
    fclose(amFile);
    fclose(asFile);

}

int checkMcr(mcrHead *mcr, char *line, FILE *amFile, FILE *source){

    int i = 0;
    char copyLine[MAX_LINE] = {'\0'};
    mcrList *temp = mcr->head;

    while(temp != NULL){

        if(goodMcr(line, temp->mcrName)){

            fseek(source, temp->mcrLocation, SEEK_SET); /* we point with the file pointer to the start of the macro first line */

            while(i < temp->numberOfLines){

                fgets(copyLine, MAX_LINE, source);
                fputs(copyLine, amFile);
                i++ ;
            } /* end second while loop */
            return 1;
        } /* end if */
        temp = temp->next;
    } /* end first while loop */

    return 0;
}

int goodMcr (char *line, char *word){

    int i = 0;
    int charCount = 0;
    int nameLength = strlen(word);

    while((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z') || (line[i] >= '0' && line[i] <= '9') || line[i] == '#' || line[i] == '.' ){

        charCount++;
        i++;
    } /* end while loop */

    if(nameLength != charCount)
        return 0;

    if(strncmp(line, word, charCount) == 0)
        return 1;

    return 0;
}




