//
// Created by danish on 3/25/25.
//

#include "card_deck.h"
#include "game_utils.h"

LinkedList* createDeck() {
    LinkedList* deck = createList(sizeof(Card));

    // Generate deck in reverse order, since it is built like a stack
    for (int suit = SPADES; suit >= CLUBS; suit--) {
        for (int rank = KING; rank >= ACE; rank--) {
            Card card = createCard(suit, rank, false);
            addNode(deck, &card);
        }
    }

    return deck;
}


