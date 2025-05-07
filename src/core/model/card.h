#pragma once

#include <stdbool.h>

typedef enum Suit {
    INVALID_SUIT = -1,
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES
} Suit;
typedef enum Rank {
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
char *cardToString(Card *card);
char *cardTopSideToString(Card *card);
int convertRankToInt(char rank);
int convertSuitToInt(char suit);