

#include "assembler.h"

/* main function */
int main(int args, char * arguv[]){

    int i;
    chkNumArgs(args);

    for (i = 1; i < args ; i++){
        process(arguv[i]);
    } /* end for loop */

    return 0;
}

void removeEmptyFile(char *filename){
    FILE *fp;
    fp = fopen(filename, "r");
    /* Moving pointer to end*/
    fseek(fp, 0, SEEK_END);
    if(ftell(fp) == 0)
        remove(filename);
}

void createFile (char *newFileName, char *currentFile, char *endFileName){

    sprintf(newFileName, "%s.%s", currentFile, endFileName);
}

void process(char * currentFile){

    essentials *asmParam;
    symHead *headSymTbl;
    headData *headDataTbl;
    char amFile[MAX_LENGTH] = {"\0"};
    char asFile[MAX_LENGTH] = {"\0"};
    char extFile[MAX_LENGTH] = {"\0"};
    char entFile[MAX_LENGTH] = {"\0"};
    char obFile[MAX_LENGTH] = {"\0"};
    int error = 0;
    int entFlag = 0;
    int extFlag = 0;

    createFile(amFile, currentFile, "am");
    createFile(asFile, currentFile, "as");

    openMcr(amFile, asFile, &error); /* updates the am file by arranging the macros in the as file */

    if(error){

        printf("The program ended, an error was found in the pre-assembler.\n");
        remove(amFile);
        return;
    } /* end if */

    asmParam = crtEsn();  /* initialize IC to 100 and DC to 0 */
    headDataTbl = crtDataTbl();
    headSymTbl = crtSymTbl();

    first(amFile, asmParam, headSymTbl, headDataTbl, &error, &entFlag, &extFlag); /* updates symbol table and a first search for errors in the file */

    if(error){

        freeSymTbl(headSymTbl);
        freeDataTbl(headDataTbl);
        free(asmParam);
        remove(amFile);

        printf("The program ended, an error was found in the first pass.\n");
        return;
    } /* end if */

    createFile(extFile, currentFile, "ext");
    createFile(entFile, currentFile, "ent");
    createFile(obFile, currentFile, "ob"); /* the final file */

    second(asmParam, headSymTbl, headDataTbl, amFile, extFile, entFile, obFile, &error, &extFlag, &entFlag);

    freeSymTbl(headSymTbl);
    free(asmParam);

    if(error){

        remove(amFile);
        remove(obFile);

        if(extFlag)
            remove(extFile);
        if(entFlag)
            remove(entFile);

        printf("The program ended, an error was found in the second pass.\n");
        exit(0);
    } /* end if */

    removeEmptyFile(extFile);
    removeEmptyFile(entFile);

    printf("finished assembler process for: %s .\n", asFile);

}




