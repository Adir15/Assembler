/*   0      1        2       3     4 */
typedef enum {
    EMPTY, ENTRY, EXTERNAL, CODE, DATA
} symbolAttribute;


typedef struct symbolNode * symbolPtr;

typedef struct symbolNode {
        char symbolName[MAX_LABEL_LEN+1];
        int  value;
        symbolAttribute  attribute1;
        symbolAttribute  attribute2;
        symbolPtr next;
} symbol;

