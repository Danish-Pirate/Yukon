#include <SDL.h>
#include <stdio.h>
#include "scene_manager.h"
#include "model/game.h"
#include "model/ui_card.h"
#include "controller/game_controller.h"
#include "play_scene.h"
#include "game_utils.h"
#include "model/ui_button.h"
#include "SDL_ttf.h"

typedef struct {
    bool isDragging;
    int sourceColumnIndex;
    Node* selectedNode;        // First node in the chain being dragged
    int dragOffsetX;
    int dragOffsetY;
    SDL_Rect dragBaseRect;     // Base rectangle for the first card
    int cardCount;             // Number of cards being dragged
} DragState;

static DragState dragState = {0};
static UI_Button backButton;

static bool tryStartDragging(int mouseX, int mouseY) {
    GameState* gameState = gameManager_getGameState();

    // Check card columns
    for (int colIdx = 0; colIdx < COLUMNS_SIZE; colIdx++) {
        LinkedList* column = gameState->cardColumns[colIdx];
        if (column->head == NULL) continue;

        // Store the card that is clickable (topmost at each position)
        Node* clickableNode = NULL;
        SDL_Rect clickableRect = {0};
        int cardCount = 0;

        // Calculate positions for all cards to find the one under the cursor
        Node* tempNode = column->head;
        int cardY = COLUMN_Y_START;

        while (tempNode != NULL) {
            Card* card = (Card*)tempNode->data;

            // Only face-up cards can be dragged
            if (card->isFaceUp) {
                SDL_Rect cardRect = {
                        .x = COLUMN_X_START + (colIdx * COLUMN_X_SPACING),
                        .y = cardY,
                        .w = CARD_DISPLAY_WIDTH,
                        .h = CARD_DISPLAY_HEIGHT
                };

                // If mouse is over this card
                if (isCardHovered(mouseX, mouseY, cardRect)) {
                    // This is a candidate for selection
                    clickableNode = tempNode;
                    clickableRect = cardRect;

                    // Count how many cards are below this one (including this one)
                    Node* countNode = tempNode;
                    cardCount = 0;
                    while (countNode != NULL) {
                        cardCount++;
                        countNode = countNode->nextNode;
                    }
                }
            }

            cardY += COLUMN_Y_CARD_OFFSET;
            tempNode = tempNode->nextNode;
        }

        // If we found a card to drag
        if (clickableNode != NULL) {
            dragState.isDragging = true;
            dragState.sourceColumnIndex = colIdx;
            dragState.selectedNode = clickableNode;
            dragState.dragOffsetX = mouseX - clickableRect.x;
            dragState.dragOffsetY = mouseY - clickableRect.y;
            dragState.dragBaseRect = clickableRect;
            dragState.cardCount = cardCount;
            return true;
        }
    }

    return false;
}

static void updateDragPosition(int mouseX, int mouseY) {
    dragState.dragBaseRect.x = mouseX - dragState.dragOffsetX;
    dragState.dragBaseRect.y = mouseY - dragState.dragOffsetY;
}

static void handleDrop(int mouseX, int mouseY) {
    // Variables to track drop target
    int targetFoundation = -1;
    int targetColumn = -1;
    GameState* gameState = gameManager_getGameState();

    // Check foundation piles first
    for (int i = 0; i < PILES_SIZE; i++) {
        SDL_Rect foundRect = {
                .x = FOUNDATION_X_START,
                .y = FOUNDATION_Y + (i * (FOUNDATION_Y_SPACING + CARD_DISPLAY_HEIGHT)),
                .w = CARD_DISPLAY_WIDTH,
                .h = CARD_DISPLAY_HEIGHT
        };

        if (isPointWithinRect(mouseX, mouseY, foundRect)) {
            targetFoundation = i;
            break;
        }
    }

    // Process foundation drop
    if (targetFoundation != -1) {
        // Only allow drops if dragging from a column
        if (dragState.sourceColumnIndex < COLUMNS_SIZE) {
            Card* card = (Card*)dragState.selectedNode->data;

            // Make sure we're dragging exactly one card (foundation only takes single cards)
            if (dragState.cardCount == 1) {
                // Foundation index is column index + COLUMNS_SIZE
                int foundationIndex = targetFoundation + COLUMNS_SIZE;

                // Move the card to the foundation
                gameManager_moveCard(gameState, card->rank, card->suit,
                                     dragState.sourceColumnIndex, foundationIndex);
            }
            // Check if the game has been won after a foundation pile move
            gameManager_isGameWon(gameManager_getGameState());
        }
    }
    else {
        // Check columns (keeping the original column detection code)
        for (int colIdx = 0; colIdx < COLUMNS_SIZE; colIdx++) {
            int colX = COLUMN_X_START + (colIdx * COLUMN_X_SPACING);

            if (mouseX >= colX && mouseX <= colX + CARD_DISPLAY_WIDTH) {
                if (mouseY >= COLUMN_Y_START) {
                    targetColumn = colIdx;
                    break;
                }
            }
        }

        if (targetColumn != -1 && targetColumn != dragState.sourceColumnIndex) {
            Card* firstCard = (Card*)dragState.selectedNode->data;

            gameManager_moveCard(gameState, firstCard->rank, firstCard->suit,
                                 dragState.sourceColumnIndex, targetColumn);
        }
    }

    // Reset drag state
    dragState.isDragging = false;
    dragState.selectedNode = NULL;
    dragState.cardCount = 0;
}

