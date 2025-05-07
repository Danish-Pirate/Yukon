#pragma once

#include "SDL2/SDL.h"
#include "core/include/yukon_core.h"
#include "../utils/event_system.h"

void initGameController(SDL_Window *window, SDL_Renderer *renderer);
void loopGameController();
void cleanUpGameController();