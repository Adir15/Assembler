#include "generateCodeOBJ.h"

extern commandConst cc;

/*****************************************/

int generateCodeOBJ (char *fileName, int IC, int DC, symbolPtr symbolHead) {
    char line[MAX_LINE_LEN+3];
    int try;
    FILE *filePtr;
    FILE *objFilePtr;
    char inputFile[MAX_FILE_LEN+5];
    char objFile[MAX_FILE_LEN+5];
    int okFlag =1;
    int lineNum =0;
    int newIC =IC_INIT;

  	strncpy(inputFile, fileName, MAX_FILE_LEN+1);
  	strncpy(objFile, fileName, MAX_FILE_LEN+1);

    if ((filePtr = fopen(strcat(inputFile,".am"),"r")) == NULL) {
        printf("ERROR - Can't open %s\n",inputFile);
        return 0;
    }
    if ((objFilePtr = fopen(strcat(objFile,".ob"),"w")) == NULL) {
        printf("ERROR - Can't open %s\n",objFile);
        return 0;
    }
    fprintf(objFilePtr, "%0d %0d\n", IC-IC_INIT,DC);
	fclose(objFilePtr);

    while(fgets(line, MAX_LINE_LEN+2, filePtr)) {
        lineNum++;
        try = generateCodeOBJLine(line, fileName, lineNum, &newIC, IC, symbolHead);
        okFlag = okFlag && try;
	}

	fclose(filePtr);
	return okFlag;
}

/*****************************************/

int generateCodeOBJLine (char line[], char *fileName, int lineNum, int *IC, int DCinit, symbolPtr symbolHead) {
    int try;
    int okFlag =1;
    char inputFile[MAX_FILE_LEN+5];
    int command;
    char label[MAX_LABEL_LEN+2];
    int addressingModeOP1 =-1;
    int addressingModeOP2 =-1;
    int immidiateOP1;
    int immidiateOP2;
    char cmdLabelOP1[MAX_LABEL_LEN+2];
    char cmdLabelOP2[MAX_LABEL_LEN+2];
    char *str = line;
    FILE *filePtr;
    int isLabel = 0;
    int ICinc=0;
    int commandNum;
    char strOut[50];
    int srcAm, dstAm;
    int srcR, dstR;
    int base, offset, are;
    int printICinc=0;


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

		/*-----------------------------*/

  		strncpy(inputFile, fileName, MAX_FILE_LEN+1);
    	if ((filePtr = fopen(strcat(inputFile,".ob"),"a")) == NULL) {
        	printf("ERROR - Can't open %s\n",inputFile);
        	return 0;
		}

        /* opcode */
        commandNum = (4<<16) | (1<<cc.commandOpcode[command]);
        num2outstr(*IC,commandNum,strOut);
        fprintf(filePtr, "%s\n",strOut);
        printICinc++;

        /* Addressing Mode */
        if (cc.commandOperands[command]>0) {
            if (cc.commandOperands[command]==1) {
                srcAm=0;
                dstAm=addressingModeOP1;
            }  
            if (cc.commandOperands[command]==2) {
                srcAm=addressingModeOP1;
                dstAm=addressingModeOP2;
            }  
        /* Registers + addressing mode + funct */
            if (cc.commandOperands[command]==0) {
                srcR =0;
                dstR =0;
            } else if (cc.commandOperands[command]==1) {
                srcR =0;
                if ((addressingModeOP1==2) || (addressingModeOP1==3)) {
                    dstR =immidiateOP1;
                } else dstR =0;
            } else if (cc.commandOperands[command]==2) {
                if ((addressingModeOP1==2) || (addressingModeOP1==3)) {
                    srcR =immidiateOP1;
                } else srcR = 0;
                if ((addressingModeOP2==2) || (addressingModeOP2==3)) {
                    dstR =immidiateOP2;
                } else dstR =0;
            }

            commandNum = (4<<16) | (cc.commandFunct[command]<<12) | (srcR<<8) | (srcAm<<6) | (dstR<<2) | (dstAm<<0);
            num2outstr(*IC+printICinc,commandNum,strOut);
            fprintf(filePtr, "%s\n",strOut);
            printICinc++;
        }

        /* immidiate op1 */
        if (addressingModeOP1 == 0) {
            commandNum = (4<<16) | (immidiateOP1<<0);
            num2outstr(*IC+printICinc,commandNum,strOut);
            fprintf(filePtr, "%s\n",strOut);
            printICinc++;
        }

        /* label op1 */
        if (addressingModeOP1 == 1 || addressingModeOP1 == 2) {
            try = label2BaseOffset (cmdLabelOP1, fileName, lineNum, symbolHead, DCinit, &base, &offset, &are);
       	    okFlag = okFlag && try;
            num2outstr(*IC+printICinc,(are<<16) | base,strOut);
            fprintf(filePtr, "%s\n",strOut);
            printICinc++;
            num2outstr(*IC+printICinc,(are<<16) | offset,strOut);
            fprintf(filePtr, "%s\n",strOut);
            printICinc++;
        }

        /* immidiate op2 */
        if (addressingModeOP2 == 0) {
            commandNum = (4<<16) | (immidiateOP2<<0);
            num2outstr(*IC+printICinc,commandNum,strOut);
            fprintf(filePtr, "%s\n",strOut);
            printICinc++;
        }


        /* label op2 */
        if (addressingModeOP2 == 1 || addressingModeOP2 == 2) {
            try = label2BaseOffset (cmdLabelOP2, fileName, lineNum, symbolHead, DCinit, &base, &offset, &are);
       	    okFlag = okFlag && try;
            num2outstr(*IC+printICinc,(are<<16) | base,strOut);
            fprintf(filePtr, "%s\n",strOut);
            printICinc++;
            num2outstr(*IC+printICinc,(are<<16) | offset,strOut);
            fprintf(filePtr, "%s\n",strOut);
            printICinc++;
        }

		/*-----------------------------*/

        *IC += ICinc;
		fclose(filePtr);
    }

    return okFlag;
}

/*****************************************/

int label2BaseOffset (char *cmdLabel, char *inputFile, int lineNum, symbolPtr symbolHead, int DCinit, int *base, int *offset, int *are) {
    symbolPtr p1;
    int labelNum;

    p1 = symbolFindVal(&symbolHead, cmdLabel);
    if (p1 == NULL) {
        printf("ERROR in %s.am line %d - Can't find label \"%s\"\n", inputFile, lineNum, cmdLabel);
        return 0;
    }
    labelNum = p1 -> value;
    if ((p1->attribute1==DATA) || (p1->attribute2==DATA)) labelNum+=DCinit;
    lineNum2BaseOffset(labelNum, base, offset);

    /* ARE */
    if ((p1->attribute1==EXTERNAL) || (p1->attribute2==EXTERNAL)) *are=1;
    else *are = 2;

    return 1;
}
