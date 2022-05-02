#include <stdio.h>
#include <string.h> 
#include "symbolLinkList.h" 

/* main function */
void assembler(char *filename);

/* pre-assembler. macro handler */
int macroHandler(char *fileName);

/* generate symmbol table */
int generateSymbolTable(char *fileName, int *IC, int *DC, symbolPtr *symbolHead);

/* generate code object file */
int generateCodeOBJ(char *fileName, int IC, int DC, symbolPtr symbolHead);

/* generate data object file after code */
int generateDataOBJ(char *fileName, int IC, int DC, symbolPtr symbolHead);

/* generate entry file if needed */
int createEntry (char *fileName, symbolPtr symbolHead, int IC);

/* generate extern file if needed */
int createExtern (char *fileName, symbolPtr symbolHead);

/* remove files if error(s) found */
void removeFiles(char *fileName);



