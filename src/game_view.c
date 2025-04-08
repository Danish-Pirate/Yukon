//
// Created by danish on 3/25/25.
//

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include "game_view.h"
#include "game_utils.h"


#define HEADER_HEIGHT 2
#define CARD_WIDTH 2
#define CARD_PADDING_RIGHT 2
#define FOUNDATION_PADDING_BOTTOM 1
#define FOUNDATION_PADDING_LEFT 2
#define CONTROLS_PADDING_TOP 2

void setColors(){printf("\033[1;30;43m");}

void clearScreen(){printf("\033[2J");}

void cursorSet(int column, int row){
    printf("\033[%d;%dH",row,column);
};

void initView(){
    setColors();
    sleep(1);
    clearScreen();
}

void updateView(GameState *gameState) {
    clearScreen();

    // Draw Main board
    int stackHeight = 3;
    for (int col=0;col<COLUMNS_SIZE;col++){
        // Print stack header
        int displayColumn = col*(CARD_WIDTH+CARD_PADDING_RIGHT);
        cursorSet(displayColumn,1);
        printf("C%d",col+1);

        // Print cards in stack
        Node* head = gameState->cardColumns[col]->head;
        int i = 0;
        while (head != NULL){
            // Set cursor and print
            cursorSet(displayColumn,HEADER_HEIGHT+(i++)+1);
            printf("%s",convertCardToString(head->data));
            head = head->nextNode;
        }
        // Increase stack height if current stack is higher
        stackHeight = (i > stackHeight) ? i : stackHeight;
    }

    // Draw foundations
    int foundationsColumn = COLUMNS_SIZE * (CARD_WIDTH+CARD_PADDING_RIGHT)+FOUNDATION_PADDING_LEFT;
    for (int pile=0;pile<PILES_SIZE;pile++){
        // Get top card of pile
        Node* lastNode = getLastNode(gameState->cardFoundationPiles[pile]);
        // Set to [] if pile is empty
        char* topCardAsString = "[]";
        if (lastNode != NULL){
            topCardAsString = convertCardToString(lastNode->data);
        }
        // Set cursor and print
        cursorSet(foundationsColumn,pile*(1+FOUNDATION_PADDING_BOTTOM)+HEADER_HEIGHT+1);
        printf("%s F%d",topCardAsString,pile+1);
    }

    // Draw control
    cursorSet(0,HEADER_HEIGHT+stackHeight+CONTROLS_PADDING_TOP);
    printf("LAST Command:%s\n",gameState->lastCommand);
    printf("Message:%s\n",gameState->lastResponse);
    printf("INPUT >");

    fflush(stdout);
}