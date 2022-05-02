#include "generateSymbolTable.h"

extern commandConst cc;

/*****************************************/

int generateSymbolTable (char *fileName, int *IC, int *DC, symbolPtr * symbolHead) {
    int try;
    FILE *filePtr;
    char line[MAX_LINE_LEN+3];
    int lineNum = 0;
    int okFlag = 1;
    char inputFile[MAX_FILE_LEN+5];

    strncpy(inputFile, fileName, MAX_FILE_LEN+1);

    if ((filePtr = fopen(strcat(inputFile,".am"),"r")) == NULL) {
        printf("ERROR - Can't open %s\n",inputFile);
        return 0;
    }

    while(fgets(line, MAX_LINE_LEN+2, filePtr)) {
        lineNum++;
        if (DEBUG) printf("======================\n%s", line);
        try = generateSymbolTableHandleLine(line, inputFile, lineNum, IC, DC, symbolHead);
        if (DEBUG) printf("okFlag for this line = %d\n",try);
        okFlag = okFlag && try;
    }

    fclose(filePtr);
    if (DEBUG) {
        printf("======================\n");
        printf("IC = %d\n",*IC);
        printf("DC = %d\n",*DC);
        symbolPrint(symbolHead);
    }
    if ((*DC>MAX_PROG_SIZE) || (*IC>MAX_PROG_SIZE) || ((*IC+*DC)>MAX_PROG_SIZE)) {
        okFlag = 0;
        printf("ERROR in file %s - program size is bound to %d. IC=%d, DC=%d\n",inputFile,MAX_PROG_SIZE,*IC,*DC);
    }

    return okFlag;
}

/*****************************************/

int generateSymbolTableHandleLine(char line[], char *fileName, int lineNum, int *IC, int *DC, symbolPtr *symbolHead) {
    int i0, try;
    int command;
    char string[MAX_LINE_LEN];
    char label[MAX_LABEL_LEN+2];
    char dotLabel[MAX_LABEL_LEN+2];
    int data[MAX_LINE_LEN/2] ={0};
    int addressingModeOP1 =-1;
    int addressingModeOP2 =-1;
    int immidiateOP1;
    int immidiateOP2;
    char cmdLabelOP1[MAX_LABEL_LEN+2];
    char cmdLabelOP2[MAX_LABEL_LEN+2];
    char *str = line;
    int isLabel = 0;
    int isEntry = 0;
    int isExtern = 0;
    int isString = 0;
    int isData = 0;
    int ICinc=0;
    int DCinc=0;
    int okFlag =1;

    str = skipBlanks(str);

    if (str[0] == '\n' || str[0] == '\0') return okFlag; /* empty line */
    if (str[0] == ';') return okFlag; /* comment */


    try =  getLabel(str, label, &isLabel, fileName, lineNum);
    okFlag = okFlag && try;
    if (isLabel) {
        if (DEBUG) printf("Label found = \"%s\"\n",label);
        str = skipLabel(str);
    }

    try = getDotLabel(str, dotLabel, &isExtern, ".extern", fileName, lineNum);
    okFlag = okFlag && try;
    if (isExtern) {
        if (DEBUG) printf(".extern found = \"%s\"\n",dotLabel);
    }

    if (isLabel & isExtern) {
        printf("Warning in %s line %d - Label and .extern. Label ignored\n",fileName, lineNum);
    }

    try = getDotLabel(str, dotLabel, &isEntry, ".entry", fileName, lineNum);
    okFlag = okFlag && try;
    if (isEntry) {
        if (DEBUG) printf(".entry found = \"%s\"\n",dotLabel);
    }

    if (isLabel & isEntry) {
        printf("Warning in %s line %d - Label and .entry. Label \"%s\" ignored\n",fileName, lineNum, label);
    }

    command = getCommand(str, fileName, lineNum); /* get command */
    if (command >=0) {
        if (DEBUG) printf("Found a command \"%s\" at IC=%d\n",cc.commandNames[command],*IC);
        str = skipBlanks(str);
        str += strlen(cc.commandNames[command]); /* skip command name */
        try = str2AddressingModes(str, &addressingModeOP1, &addressingModeOP2, \
                                       &immidiateOP1, &immidiateOP2, \
                                       cmdLabelOP1, cmdLabelOP2, fileName, lineNum);
        okFlag = okFlag && try;
        if (okFlag) {
            ICinc = getICinc(command, addressingModeOP1, addressingModeOP2, fileName, lineNum);
            if (DEBUG) printf("ICinc=%d\n",ICinc);
            try = checkAddressingMode (command, addressingModeOP1, addressingModeOP2, fileName, lineNum);
            okFlag = okFlag && try;
        }
    }

    if (DEBUG) {
    if (addressingModeOP1 == 0)
        printf("Addressing mode OP1 %d, #%d\n",addressingModeOP1, immidiateOP1);
    else if (addressingModeOP1 == 1)
        printf("Addressing mode OP1 %d, label=\"%s\"\n",addressingModeOP1, cmdLabelOP1);
    else if (addressingModeOP1 == 2)
        printf("Addressing mode OP1 %d, label=\"%s\"[r%d]\n",addressingModeOP1, cmdLabelOP1, immidiateOP1);
    else if (addressingModeOP1 == 3)
        printf("Addressing mode OP1 %d, r%d\n",addressingModeOP1, immidiateOP1);
    if (addressingModeOP2 == 0)
        printf("Addressing mode OP2 %d, #%d\n",addressingModeOP2, immidiateOP2);
    else if (addressingModeOP2 == 1)
        printf("Addressing mode OP2 %d, label=\"%s\"\n",addressingModeOP2, cmdLabelOP2);
    else if (addressingModeOP2 == 2)
        printf("Addressing mode OP2 %d, label=\"%s\"[r%d]\n",addressingModeOP2, cmdLabelOP2, immidiateOP2);
    else if (addressingModeOP2 == 3)
        printf("Addressing mode OP2 %d, r%d\n",addressingModeOP2, immidiateOP2);
    }


    try = getData(str, &data[0], fileName, lineNum); /* get data */
    okFlag = okFlag && try;
    if (okFlag)
        if (data[0] >0) {
            isData = 1;
            DCinc+=data[0];
            if (DEBUG) printf("%d Numbers Found = ",data[0]);
            for (i0 = 1; i0 <= data[0]; i0 = i0+1) 
                if (DEBUG) printf("%d, ",data[i0]);
            if (DEBUG) printf("\n");
        } 

    try = getString(str, string, &isString, fileName, lineNum);
    okFlag = okFlag && try;
    if (isString) {
        DCinc+=(strlen(string)+1);
        if (DEBUG) printf("%d Strings Found =\"%s\"\n",strlen(string), string);
    }

    if (str[0] == ',') { /* checks if the first character is a comma */
        printf("ERROR in %s line %d - Illegal comma\n",fileName, lineNum);
        return 0;
    }
    
    /* Can't find anything */
	if (okFlag && (!isLabel && !isEntry && !isExtern && !isData && !isString && command==-1)) {
        try = getLabelName(str, label, &isLabel, fileName, lineNum);
        printf("ERROR in %s line %d - Illegal command \"%s\"\n",fileName, lineNum,label);
        return 0;
    } 

    /* OK */
	if (okFlag && (isEntry || isExtern || isLabel)) 
        try = handleSymbolLinkList(command, isEntry, isExtern, isLabel, isData, isString, *DC, *IC, \
                dotLabel, label, symbolHead, fileName, lineNum);
    okFlag = okFlag && try;


    *DC += DCinc;
    *IC += ICinc;
    return okFlag;
    
}

