
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

char *cardToString(Card *card) {
    static char result[3];

    // lookup arrays
    char *ranks = "A23456789TJQK";
    char *suits = "CDHS";

    result[0] = ranks[card->rank];
    result[1] = suits[card->suit];
    result[2] = '\0';

    return result;

}

char *cardTopSideToString(Card *card) {
    static char result[3];
    if (card->isFaceUp == false) {
        strcpy(result, "[]");
        return result;
    }
    return cardToString(card);

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

void spliceList(LinkedList* srcList, LinkedList* dstList, Node* start) {
    // Save the tail of the sequence being moved
    Node* movedTail = srcList->tail;

    // Handle removal from source list
    if (start == srcList->head) {
        // Moving from head of source list
        srcList->head = NULL;
        srcList->tail = NULL; // Important! Set tail to NULL for empty list
    } else {
        // Moving from middle/end of source list
        start->prevNode->nextNode = NULL;
        srcList->tail = start->prevNode;
    }

    // Detach chain from previous node
    start->prevNode = NULL;

    // Attach to destination list
    if (dstList->head == NULL) {
        // Empty destination list
        dstList->head = start;
    } else {
        // Non-empty destination list
        dstList->tail->nextNode = start;
        start->prevNode = dstList->tail;
    }

    // Update destination tail
    dstList->tail = movedTail;
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

void deleteNode(LinkedList* list, int index) {
    if (list == NULL || list->head == NULL || index < 0) return;

    Node* toDelete = getNode(list, index);
    if (toDelete == NULL) return;

    // Update the previous node's next pointer
    if (toDelete->prevNode != NULL) {
        toDelete->prevNode->nextNode = toDelete->nextNode;
    } else {
        // Deleting the head
        list->head = toDelete->nextNode;
    }

    // Update the next node's previous pointer
    if (toDelete->nextNode != NULL) {
        toDelete->nextNode->prevNode = toDelete->prevNode;
    } else {
        list->tail = toDelete->prevNode;
    }

    free(toDelete);
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

bool isPointWithinRect(int x, int y, SDL_Rect rect) {
    return
    (x >= rect.x &&
     x <= rect.x + rect.w &&
     y >= rect.y &&
     y <= rect.y + rect.h);
}