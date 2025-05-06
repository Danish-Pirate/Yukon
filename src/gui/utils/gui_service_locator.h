#pragma once

#include "SDL2/SDL.h"
#include "core/include/yukon_core.h"

void guiServiceLocator_init();
void guiServiceLocator_cleanup();
void guiServiceLocator_registerRenderer(SDL_Renderer* renderer);
SDL_Renderer* guiServiceLocator_getRenderer();
void guiServiceLocator_registerWindow(SDL_Window* window);
SDL_Window* guiServiceLocator_getWindow();