#include "service_locator.h"
#include <stdlib.h>

static SDL_Renderer* rendererInstance = NULL;
static SDL_Window* windowInstance = NULL;
static GameState* gameStateInstance = NULL;

void serviceLocator_init() {
    rendererInstance = NULL;
    windowInstance = NULL;
    gameStateInstance = NULL;
}

void serviceLocator_cleanup() {
    rendererInstance = NULL;
    windowInstance = NULL;
    gameStateInstance = NULL;
}

void serviceLocator_registerRenderer(SDL_Renderer* renderer) {
    rendererInstance = renderer;
}

SDL_Renderer* serviceLocator_getRenderer() {
    return rendererInstance;
}

void serviceLocator_registerWindow(SDL_Window* window) {
    windowInstance = window;
}

SDL_Window* serviceLocator_getWindow() {
    return windowInstance;
}

void serviceLocator_registerGameState(GameState* state) {
    gameStateInstance = state;
}

GameState* serviceLocator_getGameState() {
    return gameStateInstance;
}