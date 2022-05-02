#include <stdlib.h>
#ifndef LINKLIST_H
#define LINKLIST_H

typedef  struct node* linklist_node;

linklist_node listCreatNode(void* data, linklist_node next);
linklist_node listPush(linklist_node* head, linklist_node node_to_insert);
linklist_node listFind(linklist_node head, const void* data, int(*comper)(const void* data1,const void* data2)); 
void freeList(linklist_node head);
#endif 
