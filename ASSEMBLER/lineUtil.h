#include <ctype.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 
#include "assemblerDefine.h" 
#include "commandConstTypeDef.h" 

/* get the command. if it can't find one, it returns -1 */
int getCommand(char *line, char *flieName, int lineNum);

/*A function that accepts a char pointer (to the beginning of a number)
 * and returns a pointer to the end of the number */
char *skipNumber(char *s);

/*A function that accepts a char pointer and returns a pointer after removing all blanks at the begining */
char *skipBlanks(char *s);

/* get the label. if one found, it returns 1 in isLabel, and label gets the label value 
 * If error found, it returns 0, otherwise 1 */
int getLabelName(char* line, char *label, int *isLabel, char *fileName, int lineNum);

/* get the label. if one found, it returns 1 in isLabel, and label gets the label value 
 * If error found, it returns 0, otherwise 1 */
int getLabel(char* line, char *label, int *isLabel, char *fileName, int lineNum);

/*A function that accepts a char pointer and returns a pointer after the label */
char *skipLabel(char *s);

/* get .data numbers.  if it can't find one, res[0]=0. 
 * if numbers are found, res[0] has the amount of numbers, and res[1-n] will have the values
 * If error found, it returns 0, otherwise 1 */
int getData (char* line, int *res, char *fileName, int lineNum);

/* get .extern or .enty label. if one found, isDotLabel becomes 1, and dotLabel has the label.
 * if error found, it returns 0, otherwise 1 */
int getDotLabel(char* line, char *dotLabel, int *isDotLabel, char *dot, char *fileName, int lineNum);

/* get .string string. if one found, isString becomes 1, and string gets the string. 
 * if error found, it returns 0, otherwise 1 */
int getString(char* line, char *string, int *isString, char *flieName, int lineNum);

/* get addressing mode after a command was found for each operand.  
 * It is the first and second operand. not source and destination.
 * addressingMode gets the addressingMode number (0-3) if exist.
 * immidiate gets the number of the register(addressing mode 2 or 3), or the immidiate value (addressing mode 0).
 * label gets the label(addressing mode 1 or 2).
 * if error found, it returns 0, otherwise 1 */
int str2AddressingModes(char *str, int * addressingModeOP1, int * addressingModeOP2, \
                                   int *immidiateOP1, int *immidiateOP2, \
                                   char *labelOP1, char *labelOP2, char *flieName, int lineNum);

/* get addressing mode after a command was found.  
 * addressingMode gets the addressingMode number (0-3) if exist.
 * immidiate gets the number of the register(addressing mode 2 or 3), or the immidiate value (addressing mode 0).
 * label gets the label(addressing mode 1 or 2).
 * if error found, it returns 0, otherwise 1 */
int getAddressingMode(char *str, int * addressingMode, int *immidiate, char * label, char *flieName, int lineNum);

/* check for addressing mode 0 (immidiate).
 * if it is not addressing mode 0, addressingMode will be equal -1
 * otherwise it will be equal to 0, and the value will be stored in immidiate. 
 * If error accured, the function return 0. Otherwise 1 
 */
int checkAddressingMode0(char *str, int *addressingMode, int *immidiate, char *flieName, int lineNum);

/* check for addressing mode 3 (register direct).
 * if it is not addressing mode 3, addressingMode will be equal -1
 * otherwise it will be equal to 3, and the value will be stored in regNumber. 
 * If error accured, the function return 0. Otherwise 1 
 */
int checkAddressingMode3(char *str, int *addressingMode, int *regNumber, char *flieName, int lineNum);


/* check for addressing mode 1 & 2 (label direct & label with register).
 * if it is not addressing mode 1 or 2, addressingMode will be equal -1
 * otherwise it will be equal to 1 or 2. The label will be stored in label and the register value will be stored in regNumber. 
 * If error accured, the function return 0. Otherwise 1 
 */
int checkAddressingMode12(char *str, int *addressingMode, char *label, int *regNumber, char *flieName, int lineNum);

/* get a list of numbers from a string, seperated by commas. 
 * It return an array of integers.
 * res[0] holds the amount of integers in the array
 * res[1] - res[n] hold the integers themselves  
 * If error accured, the function return 0. Otherwise 1 
 */
int getNumbers(char *s, int *res, char *flieName, int lineNum);

/* get a number from a string and put it in res.  
 * If error accured, the function return 0. Otherwise 1 
 */
int getNumber(char *s, int *res, char *flieName, int lineNum);

/* convert a number to out format */  
void num2outstr(int lineNum, int num, char * res);

/* return or between all the bits  */
int bitWiseOrAll(int inNum);

/* return and between all the bits  */
int bitWiseAndAll(int inNum);

/* convert a number to base and offset  */
void lineNum2BaseOffset(int lineNum, int *base, int *offset);

/* calculate how many IC lines takes a command with the specific addressing modes */
int getICinc (int command, int addressingModeOP1, int addressingModeOP2, char *fileName, int lineNum);

/* checks if a number has more than 16 bits (if so returns 1, otherwise returns 0) */
int exceed16Bits(int number);

