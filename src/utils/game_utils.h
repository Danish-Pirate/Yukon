#pragma once

#include <SDL_rect.h>
#include <stddef.h>
#include "linked_list.h"
#include "../model/card.h"

void spliceList(LinkedList* srcList, LinkedList* dstList, Node* start);
bool isPointWithinRect(int x, int y, SDL_Rect rect);
char* getLoadFilePathFromDialog();
char* getSaveFilePathFromDialog();
