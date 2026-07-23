#ifndef GAME_H
#define GAME_H
#include "deck.h"
#include "stack.h"
#include "player.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    card cardsInBook[4];
    int bookSize;
    bool completed;
    int ownerId;
    values bookValue;
} book;

typedef struct {
    bool hasWon;
    int winnerId;
} win;

typedef struct {
    win winCondition;
    stack_card drawPile;
    player players[6];
    int playerCount;
    book books[13];
    int sizeOfBooks;
} game_state;

void gameInit();

void startGame();

#endif