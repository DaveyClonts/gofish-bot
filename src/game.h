#ifndef GAME_H
#define GAME_H
#include "deck.h"
#include "stack.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    card cardsInBook[4];
    bool completed;
    int ownerID;
} book;

typedef struct {
    int playerNum;
    card *hand;
    int handSize;
    int capacity;
} player;

typedef struct {
    stack_card drawPile;
    player players[6];
} game_state;

void deckToStack(game_state *game, card deck[]);

void playersInit(game_state *game);

void checkHandCapacity(player *player);

void drawCard(game_state *game, int playerId);

void giveCard(game_state *game, int giverId, int recieverId, int cardIndex);

void gameInit();

void gameplayLoop();

void startGame();

#endif