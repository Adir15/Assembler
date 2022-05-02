#include <stdio.h>
#include <stdlib.h>
#include "assemblerDefine.h"
#include "symbolLinkListTypeDef.h"

void symbolUpdateItem (symbolPtr *symbolHead, char *symbolName, symbolAttribute attribute);
void symbolPush (symbolPtr *symbolHead, symbolPtr newItem);
symbolPtr  symbolFindVal(symbolPtr *symbolHead, char *symbolName);
void symbolFreeList (symbolPtr *symbolHead);
void symbolPrint(symbolPtr *symbolHead);


