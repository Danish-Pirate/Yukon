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

    // allocating memory for data and copying it to the list
    newNode->data = (void *)malloc(list->elementSize);
    memcpy(newNode->data, data, list->elementSize);


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

    // allocating memory for data and copying it to the list
    newNode->data = (void *)malloc(list->elementSize);
    memcpy(newNode->data, data, list->elementSize);


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

Node* getNode(LinkedList* list, int index) {
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
