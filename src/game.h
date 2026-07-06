#ifndef GAME_H
#define GAME_H
#include "deck.h"
#include "stack.h"

typedef struct {
    card cardsInBook[4];
    bool completed;
    int ownerID;
} book;

typedef struct {
    int playerNum;
    card hand[];
} player;

void deckToStack(card deck[]);

void gameInit();

#endif