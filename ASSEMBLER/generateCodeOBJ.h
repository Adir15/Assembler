#include "assemblerDefine.h"
#include "lineUtil.h"
#include "symbolLinkList.h"
#include <string.h>
#include <stdio.h>

/* print the data result to the obj file */
int generateCodeOBJLine (char line[], char *fileName, int lineNum, int *IC, int DCinit, symbolPtr symbolHead);

/* convert label to base, offset and ARE */
int label2BaseOffset (char *cmdLabel, char *inputFile, int lineNum, symbolPtr symbolHead, int DCinit, int *base, int *offset, int *are);










