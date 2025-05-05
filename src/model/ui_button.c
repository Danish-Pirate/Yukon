#include <SDL_ttf.h>
#include "scene_manager.h"
#include "ui_button.h"
#include "../utils/game_utils.h"
#include <stdio.h>

static TTF_Font* buttonFont = NULL;

void initButtonFont() {
    if (buttonFont == NULL) {
        buttonFont = TTF_OpenFont("assets/fonts/arial.ttf", 24);
        if (buttonFont == NULL) {
            printf("Failed to load font: %s\n", TTF_GetError());
        }
    }
}

void drawButton(UI_Button button) {
    SDL_Renderer* renderer = getRenderer();

    // Draw button background
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &button.displayRect);

    // Draw button border
    SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
    SDL_RenderDrawRect(renderer, &button.displayRect);

    // Render text
    if (button.label != NULL && buttonFont != NULL) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(buttonFont, button.label, textColor);

        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            if (textTexture) {
                SDL_Rect textRect = {
                        button.displayRect.x + (button.displayRect.w - textSurface->w) / 2,
                        button.displayRect.y + (button.displayRect.h - textSurface->h) / 2,
                        textSurface->w,
                        textSurface->h
                };

                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }

            SDL_FreeSurface(textSurface);
        }
    }
}

bool isButtonHovered(int mouse_PosX, int mouse_PosY, SDL_Rect displayRect) {
    return isPointWithinRect(mouse_PosX, mouse_PosY, displayRect);
}