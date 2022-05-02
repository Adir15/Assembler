#include "createEntry.h"

/*****************************************/

int createEntry (char *fileName, symbolPtr symbolHead, int IC) {
    int entryFileOpened =0;
    FILE *filePtr;
    char inputFile[MAX_FILE_LEN+5];
    symbolPtr p1;
    int value, base, offset;

    strncpy(inputFile, fileName, MAX_FILE_LEN+1);

    p1 = symbolHead;
    while (p1) {
		if ((p1-> attribute1 == ENTRY) || (p1-> attribute2 == ENTRY)) {
            if (p1 -> value == -1) {
        			printf("ERROR in %s.am - label %s marked as entry but wasn't defined\n",inputFile,p1->symbolName);
                    p1 -> value =0;
        			return 0;
            }
			if (!entryFileOpened) {
    			if ((filePtr = fopen(strcat(inputFile,".ent"),"w")) == NULL) {
        			printf("ERROR - Can't open %s\n",inputFile);
        			return 0;
    			}
                entryFileOpened = 1;
            }

            value = p1 -> value;
		    if ((p1-> attribute1 == DATA) || (p1-> attribute2 == DATA)) value+=IC;
            lineNum2BaseOffset(value, &base, &offset);
            fprintf(filePtr, "%s,%04d,%04d\n",p1->symbolName, base, offset);
		}

        p1 = p1->next;
    }

	if (entryFileOpened) fclose(filePtr);
    return 1;
}
