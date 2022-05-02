#include "symbolLinkList.h"
#include <unistd.h>
#include <string.h>


void symbolPush(symbolPtr *symbolHead, symbolPtr newItem) {
    symbolPtr p1,p2;
    p2 = newItem;
    p2->next = NULL;
    if (*symbolHead == NULL) *symbolHead = p2;
    else {
        p1 = *symbolHead;
        while (p1->next) p1 = p1 -> next;
        p1->next = p2;
    }
}

symbolPtr symbolFindVal(symbolPtr *symbolHead, char *symbolName) {
    symbolPtr p1;

    if (*symbolHead == NULL) return NULL;

    p1 = *symbolHead;
    while (strcmp(p1->symbolName,symbolName) != 0) {
        if (p1->next == NULL) return NULL;
        p1 = p1->next;
    }
    return p1;
}

void symbolFreeList(symbolPtr *symbolHead) {
    symbolPtr p1;
    while (*symbolHead) {
        p1 = *symbolHead;
        *symbolHead = p1->next;
        free(p1);
    }
    *symbolHead = NULL;
}

void symbolPrint(symbolPtr *symbolHead) {
    symbolPtr p1;
    p1 = *symbolHead;
    printf("Symbol Table:\n");
    while (p1) {
        printf("    %-15s\t%d\t%d\t%d\n",p1->symbolName, p1->value, p1->attribute1, p1->attribute2);
        p1 = p1->next;
    }
}





