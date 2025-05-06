#include <SDL.h>
#include "scene_manager.h"
#include "service/game_service.h"
#include "utils/service_locator.h"
#include "view/ui_card.h"
#include "view/ui_button.h"
#include "play_scene.h"
#include "SDL_ttf.h"
#include "view/ui_manager.h"

typedef struct {
    bool isDragging;
    int sourceColumnIndex;
    Node* selectedNode;
    int dragOffsetX;
    int dragOffsetY;
    SDL_Rect dragBaseRect;
    int cardCount;
} DragState;

static DragState dragState = {0};
static UI_Button backButton;

static void handleGameWonEvent(Event* event) {
    if (event->type != EVENT_GAME_WON) return;

    const GameView* gameView = gameService_getView();

    if (gameView->isGameWon) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Game won!");
    }
}

static bool tryStartDragging(int mouseX, int mouseY) {
    GameState* gameState = serviceLocator_getGameState();

    for (int colIdx = 0; colIdx < COLUMNS_SIZE; colIdx++) {
        LinkedList* column = gameState->cardColumns[colIdx];
        if (column->head == NULL) continue;

        Node* clickableNode = NULL;
        SDL_Rect clickableRect = {0};
        int cardCount = 0;

        Node* tempNode = column->head;
        int cardY = COLUMN_Y_START;

        while (tempNode != NULL) {
            Card* card = (Card*)tempNode->data;

            if (card->isFaceUp) {
                SDL_Rect cardRect = {
                        .x = COLUMN_X_START + (colIdx * COLUMN_X_SPACING),
                        .y = cardY,
                        .w = CARD_DISPLAY_WIDTH,
                        .h = CARD_DISPLAY_HEIGHT
                };

                if (isCardHovered(mouseX, mouseY, cardRect)) {
                    clickableNode = tempNode;
                    clickableRect = cardRect;

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
    int targetFoundation = -1;
    int targetColumn = -1;

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

    if (targetFoundation != -1) {
        if (dragState.sourceColumnIndex < COLUMNS_SIZE) {
            Card* card = (Card*)dragState.selectedNode->data;

            if (dragState.cardCount == 1) {
                int foundationIndex = targetFoundation + COLUMNS_SIZE;
                gameService_moveCard(card->rank, card->suit,
                                     dragState.sourceColumnIndex, foundationIndex);
            }
            gameService_checkGameWon();
        }
    }
    else {
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
            gameService_moveCard(firstCard->rank, firstCard->suit,
                                 dragState.sourceColumnIndex, targetColumn);
        }
    }

    dragState.isDragging = false;
    dragState.selectedNode = NULL;
    dragState.cardCount = 0;
}

static void backToStartupCallback() {
    eventSystem_publish(EVENT_PLAY_MODE_EXIT, NULL);

    SceneChangeData* data = malloc(sizeof(SceneChangeData));
    data->type = SCENE_STARTUP_MODE;
    data->data = NULL;
    eventSystem_publish(EVENT_SCENE_CHANGE, data);
}

static void drawVictoryDialog() {
    SDL_Renderer *renderer = serviceLocator_getRenderer();
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect bgRect = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &bgRect);

    int dialogWidth = 400;
    int dialogHeight = 200;
    SDL_Rect dialogRect = {
            (width - dialogWidth) / 2,
            (height - dialogHeight) / 2,
            dialogWidth,
            dialogHeight
    };

    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &dialogRect);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderDrawRect(renderer, &dialogRect);

    TTF_Font* largeFont = TTF_OpenFont("assets/fonts/arial.ttf", 36);
    TTF_Font* smallFont = TTF_OpenFont("assets/fonts/arial.ttf", 24);

    if (largeFont && smallFont) {
        SDL_Color titleColor = {0, 100, 0, 255};
        SDL_Color textColor = {0, 0, 0, 255};

        SDL_Surface* titleSurface = TTF_RenderText_Solid(largeFont, "VICTORY!", titleColor);
        if (titleSurface) {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            if (titleTexture) {
                SDL_Rect titleRect = {
                        (width - titleSurface->w) / 2,
                        dialogRect.y + 30,
                        titleSurface->w,
                        titleSurface->h
                };
                SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
                SDL_DestroyTexture(titleTexture);
            }
            SDL_FreeSurface(titleSurface);
        }

        int buttonWidth = 100;
        int buttonHeight = 40;
        SDL_Rect buttonRect = {
                (width - buttonWidth) / 2,
                dialogRect.y + dialogHeight - 50,
                buttonWidth,
                buttonHeight
        };

        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
        SDL_RenderFillRect(renderer, &buttonRect);

        SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);
        SDL_RenderDrawRect(renderer, &buttonRect);

        SDL_Surface* buttonSurface = TTF_RenderText_Solid(smallFont, "OK", (SDL_Color){255, 255, 255, 255});
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

        TTF_CloseFont(largeFont);
        TTF_CloseFont(smallFont);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

static void drawFoundationPiles() {
    SDL_Renderer* renderer = serviceLocator_getRenderer();
    GameState* gameState = serviceLocator_getGameState();

    for (int i = 0; i < PILES_SIZE; i++) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect foundationRect = {
                .x = FOUNDATION_X_START,
                .y = FOUNDATION_Y + (i * (FOUNDATION_Y_SPACING + CARD_DISPLAY_HEIGHT)),
                .w = CARD_DISPLAY_WIDTH,
                .h = CARD_DISPLAY_HEIGHT
        };
        SDL_RenderDrawRect(renderer, &foundationRect);

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
    SDL_Renderer* renderer = serviceLocator_getRenderer();
    GameState* gameState = serviceLocator_getGameState();

    for (int colIdx = 0; colIdx < COLUMNS_SIZE; colIdx++) {
        LinkedList* column = gameState->cardColumns[colIdx];

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect emptyColRect = {
                .x = COLUMN_X_START + (colIdx * COLUMN_X_SPACING),
                .y = COLUMN_Y_START,
                .w = CARD_DISPLAY_WIDTH,
                .h = CARD_DISPLAY_HEIGHT
        };
        SDL_RenderDrawRect(renderer, &emptyColRect);

        if (column->head != NULL) {
            Node* currentNode = column->head;
            int cardY = COLUMN_Y_START;
            bool skipDrawing = false;

            while (currentNode != NULL) {
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

                cardY += COLUMN_Y_CARD_OFFSET;
                currentNode = currentNode->nextNode;
            }
        }
    }

    if (dragState.isDragging && dragState.selectedNode != NULL) {
        Node* dragNode = dragState.selectedNode;
        int offsetY = 0;

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
    SDL_Renderer *renderer = serviceLocator_getRenderer();
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    eventSystem_publish(EVENT_PLAY_MODE_ENTER, NULL);
    eventSystem_subscribe(EVENT_GAME_WON, handleGameWonEvent);

    int buttonHeight = 60;
    int buttonY = height - buttonHeight - 200;

    backButton.callback = backToStartupCallback;
    backButton.displayRect.x = (width - 150) / 2;
    backButton.displayRect.y = buttonY;
    backButton.displayRect.w = 150;
    backButton.displayRect.h = buttonHeight;
    backButton.label = "Back";
}

void playScene_handleEvent(SDL_Event* event) {
    SDL_Renderer *renderer = serviceLocator_getRenderer();
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    const GameView* gameView = gameService_getView();

    if (gameView->isGameWon && event->type == SDL_MOUSEBUTTONDOWN &&
        event->button.button == SDL_BUTTON_LEFT) {
        int buttonWidth = 100;
        int buttonHeight = 40;
        int dialogWidth = 400;
        int dialogHeight = 200;

        SDL_Rect buttonRect = {
                (width - buttonWidth) / 2,
                (height - dialogHeight) / 2 + dialogHeight - 50,
                buttonWidth,
                buttonHeight
        };

        if (isPointWithinRect(event->button.x, event->button.y, buttonRect)) {
            GameState* gameState = serviceLocator_getGameState();
            gameState->gameWon = false;
            return;
        }
        return;
    }

    if (uiManager_handleButtonEvents(&backButton, 1, event)) {
        return;
    }

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
}

void playScene_render() {

    SDL_Renderer *renderer = serviceLocator_getRenderer();
    if (!renderer) {
        printf("ERROR: Renderer is NULL in playScene_render()\n");
        return;
    }

    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    const GameView* gameView = gameService_getView();

    SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);
    SDL_Rect bgRect = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &bgRect);

    drawFoundationPiles();
    drawColumns();

    uiManager_drawButton(&backButton);

    if(gameView->isGameWon) {
        drawVictoryDialog();
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect border = {10, 10, width - 20, height - 90};
    SDL_RenderDrawRect(renderer, &border);
}

void playScene_cleanup(void) {
    eventSystem_unsubscribe(EVENT_GAME_WON, handleGameWonEvent);
}