#include "scene_manager.h"
#include "startup_scene.h"
#include "model/ui_button.h"
#include "play_scene.h"
#include "utils/service_locator.h"
#include <nfd.h>

// Static variables for scene management
static SDL_Renderer* renderer = NULL;
static SDL_Window* window = NULL;
static Scene* currentScene = NULL;
static Scene sceneRegistry[2];

SDL_Renderer* getRenderer() {
    return serviceLocator_getRenderer();
}

SDL_Window* getWindow() {
    return serviceLocator_getWindow();
}

void sceneManager_handleSceneChangeEvent(Event* event) {
    if (event->type != EVENT_SCENE_CHANGE) return;

    SceneChangeData* data = (SceneChangeData*)event->data;
    sceneManager_changeScene(data->type, data->data);
    free(data);
}

void sceneManager_subscribeToEvents() {
    eventSystem_subscribe(EVENT_SCENE_CHANGE, sceneManager_handleSceneChangeEvent);
}


void sceneManager_init(SDL_Window* _window, SDL_Renderer* _renderer) {
    window = serviceLocator_getWindow();
    renderer = serviceLocator_getRenderer();

    // Initialize the scene registry
    sceneRegistry[SCENE_STARTUP_MODE] = (Scene){
            .init = startupScene_init,
            .handleEvent = startupScene_handleEvent,
            .update = startupScene_update,
            .render = startupScene_render,
            .cleanup = startupScene_cleanup,
            .type = SCENE_STARTUP_MODE
    };

    sceneRegistry[SCENE_PLAY_MODE] = (Scene){
            .init = playScene_init,
            .handleEvent = playScene_handleEvent,
            .update = playScene_update,
            .render = playScene_render,
            .cleanup = playScene_cleanup,
            .type = SCENE_PLAY_MODE
    };

    initButtonFont();

    eventSystem_init();
    sceneManager_subscribeToEvents();
}

void sceneManager_cleanup() {
    currentScene->cleanup();
    currentScene = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    NFD_Quit();
}

void sceneManager_changeScene(SceneType sceneType, void* data) {
    if (currentScene != NULL && currentScene->cleanup != NULL) {
        currentScene->cleanup();
    }

    currentScene = &sceneRegistry[sceneType];
    currentScene->init(data);
}

void sceneManager_handleEvents(SDL_Event* event) {
    currentScene->handleEvent(event);
}

void sceneManager_update() {
    currentScene->update();
}

void sceneManager_render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    currentScene->render();

    SDL_RenderPresent(renderer);
}