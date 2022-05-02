#include "assemblerDefine.h"
#include "lineUtil.h"
#include "symbolLinkList.h"
#include <string.h>
#include <stdio.h>

int createExternHandleLine (char line[], char *fileName, int lineNum, int *IC, symbolPtr symbolHead, int *externFileOpened);

int checkExtern (int addressingMode, char *cmdLabel, char *fileName, int lineNum, symbolPtr symbolHead, int *externFileOpened, int IC);