static void drawVictoryDialog() {
    SDL_Renderer *renderer = getRenderer();
    // Create a semi-transparent overlay for the victory dialog
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180); // Semi-transparent black
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &bgRect);

    // Draw the dialog box
    int dialogWidth = 400;
    int dialogHeight = 200;
    SDL_Rect dialogRect = {
            (SCREEN_WIDTH - dialogWidth) / 2,
            (SCREEN_HEIGHT - dialogHeight) / 2,
            dialogWidth,
            dialogHeight
    };

    // Dialog background
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255); // Light gray
    SDL_RenderFillRect(renderer, &dialogRect);

    // Dialog border
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Dark gray
    SDL_RenderDrawRect(renderer, &dialogRect);

    // Display the victory messages
    TTF_Font* largeFont = TTF_OpenFont("assets/fonts/arial.ttf", 36);
    TTF_Font* smallFont = TTF_OpenFont("assets/fonts/arial.ttf", 24);

    if (largeFont && smallFont) {
        SDL_Color titleColor = {0, 100, 0, 255}; // Dark green
        SDL_Color textColor = {0, 0, 0, 255};    // Black

        // Render "VICTORY!" text
        SDL_Surface* titleSurface = TTF_RenderText_Solid(largeFont, "VICTORY!", titleColor);
        if (titleSurface) {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            if (titleTexture) {
                SDL_Rect titleRect = {
                        (SCREEN_WIDTH - titleSurface->w) / 2,
                        dialogRect.y + 30,
                        titleSurface->w,
                        titleSurface->h
                };
                SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
                SDL_DestroyTexture(titleTexture);
            }
            SDL_FreeSurface(titleSurface);
        }

        // Render "Congratulations!" text
        SDL_Surface* congratsSurface = TTF_RenderText_Solid(smallFont, "Congratulations!", textColor);
        if (congratsSurface) {
            SDL_Texture* congratsTexture = SDL_CreateTextureFromSurface(renderer, congratsSurface);
            if (congratsTexture) {
                SDL_Rect congratsRect = {
                        (SCREEN_WIDTH - congratsSurface->w) / 2,
                        dialogRect.y + 80,
                        congratsSurface->w,
                        congratsSurface->h
                };
                SDL_RenderCopy(renderer, congratsTexture, NULL, &congratsRect);
                SDL_DestroyTexture(congratsTexture);
            }
            SDL_FreeSurface(congratsSurface);
        }

        // Render "You've won the game!" text
        SDL_Surface* wonSurface = TTF_RenderText_Solid(smallFont, "You've won the game!", textColor);
        if (wonSurface) {
            SDL_Texture* wonTexture = SDL_CreateTextureFromSurface(renderer, wonSurface);
            if (wonTexture) {
                SDL_Rect wonRect = {
                        (SCREEN_WIDTH - wonSurface->w) / 2,
                        dialogRect.y + 120,
                        wonSurface->w,
                        wonSurface->h
                };
                SDL_RenderCopy(renderer, wonTexture, NULL, &wonRect);
                SDL_DestroyTexture(wonTexture);
            }
            SDL_FreeSurface(wonSurface);
        }

        // Draw a button (optional)
        int buttonWidth = 100;
        int buttonHeight = 40;
        SDL_Rect buttonRect = {
                (SCREEN_WIDTH - buttonWidth) / 2,
                dialogRect.y + dialogHeight - 50,
                buttonWidth,
                buttonHeight
        };

        // Button background
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // Dark green
        SDL_RenderFillRect(renderer, &buttonRect);

        // Button border
        SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255); // Darker green
        SDL_RenderDrawRect(renderer, &buttonRect);

        // Button text ("OK")
        SDL_Surface* buttonSurface = TTF_RenderText_Solid(smallFont, "OK", (SDL_Color){255, 255, 255, 255}); // White
        if (buttonSurface) {
            SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
            if (buttonTexture) {
                SDL_Rect textRect = {
                        buttonRect.x + (buttonRect.w - buttonSurface->w) / 2,
                        buttonRect.y + (buttonRect.h - buttonSurface->h) / 2,
                        buttonSurface->w,
                        buttonSurface->h
                };
                SDL_RenderCopy(renderer, buttonTexture, NULL, &textRect);
                SDL_DestroyTexture(buttonTexture);
            }
            SDL_FreeSurface(buttonSurface);
        }

        // Clean up fonts
        TTF_CloseFont(largeFont);
        TTF_CloseFont(smallFont);
    }

    // Reset blend mode
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

