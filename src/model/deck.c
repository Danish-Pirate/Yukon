#include <string.h>
#include "deck.h"
#include "../utils/error_handler.h"

DeckLoadResult loadDeckFromFile(LinkedList **deck, char filePath[]) {
    FILE* file = fopen(filePath, "r");
    if (!file) {
        errorHandler_reportError(ERROR_FILE_IO, "Could not open deck file");
        return LOAD_ERROR_FILE_OPEN;
    }

    LinkedList* newDeck = createList(sizeof(Card));
    if (!newDeck) {
        errorHandler_reportError(ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for deck");
        fclose(file);
        return LOAD_ERROR_FILE_OPEN;
    }

    int deckCheck[52] = {0}, cardsAdded = 0;
    char line[4];
    DeckLoadResult result = LOAD_SUCCESS;

    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) < 2) continue;

        Rank rank = charToRank(line[0]);
        if (rank == INVALID_RANK) {
            errorHandler_reportError(ERROR_INVALID_PARAMETER, "Invalid rank in deck file");
            result = LOAD_ERROR_INVALID_RANK;
            break;
        }

        Suit suit = charToSuit(line[1]);
        if (suit == INVALID_SUIT) {
            errorHandler_reportError(ERROR_INVALID_PARAMETER, "Invalid suit in deck file");
            result = LOAD_ERROR_INVALID_SUIT;
            break;
        }

        int cardIndex = rank + suit * 13;
        if (deckCheck[cardIndex]) {
            errorHandler_reportError(ERROR_GAME_LOGIC, "Duplicate card in deck file");
            result = LOAD_ERROR_DUPLICATE_CARD;
            break;
        }

        Card* card = createCard(suit, rank, false);
        if (!card) {
            errorHandler_reportError(ERROR_MEMORY_ALLOCATION, "Failed to allocate memory for card");
            result = LOAD_ERROR_FILE_OPEN;
            break;
        }

        deckCheck[cardIndex] = 1;
        addNodeToBack(newDeck, card);
        cardsAdded++;
    }

    fclose(file);

    if (result == LOAD_SUCCESS && cardsAdded != 52) {
        errorHandler_reportError(ERROR_GAME_LOGIC, "Incomplete deck in file");
        result = LOAD_ERROR_INCOMPLETE_DECK;
    }

    if (result == LOAD_SUCCESS) {
        if (*deck) freeList(*deck);
        *deck = newDeck;
    } else {
        freeList(newDeck);
    }

    return result;
}

void saveDeckToFile(LinkedList *deck, char filePath[]) {
    FILE* file = fopen(filePath, "w");
    if (!file) {
        errorHandler_reportError(ERROR_FILE_IO, "Could not open file for saving deck");
        return;
    }

    Node* current = deck->head;
    while (current != NULL) {
        Card* card = (Card*)current->data;
        char* line = cardToString(card);
        fprintf(file, "%s\n", line);
        current = current->nextNode;
    }
    fclose(file);
}