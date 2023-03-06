
#include "preassembler.h"

void remSpaces(char *p) {

    int i, count = 0; /* To keep track of non-space character count */

    /* Traverse the given string. If current character
     * is not space, then place it at index 'count++' */
    for (i = 0; p[i] != '\n' ; i++) {

        if (p[i] != ' ' && p[i] != '\t')
            p[count++] = p[i]; /* here count is incremented */
    } /* end for loop */

    p[count] = '\0'; /* finish the string */
}

void skipTabSpace(char *p) {

    int i;

    for (i = 0;  *p == ' ' || *p == '\t' ; i++) {
        memmove(p, p+1, strlen(p));
    } /* end for loop */
}

void openMcr(char *amFileName, char *asFileName, int *error){

    FILE *amFile = NULL;
    FILE *asFile = NULL;
    char line [MAX_LINE]= {'\0'};
    char mcrName[MAX_LINE] = {'\0'};
    char copyLine [MAX_LINE]= {'\0'};
    int posFirstMcrLine = -1 ; /* the position of amFile in the file with ftell() */
    int cntMcrLines = 0; /* count the amount lines in the macro */
    int inMcr = 0, skipMcr = 0; /* flag if we are point in macro block, flag if we past at list one macro */
    int lineCnt = 0;
    mcrHead *mcr = crtMcrTbl(); /* head of the macro list */

    if(!(amFile = chkFileOpen(amFile, amFileName, "w+", error)))
        return;

    if(!(asFile = chkFileOpen(asFile, asFileName, "r+", error)))
        return;

    while (fgets(line, MAX_LINE, asFile) != NULL) { /* we run until we get to the end of file */

        char *ptrLine = line;
        fpos_t pos;
        fgetpos(asFile, &pos);
        strcpy(copyLine, line);
        lineCnt++;

        skipTabSpace(ptrLine);
        /* we check if we get to macro block */
        if (chkWord (ptrLine, "mcr")) {

            ptrLine += 3;
            remSpaces(ptrLine); /* we want only the name of the macro */

            if (!invalidName(ptrLine)){

                inMcr = 1;
                skipMcr = 1;

                if (posFirstMcrLine == -1)
                    posFirstMcrLine = (int) ftell(asFile); /* save the position of the file pointer from the beginning of the file to the first line of the macro  */

                strcpy(mcrName, ptrLine);
                continue;
            } /* end second if */

            illMcrName(error, lineCnt);
            continue;
        } /* end first if */

        /* we get to the end of the macro */
        if (chkWord (ptrLine, "endmcr") && inMcr) {

            inMcr = 0;
            addMcrTbl(mcr, mcrName, posFirstMcrLine, cntMcrLines);
            cntMcrLines = 0;
            posFirstMcrLine = -1 ;
            continue;
        } /* end if */

        /* we in the macro block */
        if (inMcr) {

            cntMcrLines++;
            continue;
        } /* end if */

        if (skipMcr) {

            if (chkMcrName(mcr, ptrLine, amFile, asFile)) {

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

int chkMcrName(mcrHead *mcr, char *line, FILE *amFile, FILE *source){

    int i = 0;
    char copyLine[MAX_LINE] = {'\0'};
    mcrTbl *tmp = mcr->head;

    while(tmp != NULL){

        if(chkWord(line, tmp->mcrName)){

            fseek(source, tmp->pos_FirstMcrLine, SEEK_SET); /* we point with the file pointer to the start of the macro first line */

            while(i < tmp->numOfLines){

                fgets(copyLine, MAX_LINE, source);
                fputs(copyLine, amFile);
                i++ ;
            } /* end second while loop */
            return 1;
        } /* end if */
        tmp = tmp->next;
    } /* end first while loop */

    return 0;
}

int chkWord (char *line, char *word){

    int i = 0;
    int letterCnt = 0;
    int lenWord = strlen(word);

    while((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z') || (line[i] >= '0' && line[i] <= '9') || line[i] == '#' || line[i] == '.' ){

        letterCnt++;
        i++;
    } /* end while loop */

    if(lenWord != letterCnt)
        return 0;

    if(strncmp(line, word, letterCnt) == 0)
        return 1;

    return 0;
}




