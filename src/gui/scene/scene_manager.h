#pragma once

#include "SDL2/SDL.h"
#include "gui/utils/event_system.h"
#include "../view/ui_button.h"

typedef enum {
    SCENE_STARTUP_MODE,
    SCENE_PLAY_MODE
} SceneType;

typedef struct {
    SceneType type;
    void* data;
} SceneChangeData;

typedef struct {
    void (*init)(void* data);
    void (*handleEvent)(SDL_Event* event);
    void (*update)();
    void (*render)();
    void (*cleanup)();
    SceneType type;

    UI_Button* buttons;
    int buttonCount;
    void* sceneData;
} Scene;

void sceneManager_init();
void sceneManager_cleanup();
void sceneManager_changeScene(SceneType sceneType, void* data);
void sceneManager_handleEvents(SDL_Event* event);
void sceneManager_update();
void sceneManager_render();
void sceneManager_handleSceneChangeEvent(Event* event);
void sceneManager_subscribeToEvents();