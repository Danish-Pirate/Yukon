#include "scene_manager.h"
#include "startup_scene.h"
#include "view/ui_button.h"
#include "play_scene.h"
#include "utils/service_locator.h"
#include "view/ui_manager.h"

static Scene sceneRegistry[2];
static Scene* currentScene = NULL;
static SceneChangeData* lastSceneChangeData = NULL;

void sceneManager_handleSceneChangeEvent(Event* event) {
    if (event->type != EVENT_SCENE_CHANGE) return;

    SceneChangeData* data = (SceneChangeData*)event->data;

    if (!data) {
        fprintf(stderr, "ERROR: Null scene change data\n");
        return;
    }

    // Validate scene type
    if (data->type != SCENE_STARTUP_MODE && data->type != SCENE_PLAY_MODE) {
        fprintf(stderr, "ERROR: Invalid scene type: %d\n", data->type);
        free(data);
        return;
    }

    SceneType type = data->type;
    void* sceneData = data->data;

    if (lastSceneChangeData == data) {
        fprintf(stderr, "WARNING: Attempt to process same scene change data twice\n");
        return;
    }
    lastSceneChangeData = data;

    sceneManager_changeScene(type, sceneData);
    free(data);
}

void sceneManager_subscribeToEvents() {
    eventSystem_subscribe(EVENT_SCENE_CHANGE, sceneManager_handleSceneChangeEvent);
}

void sceneManager_init() {
    currentScene = NULL;

    uiManager_init(serviceLocator_getRenderer());

    sceneRegistry[SCENE_STARTUP_MODE] = (Scene){
            .init = startupScene_init,
            .handleEvent = startupScene_handleEvent,
            .update = startupScene_update,
            .render = startupScene_render,
            .cleanup = startupScene_cleanup,
            .type = SCENE_STARTUP_MODE,
            .buttons = NULL,
            .buttonCount = 0,
            .sceneData = NULL
    };

    sceneRegistry[SCENE_PLAY_MODE] = (Scene){
            .init = playScene_init,
            .handleEvent = playScene_handleEvent,
            .update = playScene_update,
            .render = playScene_render,
            .cleanup = playScene_cleanup,
            .type = SCENE_PLAY_MODE,
            .buttons = NULL,
            .buttonCount = 0,
            .sceneData = NULL
    };

    initButtonFont();
    sceneManager_subscribeToEvents();
}

void sceneManager_cleanup() {
    if (currentScene && currentScene->cleanup) {
        currentScene->cleanup();
    }
    currentScene = NULL;

    uiManager_cleanup();
}

// In scene_manager.c
void sceneManager_changeScene(SceneType sceneType, void* data) {
    // Validate scene type to prevent crashes
    if (sceneType != SCENE_STARTUP_MODE && sceneType != SCENE_PLAY_MODE) {
        fprintf(stderr, "ERROR: Invalid scene type: %d\n", sceneType);
        return; // Don't crash with invalid scene
    }

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

    if (!renderer) {
        printf("Renderer is NULL in sceneManager_render!\n");
        return;
    }

    // Clear screen with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render current scene
    if (currentScene && currentScene->render) {
        currentScene->render();
    } else if (!currentScene) {
        printf("No current scene in sceneManager_render!\n");
    }

    SDL_RenderPresent(renderer);
}