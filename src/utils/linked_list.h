#pragma once

#include <malloc.h>

// ### Linked List ###
typedef struct Node {
    void *data;
    struct Node *prevNode;
    struct Node *nextNode;
} Node;

typedef struct {
    Node *tail;
    Node *head;
    size_t elementSize;
} LinkedList;

LinkedList* createList(size_t elementSize);
void addNodeToFront(LinkedList* list, void* data);
void addNodeToBack(LinkedList* list, void* data);
Node* getNode(LinkedList* list, unsigned int index);
Node* getLastNode(LinkedList* list);
void freeList(LinkedList* list);
void freeListExcludeData(LinkedList* list);
void deleteNode(LinkedList* list, int index);
size_t size(LinkedList *list);