//
// Created by danish on 3/26/25.
//

#ifndef YUKON_CARDGAME_COMMAND_PARSER_H
#define YUKON_CARDGAME_COMMAND_PARSER_H

#include "card.h"

typedef enum {
    CMD_LD, // Load Deck from file
    CMD_SW, // Reveal cards in deck
    CMD_SI, // Split deck
    CMD_SR, // Randomly shuffle deck
    CMD_SD, // Save deck to file
    CMD_QQ, //n Quit program
    CMD_P,  // Enter play mode
    CMD_Q,  // Exit play mode back to STARTUP mode
    CMD_MV  // Move card(s)
} CommandType;

// STARTUP mode commands
typedef struct {
    CommandType type;

    char filePath[100];
} LDCommand;

typedef struct {
    CommandType type;
    unsigned int splitIndex;
} SICommand;

typedef struct {
    CommandType type;
} SRCommand;

typedef struct {
    CommandType type;
    char filepath[100];
} SDCommand;

typedef struct {
    CommandType type;
} QQCommand;

typedef struct {
    CommandType type;
} SWCommand;

typedef struct {
    CommandType type;
} PCommand;

// PLAY mode commands

typedef struct {
    CommandType type;
} QCommand;

typedef struct {
    CommandType type;

    char fromColumnIndex; // Index for the column to move the card from
    // The index also includes the foundation piles so (0-10)
    Suit suit;            // The suit of the card to move
    Rank rank;            // The rank of the card to move

    char toColumnIndex;   // Index for the column to move the card to. Also includes foundation piles
} MVCommand;

typedef union {
    CommandType type;
    MVCommand mv;
    LDCommand ld;
    SICommand si;
    SRCommand sr;
    SDCommand sd;
    QQCommand qq;
    PCommand  p;
    QCommand q;
    SWCommand sw;
} Command;

Command parseCommand();

#endif //YUKON_CARDGAME_COMMAND_PARSER_H
