#include <stdlib.h>

#include "list.h"

struct List* createList()
{
    struct List* l = malloc(sizeof(struct List));

    l->head = NULL;

    return l;
}

void freeList(struct List* list)
{
    struct Node* n = list->head;

    while (n->next != NULL) {
        struct Node* next = n->next;
        free(n->data);
        free(n);
        n = next;
    }

    free(list);
}

struct Node* createNode(void* data)
{
    struct Node* node = malloc(sizeof(struct Node));

    if (node != NULL) {
        node->data = data;
        node->next = NULL;
    }

    return node;
}

void append(struct List* list, void* data)
{
    struct Node* node = createNode(data);

    if (list->head == NULL) {
        list->head = node;
    }
    else {
        struct Node* current = list->head;
        while (current != NULL) {
            current = current->next;
        }
        current->next = node;
    }
}

void insert(struct List* list, void* key, void* data)
{
    struct Node* newNode = createNode(data);
    struct Node* current = list->head;

    while (current != NULL && current->data != key) {
        current = current->next;
    }

    if (current == NULL) {
        newNode->next = current->next;
        current->next = newNode;
    }
    else {
        append(list, data);
    }
}
