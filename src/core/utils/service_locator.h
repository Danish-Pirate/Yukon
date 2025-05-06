#pragma once

#include "model/game.h"

void serviceLocator_init();
void serviceLocator_cleanup();
void serviceLocator_registerGameState(GameState* state);
GameState* serviceLocator_getGameState();