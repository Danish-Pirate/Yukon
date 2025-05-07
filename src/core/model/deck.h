#pragma once

#include "card.h"
#include "yukon_core.h"

#define DECK_FILEPATH "assets/decks/default.txt"

typedef enum DeckLoadResult {
    LOAD_SUCCESS,
    LOAD_ERROR_FILE_OPEN,
    LOAD_ERROR_INVALID_RANK,
    LOAD_ERROR_INVALID_SUIT,
    LOAD_ERROR_DUPLICATE_CARD,
    LOAD_ERROR_INCOMPLETE_DECK
} DeckLoadResult;

void saveDeckToFile(LinkedList *deck, const char *filePath);
DeckLoadResult loadDeckFromFile(LinkedList **deck, const char *filePath);
void toggleShowDeck(LinkedList *deck);
void showDeck(LinkedList *deck);
void randomShuffleDeck(LinkedList *deck);
void splitDeck(LinkedList *deck, int splitIndex);