/*****************************************/

int handleSymbolLinkList (int command, int isEntry, int isExtern, int isLabel, int isData, int isString, int DC, int IC,  \
						  char *dotLabel, char *label, symbolPtr *symbolHead, char *fileName, int lineNum) {
    symbolPtr newItem;
    symbolPtr findItem;
	int okFlag =1;

    if (isEntry) {
        findItem = symbolFindVal(symbolHead, dotLabel);
        if (findItem) {
			if (findItem->attribute2 != EMPTY) {
                printf("ERROR in %s line %d - double declaration of label %s\n",fileName, lineNum, dotLabel);
                okFlag = 0;
                return okFlag;
            }
			if ((findItem->attribute1 == ENTRY) || (findItem->attribute2 == ENTRY)) {
                printf("Warning in %s line %d - .entry is already declared for label %s\n",fileName, lineNum, dotLabel);
                okFlag = 1;
                return okFlag;
            }
			if (findItem->attribute1 == EXTERNAL) {
                printf("ERROR in %s line %d - label %s can't be .extern and .entry together\n",fileName, lineNum, dotLabel);
                okFlag = 0;
                return okFlag;
            }
            findItem->attribute2 = ENTRY;
            return okFlag;
        }
    }

    if (isExtern) {
        findItem = symbolFindVal(symbolHead, dotLabel);
        if (findItem) {
            if (findItem->attribute2 != EMPTY) {
                printf("ERROR in %s line %d - double declaration of label %s\n",fileName, lineNum, dotLabel);
                okFlag = 0;
                return okFlag;
            }
            if (findItem->attribute1 == ENTRY) {
                printf("ERROR in %s line %d - label %s can't be .extern and .entry together\n",fileName, lineNum, dotLabel);
                okFlag = 0;
                return okFlag;
            }
            if ((findItem->attribute1 == EXTERNAL) || (findItem->attribute2 == EXTERNAL)) {
                printf("Warning in %s line %d - .extern is already declared for label %s\n",fileName, lineNum, dotLabel);
                okFlag = 1;
                return okFlag;
            }
            if (findItem->attribute1 == CODE || findItem->attribute1 == DATA) {
                printf("ERROR in %s line %d - .extern can't be also a label %s in this file \n",fileName, lineNum, dotLabel);
                okFlag = 0;
                return okFlag;
            }
            findItem->attribute2 = EXTERNAL;
            return okFlag;
        }
    }

    if (isLabel) {
        findItem = symbolFindVal(symbolHead, label);
        if (findItem) {
            if (findItem->attribute2 != EMPTY) {
                printf("ERROR in %s line %d - double declaration of label %s\n",fileName, lineNum, label);
                okFlag = 0;
                return okFlag;
            }
            if (findItem->attribute1 == EXTERNAL) {
                printf("ERROR in %s line %d - label %s can't be also .extern in this file\n",fileName, lineNum, label);
                okFlag = 0;
                return okFlag;
            }
            if (findItem->attribute1 != ENTRY) {
                printf("ERROR in %s line %d - label %s is used twice\n",fileName, lineNum, label);
                okFlag = 0;
                return okFlag;
            }
            if (command>-1) {
                findItem->attribute2 = CODE;
                findItem->value = IC;
                return okFlag;
            } else if (isData || isString) {
                findItem->attribute2 = DATA;
                findItem->value =DC;
                return okFlag;
            } else {
                printf("ERROR in %s line %d - Can't determine (label) label %s \n",fileName, lineNum, label);
                okFlag = 0;
                return okFlag;
            }
        }
    }

    /* Label not found */
    newItem = (symbolPtr) malloc(sizeof(symbol));
    newItem->attribute2 = EMPTY;
    newItem->value = 0;
    if (!newItem) {
        printf("ERROR during generateSymbolTable - Can't allocate memory to symbol table\n");
        exit(0);
    }
    
    if (isExtern) {
        strncpy(newItem->symbolName, dotLabel, MAX_LABEL_LEN);
        newItem->attribute1 = EXTERNAL;
        symbolPush(symbolHead, newItem);
        return okFlag;
    }
    if (isEntry) {
        strncpy(newItem->symbolName, dotLabel, MAX_LABEL_LEN);
        newItem->attribute1 = ENTRY;
        newItem->value = -1;
        symbolPush(symbolHead, newItem);
        return okFlag;
    }

    if (isLabel && !isExtern) {
        strncpy(newItem->symbolName, label, MAX_LABEL_LEN);
        if (command>-1) {
            newItem->attribute1 = CODE;
            newItem->value = IC;
            symbolPush(symbolHead, newItem);
            return okFlag;
        } else if (isData || isString) {
            newItem->attribute1 = DATA;
            newItem->value = DC;
            symbolPush(symbolHead, newItem);
            return okFlag;
        } else {
            printf("ERROR in %s line %d - label without data or code is not allowed\n",fileName,lineNum);
            okFlag = 0;
            free(newItem);
            return okFlag;
        }
    }
    printf("ERROR in %s line %d - Can't determine (end) label %s \n",fileName, lineNum, label);
    free(newItem);
    okFlag = 0;
    return okFlag;
}

