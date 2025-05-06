#pragma once

#include "SDL2/SDL.h"
#include "core/include/yukon_core.h"

void serviceLocator_init();
void serviceLocator_cleanup();

// Game state
void serviceLocator_registerGameState(GameState* state);
GameState* serviceLocator_getGameState();

// GUI components
void serviceLocator_registerRenderer(SDL_Renderer* renderer);
SDL_Renderer* serviceLocator_getRenderer();
void serviceLocator_registerWindow(SDL_Window* window);
SDL_Window* serviceLocator_getWindow();