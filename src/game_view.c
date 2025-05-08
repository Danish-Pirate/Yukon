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

// Include windows.h and enable VT mode if compiled for windows
#if defined(_WIN32)
#include <windows.h>
  static void enable_vt_mode(void) {
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hOut == INVALID_HANDLE_VALUE) return;
      DWORD dwMode = 0;
      if (!GetConsoleMode(hOut, &dwMode)) return;
      dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      SetConsoleMode(hOut, dwMode);
  }
#else
static void enable_vt_mode(void) {
    /* no-op on non-Windows; ANSI escapes just work on Unix terminals */
}
#endif


void setColors(){printf("\033[1;30;103m");}

void clearScreen(){printf("\033[2J");}

void cursorSet(int column, int row){
    printf("\033[%d;%dH",row+1,column+1);
};

void initView(){
    enable_vt_mode();
    setColors();
    sleep(1);
    clearScreen();
}

void updateView(GameState *gameState) {
    clearScreen();
    int mainSectionHeight;
    drawColumnHeaders();

    if (gameState->gamePhase == StartupPhase){
        mainSectionHeight = drawDeckView(gameState);
    } else {
        mainSectionHeight = drawColumns(gameState);
    }

    drawFoundations(gameState);
    drawControls(gameState,mainSectionHeight);

    fflush(stdout);
}

void drawColumnHeaders(){
    for (int col=0;col<COLUMNS_SIZE;col++){
        cursorSet(col*(CARD_WIDTH+CARD_PADDING_RIGHT),0);
        printf("C%d",col+1);
    }
}

int drawColumns(GameState *gameState){
    int mainSectionHeight = 3;
    for (int col=0;col<COLUMNS_SIZE;col++){
        // Print cards in stack
        Node* current = gameState->cardColumns[col]->head;
        int i = 0;
        while (current != NULL){
            // Set cursor and print
            cursorSet(col*(CARD_WIDTH+CARD_PADDING_RIGHT),HEADER_HEIGHT+(i++));
            printf("%s",cardTopSideToString(current->data));
            current = current->nextNode;
        }
        // Increase stack height if current stack is higher
        mainSectionHeight = (i > mainSectionHeight) ? i : mainSectionHeight;
    }
    return mainSectionHeight;
}

int drawDeckView(GameState *gameState){
    if (gameState->deck == NULL){
        return 7;
    }
    int column = 0;
    int row = HEADER_HEIGHT;
    Node* current = gameState->deck->head;
    while (current != NULL){
        cursorSet(column*(CARD_WIDTH+CARD_PADDING_RIGHT),row);
        printf("%s",cardTopSideToString(current->data));
        if (++column >= COLUMNS_SIZE){
            row++;
            column=0;
        }
        current = current->nextNode;
    }
    return row-HEADER_HEIGHT;
}


void drawFoundations(GameState *gameState){
    int foundationsColumn = COLUMNS_SIZE * (CARD_WIDTH+CARD_PADDING_RIGHT)+FOUNDATION_PADDING_LEFT;
    for (int pile=0;pile<PILES_SIZE;pile++){
        // Get top card of pile
        Node* lastNode = gameState->cardFoundationPiles[pile]->tail;
        // Set to [] if pile is empty
        char* topCardAsString = "[]";
        if (lastNode != NULL){
            topCardAsString = cardTopSideToString(lastNode->data);
        }
        // Set cursor and print
        cursorSet(foundationsColumn,pile*(1+FOUNDATION_PADDING_BOTTOM)+HEADER_HEIGHT);
        printf("%s F%d",topCardAsString,pile+1);
    }
}
void drawControls(GameState *gameState,int topOffset){
    cursorSet(0,HEADER_HEIGHT+topOffset+CONTROLS_PADDING_TOP);
    printf("LAST Command:%s\n",gameState->lastCommand);
    printf("Message:%s\n",gameState->lastResponse);
    printf("INPUT >");
}