#include <SDL.h>
#include "view/ui_button.h"
#include "data_structures.h"
#include "view/ui_card.h"
#include "scene_manager.h"
#include "gui/utils/gui_utils.h"
#include <nfd.h>
#include <SDL_ttf.h>
#include "view/ui_manager.h"
#include "model/deck.h"
#include "service/core_service.h"
#include "view/window_manager.h"
#include "utils/linked_list.h"

static UI_Button UI_Buttons[12];
static int buttonCount = 0;

// Split deck dialog components
typedef struct {
    SDL_Rect rect;
    char text[32];
    int cursor;
} InputDialog;

static InputDialog splitIndexDialog = {0};
static bool showSplitDialog = false;
static int deckSize = 0;

static void drawDeck() {
    int card_PosY = 20;
    int card_PosX = 200;
    int cardRowCount = 0;

    int cardCount = deckSize;

    for (int i = 0; i < cardCount; i++) {
        if (cardRowCount == 13) {
            card_PosY += CARD_DISPLAY_HEIGHT + CARD_SPACING_Y;
            card_PosX = 200;
            cardRowCount = 0;
        }

        Card* card = coreService_getDeckCard(i);
        if (card) {
            SDL_Rect screenRect = {
                    .x = card_PosX,
                    .y = card_PosY,
                    .w = CARD_DISPLAY_WIDTH,
                    .h = CARD_DISPLAY_HEIGHT
            };
            drawCard(screenRect, card);
        } else {
            fprintf(stderr, "ERROR: Card data is NULL\n");
        }

        card_PosX += CARD_DISPLAY_WIDTH + CARD_SPACING_X;
        cardRowCount++;
    }
}

void playModeCallback() {
    eventSystem_publish(EVENT_PLAY_MODE_ENTER, NULL);
}

void saveDeckCallback() {
    char* filePath = getSaveFilePathFromDialog();
    if (filePath[0] == '\0') {
        NFD_FreePathU8(filePath);
        return;
    }
    eventSystem_publish(EVENT_DECK_SAVED, filePath);
    NFD_FreePathU8(filePath);
}

void loadDeckCallback() {
    char* filePath = getLoadFilePathFromDialog();
    if (filePath[0] == '\0') { return; }
    eventSystem_publish(EVENT_DECK_LOADED_SUCCESS, filePath);
}

void revealDeckCallback() {
    eventSystem_publish(EVENT_DECK_TOGGLED, NULL);
}

void shuffleDeckCallback() {
    eventSystem_publish(EVENT_DECK_SHUFFLED, NULL);
}

void splitDeckCallback() {
    showSplitDialog = true;
    splitIndexDialog.cursor = 0;
    splitIndexDialog.text[0] = '\0';
}

void processSplitDialogEvent(SDL_Event* event) {
    if (!showSplitDialog) return;

    if (event->type == SDL_KEYDOWN) {
        // Handle key presses for the input field
        if (event->key.keysym.sym == SDLK_RETURN) {
            // Process the input when Enter is pressed
            int cardNumber = atoi(splitIndexDialog.text);
            // Validate: 1 <= cardNumber < deckSize (only up to card 51)
            if (cardNumber >= 1 && cardNumber < deckSize) {
                // Split deck using the card number as the index
                int* indexPtr = malloc(sizeof(int));
                if (indexPtr) {
                    *indexPtr = cardNumber;
                    eventSystem_publish(EVENT_DECK_SPLIT, indexPtr);
                }
                showSplitDialog = false;
            } else {
                // Invalid input - just clear the field without feedback
                splitIndexDialog.cursor = 0;
                splitIndexDialog.text[0] = '\0';
            }
        } else if (event->key.keysym.sym == SDLK_ESCAPE) {
            // Cancel with Escape
            showSplitDialog = false;
        } else if (event->key.keysym.sym == SDLK_BACKSPACE && splitIndexDialog.cursor > 0) {
            // Handle backspace
            splitIndexDialog.text[--splitIndexDialog.cursor] = '\0';
        } else if (event->key.keysym.sym >= SDLK_0 && event->key.keysym.sym <= SDLK_9 &&
                   splitIndexDialog.cursor < 3) { // Limit to reasonable digits for deck size
            // Add digit to input
            char newDigit = (char)('0' + (event->key.keysym.sym - SDLK_0));

            // Only allow digits that could form valid numbers
            if (splitIndexDialog.cursor == 0) {
                // First digit can't be 0 (we need positive integers)
                if (newDigit > '0') {
                    splitIndexDialog.text[splitIndexDialog.cursor++] = newDigit;
                    splitIndexDialog.text[splitIndexDialog.cursor] = '\0';
                }
            } else {
                // For subsequent digits, check if number would still be in range
                char tempBuffer[10];
                strcpy(tempBuffer, splitIndexDialog.text);
                tempBuffer[splitIndexDialog.cursor] = newDigit;
                tempBuffer[splitIndexDialog.cursor + 1] = '\0';

                int tempValue = atoi(tempBuffer);

                if (tempValue < deckSize) {  // Only allow values up to deckSize-1
                    splitIndexDialog.text[splitIndexDialog.cursor++] = newDigit;
                    splitIndexDialog.text[splitIndexDialog.cursor] = '\0';
                }
            }
        }
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        // Check if click is outside dialog to cancel
        if (!isPointWithinRect(event->button.x, event->button.y, splitIndexDialog.rect)) {
            showSplitDialog = false;
        }
    }
}

