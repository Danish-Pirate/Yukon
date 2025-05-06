#pragma once

#include <stddef.h>
#include "linked_list.h"

void spliceList(LinkedList* srcList, LinkedList* dstList, Node* start);
char* getLoadFilePathFromDialog();
char* getSaveFilePathFromDialog();
