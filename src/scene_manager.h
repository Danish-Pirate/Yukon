#include "view/ui_button.h"

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