void renderSplitDialog() {
    if (!showSplitDialog) return;

    SDL_Renderer *renderer = windowManager_getRenderer();
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    // Darken the background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect fullScreen = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &fullScreen);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Draw dialog background
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &splitIndexDialog.rect);

    // Draw dialog border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &splitIndexDialog.rect);

    // Render prompt text
    TTF_Font* font = TTF_OpenFont(FONT_FILEPATH, 18);
    if (font) {
        SDL_Color textColor = {0, 0, 0, 255};

        // Use more horizontal space for prompt text
        int textMargin = 30; // Increased left margin
        int textAreaWidth = splitIndexDialog.rect.w - (textMargin * 2); // Width with margins on both sides

        // Create prompt with card-based language
        char promptBuffer[100];
        sprintf(promptBuffer, "Enter card # to split after (1 to %d):", deckSize-1);

        SDL_Surface* promptSurface = TTF_RenderText_Solid(font, promptBuffer, textColor);
        if (promptSurface) {
            SDL_Texture* promptTexture = SDL_CreateTextureFromSurface(renderer, promptSurface);
            if (promptTexture) {
                SDL_Rect promptRect = {
                        splitIndexDialog.rect.x + textMargin,
                        splitIndexDialog.rect.y + 20,
                        promptSurface->w,
                        promptSurface->h
                };
                SDL_RenderCopy(renderer, promptTexture, NULL, &promptRect);
                SDL_DestroyTexture(promptTexture);
            }
            SDL_FreeSurface(promptSurface);
        }

        // Draw input field box - wider to match the wider dialog
        SDL_Rect inputFieldRect = {
                splitIndexDialog.rect.x + textMargin,
                splitIndexDialog.rect.y + 58,
                300,  // Even wider input field (300px)
                30
        };

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderFillRect(renderer, &inputFieldRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &inputFieldRect);

        // Render input text (with blinking cursor)
        char displayText[33];
        if (SDL_GetTicks() % 1000 < 500) {
            // Show cursor
            sprintf(displayText, "%s|", splitIndexDialog.text);
        } else {
            // Hide cursor
            strcpy(displayText, splitIndexDialog.text);
        }

        SDL_Surface* inputSurface = TTF_RenderText_Solid(font,
                                                         displayText[0] ? displayText : " ",
                                                         textColor);
        if (inputSurface) {
            SDL_Texture* inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);
            if (inputTexture) {
                SDL_Rect inputRect = {
                        splitIndexDialog.rect.x + textMargin + 5, // Small additional padding inside the input field
                        splitIndexDialog.rect.y + 62,
                        inputSurface->w,
                        inputSurface->h
                };
                SDL_RenderCopy(renderer, inputTexture, NULL, &inputRect);
                SDL_DestroyTexture(inputTexture);
            }
            SDL_FreeSurface(inputSurface);
        }

        // Add helper text explaining what will happen - using more horizontal space
        SDL_Surface* helpTextSurface = TTF_RenderText_Solid(font,
                                                            "The deck will be split after this card",
                                                            textColor);
        if (helpTextSurface) {
            SDL_Texture* helpTextTexture = SDL_CreateTextureFromSurface(renderer, helpTextSurface);
            if (helpTextTexture) {
                SDL_Rect helpTextRect = {
                        splitIndexDialog.rect.x + textMargin,
                        splitIndexDialog.rect.y + 95,
                        helpTextSurface->w,
                        helpTextSurface->h
                };
                SDL_RenderCopy(renderer, helpTextTexture, NULL, &helpTextRect);
                SDL_DestroyTexture(helpTextTexture);
            }
            SDL_FreeSurface(helpTextSurface);
        }

        // Render instruction/hint text - using more horizontal space
        SDL_Surface* hintSurface = TTF_RenderText_Solid(font,
                                                        "Press Enter to confirm, Esc to cancel",
                                                        textColor);
        if (hintSurface) {
            SDL_Texture* hintTexture = SDL_CreateTextureFromSurface(renderer, hintSurface);
            if (hintTexture) {
                SDL_Rect hintRect = {
                        splitIndexDialog.rect.x + textMargin,
                        splitIndexDialog.rect.y + splitIndexDialog.rect.h - 40,
                        hintSurface->w,
                        hintSurface->h
                };
                SDL_RenderCopy(renderer, hintTexture, NULL, &hintRect);
                SDL_DestroyTexture(hintTexture);
            }
            SDL_FreeSurface(hintSurface);
        }

        TTF_CloseFont(font);
    }
}

