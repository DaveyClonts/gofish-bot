#ifndef GAME_H
#define GAME_H
#include "deck.h"
#include "player.h"
#include "stack.h"
#include "event_stream.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    Card cardsInBook[4];
    int bookSize;
    bool completed;
    int ownerId;
    values bookValue;
} Book;

typedef struct {
    bool hasWon;
    int winnerId;
} Win;

typedef struct {
    Win winCondition;
    StackCard drawPile;
    Player players[6];
    int playerCount;
    Book books[13];
    int sizeOfBooks;
    EventStream stream;
} GameState;

void gameInit();

void startGame();

#endif
