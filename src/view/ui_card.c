#include "SDL.h"
#include <stdio.h>
#include "../model/card.h"
#include "ui_card.h"
#include "scene_manager.h"
#include "texture_manager.h"
#include "../utils/game_utils.h"

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

// Returns the coordinates (SDT_RECT) in the card texture where the rank number is used to find the x-coords and suit number for y-coords.
// rankNumber: 0 is A and 12 is King
// suitNumber: 0 (clubs), 1 (diamonds), 2 (hearts), 3 (spades)
void drawCard(SDL_Rect screenRect, Card *card) {
    SDL_Rect texRect = getCardTexRect(card->rank, card->suit, card->isFaceUp);
    SDL_RenderCopy(getRenderer(), getCardTexAtlas(), &texRect, &screenRect);
}

bool isCardHovered(int mouse_PosX, int mouse_PosY, SDL_Rect displayRect) {
    return isPointWithinRect(mouse_PosX, mouse_PosY, displayRect);
}