void startupScene_init(void* data) {
    SDL_Renderer *renderer = windowManager_getRenderer();
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    eventSystem_publish(EVENT_DECK_SHOWN, NULL);

    deckSize = coreService_getDeckSize();

    int buttonWidth = 150;
    int buttonHeight = 60;
    int buttonSpacing = 20;
    int totalWidth = (buttonWidth * 6) + (buttonSpacing * 5);
    int startX = (width - totalWidth) / 2;
    int buttonY = height - buttonHeight - 200; // 200px margin from bottom

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
            .label = "Show/Hide"
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

    // Button 6: Split Deck
    UI_Button splitDeckButton = {
            .callback = splitDeckCallback,
            .displayRect = {
                    .x = startX + (buttonWidth + buttonSpacing) * 5,
                    .y = buttonY,
                    .w = buttonWidth,
                    .h = buttonHeight
            },
            .label = "Split Deck"
    };
    UI_Buttons[5] = splitDeckButton;

    buttonCount = 6;

    // Initialize the split index dialog
    int dialogWidth = 400;  // Increased width from 300 to 400
    int dialogHeight = 170; // Keep the same height

    splitIndexDialog.rect.x = (width - dialogWidth) / 2;
    splitIndexDialog.rect.y = (height - dialogHeight) / 2;
    splitIndexDialog.rect.w = dialogWidth;
    splitIndexDialog.rect.h = dialogHeight;
    splitIndexDialog.cursor = 0;
    splitIndexDialog.text[0] = '\0';
}

void startupScene_handleEvent(SDL_Event* event) {
    if (showSplitDialog) {
        processSplitDialogEvent(event);
        return;
    }

    if (uiManager_handleButtonEvents(UI_Buttons, buttonCount, event)) {
        return;
    }
}

void startupScene_update() {}

void startupScene_render() {
    SDL_Renderer *renderer = windowManager_getRenderer();
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);
    SDL_Rect bgRect = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &bgRect);

    drawDeck();

    for (int i = 0; i < buttonCount; ++i) {
        uiManager_drawButton(&UI_Buttons[i]);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect border = {10, 10, width - 20, height - 90};
    SDL_RenderDrawRect(renderer, &border);

    if (showSplitDialog) {
        renderSplitDialog();
    }
}

void startupScene_cleanup(void) {}