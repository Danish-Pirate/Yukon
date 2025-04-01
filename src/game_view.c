//
// Created by danish on 3/25/25.
//

#include <stddef.h>
#include <stdio.h>
#include "game_view.h"
#include "game_utils.h"

void updateView(GameState *gameState) {
    for (int i = 0; i < COLUMNS_SIZE; ++i) {
        Card *currentCard = (Card *) gameState->cardColumns[i];
        while (currentCard != NULL) {
            char *elementString = convertCardToString(currentCard);
            printf("%s\n", elementString);

            currentCard = currentCard->nextCard;
        }
    }
}