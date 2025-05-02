#include <stdio.h>
#include <string.h>
#include "SDL_render.h"
#include "scene_manager.h"

#define cardTexAtlas_FILEPATH "assets/textures/card_atlas.bmp"

static SDL_Texture* cardTexAtlas = NULL;

SDL_Texture* getCardTexAtlas() {
    return cardTexAtlas;
}

void textureManager_init() {
    SDL_Surface* surface = SDL_LoadBMP(cardTexAtlas_FILEPATH);
    if (surface == NULL) {
        fprintf(stderr, "Failed to load texture: %s\n", SDL_GetError());
        return;
    }

    cardTexAtlas = SDL_CreateTextureFromSurface(getRenderer(), surface);
    SDL_FreeSurface(surface);

    if (cardTexAtlas == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
    }
}

void textureManager_cleanup() {

}
