

#include "../model/card.h"
#include "SDL_rect.h"
#include <stddef.h>
#ifndef YUKON_CARDGAME_UTIL_H
#define YUKON_CARDGAME_UTIL_H

void spliceList(LinkedList* srcList, LinkedList* dstList, Node* start);
bool isPointWithinRect(int x, int y, SDL_Rect rect);
char* getLoadFilePathFromDialog();
char* getSaveFilePathFromDialog();
// #################

#endif //YUKON_CARDGAME_UTIL_H
