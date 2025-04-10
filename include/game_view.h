//
// Created by danish on 3/25/25.
//

#include "game_manager.h"

#ifndef YUKON_CARDGAME_GAME_VIEW_H
#define YUKON_CARDGAME_GAME_VIEW_H

void setColors();
void clearScreen();
void cursorSet(int column, int row);
void initView();
void updateView(GameState *gameState);
void drawColumnHeaders();
int drawColumns(GameState *gameState);
int drawDeckView(GameState *gameState);
void drawFoundations(GameState *gameState);
void drawControls(GameState *gameState,int topOffset);


#endif //YUKON_CARDGAME_GAME_VIEW_H
