#include "scene_manager.h"
#include "startup_scene.h"
#include "model/ui_button.h"
#include "play_scene.h"
#include <nfd.h>

// Static variables for scene management
static SDL_Renderer* renderer = NULL;
static SDL_Window* window = NULL;
static Scene* currentScene = NULL;
static Scene sceneRegistry[2];

SDL_Renderer* getRenderer() {
    return renderer;
}

SDL_Window * getWindow() {
    return window;
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
    window = _window;
    renderer = _renderer;

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

    // Clean up SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    NFD_Quit();
}

void sceneManager_changeScene(SceneType sceneType, void* data) {
    // Clean up current scene if it exists
    if (currentScene != NULL && currentScene->cleanup != NULL) {
        currentScene->cleanup();
    }

    // Validate scene type
    if (sceneType < 0 || sceneType >= 2) {
        return; // Invalid scene type
    }

    // Set the current scene pointer to the appropriate scene in the registry
    currentScene = &sceneRegistry[sceneType];

    // Initialize the new scene with the provided data
    currentScene->init(data);
}

void sceneManager_handleEvents(SDL_Event* event) {
    currentScene->handleEvent(event);
}

void sceneManager_update() {
    // Update the current scene
    currentScene->update();
}

void sceneManager_render() {
    // Clear screen for rendering
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    currentScene->render();

    // Present the rendered frame
    SDL_RenderPresent(renderer);
}