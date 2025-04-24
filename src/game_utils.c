//
// Created by danish on 3/27/25.
//
#include <stddef.h>
#include <string.h>
#include "game_utils.h"
#include <stdlib.h>


int convertRankToInt(char rank) {
    switch (rank) {
        case 'A': return ACE;
        case '2': return TWO;
        case '3': return THREE;
        case '4': return FOUR;
        case '5': return FIVE;
        case '6': return SIX;
        case '7': return SEVEN;
        case '8': return EIGHT;
        case '9': return NINE;
        case 'T': return TEN;
        case 'J': return JACK;
        case 'Q': return QUEEN;
        case 'K': return KING;
        default: return -1; // Invalid rank
    }
}

int convertSuitToInt(char suit) {
    switch (suit) {
        case 'C': return CLUBS;
        case 'D': return DIAMONDS;
        case 'H': return HEARTS;
        case 'S': return SPADES;
        default: return -1; // Invalid suit
    }
}

char *convertCardToString(Card *card) {
    static char result[3];

    // Return early if card is face-down
    if (card->isFaceUp == false) {
        strcpy(result, "[]");
        return result;
    }

    // lookup arrays
    char *ranks = "A23456789TJQK";
    char *suits = "CDHS";

    result[0] = ranks[card->rank];
    result[1] = suits[card->suit];
    result[2] = '\0';

    return result;
}


// ### Linked List ###
LinkedList* createList(size_t elementSize) {
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->tail = NULL;
    list->head = NULL;
    list->elementSize = elementSize;
    return list;
}


void addNodeToFront(LinkedList* list, void* data) {
    Node* newNode = (Node *)malloc(sizeof(Node));

    newNode->data = data;

    newNode->nextNode = list->head;
    newNode->prevNode = NULL;

    // If list is empty, this node is also the tail
    if (list->head == NULL) {
        list->tail = newNode;
    } else {
        list->head->prevNode = newNode;
    }
    list->head = newNode;
}

void addNodeToBack(LinkedList* list, void* data) {
    Node* newNode = (Node *)malloc(sizeof(Node));

    newNode->data = data;

    newNode->prevNode = list->tail;
    newNode->nextNode = NULL;

    // If list is empty, this node is also the tail
    if (list->tail == NULL) {
        list->head = newNode;
    } else {
        list->tail->nextNode = newNode;
    }
    list->tail = newNode;
}

// Move start node and subsequent nodes from srcList to tail of dstList
void spliceList(LinkedList* srcList,LinkedList* dstList, Node* start){

    // Cut start node from srcList
    if (srcList->head == start) {
        srcList->head = NULL;
    }
    else {
        start->prevNode->nextNode = NULL;
    }
    Node* dstListNewTail = srcList->tail;
    srcList->tail = start->prevNode;
    start->prevNode = NULL;

    // Join startNode to end of dstList
    if (dstList->head == NULL) {
        dstList->head = start;
    }
    else {
        Node* dstListTail = dstList->tail;
        dstListTail->nextNode = start;
        start->prevNode = dstListTail;
    }
    dstList->tail = dstListNewTail;
}

Node* getNode(LinkedList* list, unsigned int index) {
    Node* current = list->head;
    int i = 0;
    while (current != NULL && i < index) {
        current = current->nextNode;
        i++;
    }
    return current;
}

void freeList(LinkedList* list) {
    Node* current = list->head;

    while (current != NULL) {
        Node* next = current->nextNode;
        free(current->data);
        free(current);
        current = next;
    }

    free(list);
}

void freeListExcludeData(LinkedList* list) {
    Node* current = list->head;

    while (current != NULL) {
        Node* next = current->nextNode;
        free(current);
        current = next;
    }

    free(list);
}