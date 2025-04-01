//
// Created by danish on 3/27/25.
//

#include "card.h"

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
void* getNode(LinkedList* list, int index);
void freeList(LinkedList* list);
// #################

#endif //YUKON_CARDGAME_UTIL_H
