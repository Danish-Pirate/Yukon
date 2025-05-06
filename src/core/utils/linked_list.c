#include <malloc.h>
#include "yukon_core.h"

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

size_t size(LinkedList *list) {
    size_t size = 0;
    Node *currentNode = list->head;
    while (currentNode != NULL) {
        size++;
        currentNode = currentNode->nextNode;
    }
    return size;
}
