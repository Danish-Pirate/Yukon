#include "service_locator.h"

static GameState* gameStateInstance = NULL;

void serviceLocator_init() {
    gameStateInstance = NULL;
}

void serviceLocator_cleanup() {
    gameStateInstance = NULL;
}

void serviceLocator_registerGameState(GameState* state) {
    gameStateInstance = state;
}

GameState* serviceLocator_getGameState() {
    return gameStateInstance;
}