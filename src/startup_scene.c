#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scene_manager.h"
#include "texture_manager.h"
#include "controller/game_controller.h"
#include "view/ui_card.h"
#include "model/game.h"
#include "view/ui_button.h"
#include "nfd.h"
#include "utils/game_utils.h"
#include "SDL_ttf.h"

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
    char* filePath = getSaveFilePathFromDialog();
    if (filePath[0] == '\0') {
        NFD_FreePathU8(filePath);
        return;
    }
    gameManager_saveDeckToFile(gameManager_getGameState(), filePath);
    NFD_FreePathU8(filePath);
}

void loadDeckCallback() {
    char* filePath = getLoadFilePathFromDialog();
    if (filePath[0] == '\0') { return; }
    gameManager_loadDeck(gameManager_getGameState(), filePath);
}

void revealDeckCallback() {
    gameManager_revealDeck(gameManager_getGameState());
}

void shuffleDeckCallback() {
    gameManager_randomShuffleDeck(gameManager_getGameState());
}

void splitDeckCallback() {
    // Show dialog to get split index
    showSplitDialog = true;
    splitIndexDialog.cursor = 0;
    splitIndexDialog.text[0] = '\0';
}

void processSplitDialogEvent(SDL_Event* event) {
    if (!showSplitDialog) return;

    // Get the current deck size for validation
    int deckSize = 52; // Default to standard deck size
    GameState* gameState = gameManager_getGameState();
    if (gameState && gameState->deck) {
        // Count the actual cards in the deck
        Node* current = gameState->deck->head;
        deckSize = 0;
        while (current != NULL) {
            deckSize++;
            current = current->nextNode;
        }
    }

    if (event->type == SDL_KEYDOWN) {
        // Handle key presses for the input field
        if (event->key.keysym.sym == SDLK_RETURN) {
            // Process the input when Enter is pressed
            int cardNumber = atoi(splitIndexDialog.text);
            // Validate: 1 <= cardNumber < deckSize (only up to card 51)
            if (cardNumber >= 1 && cardNumber < deckSize) {
                // Split deck using the card number as the index
                gameManager_splitDeck(gameManager_getGameState(), cardNumber);
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

    SDL_Renderer* renderer = getRenderer();

    // Get the current deck size for prompt
    int deckSize = 52; // Default to standard deck size
    GameState* gameState = gameManager_getGameState();
    if (gameState && gameState->deck) {
        // Count the actual cards in the deck
        Node* current = gameState->deck->head;
        deckSize = 0;
        while (current != NULL) {
            deckSize++;
            current = current->nextNode;
        }
    }

    // Darken the background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect fullScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &fullScreen);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Draw dialog background
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &splitIndexDialog.rect);

    // Draw dialog border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &splitIndexDialog.rect);

    // Render prompt text
    TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 18);
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
    // Define some button properties
    int buttonWidth = 150;  // Increased width from 125 to 150
    int buttonHeight = 60;
    int buttonSpacing = 20;
    int totalWidth = (buttonWidth * 6) + (buttonSpacing * 5);
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

    splitIndexDialog.rect.x = (SCREEN_WIDTH - dialogWidth) / 2;
    splitIndexDialog.rect.y = (SCREEN_HEIGHT - dialogHeight) / 2;
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

void startupScene_update() {
    // Update animation or state if needed
}

void startupScene_render() {
    SDL_Renderer *renderer = serviceLocator_getRenderer();
    if (renderer == NULL) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &bgRect);

    drawDeck();

    for (int i = 0; i < buttonCount; ++i) {
        uiManager_drawButton(&UI_Buttons[i]);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect border = {10, 10, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 90};
    SDL_RenderDrawRect(renderer, &border);

    if (showSplitDialog) {
        renderSplitDialog();
    }
}

void startupScene_cleanup(void) {
    // Free all textures
    textureManager_cleanup();
}