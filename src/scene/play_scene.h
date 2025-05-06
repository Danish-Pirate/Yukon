#pragma once

#include "SDL.h"
#include <stdbool.h>
#include "../view/window.h"
#include "../view/ui_card.h"

// Adjust these values to match your display needs
#define FOUNDATION_X_START (SCREEN_WIDTH - CARD_DISPLAY_WIDTH - 50)
#define FOUNDATION_Y_SPACING 30
#define FOUNDATION_Y 30

#define COLUMN_X_START 350
#define COLUMN_X_SPACING 180
#define COLUMN_Y_START 30
#define COLUMN_Y_CARD_OFFSET 30

// Play scene function declarations
void playScene_init(void* data);
void playScene_handleEvent(SDL_Event* event);
void playScene_update();
void playScene_render();
void playScene_cleanup(void);