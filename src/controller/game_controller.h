#pragma once

#include <SDL.h>
#include "../model/game.h"
#include "../utils/event_system.h"

void initGameController(SDL_Window *window, SDL_Renderer *renderer);
void loopGameController();
void cleanUpGameController();