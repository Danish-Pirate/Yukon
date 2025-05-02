#include "SDL.h"
#include "scene_manager.h"
#include "game_manager.h"
#include "ui_card.h"
#include "game_controller.h"
#include "play_scene.h"
#include "game_utils.h"

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
    // Find which column the mouse is over
    int targetColumn = -1;
    for (int colIdx = 0; colIdx < COLUMNS_SIZE; colIdx++) {
        SDL_Rect colRect = {
                .x = COLUMN_X_START + (colIdx * COLUMN_X_SPACING),
                .y = COLUMN_Y_START,
                .w = CARD_DISPLAY_WIDTH,
                .h = CARD_DISPLAY_HEIGHT * 5  // Extend the drop area
        };

        if (isPointWithinRect(mouseX, mouseY, colRect)) {
            targetColumn = colIdx;
            break;
        }
    }

    // If dropped on a valid column and not the same column
    if (targetColumn != -1 && targetColumn != dragState.sourceColumnIndex) {
        // Get the first card in the chain
        Card* firstCard = (Card*)dragState.selectedNode->data;

        // Move the card and all cards below it
        gameManager_moveCard(firstCard->rank, firstCard->suit,
                                  dragState.sourceColumnIndex, targetColumn);
    }

    // Reset drag state
    dragState.isDragging = false;
    dragState.selectedNode = NULL;
    dragState.cardCount = 0;
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

void playScene_init(void* data) {
    gameManager_enterPlayMode();
}

void playScene_handleEvent(SDL_Event* event) {
    // Handle card dragging based on event type
    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
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

    // Draw border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    SDL_Rect border = {10, 10, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 90};
    SDL_RenderDrawRect(renderer, &border);
}

void playScene_cleanup(void) {
    // Any cleanup needed when leaving the play scene
}