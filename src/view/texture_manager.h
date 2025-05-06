#pragma once

#include <SDL.h>

SDL_Texture *getCardTexAtlas();

// Initialize the texture manager
void textureManager_init();


// Free all textures and the texture manager
void textureManager_cleanup();
