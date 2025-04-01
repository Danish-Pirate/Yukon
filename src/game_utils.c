//
// Created by danish on 3/27/25.
//

#include <string.h>
#include "game_utils.h"

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

#include <stdlib.h>

// ### Linked List ###
LinkedList* createList(size_t elementSize) {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->elementSize = elementSize;
    return list;
}

void addNode(LinkedList* list, void* data) {
    Node* newNode = malloc(sizeof(Node));

    // allocating memory for data and copying it to the list
    newNode->data = malloc(list->elementSize);
    memcpy(newNode->data, data, list->elementSize);

    // The added node becomes the new head
    newNode->nextNode = list->head;
    list->head = newNode;
}

void *getNode(LinkedList* list, int index) {
    Node* current = list->head;
    int i = 0;

    while (current != NULL && i < index) {
        current = current->nextNode;
        i++;
    }

    return (current != NULL) ? current->data : NULL;
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
// ####################