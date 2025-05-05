#include "controller/game_controller.h"
#include "nfd.h"
#include <SDL_ttf.h>
#include <stdio.h>
#include <time.h>

int main( int argc, char* args[] ) {
    srand(time(NULL));
    NFD_Init();

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    gameInit();
    gameLoop();
    return 0;
}
