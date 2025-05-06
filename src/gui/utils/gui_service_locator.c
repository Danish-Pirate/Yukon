#include <stdlib.h>
#include "gui_service_locator.h"

static GameState* gameStateInstance = NULL;
static SDL_Renderer* rendererInstance = NULL;
static SDL_Window* windowInstance = NULL;

void serviceLocator_init() {
    gameStateInstance = NULL;
    rendererInstance = NULL;
    windowInstance = NULL;
}

void serviceLocator_cleanup() {
    gameStateInstance = NULL;
    rendererInstance = NULL;
    windowInstance = NULL;
}

void serviceLocator_registerGameState(GameState* state) {
    gameStateInstance = state;
}

GameState* serviceLocator_getGameState() {
    return gameStateInstance;
}

void serviceLocator_registerRenderer(SDL_Renderer* renderer) {
    rendererInstance = renderer;
}

SDL_Renderer* windowManager_getRenderer() {
    return rendererInstance;
}

void serviceLocator_registerWindow(SDL_Window* window) {
    windowInstance = window;
}

SDL_Window* serviceLocator_getWindow() {
    return windowInstance;
}