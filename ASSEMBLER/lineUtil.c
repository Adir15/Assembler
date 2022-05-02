#include "lineUtil.h"
#include "commandConst.c"

/*****************************************/

int getCommand(char *str, char *fileName, int lineNum) {
    int i;
    int count = 0;

	str = skipBlanks(str);

    if (str[0] == '\n') { /* check if the line is empty */
        return -1;
    }

    for (i = 0; str[i] != ' ' && str[i] != '\t' && str[i] != ',' && str[i] != '\n' && str[i] != '\0' && i<MAX_LINE_LEN-1; i++) {
        count++; /* Count how many characters the first word have */
    }
    
    for (i = 0; i < NUM_COMMANDS;   i++) { /* compare the command with each of the command names */
        if (strlen(cc.commandNames[i]) == count && strncmp(cc.commandNames[i], str, count) == 0) /* equals */
            return i;
    }
    return -1; 
}

/*****************************************/

int checkAddressingMode0(char *str, int *addressingMode, int *immidiate, char *fileName, int lineNum) {
    int okFlag = 1;
    *addressingMode = -1;

    if (str[0] == '#') { 
		okFlag = getNumber(++str, immidiate, fileName, lineNum);

        if (okFlag) {
            if (exceed16Bits(*immidiate)) {
                printf("ERROR in %s line %d - immidiate \"%d\" exceed 16 bits\n",fileName, lineNum,*immidiate);
                okFlag= 0;
            }
        }
        *immidiate = (*immidiate & 0xffff);

        if (okFlag) {
            *addressingMode = 0;
            str = skipNumber(str);
            str = skipBlanks(str);
            if (str[0] != ',' && !isspace(str[0])) {  /* no other option */
                okFlag = 0;
                printf("ERROR in %s line %d - unrecognized char \"%c\" after #%d\n",fileName, lineNum, str[0], *immidiate);
            }
        }
    }
    return okFlag;
}

/*****************************************/

int checkAddressingMode3(char *str, int *addressingMode, int *regNumber, char *fileName, int lineNum) {
    int okFlag = 1;
    *addressingMode = -1;
    if (str[0] == 'r') { 
		okFlag = getNumber(++str, regNumber, fileName, lineNum);
        if (okFlag && (*regNumber >= 0) && (*regNumber <= 15)) {
            str = skipNumber(str);
            if (str[0] != ',' && !isspace(str[0]))  return okFlag; /* not a register */
            *addressingMode = 3;
            str = skipBlanks(str);
            if (str[0] != ',' && !isspace(str[0])) {  /* no other option */
                okFlag = 0;
                printf("ERROR in %s line %d - unrecognized char \"%c\" after r%d\n",fileName, lineNum, str[0], *regNumber);
            }
        }
    }
    return okFlag;
}

/*****************************************/

int checkAddressingMode12(char *str, int *addressingMode, char *label, int *regNumber, char *fileName, int lineNum) {
    int try;
    int okFlag = 1;
    int isLabel;

    if (!isalpha(str[0])) { /* label must begin with alpha */
        return okFlag;
    }
	try = getLabelName(str, label, &isLabel, fileName, lineNum);
    okFlag = okFlag && try;
    if (!okFlag) return 0;
	if (okFlag && isLabel) {
        str += strlen(label) ;
        str = skipBlanks(str);
        if (str[0] == ',' || str[0] == '\n' || str[0] == '\0' ) {
            *addressingMode = 1;
            return okFlag;
        }
        if (str[0] == '[') {
            str++;
            if (str[0] != 'r') {
                okFlag = 0;
                printf("ERROR in %s line %d - must be a register in the [] brackets\n",fileName, lineNum);
                return okFlag;
            } else {
                str++;
		        try = getNumber(str, regNumber, fileName, lineNum);
                okFlag = okFlag && try;
                if (!okFlag) return 0;
                if (okFlag) { 
                    if ((*regNumber >= 10) && (*regNumber <= 15)) {
                        *addressingMode = 2;
                        str = skipNumber(str);
                        if (str[0] != ']') {
                            okFlag = 0;
                            printf("ERROR in %s line %d - Add Mode Index must have ] after the reg\n",fileName, lineNum);
                            return okFlag;
                        }
                        str++;
                        str = skipBlanks(str);
                        if (str[0] != ',' && !isspace(str[0])) {
                            okFlag = 0;
                            printf("ERROR in %s line %d - Unrecognized char \"%c\"\n",fileName, lineNum,str[0]);
                            return okFlag;
                        }
                        return okFlag;
                    } else {
                        okFlag = 0;
                        printf("ERROR in %s line %d - Register must be between 10 to 15\n",fileName, lineNum);
                        return okFlag;
                    }
                }
            }
        }
    }
    printf("ERROR in %s line %d - Unrecognized char \"%c\"\n",fileName, lineNum,str[0]);
    return 0;
}

