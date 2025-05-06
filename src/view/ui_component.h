#pragma once

#include <SDL.h>
#include <stdbool.h>

typedef struct UIComponent {
    void (*render)(struct UIComponent* component, SDL_Renderer* renderer);
    bool (*handleEvent)(struct UIComponent* component, SDL_Event* event);
    SDL_Rect bounds;
    void* data;
    bool isVisible;
    int zIndex;
} UIComponent;

void uiComponent_init(UIComponent* component);
void uiComponent_setVisible(UIComponent* component, bool visible);
bool uiComponent_isPointInside(UIComponent* component, int x, int y);