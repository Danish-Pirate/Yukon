#pragma once

#include "SDL2/SDL.h"
#include <stdbool.h>

// Startup scene function declarations
void startupScene_init(void* data);
void startupScene_handleEvent(SDL_Event* event);
void startupScene_update();
void startupScene_render();
void startupScene_cleanup(void);