/*****************************************/

int str2AddressingModes(char *str, int *addressingModeOP1, int *addressingModeOP2, \
                                   int *immidiateOP1, int *immidiateOP2, \
                                   char *labelOP1, char *labelOP2,  char *fileName, int lineNum) {

    int try;
    int okFlag = 1;
    
    str = skipBlanks(str);
    if (str[0] == '\0' || str[0] == '\n') return okFlag;  /* no operand */


    try = getAddressingMode(str, addressingModeOP1, immidiateOP1, labelOP1, fileName, lineNum);
    okFlag = okFlag && try;

    if ((*addressingModeOP1 == -1) & okFlag) {
        printf("ERROR in %s line %d - Can't evaluate first operand in this line\n",fileName, lineNum);
        okFlag = 0;
    }

    while (str[0] != ',' && str[0] !='\0' && str[0] != '\n') str++; /* skip to , or EOL */

    if (str[0] == '\0' || str[0] == '\n') return okFlag; 
    if (!okFlag) return okFlag;
    str++;
    str = skipBlanks(str);

    try = getAddressingMode(str, addressingModeOP2, immidiateOP2, labelOP2, fileName, lineNum);
    okFlag = okFlag && try;

    if ((*addressingModeOP2 == -1) & okFlag) {
        printf("ERROR in %s line %d - Can't evaluate second operand in this line\n",fileName, lineNum);
        okFlag = 0;
        return okFlag;
    }

    while (str[0] != ',' && str[0] !='\0' && str[0] != '\n' && str[0]!=' ' && str[0]!='\t') str++; /* skip to , or EOL or blank */
    str = skipBlanks(str);
    if (str[0] != '\0' && str[0] != '\n' && okFlag) {
        printf("ERROR in %s line %d - extra char \"%c\" after second operand\n",fileName, lineNum,str[0]);
        okFlag = 0;
        return okFlag;
    } 

    return okFlag;
}

/*****************************************/

int getAddressingMode(char *str, int *addressingMode, int *immidiate, char *label, char *fileName, int lineNum) {
    int try;
    int okFlag = 1;

    str = skipBlanks(str);

    if (str[0] == '\n' || str[0] == '\0') { /* check if no operands */
        return okFlag;
    }

    try =  checkAddressingMode0(str, addressingMode, immidiate, fileName, lineNum);
    okFlag = okFlag && try;
    if (okFlag && *addressingMode == -1) {
        try =  checkAddressingMode3(str, addressingMode, immidiate, fileName, lineNum);
        okFlag = okFlag && try;
    }
    if (okFlag && *addressingMode == -1) {
        try =  checkAddressingMode12(str, addressingMode, label, immidiate, fileName, lineNum);
        okFlag = okFlag && try;
    }

    if (okFlag && *addressingMode == -1) {
        okFlag = 0;
        printf("ERROR in %s line %d - Can't evaluate opernd(s) in this line\n",fileName, lineNum);
    }

    return okFlag;
}
    
/*****************************************/

char *skipNumber(char *s) {
    if (s[0] == '+' || s[0] =='-') s++; /* skip sign if exist */

    while (isdigit(s[0])) s++; /* skip digits */
    return s; 
}

/*****************************************/

char *skipBlanks(char *s) {
    while (s[0] == ' ' || s[0] == '\t') s++; /* skip blanks */
    return s; 
}

/*****************************************/

char *skipLabel(char *s) {
    while (s[0] != ':') s++; /* skip everything until colon */
    s = skipBlanks(s); /* skip blanks */
	while (isalnum(*s)) s++; /* skip label name */
    return ++s;
}

/*****************************************/

