#ifndef GAME_H
#define GAME_H
#include "deck.h"
#include "stack.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    card cardsInBook[4];
    int bookSize;
    bool completed;
    int ownerId;
} book;

typedef struct {
    int playerNum;
    card *hand;
    int handSize;
    int capacity;
} player;

typedef struct {
    bool hasWon;
    player winner;
} win;

typedef struct {
    win winCondition;
    stack_card drawPile;
    player players[6];
    book books[13];
    int sizeOfBooks;
} game_state;

void deckToStack(game_state *game, card deck[]);

void playersInit(game_state *game);

void checkHandCapacity(player *player);

void drawCard(game_state *game, int playerId);

void giveCardToPlayer(game_state *game, int giverId, int recieverId, int cardIndex);

void giveCardToBook(game_state *game, book *book, int giverId, int cardIndex);

void transferBookCards(game_state *game, player *player, values bookValue);

void checkHandForBook(game_state *game, player *player);

void checkPlayersForBook(game_state *game);

void gameInit();

void gameplayLoop(game_state *game);

void startGame();

#endif