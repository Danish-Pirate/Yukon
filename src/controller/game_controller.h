#pragma once

#include "../model/game.h"
#include "../utils/event_system.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS 60

void gameController_init();
void gameController_loop();
void gameController_cleanup();

GameState* gameController_getGameState();