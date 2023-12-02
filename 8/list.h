#ifndef LIST_H
#define LIST_H

struct Node
{
    struct Node* next;
    void*        data;
};

struct List
{
    struct Node* head;
};

struct List* createList();
struct Node* createNode(void* data);
void         freeList(struct List* list);
void         append(struct List* list, void* data);
void         insert(struct List* list, void* key, void* data);


#endif
