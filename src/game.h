#ifndef GAME_H
#define GAME_H
#include "bot.h"
#include "deck.h"
#include "event_stream.h"
#include "player.h"
#include "stack.h"
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

typedef struct GameState {
    Win winCondition;
    StackCard drawPile;
    Player players[6];
    int playerCount;
    BotManager botManager;
    Book books[13];
    int sizeOfBooks;
    EventStream stream;
} GameState;

Card drawCard(GameState *game, Player *player);

bool checkHandForCard(Player *actor, Player *target, values targetedValue);

void checkPlayersForBook(GameState *game);

bool checkForWin(GameState *game);

void gameInit();

void startGame();

#endif
