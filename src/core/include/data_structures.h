#pragma once

#include <stdlib.h>

typedef struct Node {
    void *data;
    struct Node *prevNode;
    struct Node *nextNode;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
    size_t elementSize;
} LinkedList;