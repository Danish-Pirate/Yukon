#include <stdlib.h>
#include <string.h>
#include "card.h"

Card* createCard(Suit suit, Rank rank, bool isFaceUp) {
    Card *card = (Card *)malloc(sizeof(Card));
    card->suit = suit;
    card->rank = rank;
    card->isFaceUp = isFaceUp;
    return card;
}
Rank charToRank(char c) {
    switch (c) {
        case 'A': return ACE;
        case '2': return TWO;
        case '3': return THREE;
        case '4': return FOUR;
        case '5': return FIVE;
        case '6': return SIX;
        case '7': return SEVEN;
        case '8': return EIGHT;
        case '9': return NINE;
        case 'T': return TEN;
        case 'J': return JACK;
        case 'Q': return QUEEN;
        case 'K': return KING;
        default : return INVALID_RANK;
    }
}
Suit charToSuit(char c) {
    switch (c) {
        case 'C': return CLUBS;
        case 'D': return DIAMONDS;
        case 'H': return HEARTS;
        case 'S': return SPADES;
        default : return INVALID_SUIT;
    }
}
int convertRankToInt(char rank) {
    switch (rank) {
        case 'A': return ACE;
        case '2': return TWO;
        case '3': return THREE;
        case '4': return FOUR;
        case '5': return FIVE;
        case '6': return SIX;
        case '7': return SEVEN;
        case '8': return EIGHT;
        case '9': return NINE;
        case 'T': return TEN;
        case 'J': return JACK;
        case 'Q': return QUEEN;
        case 'K': return KING;
        default: return -1; // Invalid rank
    }
}
int convertSuitToInt(char suit) {
    switch (suit) {
        case 'C': return CLUBS;
        case 'D': return DIAMONDS;
        case 'H': return HEARTS;
        case 'S': return SPADES;
        default: return -1; // Invalid suit
    }
}
char *cardToString(Card *card) {
    static char result[3];

    // lookup arrays
    char *ranks = "A23456789TJQK";
    char *suits = "CDHS";

    result[0] = ranks[card->rank];
    result[1] = suits[card->suit];
    result[2] = '\0';

    return result;

}
char *cardTopSideToString(Card *card) {
    static char result[3];
    if (card->isFaceUp == false) {
        strcpy(result, "[]");
        return result;
    }
    return cardToString(card);

}