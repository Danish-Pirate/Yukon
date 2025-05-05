#pragma once

// Layout settings for card display
#include "card.h"

#define CARDS_PER_ROW 13
#define CARD_SPACING_X 5     // Reduced spacing between cards horizontally
#define CARD_SPACING_Y 5    // Spacing between rows
#define CARD_START_X 20
#define CARD_START_Y 20

// Card display dimensions (scaled down from texture)
#define CARD_DISPLAY_WIDTH 111    // Smaller width for display
#define CARD_DISPLAY_HEIGHT 173   // Smaller height for display

// Card texture dimensions in pixels
#define CARD_WIDTH 148
#define CARD_HEIGHT 230

void drawCard(SDL_Rect screenRect, Card *card);
bool isCardHovered(int mouse_PosX, int mouse_PosY, SDL_Rect displayRect);