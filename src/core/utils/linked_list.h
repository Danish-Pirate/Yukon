#pragma once

#include <stdlib.h>
#include "../include/data_structures.h"

void spliceList(LinkedList* srcList, LinkedList* dstList, Node* start);
LinkedList* createList(size_t elementSize);
void addNodeToFront(LinkedList* list, void* data);
void addNodeToBack(LinkedList* list, void* data);
Node* getNode(LinkedList* list, unsigned int index);
Node* getLastNode(LinkedList* list);
void freeList(LinkedList* list);
void freeListExcludeData(LinkedList* list);
void deleteNode(LinkedList* list, int index);
size_t size(LinkedList *list);