#include "macroLinkList.h"

typedef struct node* macroTbl;
typedef struct node* macro_list;


struct node 
{
    void* data;
    linklist_node next;
};

linklist_node listCreatNode(void* data, linklist_node next)
{
    linklist_node newnode = malloc(sizeof(*newnode));
    if(newnode == NULL)
    {
        return NULL;
    }
    newnode -> data = data;
    newnode -> next = next;
    return newnode;
}

linklist_node listPush(linklist_node* head, linklist_node node_to_insert)
{
     linklist_node p1;
   
    if (*head == NULL) 
    {
    	*head = node_to_insert;
    }
    else 
    {
        p1 = *head;
        while (p1->next) p1 = p1 -> next;
        p1->next = node_to_insert;
    }
    return node_to_insert;
}


linklist_node listFind(linklist_node head, const void* data, int(*comper)(const void* data1,const void* data2))
{
    linklist_node corrent_node = head;
    while(corrent_node != NULL)
    {
        if(comper(data, corrent_node ->data))
        {
            return corrent_node;
        }
        corrent_node = corrent_node->next;        
    }
    return NULL;
}

void freeList(linklist_node head)
{
    linklist_node corrent_node = head;
    linklist_node previos_node = head;
while(corrent_node != NULL)
{

    corrent_node = corrent_node ->next;
    free(previos_node);
    previos_node = corrent_node;

}
}



