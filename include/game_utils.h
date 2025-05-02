//
// Created by danish on 3/27/25.
//

#include "card.h"
#include "SDL_rect.h"
#include <stddef.h>
#ifndef YUKON_CARDGAME_UTIL_H
#define YUKON_CARDGAME_UTIL_H

char *cardToString(Card *card);
char *cardTopSideToString(Card *card);
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
void spliceList(LinkedList* srcList,LinkedList* dstList, Node* start);
Node* getNode(LinkedList* list, unsigned int index);
Node* getLastNode(LinkedList* list);
void freeList(LinkedList* list);
void freeListExcludeData(LinkedList* list);
void deleteNode(LinkedList* list, int index);
bool isPointWithinRect(int x, int y, SDL_Rect rect);
// #################

#endif //YUKON_CARDGAME_UTIL_H
