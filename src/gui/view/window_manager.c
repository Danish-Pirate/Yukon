#include <SDL.h>

static SDL_Renderer* renderer = NULL;
static SDL_Window* window = NULL;

void windowManager_init(SDL_Window* w, SDL_Renderer* r) {
    renderer = r;
    window = w;
}

void windowManager_cleanup() {
    renderer = NULL;
    window = NULL;
}

SDL_Renderer* windowManager_getRenderer() {
    return renderer;
}

SDL_Window* windowManager_getWindow() {
    return window;
}