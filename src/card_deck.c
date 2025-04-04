//
// Created by danish on 3/25/25.
//

#include "card_deck.h"
#include "game_utils.h"

LinkedList* createDeck() {
    LinkedList* deck = createList(sizeof(Card));

    for (int suit = CLUBS; suit <= SPADES; suit++) {
        for (int rank = ACE; rank <= KING; rank++) {
            Card card = createCard(suit, rank, false);
            addNode(deck, &card);
        }
    }

    return deck;
}
