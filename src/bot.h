#ifndef BOT_H
#define BOT_H
#include "deck.h"
#include "game.h"
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
    int botId;
    Memory memory;
    int processedMemory;
} BotState;

void initBot(BotState *bot, int botId);

void doTurn(GameState *game);

#endif
