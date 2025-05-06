#pragma once

#include <stdbool.h>
#include "SDL.h"

typedef struct {
    void (*callback)();
    SDL_Rect displayRect;
    const char* label;
} UI_Button;

void initButtonFont();
void drawButton(UI_Button button);
bool isButtonHovered(int mouse_PosX, int mouse_PosY, SDL_Rect displayRect);