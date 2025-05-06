#include "scene_manager.h"
#include "startup_scene.h"
#include "view/ui_button.h"
#include "play_scene.h"
#include "utils/service_locator.h"

static Scene sceneRegistry[2];
static Scene* currentScene = NULL;

void sceneManager_handleSceneChangeEvent(Event* event) {
    if (event->type != EVENT_SCENE_CHANGE) return;

    SceneChangeData* data = (SceneChangeData*)event->data;
    sceneManager_changeScene(data->type, data->data);
    free(data);
}

void sceneManager_subscribeToEvents() {
    eventSystem_subscribe(EVENT_SCENE_CHANGE, sceneManager_handleSceneChangeEvent);
}

void sceneManager_init() {
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
    sceneManager_subscribeToEvents();
}

void sceneManager_cleanup() {
    if (currentScene && currentScene->cleanup) {
        currentScene->cleanup();
    }
    currentScene = NULL;
}

void sceneManager_changeScene(SceneType sceneType, void* data) {
    if (currentScene && currentScene->cleanup) {
        currentScene->cleanup();
    }

    currentScene = &sceneRegistry[sceneType];
    currentScene->init(data);
}

void sceneManager_handleEvents(SDL_Event* event) {
    if (currentScene && currentScene->handleEvent) {
        currentScene->handleEvent(event);
    }
}

void sceneManager_update() {
    if (currentScene && currentScene->update) {
        currentScene->update();
    }
}

void sceneManager_render() {
    SDL_Renderer* renderer = serviceLocator_getRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (currentScene && currentScene->render) {
        currentScene->render();
    }

    SDL_RenderPresent(renderer);
}