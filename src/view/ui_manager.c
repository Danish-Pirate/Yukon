#include "ui_manager.h"
#include "ui_button.h"
#include "ui_card.h"

static SDL_Renderer* renderer = NULL;

void uiManager_init(SDL_Renderer* rendererInstance) {
    renderer = rendererInstance;
    initButtonFont();
}

void uiManager_cleanup() {
    renderer = NULL;
}

void uiManager_drawButton(UI_Button* button) {
    drawButton(*button);
}

bool uiManager_handleButtonEvents(UI_Button* buttons, int buttonCount, SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        for (int i = 0; i < buttonCount; i++) {
            if (isButtonHovered(mouseX, mouseY, buttons[i].displayRect)) {
                buttons[i].callback();
                return true;
            }
        }
    }
    return false;
}

void uiManager_drawCard(SDL_Rect rect, Card* card) {
    drawCard(rect, card);
}