#include "assembler.h"

/* The main method: */
int main(int argc, char *argv[]) {
    int fileCount; 

    if (argc==1) {
        printf("ERROR - no files given \n");
        return 0;
    }

    for (fileCount = 1; fileCount < argc; fileCount++) {
        if (strlen(argv[fileCount]) > MAX_FILE_LEN) 
            printf("ERROR - file name \"%s\" has more characters than allowed\n",argv[fileCount]);
        else assembler(argv[fileCount]);
    } 
    return 1;
}

/*************************************/

void assembler(char *fileName) {
    int try;
    int okFlag=1;
    int IC = IC_INIT;
    int DC = 0;
    symbolPtr symbolHead = NULL;


    if (okFlag) {
        try = macroHandler(fileName);
        okFlag = okFlag & try;
    }

    if (okFlag) {
        try = generateSymbolTable(fileName, &IC, &DC, &symbolHead);
        okFlag = okFlag & try;
    }

    if (okFlag) {
        try =  generateCodeOBJ(fileName, IC, DC, symbolHead);
        okFlag = okFlag & try;
    }

    if (okFlag) {
        try =  generateDataOBJ(fileName, IC, DC, symbolHead);
        okFlag = okFlag & try;
    }

    if (okFlag) {
        try = createEntry(fileName, symbolHead, IC);
        okFlag = okFlag & try;
    }

    if (okFlag) {
        try = createExtern (fileName, symbolHead);
        okFlag = okFlag & try;
    }

    symbolFreeList(&symbolHead);

    if (okFlag) {
        printf("%s finished successfully\n",fileName);
    }
    else {
        printf ("%s FAILED\n",fileName);
        removeFiles(fileName);
    }
}

void removeFiles(char *fileName) {
char inputFile[MAX_FILE_LEN+5];

   strncpy(inputFile, fileName, MAX_FILE_LEN+1);
   remove(strcat(inputFile,".ent"));
   strncpy(inputFile, fileName, MAX_FILE_LEN+1);
   remove(strcat(inputFile,".ext"));
   strncpy(inputFile, fileName, MAX_FILE_LEN+1);
   remove(strcat(inputFile,".ob"));
}