int getLabel(char* line, char *label, int *isLabel, char *fileName, int lineNum) {
	int len = 0;
	int try;
	int number;
	char *c = line;
    int okFlag;

    *isLabel = 0;

    okFlag = sscanf( line, "%s", label);
    if (okFlag && !isalpha(label[0]) && label[strlen(label)-1]==':') {
        printf("ERROR in %s line %d - label must begin with an alphabetic char\n",fileName, lineNum);
		return 0;
    }

    okFlag = getLabelName(line, label, isLabel, fileName, lineNum);
    if (!okFlag) return okFlag;

    if (*isLabel) {
	    c = skipBlanks(c);
        c+= strlen(label);

	    if (c[0] != ':') {
	        while (c[len] != ':' && c[len] != '\n' && c[len] != '\0' && len < MAX_LABEL_LEN-strlen(label)-1) {
                len++;
            }
            if (c[len] == ':') {
                printf("ERROR in %s line %d - character \"%c\" is not allowed in label\n",fileName, lineNum,c[0]);
                okFlag = 0;
                *isLabel = 0;
		        return okFlag;
            }
	    }

	    if (c[0] == ':') { 
		/* Label found. */
	    /* Check if label name is a name of a command. */
            try = getCommand(label, fileName, lineNum);
            if (try >=0) {
                printf("ERROR in %s line %d - label can't be a command name\n",fileName, lineNum);
                okFlag = 0;
                return okFlag;
            }

	    /* Check if label name is r0-r15. */
            if (label[0] == 'r' && isdigit(label[1])) {
                try = getNumber(&label[1],&number, fileName, lineNum);
                label = skipNumber(&label[1]);
                if (try && (number>=0) && (number <=15) && label[0]=='\0') {
                    printf("ERROR in %s line %d - label can't be a register\n",fileName, lineNum);
                    okFlag = 0;
                    return okFlag;
                }
            }
	    /* Check if after the label ther is space */
	        if (!isspace(c[1])) { 
                printf("ERROR in %s line %d - after a label must be a space\n",fileName, lineNum);
                okFlag = 0;
                return okFlag;
            }
		    return okFlag;
        }
	}

    *isLabel = 0;
	return okFlag;
}

/*****************************************/

int getLabelName(char* line, char *label, int *isLabel, char *fileName, int lineNum) {
    int len1 = 0;
    char *c = line;
	char labeli[MAX_LABEL_LEN+3];
    int okFlag = 1;

	c = skipBlanks(c);
     /* Label must start with a alpha */
    if (!isalpha(*c)) {
        return okFlag;
    }

    /* All other characters must be alpha or digit */
    while (isalnum(*c) && len1 < MAX_LABEL_LEN+1) {
        c++;
        len1++;
    }

	strncpy(labeli, line, len1);
    labeli[len1]='\0';
    if (len1==MAX_LABEL_LEN+1) {
        printf("ERROR in %s line %d - label %s  has more charecters than allowed\n",fileName, lineNum, labeli);
        okFlag = 0;
        return okFlag;
    }
	/* Label found. */
	strncpy(label, line, len1);
	label[len1] = '\0';

    *isLabel = 1;
	return okFlag;
}

/*****************************************/

int getData(char* line, int *res, char *fileName, int lineNum) {
    int try;
    int okFlag = 1;
    char *c = line;
    
    res[0] = -1;
    c = skipBlanks(c);

    if (c[0] == '\n' || c[0] == '\0') return okFlag;

    try = strncmp(c, ".data", 5);
    if (try != 0) return okFlag;
    c+=5;

    if (c[0] != ' ' && c[0] != '\t' && c[0]!='\0' && c[0]!='\n') {
        printf("ERROR in %s line %d - after .data must be space or tab\n",fileName, lineNum);
        return 0;
    }
    c++;
    try = getNumbers(c, res, fileName, lineNum);
	if (!try) okFlag = 0;
    return okFlag;
}

/*****************************************/

int getDotLabel(char* line, char *label, int *isDotLabel, char* dot, char *fileName, int lineNum) {
	int try;
	int isLabel;
    int okFlag = 1;
    char *c = line;
    *isDotLabel = 0;

    c = skipBlanks(c);

	try = strncmp(c, dot, strlen(dot));
	if (try != 0) return okFlag;
	c+=strlen(dot);

    if (c[0] != ' ' && c[0] != '\t') {
        printf("ERROR in %s line %d - after %s must be space or tab\n",fileName, lineNum, dot);
		return 0;
	}

    c = skipBlanks(c);
	try = getLabelName(c, label, &isLabel, fileName, lineNum);
    if (!try) return 0;

    if (isLabel) {
        c += strlen(label) ;
        c = skipBlanks(c);
        if (c[0] != '\n' && c[0] != '\0') {
            printf("ERROR in %s line %d - extra char \"%c\" after label in %s command\n",fileName, lineNum,c[0],dot);
            return 0;
        }
        *isDotLabel = 1;
        return okFlag;
    }
    printf("ERROR in %s line %d - Can't evaluate Label\n",fileName, lineNum);
    return 0;
}

/*****************************************/

