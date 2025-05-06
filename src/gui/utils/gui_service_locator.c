#include "gui_service_locator.h"
#include <stdlib.h>

static SDL_Renderer* rendererInstance = NULL;
static SDL_Window* windowInstance = NULL;

void guiServiceLocator_init() {
    rendererInstance = NULL;
    windowInstance = NULL;
}

void guiServiceLocator_cleanup() {
    rendererInstance = NULL;
    windowInstance = NULL;
}

void guiServiceLocator_registerRenderer(SDL_Renderer* renderer) {
    rendererInstance = renderer;
}

SDL_Renderer* guiServiceLocator_getRenderer() {
    return rendererInstance;
}

void guiServiceLocator_registerWindow(SDL_Window* window) {
    windowInstance = window;
}

SDL_Window* guiServiceLocator_getWindow() {
    return windowInstance;
}