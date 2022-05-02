#include "createExtern.h"

extern commandConst cc;

/*****************************************/

int createExtern (char *fileName, symbolPtr symbolHead) {
    char line[MAX_LINE_LEN+3];
    int try;
    FILE *filePtr;
    char inputFile[MAX_FILE_LEN+5];
    int okFlag =1;
    int lineNum =0;
    int IC =IC_INIT;
    int externFileOpened =0;

  	strncpy(inputFile, fileName, MAX_FILE_LEN+1);

    if ((filePtr = fopen(strcat(inputFile,".am"),"r")) == NULL) {
        printf("ERROR - Can't open %s\n",inputFile);
        return 0;
    }

    while(fgets(line, MAX_LINE_LEN+2, filePtr)) {
        lineNum++;
        try = createExternHandleLine(line, fileName, lineNum, &IC, symbolHead, &externFileOpened);
        okFlag = okFlag && try;
	}

	fclose(filePtr);
	return okFlag;
}

/*****************************************/

int createExternHandleLine (char line[], char *fileName, int lineNum, int *IC, symbolPtr symbolHead, int *externFileOpened) {
    int try;
    int okFlag =1;
    int command;
    char label[MAX_LABEL_LEN+2];
    int addressingModeOP1 =-1;
    int addressingModeOP2 =-1;
    int immidiateOP1;
    int immidiateOP2;
    char cmdLabelOP1[MAX_LABEL_LEN+2];
    char cmdLabelOP2[MAX_LABEL_LEN+2];
    char *str = line;
    int isLabel = 0;
    int ICinc=0;

   	str = skipBlanks(str);

   	if (str[0] == '\n' || str[0] == '\0') return okFlag; /* empty line */
   	if (str[0] == ';') return okFlag; /* comment */


   	try =  getLabel(str, label, &isLabel, fileName, lineNum);
   	if (isLabel) {
       	str = skipLabel(str);
   	}

   	command = getCommand(str, fileName, 0); /* get command */
   	if (command >=0) {
       	str = skipBlanks(str);
       	str += strlen(cc.commandNames[command]); /* skip command name */
       	try = str2AddressingModes(str, &addressingModeOP1, &addressingModeOP2, \
                                      &immidiateOP1, &immidiateOP2, \
                                      cmdLabelOP1, cmdLabelOP2, fileName, lineNum);
        ICinc = getICinc(command, addressingModeOP1, addressingModeOP2, fileName, lineNum);

        try = checkExtern(addressingModeOP1, cmdLabelOP1, fileName, lineNum, symbolHead, externFileOpened, *IC+2);
       	okFlag = okFlag && try;
        try = checkExtern(addressingModeOP2, cmdLabelOP2, fileName, lineNum, symbolHead, externFileOpened, *IC+2+2*((addressingModeOP1 == 1 || addressingModeOP1 == 2)));
       	okFlag = okFlag && try;

        *IC += ICinc;
    }

    return okFlag;
}


/*****************************************/

int checkExtern (int addressingMode, char *cmdLabel, char *fileName, int lineNum, symbolPtr symbolHead, int *externFileOpened, int IC) {
    symbolPtr p1;
    FILE *filePtr;
    char inputFile[MAX_FILE_LEN+5];

  	strncpy(inputFile, fileName, MAX_FILE_LEN+1);
    if (addressingMode == 1 || addressingMode == 2) {
        p1 = symbolFindVal(&symbolHead, cmdLabel);
        if (p1 == NULL) {
            printf("ERROR in %s line %d - Can't find label \"%s\"\n", inputFile, lineNum, cmdLabel);
            return 0;
        } else if (p1->attribute1==EXTERNAL || p1->attribute2==EXTERNAL) {
            if (!(*externFileOpened)) {
                if ((filePtr = fopen(strcat(inputFile,".ext"),"w")) == NULL) {
                    printf("ERROR - Can't open %s\n",inputFile);
                    return 0;
                }
                *externFileOpened = 1;
            } else {
                if ((filePtr = fopen(strcat(inputFile,".ext"),"a")) == NULL) {
                    printf("ERROR - Can't open %s\n",inputFile);
                    return 0;
                }
            }
            fprintf(filePtr, "%s BASE %04d\n",p1->symbolName, IC);
            fprintf(filePtr, "%s OFFSET %04d\n",p1->symbolName, IC+1);
            fprintf(filePtr, "\n");
	        fclose(filePtr);
        }
    }
    return 1;
}
