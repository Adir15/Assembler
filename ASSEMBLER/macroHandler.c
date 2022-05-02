#include "macroHandler.h"

struct node 
{
    void* data;
    struct node* next; 
};



typedef struct data_in_macrotable
{
    char macro_name[MAX_LINE_LEN+1];
    macro_list head_list;
} data_type;

enum boolean {FALSE, TRUE};

 int isEndOfMacro(char* line);
 char* getMacroName(char* line,char* macroname );
 int macroTabaleStrCompare(const void* str1, const void* str2);
 int isMacro(char* line);
 void freeAll(macroTbl macroHead);



int macroHandler (char *fileName)
{
    FILE *inputFile;
    FILE *outputFile;

    int okFlag =1;
    int lineNum =0;
    char line[MAX_LINE_LEN+4];
    macroTbl macroHead = NULL;
    int macroFlag = FALSE;
    macroTbl newmacro;
    data_type *datamacro; 
	char input [MAX_FILE_LEN+5] ;
	char output [ MAX_FILE_LEN+5] ;	
    char macroName[MAX_LINE_LEN+1];
    macro_list macro;
    char *new_line;
    int macroLines;

	strcpy(input, fileName);
	strcpy(output, fileName); 

    if ((inputFile = fopen(strcat(input,".as"),"r")) == NULL) {
        printf("ERROR - Can't open %s\n",input);
        return 0;
    }
    if ((outputFile = fopen(strcat(output,".am"),"w")) == NULL) {
        printf("ERROR - Can't open %s\n",output);
        return 0;
    }

    while(fgets(line, (MAX_LINE_LEN+3), inputFile)) 
    {   
        lineNum++;
        if (strlen(line) > MAX_LINE_LEN) {
            okFlag = 0;
            printf("ERROR in %s line %d - line has more characters than %d\n", input, lineNum,MAX_LINE_LEN);
        }
        
        if (okFlag) {
        if(macroFlag)
        {  
			 if(isEndOfMacro(line))
            {
                macroFlag = FALSE;
                continue;
            }
            
            new_line = malloc(MAX_LINE_LEN+1);
            strcpy(new_line, line);
            macro = listCreatNode(new_line ,NULL);
            listPush(&(((data_type *)(newmacro->data)) -> head_list), macro);
        }
       else
        {
            
            macroFlag = isMacro(line);
            if(macroFlag)
            {
                macroTbl macro; 
                getMacroName(line,macroName);
                if(macroHead != NULL)
                {
                macro =  listFind(macroHead, macroName, macroTabaleStrCompare);
                if(macro != NULL)
                {
                    printf("ERROR in %s line %d - macro %s is already exist\n", input, lineNum,macroName);
                    okFlag = 0;
                    continue;
                }
                }
				
				datamacro = malloc(sizeof(data_type));
                strcpy(datamacro -> macro_name , macroName);
                datamacro-> head_list = NULL;
                newmacro = listCreatNode(datamacro, NULL);
                listPush(&macroHead, newmacro);
				 
				 
 
            }
            else 
            {
                macroTbl find_result = listFind(macroHead, line, macroTabaleStrCompare);
                
                if(find_result)
                {
                	macro_list current = ((data_type*)(find_result -> data)) -> head_list;
                    macroLines=0;
                    while(current != NULL)
                    {
                        fprintf(outputFile,"%s",(char*)(current -> data));
                        current = current -> next;
                        macroLines++;
                    }
                    if (macroLines==0) 
                        printf("Warning in %s line %d - macro %s has no lines\n", input, lineNum,macroName);
                    
                }
                else
                {
                	fprintf(outputFile, "%s", line);
                }
            }
            }
        }
    }
    fclose(outputFile);
    fclose(inputFile);
    freeAll(macroHead);

    return okFlag;
}

int macroTabaleStrCompare(const void* str1, const void* str2)
{ 
    char* string_without_whit_spac ;

	char cpy_of_str1[MAX_LINE_LEN+1];
	strcpy(cpy_of_str1, (const char *)str1);
	string_without_whit_spac = strtok(cpy_of_str1, " \t\n");
    if(string_without_whit_spac == NULL)
    {
        return 0;
    }
    
    return !strcmp( string_without_whit_spac ,((data_type*)str2) -> macro_name);
}


int isEndOfMacro(char* line) 
{
 	char cpy_of_line[MAX_LINE_LEN+1];
 	strcpy(cpy_of_line, line);
 	
	return (!strcmp(strtok(cpy_of_line, " \t\n"), "endm"));   
}

 char* getMacroName(char* line,char* macroname )
{
    while(isspace(*line))
    {
        line++;
    }
    while(!isspace(*line))
    {
        line++;
    }
    while(isspace(*line))
    {
        line++;
    }
    while(!isspace(*line))
    {
        *macroname = *line;
        line++;
        macroname++;
    }
	*macroname = '\0';
    return line;

} 

int isMacro(char* line)
{
    int i = 0;
    char command [6] = {0};
    while (isspace(*line))
    {
        line++;
    }

    for(i = 0; i< 5; i++)
    {
        command[i] = line[i];
    }
    return !strcmp(command,"macro");
}    


void freeListWithAloocatedData(macro_list head)
{

	macro_list saveHead = head;
	
	while(head != NULL)
	{
		free((head -> data));
		head = head -> next;
	}

freeList(saveHead);

}


 void freeAll(macroTbl macroHead)
{
    macroTbl start_of_table = macroHead;

    while(macroHead != NULL)
    {
		
        freeListWithAloocatedData(((data_type*)(macroHead -> data)) -> head_list);
        macroHead = macroHead ->next;
    }
    freeListWithAloocatedData(start_of_table);   
}






    