static void drawFoundationPiles() {
    SDL_Renderer* renderer = getRenderer();
    GameState* gameState = gameManager_getGameState();

    // Draw the 4 foundation slots
    for (int i = 0; i < PILES_SIZE; i++) {
        // Draw empty foundation slot outline
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect foundationRect = {
                .x = FOUNDATION_X_START,
                .y = FOUNDATION_Y + (i * (FOUNDATION_Y_SPACING + CARD_DISPLAY_HEIGHT)),
                .w = CARD_DISPLAY_WIDTH,
                .h = CARD_DISPLAY_HEIGHT
        };
        SDL_RenderDrawRect(renderer, &foundationRect);

        // If there are cards in the foundation pile, draw the top card
        if (gameState->cardFoundationPiles[i]->head != NULL) {
            Card* topCard = (Card*)gameState->cardFoundationPiles[i]->tail->data;
            SDL_Rect cardRect = {
                    .x = FOUNDATION_X_START,
                    .y = FOUNDATION_Y + (i * (FOUNDATION_Y_SPACING + CARD_DISPLAY_HEIGHT)),
                    .w = CARD_DISPLAY_WIDTH,
                    .h = CARD_DISPLAY_HEIGHT
            };
            drawCard(cardRect, topCard);
        }
    }
}

static void drawColumns() {
    GameState* gameState = gameManager_getGameState();

    // Draw the 7 columns
    for (int colIdx = 0; colIdx < COLUMNS_SIZE; colIdx++) {
        LinkedList* column = gameState->cardColumns[colIdx];

        // Draw outline for column
        SDL_SetRenderDrawColor(getRenderer(), 100, 100, 100, 255);
        SDL_Rect emptyColRect = {
                .x = COLUMN_X_START + (colIdx * COLUMN_X_SPACING),
                .y = COLUMN_Y_START,
                .w = CARD_DISPLAY_WIDTH,
                .h = CARD_DISPLAY_HEIGHT
        };
        SDL_RenderDrawRect(getRenderer(), &emptyColRect);

        // Draw all cards in the column if it has cards
        if (column->head != NULL) {
            Node* currentNode = column->head;
            int cardY = COLUMN_Y_START;
            bool skipDrawing = false;

            while (currentNode != NULL) {
                // If this is the card being dragged, skip it and all subsequent cards
                if (dragState.isDragging && currentNode == dragState.selectedNode &&
                    colIdx == dragState.sourceColumnIndex) {
                    skipDrawing = true;
                }

                if (!skipDrawing) {
                    Card* card = (Card*)currentNode->data;
                    SDL_Rect cardRect = {
                            .x = COLUMN_X_START + (colIdx * COLUMN_X_SPACING),
                            .y = cardY,
                            .w = CARD_DISPLAY_WIDTH,
                            .h = CARD_DISPLAY_HEIGHT
                    };
                    drawCard(cardRect, card);
                }

                // Move down for the next card in the column, with offset
                cardY += COLUMN_Y_CARD_OFFSET;
                currentNode = currentNode->nextNode;
            }
        }
    }

    // Draw the dragged cards on top if dragging
    if (dragState.isDragging && dragState.selectedNode != NULL) {
        Node* dragNode = dragState.selectedNode;
        int offsetY = 0;

        // Draw each card in the stack
        while (dragNode != NULL) {
            Card* card = (Card*)dragNode->data;
            SDL_Rect cardRect = {
                    .x = dragState.dragBaseRect.x,
                    .y = dragState.dragBaseRect.y + offsetY,
                    .w = CARD_DISPLAY_WIDTH,
                    .h = CARD_DISPLAY_HEIGHT
            };
            drawCard(cardRect, card);

            offsetY += COLUMN_Y_CARD_OFFSET;
            dragNode = dragNode->nextNode;
        }
    }
}

