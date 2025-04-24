//
// Created by danish on 3/27/25.
//

#include "card.h"
#include <stddef.h>
#ifndef YUKON_CARDGAME_UTIL_H
#define YUKON_CARDGAME_UTIL_H

char *convertCardToString(Card *card);
int convertRankToInt(char rank);
int convertSuitToInt(char suit);

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
// #################

#endif //YUKON_CARDGAME_UTIL_H
