//
// Created by danish on 3/27/25.
//

#include "card.h"
#include <stddef.h>
#ifndef YUKON_CARDGAME_UTIL_H
#define YUKON_CARDGAME_UTIL_H

char *convertCardToString(Card *card);

// ### Linked List ###
typedef struct Node {
    void *data;
    struct Node *nextNode;
} Node;

typedef struct {
    Node *head;
    size_t elementSize;
} LinkedList;

LinkedList* createList(size_t elementSize);
void addNode(LinkedList* list, void* data);
Node* getNode(LinkedList* list, int index);
Node* getLastNode(LinkedList* list);
void freeList(LinkedList* list);
// #################

#endif //YUKON_CARDGAME_UTIL_H
