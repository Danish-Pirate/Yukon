#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "SDL_render.h"
#include "scene/scene_manager.h"
#include "../utils/service_locator.h"

#define cardTexAtlas_FILEPATH "assets/textures/card_atlas.bmp"

static SDL_Texture* cardTexAtlas = NULL;

SDL_Texture* getCardTexAtlas() {
    return cardTexAtlas;
}

void textureManager_init() {
    // Check if file exists
    FILE* test = fopen(cardTexAtlas_FILEPATH, "rb");
    if (test == NULL) {
        fprintf(stderr, "ERROR: Card atlas file not found at %s\n", cardTexAtlas_FILEPATH);

        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            fprintf(stderr, "Current working directory: %s\n", cwd);
        }
        return;
    }
    fclose(test);

    // Load the texture
    SDL_Surface* surface = SDL_LoadBMP(cardTexAtlas_FILEPATH);
    if (surface == NULL) {
        fprintf(stderr, "Failed to load texture: %s\n", SDL_GetError());
        return;
    }

    cardTexAtlas = SDL_CreateTextureFromSurface(serviceLocator_getRenderer(), surface);
    SDL_FreeSurface(surface);

    if (cardTexAtlas == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    }
}

void textureManager_cleanup() {
    if (cardTexAtlas != NULL) {
        SDL_DestroyTexture(cardTexAtlas);
        cardTexAtlas = NULL;
    }
}
