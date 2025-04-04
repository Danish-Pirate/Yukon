//
// Created by danish on 3/25/25.
//
#include <stdlib.h>
#include "card.h"
Card createCard(Suit suit, Rank rank, bool isFaceUp) {
    Card card;
    card.suit = suit;
    card.rank = rank;
    card.isFaceUp = isFaceUp;
    return card;
}