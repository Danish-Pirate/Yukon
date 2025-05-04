#pragma once

#include <SDL.h>
#include <stdbool.h>

typedef enum {
    SCENE_STARTUP_MODE,
    SCENE_PLAY_MODE,
} SceneType;

typedef struct {
    void (*init)(void* data);
    void (*handleEvent)(SDL_Event* event);
    void (*update)();
    void (*render)();
    void (*cleanup)(void);
    SceneType type;
} Scene;

SDL_Renderer* getRenderer();
SDL_Window * getWindow();
void sceneManager_init(SDL_Window *window, SDL_Renderer* renderer);
void sceneManager_cleanup(void);
void sceneManager_changeScene(SceneType sceneType, void* data);
void sceneManager_handleEvents(SDL_Event* event);
void sceneManager_update();
void sceneManager_render(void);