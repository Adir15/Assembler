#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "macroLinkList.h"
#include "assemblerDefine.h"

typedef struct node* macroTbl;
typedef struct node* macro_list;


int isMacro(char *string);
int macroHandler(char *fileName); 
