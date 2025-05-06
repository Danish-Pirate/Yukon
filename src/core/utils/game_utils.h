#pragma once

#include <stddef.h>
#include "linked_list.h"
#include "../model/card.h"

void spliceList(LinkedList* srcList, LinkedList* dstList, Node* start);
char* getLoadFilePathFromDialog();
char* getSaveFilePathFromDialog();