static void backToStartupCallback() {
    gameManager_exitPlayMode(gameManager_getGameState());
    sceneManager_changeScene(SCENE_STARTUP_MODE, NULL);
}

void playScene_init(void* data) {
    gameManager_enterPlayMode(gameManager_getGameState());

    // Create the back button
    int buttonHeight = 60;
    int buttonY = SCREEN_HEIGHT - buttonHeight - 200; // Match startup scene button height

    backButton.callback = backToStartupCallback;
    backButton.displayRect.x = (SCREEN_WIDTH - 150) / 2;  // Center horizontally
    backButton.displayRect.y = buttonY;                   // Same height as startup scene buttons
    backButton.displayRect.w = 150;                       // Width
    backButton.displayRect.h = buttonHeight;              // Height
    backButton.label = "Back";
}

void playScene_handleEvent(SDL_Event* event) {
    GameState* gameState = gameManager_getGameState();

    // First check if the game is won and handle OK button clicks
    if (gameState->gameWon && event->type == SDL_MOUSEBUTTONDOWN &&
        event->button.button == SDL_BUTTON_LEFT) {

        // Define the OK button rectangle (must match the one in drawVictoryDialog)
        int buttonWidth = 100;
        int buttonHeight = 40;
        int dialogWidth = 400;
        int dialogHeight = 200;

        SDL_Rect buttonRect = {
                (SCREEN_WIDTH - buttonWidth) / 2,
                (SCREEN_HEIGHT - dialogHeight) / 2 + dialogHeight - 50,
                buttonWidth,
                buttonHeight
        };

        // Check if click is within the button
        if (isPointWithinRect(event->button.x, event->button.y, buttonRect)) {
            // Handle the button click - either dismiss the dialog or reset the game
            gameState->gameWon = false;  // This will hide the victory dialog

            // Optional: Add additional actions like resetting the game or returning to menu
            // For example: backToStartupCallback();

            return;  // Important! Return to prevent processing other input
        }

        // If click is outside the button but game is won, consume the event
        return;
    }

    // Rest of the existing event handling code
    // Handle card dragging based on event type
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        // Check if the back button was clicked
        if (isButtonHovered(event->button.x, event->button.y, backButton.displayRect)) {
            backButton.callback();
            return;
        }

        tryStartDragging(event->button.x, event->button.y);
    }
    else if (event->type == SDL_MOUSEMOTION && dragState.isDragging) {
        updateDragPosition(event->motion.x, event->motion.y);
    }
    else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && dragState.isDragging) {
        handleDrop(event->button.x, event->button.y);
    }
}

void playScene_update() {
    // Update game state, check for victories, etc.
}

void playScene_render() {
    SDL_Renderer* renderer = getRenderer();

    // Set background color
    SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255); // Dark green background
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &bgRect);

    // Draw foundation piles at the top
    drawFoundationPiles();

    // Draw the 7 columns
    drawColumns();

    // Draw the back button
    drawButton(backButton);

    if(gameManager_getGameState()->gameWon) {
        drawVictoryDialog();
    }

    // Draw border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    SDL_Rect border = {10, 10, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 90};
    SDL_RenderDrawRect(renderer, &border);
}

void playScene_cleanup(void) {
    // Any cleanup needed when leaving the play scene
}