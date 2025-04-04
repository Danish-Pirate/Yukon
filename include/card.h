#include <stdbool.h>

#ifndef YUKON_CARDGAME_CARD_H
#define YUKON_CARDGAME_CARD_H

typedef enum {
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
} Suit;

typedef enum {
    ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING
} Rank;

typedef struct Card {
    Suit suit;
    Rank rank;
    bool isFaceUp;
    struct Card* nextCard;
} Card;

Card createCard(Suit suit, Rank rank, bool isFaceUp);
void deleteCard(Card* card);
void flipCard(Card* card);
void connectCard(Card* card);

#endif //YUKON_CARDGAME_CARD_H