int getString(char* line, char *string, int *isString, char *fileName, int lineNum) {
    int try;
    int len=0;
    int i0;
    int okFlag = 1;
    char *c = line;

    c = skipBlanks(c);

    try = strncmp(c, ".string", 7);
    if (try != 0) return okFlag;
    c+=7;

    if (c[0] != ' ' && c[0] != '\t' && c[0]!='\n' && c[0]!='\0') {
        printf("ERROR in %s line %d - after \".string\" must be space or tab\n",fileName, lineNum);
        okFlag = 0;
        return okFlag;
    }
    c++;
    c = skipBlanks(c);

    if (c[0] == '\n' || c[0] == '\0') {
        printf("ERROR in %s line %d - no string given \n",fileName, lineNum);
        return 0;
    }

     /* string must start with an " */
    if (c[0] != '\"') {
        printf("ERROR in %s line %d - The string must be inside \"\"\n",fileName, lineNum);
        return 0;
	}
    c++;
    len = strlen(c);

	/* All other characters must be alpha or digit */
    while (*(c+len) != '\"' && len>0 && !isalnum(*(c+len))) {
        len--;
    }

    if (len==0 && *(c+len)!='\"') {
        printf("ERROR in %s line %d - string must end with \"\n",fileName, lineNum);
        return 0;
	}
    if (isalnum(*(c+len))) {
        printf("ERROR in %s line %d - string must end with \"\n",fileName, lineNum);
        return 0;
	}

    for (i0=0; i0<len; i0++) 
        if (!isprint(*(c+i0))) {
            printf("ERROR in %s line %d - char %c is not printible\n",fileName, lineNum, *(c+i0));
            return 0;
        }

	strncpy(string, c, len);
	string[len] = '\0';
    c=c+len;

    /* String found. */
    *isString = 1;
    return okFlag;
}

/*****************************************/

int getICinc (int command, int addressingModeOP1, int addressingModeOP2, char *fileName, int lineNum) {
    int ICinc = 0;
    if (addressingModeOP1 == 0) ICinc +=1;
    if (addressingModeOP2 == 0) ICinc +=1;
    if (addressingModeOP1 == 1 || addressingModeOP1 == 2) ICinc +=2;
    if (addressingModeOP2 == 1 || addressingModeOP2 == 2) ICinc +=2;

    ICinc += cc.commandICinc[command];
    return ICinc;
}

/*****************************************/

int getNumber(char *s, int *res, char *fileName, int lineNum) {
    int number = 0;
    int sign = 1;
    int digit = 1;

    if (s[0] == '-') {
        sign = -1;
        s++;
    }
    if (s[0] == '+') s++;

    if (s[0] == '\n' || s[0] == '\0') { /*expect another number */
        printf("ERROR in %s line %d - Missing a number\n",fileName, lineNum);
        return 0;
    }

    if (!isdigit(s[0])) { /* must begin with digit */
        printf("ERROR in %s line %d - Expecting a number\n", fileName, lineNum);
        return 0;
    }
    while(isdigit(s[0])) { /* to confirm it's a digit */
        digit = s[0] - '0';
        number = number*10 + digit;
        s++;
    }
    *res = number*sign;
	return 1;
}

/*****************************************/

int getNumbers(char *s, int *res, char *fileName, int lineNum) {
    int try;
    int number;
    int numCnt = 0;

    s = skipBlanks(s);
    if (s[0] == '\n' || s[0] == '\0') {
        printf("ERROR in %s line %d - no numbers given\n",fileName, lineNum); 
        return 0;
    }
    while (1) {
        number = 0;
        try = getNumber(s, &number, fileName, lineNum);
        if (!try) return 0;
        if (exceed16Bits(number)) {
            printf("ERROR in %s line %d - number \"%d\" exceed 16 bits\n",fileName, lineNum,number);
            return 0;
        }
        number = (number & 0xffff);

        res[++numCnt] = number;
        s = skipNumber(s);
        s = skipBlanks(s);
        
        if (s[0] == '\n' || s[0] == '\0') {
            res[0] = numCnt++;
            return 1;
        }     
        if (s[0] != ',') {
            printf("ERROR in %s line %d - missing comma after number %d \n",fileName, lineNum, res[numCnt]);
            return 0;
        }
        s++;
        s = skipBlanks(s);

    }
    return 0;
}

/*****************************************/

int bitWiseOrAll(int inNum) {
    unsigned int result = 0;
    unsigned int num = inNum;
    while (num) {
        result |= (num & 1);
        num >>= 1;
    }
    return result;
}

/*****************************************/

int bitWiseAndAll(int inNum) {
    int i;
    int result = 1;
    unsigned int num = inNum;
    for (i=0; i<32; i++) {
        result &= (num & 1);
        num >>= 1;
    }
    return result;
}

/*****************************************/

void num2outstr(int lineNum, int num, char * res) {
    sprintf(res, "%04d    A%x-B%x-C%x-D%x-E%x", lineNum,((num>>16)&0xf),((num>>12)&0xf),((num>>8)&0xf),((num>>4)&0xf),((num>>0)&0xf));
}

/*****************************************/

void lineNum2BaseOffset(int lineNum, int *base, int *offset) {
    *base = (lineNum / 16) * 16;
    *offset = lineNum- *base;
}

/*****************************************/

int exceed16Bits(int number) {
    if (bitWiseOrAll(number & 0xffff0000) & (!((bitWiseAndAll(0x0000ffff | number))))) return 1;
    else return 0;
}

