#include "ui_component.h"

void uiComponent_init(UIComponent* component) {
    component->isVisible = true;
    component->zIndex = 0;
}

void uiComponent_setVisible(UIComponent* component, bool visible) {
    component->isVisible = visible;
}

bool uiComponent_isPointInside(UIComponent* component, int x, int y) {
    if (!component->isVisible) {
        return false;
    }

    return (x >= component->bounds.x &&
            x <= component->bounds.x + component->bounds.w &&
            y >= component->bounds.y &&
            y <= component->bounds.y + component->bounds.h);
}