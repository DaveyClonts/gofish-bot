#ifndef GAME_H
#define GAME_H
#include "deck.h"
#include "stack.h"
#include <stddef.h>

typedef struct {
    card cardsInBook[4];
    bool completed;
    int ownerID;
} book;

typedef struct {
    int playerNum;
    card *hand;
    int handSize;
    int handMemoryAllocated;
} player;

void deckToStack(card deck[]);

void playerInit(int playerNum, player *player);

void drawCard(stack_card *drawPile, player *player);

void gameInit();

void game();

#endif