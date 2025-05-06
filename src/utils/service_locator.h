// src/utils/service_locator.h
#pragma once

#include <SDL.h>
#include "../model/game.h"

void serviceLocator_init();
void serviceLocator_cleanup();

void serviceLocator_registerRenderer(SDL_Renderer* renderer);
SDL_Renderer* serviceLocator_getRenderer();

void serviceLocator_registerWindow(SDL_Window* window);
SDL_Window* serviceLocator_getWindow();

void serviceLocator_registerGameState(GameState* state);
GameState* serviceLocator_getGameState();