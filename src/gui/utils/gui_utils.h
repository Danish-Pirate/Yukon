#pragma once

#include "SDL2/SDL_rect.h"
#include <stdbool.h>

char* getLoadFilePathFromDialog();
char* getSaveFilePathFromDialog();
bool isPointWithinRect(int x, int y, SDL_Rect rect);