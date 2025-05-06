#pragma once

#include "SDL2/SDL.h"
#include "ui_button.h"
#include "../model/card.h"

#define FONT_FILEPATH "assets/fonts/arial.ttf"

void uiManager_init(SDL_Renderer* renderer);
void uiManager_cleanup();
void uiManager_drawButton(UI_Button* button);
bool uiManager_handleButtonEvents(UI_Button* buttons, int buttonCount, SDL_Event* event);
void uiManager_drawCard(SDL_Rect rect, Card* card);