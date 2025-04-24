#include <stdbool.h>

#ifndef YUKON_CARDGAME_CARD_H
#define YUKON_CARDGAME_CARD_H

typedef enum {
    INVALID_SUIT = -1,
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
} Suit;

typedef enum {
    INVALID_RANK = -1,
    ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING
} Rank;

typedef struct Card {
    Suit suit;
    Rank rank;
    bool isFaceUp;
} Card;

Card* createCard(Suit suit, Rank rank, bool isFaceUp);
void deleteCard(Card* card);
void flipCard(Card* card);
void connectCard(Card* card);

Rank charToRank(char c);
Suit charToSuit(char c);

#endif //YUKON_CARDGAME_CARD_H