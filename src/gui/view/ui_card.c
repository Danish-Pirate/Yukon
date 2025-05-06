#include <SDL.h>
#include "ui_card.h"
#include "texture_manager.h"
#include "utils/gui_service_locator.h"
#include "gui_utils.h"
#include "model/card.h"

#define FACEDOWN_CARD_TEXCOORDS_X (CARD_WIDTH * 2)
#define FACEDOWN_CARD_TEXCOORDS_Y (CARD_HEIGHT * 4)

SDL_Rect getCardTexRect(int rankNumber, int suitNumber, bool isFaceUp) {
    SDL_Rect textRect;
    if ((rankNumber < 0 || rankNumber > 12) || (suitNumber < 0 || suitNumber > 3)) {
        printf("Invalid rank or suit number! Rank: %d, Suit: %d", rankNumber, suitNumber);
        return textRect;
    }
    if (isFaceUp) {
        textRect.x = rankNumber * CARD_WIDTH;
        textRect.y = suitNumber * CARD_HEIGHT;
        textRect.w = CARD_WIDTH;
        textRect.h = CARD_HEIGHT;
        return textRect;
    } else {
        textRect.x = FACEDOWN_CARD_TEXCOORDS_X;
        textRect.y = FACEDOWN_CARD_TEXCOORDS_Y;
        textRect.w = CARD_WIDTH;
        textRect.h = CARD_HEIGHT;
        return textRect;
    }
}

void drawCard(SDL_Rect screenRect, Card *card) {
    SDL_Texture* atlas = getCardTexAtlas();
    if (!atlas) {
        printf("ERROR: Card texture atlas is NULL in drawCard()\n");
        return;
    }

    // Use accessor functions instead of direct struct access
    Rank rank = yukon_getCardRank(card);
    Suit suit = yukon_getCardSuit(card);
    bool isFaceUp = yukon_isCardFaceUp(card);

    SDL_Rect texRect = getCardTexRect(rank, suit, isFaceUp);

    int result = SDL_RenderCopy(windowManager_getRenderer(), atlas, &texRect, &screenRect);
    if (result != 0) {
        printf("SDL_RenderCopy failed: %s\n", SDL_GetError());
    }
}

bool isCardHovered(int mouse_PosX, int mouse_PosY, SDL_Rect displayRect) {
    return isPointWithinRect(mouse_PosX, mouse_PosY, displayRect);
}