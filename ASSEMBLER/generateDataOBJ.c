#include "generateDataOBJ.h"

/*****************************************/

int generateDataOBJ (char *fileName, int IC, int DC,  symbolPtr symbolHead) {
    int try;
    FILE *filePtr;
    FILE *filePtrOBJ;
    char line[MAX_LINE_LEN+2];
    int lineNum = 0;
    int okFlag = 1;
    int DCnew = 0;
    char inputFile[MAX_FILE_LEN+5];
    char objFile[MAX_FILE_LEN+5];

    strncpy(inputFile, fileName, MAX_FILE_LEN+1);
    strncpy(objFile, fileName, MAX_FILE_LEN+1);

    if ((filePtrOBJ = fopen(strcat(objFile,".ob"),"r")) != NULL) { /* file exist */
        fclose(filePtrOBJ);
    } else {
        if ((filePtrOBJ = fopen(objFile,"w")) == NULL) {
            printf("ERROR - Can't open %s\n",objFile);
            return 0;
        }
        fclose(filePtrOBJ);
    }
    if ((filePtr = fopen(strcat(inputFile,".am"),"r")) == NULL) {
        printf("ERROR - Can't open %s\n",inputFile);
        return 0;
    }

    while(fgets(line, MAX_LINE_LEN+1, filePtr)) {
        lineNum++;
        try = generateDataOBJLine(line, lineNum, fileName, IC, &DCnew, symbolHead);
        okFlag = okFlag && try;
    }

    fclose(filePtr);
    return okFlag;
}

/*****************************************/

int generateDataOBJLine(char line[], int lineNum, char *fileName, int IC, int *DC, symbolPtr symbolHead) {
    FILE *filePtr;
    int i0;
    int try;
    int dataOut;
    char string[MAX_LINE_LEN];
    char label[MAX_LABEL_LEN+2];
    int data[MAX_LINE_LEN/2] ={0};
    char *str = line;
    int isLabel = 0;
    int isString = 0;
    /*int isData = 0;*/
    int okFlag =1;
    char inputFile[MAX_FILE_LEN+5];
    char strOut[50];

    strncpy(inputFile, fileName, MAX_FILE_LEN+1);

    str = skipBlanks(str);

    if (str[0] == '\n' || str[0] == '\0') return okFlag; /* empty line */
    if (str[0] == ';') return okFlag; /* comment */

    if ((filePtr = fopen(strcat(inputFile,".ob"),"a")) == NULL) {
        printf("ERROR - Can't open %s\n",inputFile);
        return 0;
    }

    try =  getLabel(str, label, &isLabel, fileName, lineNum);
    okFlag = okFlag && try;
    if (isLabel) {
        str = skipLabel(str);
    }

    try = getData(str, &data[0], fileName, lineNum); /* get data */
    okFlag = okFlag && try;
    if (okFlag)
        if (data[0] >0) {
            /*isData = 1;*/
            for (i0 = 1; i0 <= data[0]; i0 = i0+1){ 
                dataOut = ((data[i0] & 0xffff) | (0x4 << 16));
                num2outstr(IC+*DC,dataOut,strOut);
                fprintf(filePtr, "%s\n",strOut);
                *DC +=1;
            }
        } 

    try = getString(str, string, &isString, fileName, lineNum);
    okFlag = okFlag && try;
    if (isString) {
        for (i0 = 0; i0 <= strlen(string); i0 = i0+1){
            dataOut = ((string[i0] & 0xff) | (0x4 << 16));
            num2outstr(IC+*DC,dataOut,strOut);
            fprintf(filePtr, "%s\n",strOut);
            *DC +=1;
        }
    }

    fclose(filePtr);
    return okFlag;
    
}