/*****************************************/

int checkAddressingMode (int command, int addressingModeOP1, int addressingModeOP2, char *fileName, int lineNum) {
    int i0;
    int okFlag = 1;

    /* 2 operands needed */
    if (cc.commandOperands[command] == 2) { 
        if (addressingModeOP1 == -1 || addressingModeOP2 == -1) {
            printf("ERROR in %s line %d - 2 operands are needed in command \"%s\"\n", fileName, lineNum,cc.commandNames[command]);
            return 0;
        }
    /* 1 operands needed */
    } else if (cc.commandOperands[command] == 1) {
        if (addressingModeOP1 == -1 || addressingModeOP2 != -1) {
            printf("ERROR in %s line %d - 1 operands is needed in command \"%s\"\n", fileName, lineNum,cc.commandNames[command]);
            return 0;
        }
    /* 0 operands needed */
    } else if (cc.commandOperands[command] == 0) {
        if (addressingModeOP1 != -1 || addressingModeOP2 != -1) {
            printf("ERROR in %s line %d - command \"%s\" doesn't have any operands\n", fileName, lineNum,cc.commandNames[command]);
            return 0;
        }
    }

    /* check correctness of the source addressing mode */
    okFlag=0;
    for (i0 = 0; i0 < 4; i0 = i0+1) {
        if (cc.commandOperands[command] == 2) {
            if (cc.commandSrcAdressingMode[(command*4)+i0] == addressingModeOP1) okFlag =1;
        } else if ((cc.commandOperands[command] == 0) || (cc.commandOperands[command] == 1)) {
            okFlag=1;
        }
    }
    if (!okFlag) {
        printf("ERROR in %s line %d - wrong source add mode for %s command\n", fileName,lineNum,cc.commandNames[command]);
        return 0;
    }

    /* check correctness of the destination addressing mode */
    okFlag=0;
    for (i0 = 0; i0 < 4; i0 = i0+1) {
        if (cc.commandOperands[command] == 2) {
            if (cc.commandDestAdressingMode[(command*4)+i0] == addressingModeOP2) okFlag =1;
        } else if (cc.commandOperands[command] == 1) {
            if (cc.commandDestAdressingMode[(command*4)+i0] == addressingModeOP1) okFlag =1;
        } else if (cc.commandOperands[command] == 0) {
            okFlag=1;
        }
    }
    if (!okFlag) {
        printf("ERROR in %s line %d - wrong dest add mode for %s command\n", fileName,lineNum,cc.commandNames[command]);
        return 0;
    }


    return okFlag;
}
