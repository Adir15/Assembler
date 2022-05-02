#include "assemblerDefine.h"
#include "lineUtil.h"
#include "symbolLinkList.h"
#include <string.h>
#include <stdio.h>

/* Receives an input line and handle it (does the command or prints an error) */
int generateSymbolTableHandleLine(char line[], char *fileName, int lineNum, int *IC, int *DC, symbolPtr *symbolHead);

/* check if addressing modes are legal */
int checkAddressingMode (int command, int addressingModeOP1, int addressingModeOP2, char *fileName, int lineNum);

/* insert label into symbol table */
int handleSymbolLinkList (int command, int isEntry, int isExtern, int isLabel, int isData, int isString,  \
                          int DC, int IC, \
                          char *dotLabel, char *label, symbolPtr *symbolHead, char *fileName, int lineNum);




