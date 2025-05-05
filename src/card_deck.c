#include "card_deck.h"
#include "game_utils.h"

LinkedList* createDeck() {
    LinkedList* deck = createList(sizeof(Card));
    if (deck == NULL) {
        return NULL;
    }

    for (int suit = CLUBS; suit <= SPADES; suit++) {
        for (int rank = ACE; rank <= KING; rank++) {
            Card* card = createCard(suit, rank, false);
            if (card != NULL) {
                addNodeToBack(deck, card);
            }
        }
    }
    return deck;
}

void setAllCardsFaceUp(LinkedList* deck,bool faceUp) {
    Node* current = deck->head;
    while (current!=NULL){
        ((Card*) current->data)->isFaceUp = faceUp;
        current = current->nextNode;
    }
}

