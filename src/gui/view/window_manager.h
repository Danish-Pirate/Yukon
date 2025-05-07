#pragma once

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS 60

void windowManager_init(SDL_Window* window, SDL_Renderer* renderer);
void windowManager_cleanup();
SDL_Renderer* windowManager_getRenderer();
SDL_Window* windowManager_getWindow();