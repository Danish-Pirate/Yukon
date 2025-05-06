#pragma once

#include "../model/game.h"
#include "../utils/event_system.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS 60

void initGameController(SDL_Window *window, SDL_Renderer *renderer);
void loopGameController();
void cleanUpGameController();