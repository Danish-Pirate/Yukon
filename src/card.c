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



char rankToChar(Rank r) {
    switch (r) {
        case ACE   : return 'A';
        case TWO   : return '2';
        case THREE : return '3';
        case FOUR  : return '4';
        case FIVE  : return '5';
        case SIX   : return '6';
        case SEVEN : return '7';
        case EIGHT : return '8';
        case NINE  : return '9';
        case TEN   : return 'T';
        case JACK  : return 'J';
        case QUEEN : return 'Q';
        case KING  : return 'K';
        default    : return '?';
    }
}

char suitToChar(Suit s){
    switch(s){
        case CLUBS    : return 'C';
        case DIAMONDS : return 'D';
        case HEARTS   : return 'H';
        case SPADES   : return 'S';
        default       : return '?';
    }

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