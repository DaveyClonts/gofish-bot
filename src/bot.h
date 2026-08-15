#ifndef BOT_H
#define BOT_H
#include "deck.h"
#include <stdbool.h>

// certianCards: cards that certain to be in targetId's hand
// Certain: A player just asked for Queens, so they currently have a Queen.
typedef struct {
    values cardValue;
    int targetId;
} CertainCard;

typedef struct {
    CertainCard *certainCards;
    int size;
    int capacity;
} Memory;

typedef struct {
    int playerId;
    Memory memory;
    int processedMemory;
} BotState;

typedef struct {
    BotState *bots;
    int size;
    int capacity;
} BotManager;

typedef struct GameState GameState;

void initBotManager(BotManager *bots);

void initBot(BotManager *bots, int playerId);

BotState *getBot(BotManager *bots, int playerId);

void doTurn(BotState *bot, GameState *game);

#endif
