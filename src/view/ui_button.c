#include <SDL_ttf.h>
#include "ui_button.h"
#include "../utils/game_utils.h"
#include "../utils/service_locator.h"
#include "ui_manager.h"
#include <stdio.h>

static TTF_Font* buttonFont = NULL;

void initButtonFont() {
    if (buttonFont == NULL) {
        buttonFont = TTF_OpenFont(FONT_FILEPATH, 24);
        if (buttonFont == NULL) {
            printf("Failed to load font: %s\n", TTF_GetError());
        }
    }
}

void drawButton(UI_Button button) {
    SDL_Renderer* renderer = serviceLocator_getRenderer();

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &button.displayRect);

    SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
    SDL_RenderDrawRect(renderer, &button.displayRect);

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