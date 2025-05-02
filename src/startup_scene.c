#include "SDL.h"
#include "scene_manager.h"
#include "texture_manager.h"
#include "game_controller.h"
#include "ui_card.h"
#include "game_manager.h"
#include "ui_button.h"

static UI_Button UI_Buttons[12];
static int buttonCount = 0;

static void drawDeck() {
    int card_PosY = 20;
    int card_PosX = 200;
    int cardRowCount = 0;
    LinkedList* deck = gameManager_getGameState()->deck;

    for (int i = 0; i < DECK_SIZE; ++i) {
        if (cardRowCount == 13) {
            card_PosY += CARD_DISPLAY_HEIGHT + CARD_SPACING_Y;
            card_PosX = 200;
            cardRowCount = 0;
        }
        Card *card = (Card*)getNode(deck, i)->data;
        SDL_Rect screenRect = {
                .x = card_PosX,
                .y = card_PosY,
                .w = CARD_DISPLAY_WIDTH,
                .h = CARD_DISPLAY_HEIGHT
        };
        drawCard(screenRect, card);
        card_PosX += CARD_DISPLAY_WIDTH + CARD_SPACING_X;
        cardRowCount++;
    }
}

void playModeCallback() {
    sceneManager_changeScene(SCENE_PLAY_MODE, NULL);
}

void saveDeckCallback() {

}

void loadDeckCallback() {

}

void revealDeckCallback() {
    gameManager_revealDeck();
}

void shuffleDeckCallback() {
    gameManager_randomShuffleDeck();
}

void startupScene_init(void* data) {
    // Define some button properties
    int buttonWidth = 150;  // Reduced width to fit 5 buttons
    int buttonHeight = 60;
    int buttonSpacing = 20;
    int totalWidth = (buttonWidth * 5) + (buttonSpacing * 4);
    int startX = (SCREEN_WIDTH - totalWidth) / 2;
    int buttonY = SCREEN_HEIGHT - buttonHeight - 200; // 200px margin from bottom

    // Button 1: Play
    UI_Button playButton = {
            .callback = playModeCallback,
            .displayRect = {
                    .x = startX,
                    .y = buttonY,
                    .w = buttonWidth,
                    .h = buttonHeight
            },
            .label = "Play"
    };
    UI_Buttons[0] = playButton;

    // Button 2: Save Deck
    UI_Button saveDeckButton = {
            .callback = saveDeckCallback,
            .displayRect = {
                    .x = startX + buttonWidth + buttonSpacing,
                    .y = buttonY,
                    .w = buttonWidth,
                    .h = buttonHeight
            },
            .label = "Save Deck"
    };
    UI_Buttons[1] = saveDeckButton;

    // Button 3: Load Deck
    UI_Button loadDeckButton = {
            .callback = loadDeckCallback,
            .displayRect = {
                    .x = startX + (buttonWidth + buttonSpacing) * 2,
                    .y = buttonY,
                    .w = buttonWidth,
                    .h = buttonHeight
            },
            .label = "Load Deck"
    };
    UI_Buttons[2] = loadDeckButton;

    // Button 4: Reveal Deck
    UI_Button revealDeckButton = {
            .callback = revealDeckCallback,
            .displayRect = {
                    .x = startX + (buttonWidth + buttonSpacing) * 3,
                    .y = buttonY,
                    .w = buttonWidth,
                    .h = buttonHeight
            },
            .label = "Reveal Deck"
    };
    UI_Buttons[3] = revealDeckButton;

    // Button 5: Shuffle Deck
    UI_Button shuffleDeckButton = {
            .callback = shuffleDeckCallback,
            .displayRect = {
                    .x = startX + (buttonWidth + buttonSpacing) * 4,
                    .y = buttonY,
                    .w = buttonWidth,
                    .h = buttonHeight
            },
            .label = "Shuffle Deck"
    };
    UI_Buttons[4] = shuffleDeckButton;

    buttonCount = 5;
}

void startupScene_handleEvent(SDL_Event* event) {
    if (event->button.type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        int mouse_PosX = event->button.x;
        int mouse_PosY = event->button.y;
        for (int i = 0; i < buttonCount; ++i) {
            if (isButtonHovered(mouse_PosX, mouse_PosY, UI_Buttons[i].displayRect)) {
                UI_Buttons[i].callback();
            }
        }
    }
}

void startupScene_update() {
    // Update animation or state if needed
}

void startupScene_render() {
    SDL_Renderer *renderer = getRenderer();
    if (renderer == NULL) {
        return;
    }

    drawDeck();
    // Draw buttons
    for (int i = 0; i < buttonCount; ++i) {
        drawButton(UI_Buttons[i]);
    }

    // Draw a border around the rendering area
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    SDL_Rect border = {10, 10, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 90};
    SDL_RenderDrawRect(renderer, &border);
}

void startupScene_cleanup(void) {
    // Free all textures
    textureManager_cleanup();
}